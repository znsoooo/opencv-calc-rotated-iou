#include <iostream>

#include "rotated_iou.hpp"


double CalcRotatedIou(const RotatedRect& r1, const RotatedRect& r2)
{
    double area1 = r1.size.width * r1.size.height;
    double area2 = r2.size.width * r2.size.height;
    if (area1 < 1e-14 || area2 < 1e-14) {
        return 0.0;
    }

    std::vector<Point2f> int_pts, order_pts;
    rotatedRectangleIntersection(r1, r2, int_pts);
    if (!int_pts.empty()) {
        convexHull(int_pts, order_pts, true);
        double intersec = contourArea(order_pts);
        double unionArea = area1 + area2 - intersec;
        double iou = intersec / unionArea;
        return iou;
    } else {
        return 0.0;
    }
}

int main()
{
    RotatedRect rect1 = {Point2f{2, 2}, Size2f{4, 2}, 15};
    RotatedRect rect2 = {Point2f{2, 2}, Size2f{3, 2}, 45};

    double iou = CalcRotatedIou(rect1, rect2);
    std::cout << "IoU: " << iou << std::endl;

    return 0;
}
