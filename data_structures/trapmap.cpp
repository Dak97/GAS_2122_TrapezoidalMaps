#include "trapmap.h"

#include <cg3/viewer/opengl_objects/opengl_objects2.h>

TrapMap::TrapMap() : boundingBox(Trapezoid())
{
    trapezoids.push_back(boundingBox);
}

const std::vector<Trapezoid> TrapMap::getTrapezoids() const{
    return trapezoids;
}
Trapezoid& TrapMap::getTrapezoidWithId(const int id){
    return trapezoids.at(id);
}

void TrapMap::deleteTrapezoidWithId(const int id){
   if (trapezoids.size() == 1) {
       trapezoids.pop_back();
       return;
   }

   for (size_t i=0; i <= trapezoids.size() - 1; i++){
       if (trapezoids[i].getId() == id){
           trapezoids.erase(trapezoids.begin() + i);
           return;
       }
   }
}
void TrapMap::clear(){
    trapezoids.clear();
}

// devo trovare il punto di intersezione tra il segmento e la linea verticale
// rappresentata dalla sua coordinata x
cg3::Point2d TrapMap::findIntersectionVerticalLine(const cg3::Segment2d& s, const cg3::Point2d& px){
    cg3::Point2d p1 = s.p1();
    cg3::Point2d p2 = s.p2();
    cg3::Point2d intersection;

    double a, b, c, y;

    a = p1.y() - p2.y();
    b = p2.x() - p1.x();
    c = ((p1.x() * p2.y()) - (p2.x() * p1.y()));

    y = (-c -(a * px.x())) / b;

    return cg3::Point2d(px.x(), y);
}

std::vector<Trapezoid> TrapMap::addFourTrapezoids(const cg3::Segment2d &segment, Trapezoid bb){

    cg3::Segment2d topS, bottomS;
    cg3::Point2d leftP, rightP;
    cg3::Color colorT;
    std::vector<Trapezoid> trapForDag;

    int idLastTrap = trapezoids.back().getId();


    // left trapezoid

    topS = cg3::Segment2d(bb.getSegmentUp().p1(),
                         findIntersectionVerticalLine(bb.getSegmentUp(), segment.p1()));

    bottomS = cg3::Segment2d(bb.getSegmentDown().p1(),
                           findIntersectionVerticalLine(bb.getSegmentDown(), segment.p1()));

    leftP = topS.p1();
    rightP = segment.p1();

    colorT = cg3::Color(rand()%256, rand()%256, rand()%256);

    Trapezoid left = Trapezoid(++idLastTrap, topS, bottomS, leftP, rightP, colorT);

    // top trapezoid

    topS = cg3::Segment2d(findIntersectionVerticalLine(bb.getSegmentUp(), segment.p1()),
                          findIntersectionVerticalLine(bb.getSegmentUp(), segment.p2()));

    bottomS = segment;

    leftP = segment.p1();
    rightP = segment.p2();

    colorT = cg3::Color(rand()%256, rand()%256, rand()%256);

    Trapezoid top = Trapezoid(++idLastTrap, topS, bottomS, leftP, rightP, colorT);

    // bottom trapezoid

    topS = segment;

    bottomS = cg3::Segment2d(findIntersectionVerticalLine(bb.getSegmentDown(), segment.p1()),
                             findIntersectionVerticalLine(bb.getSegmentDown(), segment.p2()));

    leftP = segment.p1();
    rightP = segment.p2();

    colorT = cg3::Color(rand()%256, rand()%256, rand()%256);
    Trapezoid bottom = Trapezoid(++idLastTrap, topS, bottomS, leftP, rightP, colorT);

//     right trapezoid

    topS = cg3::Segment2d(findIntersectionVerticalLine(bb.getSegmentUp(),segment.p2()),
                          bb.getSegmentUp().p2());

    bottomS = cg3::Segment2d(findIntersectionVerticalLine(bb.getSegmentDown(),segment.p2()),
                             bb.getSegmentDown().p2());

    leftP = segment.p2();
    rightP = topS.p2();

    colorT = cg3::Color(rand()%256, rand()%256, rand()%256);

    Trapezoid right = Trapezoid(++idLastTrap, topS, bottomS, leftP, rightP, colorT);

    // elimino il trapezoide che é il bb
    deleteTrapezoidWithId(bb.getId());

    // add new trapezoids
    trapezoids.push_back(left);
    trapezoids.push_back(top);
    trapezoids.push_back(bottom);
    trapezoids.push_back(right);

    trapForDag.push_back(left);
    trapForDag.push_back(top);
    trapForDag.push_back(bottom);
    trapForDag.push_back(right);

    return trapForDag;
}
