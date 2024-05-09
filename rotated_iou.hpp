#include <vector>

#define CV_PI 3.1415926535897932384626433832795

#define CV_Assert(...)
#define CV_INSTRUMENT_REGION()


typedef struct {
    int x, y, width, height;
} Rect;

typedef struct {
    float width, height;
} Size2f;

class Point2f
{
public:
    Point2f() = default;
    Point2f(float _x, float _y) : x(_x),  y(_y) {}

    float x = 0.f;
    float y = 0.f;

    Point2f operator-(const Point2f& other) const {
        return Point2f(x - other.x, y - other.y);
    }

    float cross(const Point2f& other) const {
        return x * other.y - y * other.x;
    }
};

class RotatedRect
{
public:
    Point2f center;
    Size2f size;
    float angle;

    void points(Point2f pts[]) const;
    void points(std::vector<Point2f>& pts) const;
};

int rotatedRectangleIntersection(const RotatedRect& rect1, const RotatedRect& rect2, std::vector<Point2f>& intersection);
double contourArea(const std::vector<Point2f>& contour, bool oriented);

double CalcRotatedIou(const RotatedRect& rect1, const RotatedRect& rect2);


//! types of intersection between rectangles
enum RectanglesIntersectTypes {
    INTERSECT_NONE = 0, //!< No intersection
    INTERSECT_PARTIAL  = 1, //!< There is a partial intersection
    INTERSECT_FULL  = 2 //!< One of the rectangle is fully enclosed in the other
};
