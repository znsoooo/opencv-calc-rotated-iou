/*
    CRI = CalcRotatedIou

    Author:      Li Shixian
    Date:        2024-05-09
    Last update: 2024-06-17
*/

#ifndef __CRI_HEADER__
#define __CRI_HEADER__

#include <math.h>
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


// User Interface

// C Interface
double CalcRotatedIouC(
    float cx1, float cy1, float w1, float h1, float a1,  // rect1
    float cx2, float cy2, float w2, float h2, float a2   // rect2
);

#endif  /* __CRI_HEADER__ */
