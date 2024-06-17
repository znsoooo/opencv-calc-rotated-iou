/*
    CRI = CalcRotatedIou

    Author:      Li Shixian
    Date:        2024-05-09
    Last update: 2024-06-17
*/

#include <math.h>
#include "cri_header.hpp"

#define CV_PI 3.1415926535897932384626433832795
#define CV_Assert(...)
#define CV_INSTRUMENT_REGION()

#define bool char
#define true 1
#define false 0


typedef struct {
    int x, y, width, height;
} Rect;

typedef struct {
    float width, height;
} Size2f;

typedef struct {
    float x, y;
} Point2f;

typedef struct {
    Point2f center;
    Size2f size;
    float angle;
} RotatedRect;


// area of a whole sequence
double contourArea(Point2f* contour, int npoints, bool oriented )
{
    if( npoints == 0 )
        return 0.;

    double a00 = 0;
    Point2f prev = contour[npoints-1];

    for( int i = 0; i < npoints; i++ )
    {
        Point2f p = contour[i];
        a00 += (double)prev.x * p.y - (double)prev.y * p.x;
        prev = p;
}

    a00 *= 0.5;
    if( !oriented )
        a00 = fabs(a00);

    return a00;
}

Point2f Point2f_sub(const Point2f self, const Point2f other)
{
    float x = self.x, y = self.y;
    Point2f result = {x - other.x, y - other.y};
    return result;
}

float Point2f_cross(const Point2f self, const Point2f other) 
{
    float x = self.x, y = self.y;
    return x * other.y - y * other.x;
}

void RotatedRect_points(const RotatedRect self, Point2f pt[])
{
    Point2f center = self.center;
    Size2f size = self.size;
    float angle = self.angle;

    double _angle = angle * CV_PI / 180.;
    float b = (float)cos(_angle) * 0.5f;
    float a = (float)sin(_angle) * 0.5f;

    pt[0].x = center.x - a * size.height - b * size.width;
    pt[0].y = center.y + b * size.height - a * size.width;
    pt[1].x = center.x + a * size.height - b * size.width;
    pt[1].y = center.y - b * size.height - a * size.width;
    pt[2].x = 2 * center.x - pt[0].x;
    pt[2].y = 2 * center.y - pt[0].y;
    pt[3].x = 2 * center.x - pt[1].x;
    pt[3].y = 2 * center.y - pt[1].y;
}

float normL2Sqr(Point2f pt)
{
    return pt.x * pt.x + pt.y * pt.y;
}

static inline bool _isOnPositiveSide(const Point2f line_vec, const Point2f line_pt, const Point2f pt)
{
    //we are interested by the cross product between the line vector (line_vec) and the line-to-pt vector (pt-line_pt)
    //the sign of the only non-null component of the result determining which side of the line 'pt' is on
    //the "positive" side meaning depends on the context usage of the current function and how line_vec and line_pt were filled
    return (line_vec.y*(line_pt.x-pt.x) >= line_vec.x*(line_pt.y-pt.y));
}

int rotatedRectangleIntersection( const RotatedRect rect1, const RotatedRect rect2, Point2f* intersection, int* intersection_size)
{
    CV_INSTRUMENT_REGION();

    Point2f vec1[4], vec2[4];
    Point2f pts1[4], pts2[4];

    RotatedRect_points(rect1, pts1);
    RotatedRect_points(rect2, pts2);

    // L2 metric
    float samePointEps = 1e-6f;

    int ret = INTERSECT_FULL;

    // Specical case of rect1 == rect2
    {
        bool same = true;

        for( int i = 0; i < 4; i++ )
        {
            if( fabs(pts1[i].x - pts2[i].x) > samePointEps || (fabs(pts1[i].y - pts2[i].y) > samePointEps) )
            {
                same = false;
                break;
            }
        }

        if(same)
        {
            intersection_size[0] = 4;

            for( int i = 0; i < 4; i++ )
            {
                intersection[i] = pts1[i];
            }

            return INTERSECT_FULL;
        }
    }

    // Line vector
    // A line from p1 to p2 is: p1 + (p2-p1)*t, t=[0,1]
    for( int i = 0; i < 4; i++ )
    {
        vec1[i].x = pts1[(i+1)%4].x - pts1[i].x;
        vec1[i].y = pts1[(i+1)%4].y - pts1[i].y;

        vec2[i].x = pts2[(i+1)%4].x - pts2[i].x;
        vec2[i].y = pts2[(i+1)%4].y - pts2[i].y;
    }

    //we adapt the epsilon to the smallest dimension of the rects
    for( int i = 0; i < 4; i++ )
    {
        samePointEps = fmin(samePointEps, sqrt(vec1[i].x*vec1[i].x+vec1[i].y*vec1[i].y));
        samePointEps = fmin(samePointEps, sqrt(vec2[i].x*vec2[i].x+vec2[i].y*vec2[i].y));
    }
    samePointEps = fmax(1e-16f, samePointEps);

    // Line test - test all line combos for intersection
    for( int i = 0; i < 4; i++ )
    {
        for( int j = 0; j < 4; j++ )
        {
            // Solve for 2x2 Ax=b
            const float x21 = pts2[j].x - pts1[i].x;
            const float y21 = pts2[j].y - pts1[i].y;

            float vx1 = vec1[i].x;
            float vy1 = vec1[i].y;

            float vx2 = vec2[j].x;
            float vy2 = vec2[j].y;

            const float det = vx2*vy1 - vx1*vy2;
            if (fabs(det) < 1e-12)//we consider accuracy around 1e-6, i.e. 1e-12 when squared
              continue;
            const float detInvScaled = 1.f/det;

            const float t1 = (vx2*y21 - vy2*x21)*detInvScaled;
            const float t2 = (vx1*y21 - vy1*x21)*detInvScaled;

            // This takes care of parallel lines
            if( isinf(t1) || isinf(t2) || isnan(t1) || isnan(t2) )
            {
                continue;
            }

            if( t1 >= 0.0f && t1 <= 1.0f && t2 >= 0.0f && t2 <= 1.0f )
            {
                const float xi = pts1[i].x + vec1[i].x * t1;
                const float yi = pts1[i].y + vec1[i].y * t1;
                Point2f point = {xi, yi};
                intersection[intersection_size[0]++] = point;
            }
        }
    }

    if (intersection_size[0])
    {
        ret = INTERSECT_PARTIAL;
    }

    // Check for vertices from rect1 inside recct2
    for( int i = 0; i < 4; i++ )
    {
        // We do a sign test to see which side the point lies.
        // If the point all lie on the same sign for all 4 sides of the rect,
        // then there's an intersection
        int posSign = 0;
        int negSign = 0;

        const Point2f pt = pts1[i];

        for( int j = 0; j < 4; j++ )
        {
            // line equation: Ax + By + C = 0 where
            // A = -vec2[j].y ; B = vec2[j].x ; C = -(A * pts2[j].x + B * pts2[j].y)
            // check which side of the line this point is at
            // A*x + B*y + C <> 0
            // + computation reordered for better numerical stability

            const bool isPositive = _isOnPositiveSide(vec2[j], pts2[j], pt);

            if( isPositive )
            {
                posSign++;
            }
            else
            {
                negSign++;
            }
        }

        if( posSign == 4 || negSign == 4 )
        {
            intersection[intersection_size[0]++] = pts1[i];
        }
    }

    // Reverse the check - check for vertices from rect2 inside recct1
    for( int i = 0; i < 4; i++ )
    {
        // We do a sign test to see which side the point lies.
        // If the point all lie on the same sign for all 4 sides of the rect,
        // then there's an intersection
        int posSign = 0;
        int negSign = 0;

        const Point2f pt = pts2[i];

        for( int j = 0; j < 4; j++ )
        {
            // line equation: Ax + By + C = 0 where
            // A = -vec1[j].y ; B = vec1[j].x ; C = -(A * pts1[j].x + B * pts1[j].y)
            // check which side of the line this point is at
            // A*x + B*y + C <> 0
            // + computation reordered for better numerical stability

            const bool isPositive = _isOnPositiveSide(vec1[j], pts1[j], pt);

            if( isPositive )
            {
                posSign++;
            }
            else
            {
                negSign++;
            }
        }

        if( posSign == 4 || negSign == 4 )
        {
            intersection[intersection_size[0]++] = pts2[i];
        }
    }

    int N = intersection_size[0];
    if (N == 0)
    {
        return INTERSECT_NONE;
    }

    // Get rid of duplicated points
    const int Nstride = N;
    float distPt[N * N];
    int ptDistRemap[N];
    for (int i = 0; i < N; ++i)
    {
        const Point2f pt0 = intersection[i];
        ptDistRemap[i] = i;
        for (int j = i + 1; j < N; )
        {
            const Point2f pt1 = intersection[j];
            const float d2 = normL2Sqr(Point2f_sub(pt1, pt0));
            if(d2 <= samePointEps)
            {
                if (j < N - 1)
                    intersection[j] =  intersection[N - 1];
                N--;
                continue;
            }
            distPt[i*Nstride + j] = d2;
            ++j;
        }
    }
    while (N > 8) // we still have duplicate points after samePointEps threshold (eliminate closest points)
    {
        int minI = 0;
        int minJ = 1;
        float minD = distPt[1];
        for (int i = 0; i < N - 1; ++i)
        {
            const float* pDist = distPt + Nstride * ptDistRemap[i];
            for (int j = i + 1; j < N; ++j)
            {
                const float d = pDist[ptDistRemap[j]];
                if (d < minD)
                {
                    minD = d;
                    minI = i;
                    minJ = j;
                }
            }
        }
        CV_Assert(fabs(normL2Sqr(intersection[minI], intersection[minJ]) - minD) < 1e-6);  // ptDistRemap is not corrupted
        // drop minJ point
        if (minJ < N - 1)
        {
            intersection[minJ] =  intersection[N - 1];
            ptDistRemap[minJ] = ptDistRemap[N - 1];
        }
        N--;
    }

    // order points
    for (int i = 0; i < N - 1; ++i)
    {
        Point2f diffI = Point2f_sub(intersection[i + 1], intersection[i]);
        for (int j = i + 2; j < N; ++j)
        {
            Point2f diffJ = Point2f_sub(intersection[j], intersection[i]);
            if (Point2f_cross(diffI, diffJ) < 0)
            {
                Point2f temp = intersection[i + 1];
                intersection[i + 1] = intersection[j];
                intersection[j] = temp;
                diffI = diffJ;
            }
        }
    }

    intersection_size[0] = N;

    return ret;
}


double CalcRotatedIou(const RotatedRect rect1, const RotatedRect rect2)
{
    double area1 = rect1.size.width * rect1.size.height;
    double area2 = rect2.size.width * rect2.size.height;
    if (area1 < 1e-14 || area2 < 1e-14) {
        return 0.0;
    }

    Point2f inter_points[100];
    int inter_points_size = 0;
    rotatedRectangleIntersection(rect1, rect2, inter_points, &inter_points_size);
    if (inter_points_size) {
        double inter_area = contourArea(inter_points, inter_points_size, false);
        double union_area = area1 + area2 - inter_area;
        double iou = inter_area / union_area;
        return iou;
    } else {
        return 0.0;
    }
}

double CalcRotatedIouC(  // CalcRotatedIou C Interface
    float cx1, float cy1, float w1, float h1, float a1,  // rect1
    float cx2, float cy2, float w2, float h2, float a2   // rect2
) {
    RotatedRect rect1 = {{cx1, cy1}, {w1, h1}, a1};
    RotatedRect rect2 = {{cx2, cy2}, {w2, h2}, a2};

    double iou = CalcRotatedIou(rect1, rect2);
    return iou;
}
