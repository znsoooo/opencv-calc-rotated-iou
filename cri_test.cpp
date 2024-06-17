#include <iostream>

#include "cri_header.hpp"


void CalcRotatedIouTest()
{
    // C++ Test
    // RotatedRect rect1 = {Point2f{2, 2}, Size2f{3, 4}, 30};
    // RotatedRect rect2 = {Point2f{2, 2}, Size2f{4, 3}, 45};
    // double iou = CalcRotatedIou(rect1, rect2);

    // C Test
    double iou = CalcRotatedIouC(2, 2, 3, 4, 30, 2, 2, 4, 3, 45);

    std::cout << "IoU: " << iou << std::endl;
}

void CalcRotatedIouCTest(
    float cx1, float cy1, float w1, float h1, float a1,  // rect1
    float cx2, float cy2, float w2, float h2, float a2,  // rect2
    float target_iou)
{
    float eps = 1e-6f;
    double result_iou = CalcRotatedIouC(cx1, cy1, w1, h1, a1, cx2, cy2, w2, h2, a2);

    if (fabs(result_iou - target_iou) <= eps) {
        std::cout << "[ok] ";
    } else {
        std::cout << "[  ] ";
    }
    std::cout << "result_iou=" << result_iou << ", target_iou=" << target_iou << std::endl;
}

int main()
{
    CalcRotatedIouTest();
    std::cout << std::endl;

    CalcRotatedIouCTest(0.7332796620432873, -0.05568840561181787, 67.51173433276774, 246.00713715300787, 39.9780246754012, -0.7332796620432873, 0.05568840561193156, 67.99761628256269, 241.24936626902104, 39.9780246754012, 0.9510122124306679);
    CalcRotatedIouCTest(-0.0813426418162635, 0.7283508314894789, 67.51173433276774, 246.00713715300787, 39.9780246754012, 0.0813426418162635, -0.7283508314895926, 66.56789606676142, 243.27944055077467, 39.9780246754012, 0.9653177590722263);
    CalcRotatedIouCTest(0.5422028515382635, 0.738093948981259, 67.51173433276774, 246.00713715300787, 39.9780246754012, -0.5422028515382635, -0.7380939489813727, 66.57803299776467, 242.30900614120762, 39.9780246754012, 0.9343037990356925);
    CalcRotatedIouCTest(-0.36935843573553484, 0.19204347350910211, 67.51173433276774, 246.00713715300787, 44.87280691486428, 0.3693584357356485, -0.1920434735092158, 67.63694126492396, 246.66056901803572, 44.87280691486428, 0.9862020744131241);
    CalcRotatedIouCTest(-0.18579558772205473, 0.40167056422956193, 67.51173433276774, 246.00713715300787, 45.16990214781219, 0.18579558772205473, -0.40167056422956193, 67.64404357551444, 251.02037162224028, 45.16990214781219, 0.9712117752979831);
    CalcRotatedIouCTest(-0.3837024899174821, -0.660090581662871, 67.51173433276774, 246.00713715300787, 39.9780246754012, 0.38370248991736844, 0.6600905816627574, 68.06768748930007, 245.6132005420425, 39.9780246754012, 0.9545587248399087);
    CalcRotatedIouCTest(-0.2571200545086185, 0.14377921865320786, 67.51173433276774, 246.00713715300787, 44.6642689148143, 0.2571200545086185, -0.14377921865320786, 68.20027585102385, 245.81571681018272, 44.6642689148143, 0.985384592497348);
    CalcRotatedIouCTest(-950.7874544640327, -977.3935634366742, 67.51173433276774, 246.00713715300787, -42.15677132205311, 950.7874544640326, 977.393563436674, 22.95902236586232, 14.575749196401016, -42.15677132205311, 0.0);
    CalcRotatedIouCTest(-950.7124070554574, -977.4649040250622, 67.51173433276774, 246.00713715300787, -43.01804457937938, 950.7124070554573, 977.4649040250624, 23.298095266185623, 14.470075802064095, -43.01804457937938, 0.0);
    CalcRotatedIouCTest(-950.8483829294386, -977.3682144047536, 67.51173433276774, 246.00713715300787, -43.12570373654516, 950.8483829294385, 977.3682144047534, 22.828563824490516, 15.020157175603762, -43.12570373654516, 0.0);
    CalcRotatedIouCTest(-950.800326011035, -977.4015070274043, 67.51173433276774, 246.00713715300787, -42.08378206295765, 950.8003260110349, 977.4015070274045, 23.171098427121592, 15.232207104250742, -42.08378206295765, 0.0);
    CalcRotatedIouCTest(-0.34801751376755874, -0.28199016302164637, 67.51173433276774, 246.00713715300787, 39.9780246754012, 0.3480175137676724, 0.28199016302164637, 67.59897698567039, 245.40222311178047, 39.9780246754012, 0.971466887649223);
    CalcRotatedIouCTest(-0.10889948729970911, 0.25714891001337037, 67.51173433276774, 246.00713715300787, 45.38129683279436, 0.1088994872998228, -0.25714891001337037, 67.87502958790778, 247.9481889559484, 45.38129683279436, 0.9859481484169936);
    CalcRotatedIouCTest(-950.8832149798269, -977.3755961580011, 67.51173433276774, 246.00713715300787, -41.286374407339885, 950.8832149798268, 977.3755961580014, 22.562498087970162, 14.8496737490095, -41.286374407339885, 0.0);
    CalcRotatedIouCTest(-950.6276687099956, -977.5240433149227, 67.51173433276774, 246.00713715300787, -41.386734638596124, 950.6276687099955, 977.5240433149224, 22.79602637617699, 14.690685426643615, -41.386734638596124, 0.0);
    CalcRotatedIouCTest(-0.045101672427222184, -0.18968017930251335, 67.51173433276774, 246.00713715300787, 39.9780246754012, 0.04510167242733587, 0.18968017930251335, 65.85600872434466, 241.5334907226097, 39.9780246754012, 0.9577359653649447);
    CalcRotatedIouCTest(-353.0994511105388, -345.1027892789489, 67.51173433276774, 246.00713715300787, -40.281278619554094, 353.0994511105389, 345.1027892789489, 103.10752802735534, 17.751641358153968, -40.281278619554094, 0.0);
    CalcRotatedIouCTest(-950.5659045084004, -977.6952591785291, 67.51173433276774, 246.00713715300787, -39.999938715782626, 950.5659045084003, 977.6952591785293, 23.669978628426037, 14.536863528274306, -39.999938715782626, 0.0);
    CalcRotatedIouCTest(-1440.209713979399, -1440.1634662822182, 67.51173433276774, 246.00713715300787, -44.44863405765015, 1440.209713979399, 1440.163466282218, 11.947682490023062, 8.025603977260879, -44.44863405765015, 0.0);
    CalcRotatedIouCTest(-352.9186930474083, -345.3492018588794, 67.51173433276774, 246.00713715300787, -44.22601681740903, 352.9186930474084, 345.3492018588794, 99.45657715455707, 18.086902934469734, -44.22601681740903, 0.0);
    CalcRotatedIouCTest(-352.48614981553453, -344.6197606243278, 67.51173433276774, 246.00713715300787, -40.281278619554094, 352.4861498155344, 344.6197606243278, 102.42877922722968, 17.941452533427658, -40.281278619554094, 0.0);
    CalcRotatedIouCTest(-1439.9926289896066, -1439.885956923489, 67.51173433276774, 246.00713715300787, -37.0566468427587, 1439.9926289896066, 1439.8859569234887, 11.890944362523623, 9.464729348820155, -37.0566468427587, 0.0);
    CalcRotatedIouCTest(-352.75563374469823, -345.0522211769612, 67.51173433276774, 246.00713715300787, -45.058094371097134, 352.7556337446981, 345.0522211769612, 99.94307320382913, 18.145697565978924, -45.058094371097134, 0.0);
    CalcRotatedIouCTest(-353.06362410968484, -344.9780174968936, 67.51173433276774, 246.00713715300787, -45.510627777488914, 353.0636241096847, 344.9780174968936, 100.5178957387779, 18.15109196258281, -45.510627777488914, 0.0);
    CalcRotatedIouCTest(0.07504740857530123, -0.07134058838801138, 22.95902236586232, 14.575749196401016, -43.01804457937938, -0.07504740857530123, 0.07134058838846613, 23.298095266185623, 14.470075802064095, -43.01804457937938, 0.9752443280148029);
    CalcRotatedIouCTest(-0.06092846540559549, 0.025349031920541165, 22.95902236586232, 14.575749196401016, -43.12570373654516, 0.060928465406050236, -0.025349031920541165, 22.828563824490516, 15.020157175603762, -43.12570373654516, 0.9602241253754157);
    CalcRotatedIouCTest(-0.012871547002305306, -0.007943590730064898, 22.95902236586232, 14.575749196401016, -42.08378206295765, 0.012871547002305306, 0.007943590730519645, 23.171098427121592, 15.232207104250742, -42.08378206295765, 0.9481451742699717);
    CalcRotatedIouCTest(-0.09576051579415434, 0.017967278672585962, 22.95902236586232, 14.575749196401016, -41.286374407339885, 0.09576051579415434, -0.01796727867304071, 22.562498087970162, 14.8496737490095, -41.286374407339885, 0.964908404708058);
    CalcRotatedIouCTest(0.15978575403732975, -0.1304798782484795, 22.95902236586232, 14.575749196401016, -41.386734638596124, -0.159785754036875, 0.1304798782484795, 22.79602637617699, 14.690685426643615, -41.386734638596124, 0.9572066888443568);
    CalcRotatedIouCTest(597.688003353494, 632.2907741577251, 22.95902236586232, 14.575749196401016, -40.281278619554094, -597.6880033534937, -632.2907741577253, 103.10752802735534, 17.751641358153968, -40.281278619554094, 0.0);
    CalcRotatedIouCTest(0.2215499556323266, -0.3016957418549282, 22.95902236586232, 14.575749196401016, -39.999938715782626, -0.2215499556323266, 0.30169574185538295, 23.669978628426037, 14.536863528274306, -39.999938715782626, 0.9175779006480643);
    CalcRotatedIouCTest(-489.4222595153665, -462.76990284554404, 22.95902236586232, 14.575749196401016, -44.44863405765015, 489.4222595153665, 462.76990284554404, 11.947682490023062, 8.025603977260879, -44.44863405765015, 0.0);
    CalcRotatedIouCTest(597.8687614166242, 632.0443615777947, 22.95902236586232, 14.575749196401016, -44.22601681740903, -597.8687614166245, -632.0443615777947, 99.45657715455707, 18.086902934469734, -44.22601681740903, 0.0);
    CalcRotatedIouCTest(598.3013046484982, 632.7738028123463, 22.95902236586232, 14.575749196401016, -40.281278619554094, -598.3013046484982, -632.7738028123463, 102.42877922722968, 17.941452533427658, -40.281278619554094, 0.0);
    CalcRotatedIouCTest(-489.20517452557397, -462.49239348681476, 22.95902236586232, 14.575749196401016, -37.0566468427587, 489.20517452557397, 462.49239348681476, 11.890944362523623, 9.464729348820155, -37.0566468427587, 0.0);
    CalcRotatedIouCTest(598.0318207193345, 632.3413422597127, 22.95902236586232, 14.575749196401016, -45.058094371097134, -598.0318207193345, -632.341342259713, 99.94307320382913, 18.145697565978924, -45.058094371097134, 0.0);
    CalcRotatedIouCTest(597.7238303543479, 632.4155459397805, 22.95902236586232, 14.575749196401016, -45.510627777488914, -597.7238303543479, -632.4155459397805, 100.5178957387779, 18.15109196258281, -45.510627777488914, 0.0);
    CalcRotatedIouCTest(-1087.1102628688602, -1095.0606770032693, 103.10752802735534, 17.751641358153968, -44.44863405765015, 1087.1102628688604, 1095.0606770032691, 11.947682490023062, 8.025603977260879, -44.44863405765015, 0.0);
    CalcRotatedIouCTest(0.18075806313049725, -0.2464125799303929, 103.10752802735534, 17.751641358153968, -44.22601681740903, -0.18075806313049725, 0.24641257993062027, 99.45657715455707, 18.086902934469734, -44.22601681740903, 0.9473331093539022);
    CalcRotatedIouCTest(0.6133012950042485, 0.48302865462096634, 103.10752802735534, 17.751641358153968, -40.281278619554094, -0.6133012950044758, -0.4830286546211937, 102.42877922722968, 17.941452533427658, -40.281278619554094, 0.836992931658217);
    CalcRotatedIouCTest(-1086.8931778790677, -1094.78316764454, 103.10752802735534, 17.751641358153968, -37.0566468427587, 1086.893177879068, 1094.7831676445398, 11.890944362523623, 9.464729348820155, -37.0566468427587, 0.0);
    CalcRotatedIouCTest(0.3438173658407777, 0.050568101987664704, 103.10752802735534, 17.751641358153968, -45.058094371097134, -0.34381736584055034, -0.050568101987664704, 99.94307320382913, 18.145697565978924, -45.058094371097134, 0.9119829379879886);
    CalcRotatedIouCTest(0.03582700085394208, 0.1247717820554044, 103.10752802735534, 17.751641358153968, -45.510627777488914, -0.035827000854169455, -0.12477178205517703, 100.5178957387779, 18.15109196258281, -45.510627777488914, 0.9512030300730651);
    CalcRotatedIouCTest(0.2170849897925109, 0.27750935872927585, 11.947682490023062, 8.025603977260879, -37.0566468427587, -0.2170849897925109, -0.27750935872927585, 11.890944362523623, 9.464729348820155, -37.0566468427587, 0.8445315498178237);

    std::cout << "==================" << std::endl;
    std::cout << "ALL TESTS FINISHED" << std::endl;
    std::cout << "==================" << std::endl;

    return 0;
}
