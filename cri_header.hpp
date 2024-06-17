/*
    CRI = CalcRotatedIou

    Author: Lishixian
    Date:   20240509
*/

#ifndef __CRI_HEADER__
#define __CRI_HEADER__

#include <math.h>
#include <vector>
#include <algorithm>

#define CV_PI 3.1415926535897932384626433832795

#define CV_Assert(...)
#define CV_INSTRUMENT_REGION()

//! types of intersection between rectangles
enum RectanglesIntersectTypes {
    INTERSECT_NONE     = 0, //!< No intersection
    INTERSECT_PARTIAL  = 1, //!< There is a partial intersection
    INTERSECT_FULL     = 2  //!< One of the rectangle is fully enclosed in the other
};


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


// User Interface

// C++ Interface
double CalcRotatedIou(const RotatedRect& rect1, const RotatedRect& rect2);

// C Interface
double CalcRotatedIouC(
    float cx1, float cy1, float w1, float h1, float a1,  // rect1
    float cx2, float cy2, float w2, float h2, float a2   // rect2
);

#endif  /* __CRI_HEADER__ */
