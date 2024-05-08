import cv2

def CalcRotatedIou(r1, r2):
    area1 = r1[1][0] * r1[1][1]
    area2 = r2[1][0] * r2[1][1]
    if (area1 < 1e-14 or area2 < 1e-14):
        return 0

    int_pts = cv2.rotatedRectangleIntersection(r1, r2)[1]
    if int_pts is not None:
        order_pts = cv2.convexHull(int_pts, returnPoints=True)
        intersec = cv2.contourArea(order_pts)
        union = area1 + area2 - intersec
        iou = intersec * 1.0 / union
    else:
        iou = 0.0
    return iou

if __name__ == '__main__':
    rect1 = [[2, 2], [4, 3], 15]
    rect2 = [[2, 2], [3, 4], 45]

    iou = CalcRotatedIou(rect1, rect2)
    print(iou)
