#include "trapmap.h"

#include <cg3/viewer/opengl_objects/opengl_objects2.h>

TrapMap::TrapMap(double boundingBox) :
    boundingBox(boundingBox)
{


}
const Trapezoid TrapMap::getTrapezoid2() const{
    return trapezoid2;
}

const Trapezoid TrapMap::getTrapezoid1() const{
    return trapezoid1;
}

void TrapMap::addFourTrapezoids(const cg3::Segment2d &segment){
    //creo il trapezoide centrale-alto
     trapezoid2.setSegmentDown(segment); // base del trapezoide

     cg3::Point2d s = cg3::Point2d(segment.p1().x(), boundingBox);
     cg3::Point2d t = cg3::Point2d(segment.p2().x(), boundingBox);
     cg3::Segment2d top = cg3::Segment2d(s, t);
     trapezoid2.setSegmentUp(top);

     trapezoid2.setSegmentLeft(cg3::Segment2d(segment.p1(), s));

     trapezoid2.setSegmentRight(cg3::Segment2d(segment.p2(),t));

     ///////////
     //left most bottom
     cg3::Point2d lmb = cg3::Point2d(-boundingBox,-boundingBox);
     cg3::Point2d m = cg3::Point2d(segment.p1().x(),-boundingBox);
     trapezoid1.setSegmentDown(cg3::Segment2d(lmb, m));

     cg3::Point2d lmt = cg3::Point2d(-boundingBox, boundingBox);
     trapezoid1.setSegmentUp(cg3::Segment2d(lmt, s));

     trapezoid1.setSegmentLeft(cg3::Segment2d(lmb, lmt));

     trapezoid1.setSegmentRight(cg3::Segment2d(m, s));







}
