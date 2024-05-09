/*
    CRI = CalcRotatedIou

    Author: Lishixian
    Date:   20240509
*/


#include "cri_header.hpp"


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

double CalcRotatedIouC(  // CalcRotatedIou C Interface
    float cx1, float cy1, float w1, float h1, float a1,  // rect1
    float cx2, float cy2, float w2, float h2, float a2   // rect2
) {
    RotatedRect rect1 = {Point2f{cx1, cy1}, Size2f{w1, h1}, a1};
    RotatedRect rect2 = {Point2f{cx2, cy2}, Size2f{w2, h2}, a2};

    double iou = CalcRotatedIou(rect1, rect2);
    return iou;
}
