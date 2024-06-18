/*

CRI = CalcRotatedIou

Base on OpenCV-4.9.0

Version:

    Author:      Li Shixian
    Create:      2024-05-09
    Last update: 2024-06-18

*/

#ifndef __CRI_HEADER__
#define __CRI_HEADER__

#ifdef __cplusplus
extern "C" {
#endif


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


#ifdef __cplusplus
}
#endif

#endif  /* __CRI_HEADER__ */
