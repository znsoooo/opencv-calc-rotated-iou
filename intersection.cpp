#include <math.h>
#include "rotated_iou.hpp"


void RotatedRect::points(Point2f pt[]) const
{
    double _angle = angle*CV_PI/180.;
    float b = (float)cos(_angle)*0.5f;
    float a = (float)sin(_angle)*0.5f;

    pt[0].x = center.x - a*size.height - b*size.width;
    pt[0].y = center.y + b*size.height - a*size.width;
    pt[1].x = center.x + a*size.height - b*size.width;
    pt[1].y = center.y - b*size.height - a*size.width;
    pt[2].x = 2*center.x - pt[0].x;
    pt[2].y = 2*center.y - pt[0].y;
    pt[3].x = 2*center.x - pt[1].x;
    pt[3].y = 2*center.y - pt[1].y;
}

float normL2Sqr(Point2f pt)
{
    return pt.x * pt.x + pt.y * pt.y;
}

static inline bool _isOnPositiveSide(const Point2f& line_vec, const Point2f& line_pt, const Point2f& pt)
{
    //we are interested by the cross product between the line vector (line_vec) and the line-to-pt vector (pt-line_pt)
    //the sign of the only non-null component of the result determining which side of the line 'pt' is on
    //the "positive" side meaning depends on the context usage of the current function and how line_vec and line_pt were filled
    return (line_vec.y*(line_pt.x-pt.x) >= line_vec.x*(line_pt.y-pt.y));
}

int rotatedRectangleIntersection( const RotatedRect& rect1, const RotatedRect& rect2, std::vector<Point2f> &intersection )
{
    CV_INSTRUMENT_REGION();

    Point2f vec1[4], vec2[4];
    Point2f pts1[4], pts2[4];

    rect1.points(pts1);
    rect2.points(pts2);

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
            intersection.resize(4);

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
        samePointEps = std::min(samePointEps, std::sqrt(vec1[i].x*vec1[i].x+vec1[i].y*vec1[i].y));
        samePointEps = std::min(samePointEps, std::sqrt(vec2[i].x*vec2[i].x+vec2[i].y*vec2[i].y));
    }
    samePointEps = std::max(1e-16f, samePointEps);

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
            if (std::abs(det) < 1e-12)//we consider accuracy around 1e-6, i.e. 1e-12 when squared
              continue;
            const float detInvScaled = 1.f/det;

            const float t1 = (vx2*y21 - vy2*x21)*detInvScaled;
            const float t2 = (vx1*y21 - vy1*x21)*detInvScaled;

            // This takes care of parallel lines
            if( std::isinf(t1) || std::isinf(t2) || std::isnan(t1) || std::isnan(t2) )
            {
                continue;
            }

            if( t1 >= 0.0f && t1 <= 1.0f && t2 >= 0.0f && t2 <= 1.0f )
            {
                const float xi = pts1[i].x + vec1[i].x*t1;
                const float yi = pts1[i].y + vec1[i].y*t1;

                intersection.push_back(Point2f(xi,yi));
            }
        }
    }

    if( !intersection.empty() )
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

        const Point2f& pt = pts1[i];

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
            intersection.push_back(pts1[i]);
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

        const Point2f& pt = pts2[i];

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
            intersection.push_back(pts2[i]);
        }
    }

    int N = (int)intersection.size();
    if (N == 0)
    {
        return INTERSECT_NONE;
    }

    // Get rid of duplicated points
    const int Nstride = N;
    std::vector<float> distPt(N * N);
    std::vector<int> ptDistRemap(N);
    for (int i = 0; i < N; ++i)
    {
        const Point2f pt0 = intersection[i];
        ptDistRemap[i] = i;
        for (int j = i + 1; j < N; )
        {
            const Point2f pt1 = intersection[j];
            const float d2 = normL2Sqr(pt1 - pt0);
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
            const float* pDist = distPt.data() + Nstride * ptDistRemap[i];
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
        Point2f diffI = intersection[i + 1] - intersection[i];
        for (int j = i + 2; j < N; ++j)
        {
            Point2f diffJ = intersection[j] - intersection[i];
            if (diffI.cross(diffJ) < 0)
            {
                std::swap(intersection[i + 1], intersection[j]);
                diffI = diffJ;
            }
        }
    }

    intersection.resize(N);

    return ret;
}
