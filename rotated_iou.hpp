#include <vector>

#define CV_PI 3.1415926535897932384626433832795


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
    Point2f center; //< the rectangle mass center
    Size2f size;    //< width and height of the rectangle
    float angle;    //< the rotation angle. When the angle is 0, 90, 180, 270 etc., the rectangle becomes an up-right rectangle.
} RotatedRect;

int rotatedRectangleIntersection(const RotatedRect& r1, const RotatedRect& r2, std::vector<Point2f>& int_pts);
int convexHull(const std::vector<Point2f>& int_pts, std::vector<Point2f>& order_pts, bool ret);
double contourArea(const std::vector<Point2f>& order_pts);

double CalcRotatedIou(const RotatedRect& r1, const RotatedRect& r2);
