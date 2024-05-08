#include <math.h>
#include "rotated_iou.hpp"


// area of a whole sequence
double contourArea( const std::vector<Point2f>& contour, bool oriented )
{
    int npoints = contour.size();
    if( npoints == 0 )
        return 0.;

    double a00 = 0;
    Point2f prev = contour[npoints-1];

    for( int i = 0; i < npoints; i++ )
    {
        Point2f p = contour[i];
        a00 += (double)prev.x * p.y - (double)prev.y * p.x;
        prev = p;
    }

    a00 *= 0.5;
    if( !oriented )
        a00 = fabs(a00);

    return a00;
}
