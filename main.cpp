#include <iostream>

#include "rotated_iou.hpp"


double CalcRotatedIou(const RotatedRect& rect1, const RotatedRect& rect2)
{
    double area1 = rect1.size.width * rect1.size.height;
    double area2 = rect2.size.width * rect2.size.height;
    if (area1 < 1e-14 || area2 < 1e-14) {
        return 0.0;
    }

    std::vector<Point2f> inter_points;
    rotatedRectangleIntersection(rect1, rect2, inter_points);
    if (!inter_points.empty()) {
        double inter_area = contourArea(inter_points, false);
        double union_area = area1 + area2 - inter_area;
        double iou = inter_area / union_area;
        return iou;
    } else {
        return 0.0;
    }
}

int main()
{
    RotatedRect rect1 = {Point2f{2, 2}, Size2f{4, 3}, 15};
    RotatedRect rect2 = {Point2f{2, 2}, Size2f{3, 4}, 45};

    double iou = CalcRotatedIou(rect1, rect2);
    std::cout << "IoU: " << iou << std::endl;

    return 0;
}
