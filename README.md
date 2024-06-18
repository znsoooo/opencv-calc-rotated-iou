# Calc Rotated IoU

A pure C source program for calculating the IoU of two rotated rectangles, base on [opencv-4.9.0](https://github.com/opencv/opencv/tree/4.9.0).


## Introduction

### Constants

- **RectanglesIntersectTypes**:

  From: `modules\imgproc\include\opencv2\imgproc.hpp`


### Types

- **Rect, Size2f, Point2f, RotatedRect**:

  From: `modules\core\include\opencv2\core\types.hpp`


### Functions

- **Point2f_sub**:

  From: `modules\core\include\opencv2\core\types.hpp`

  See at: `Point_<_Tp> operator - (const Point_<_Tp>& a, const Point_<_Tp>& b)`

- **Point2f_cross**:

  From: `modules\core\include\opencv2\core\types.hpp`

  See at: `double Point_<_Tp>::cross(const Point_& pt)`

- **RotatedRect_points**:

  From: `modules\core\src\types.cpp`

- **contourArea**:

  From: `modules\imgproc\src\shapedescr.cpp`

- **normL2Sqr**:

  From: `modules\core\include\opencv2\core\base.hpp`

- **_isOnPositiveSide**:

  From: `modules\imgproc\src\intersection.cpp`

- **rotatedRectangleIntersection**:

  From: `modules\imgproc\src\intersection.cpp`

- **CalcRotatedIou**:

  From python source file

- **CalcRotatedIouC**:

  None


## License

- __Author:__  Li Shixian

- __QQ:__ 11313213

- __Email:__ <lsx7@sina.com>

- __GitHub:__ <https://github.com/znsoooo/opencv-calc-rotated-iou>

- __License:__ MIT License

