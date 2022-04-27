#include "trapmap.h"

#include <cg3/viewer/opengl_objects/opengl_objects2.h>

TrapMap::TrapMap(double boundingBox) :
    boundingBox(boundingBox)
{


}



const std::vector<Trapezoid> TrapMap::getTrapezoids() const{
    return trapezoids;
}

void TrapMap::clear(){
    trapezoids.clear();
}

void TrapMap::addFourTrapezoids(const cg3::Segment2d &segment){

    cg3::Segment2d topS, bottomS;
    cg3::Color colorT;


    // left trapezoid

    topS = cg3::Segment2d(cg3::Point2d(-boundingBox,boundingBox),
                         cg3::Point2d(segment.p1().x(),boundingBox));

    bottomS = cg3::Segment2d(cg3::Point2d(-boundingBox,-boundingBox),
                           cg3::Point2d(segment.p1().x(),-boundingBox));

    colorT = cg3::Color(rand()%256, rand()%256, rand()%256);
    Trapezoid left = Trapezoid(topS, bottomS, colorT);

    // top trapezoid

    topS = cg3::Segment2d(cg3::Point2d(segment.p1().x(), boundingBox),
                          cg3::Point2d(segment.p2().x(), boundingBox));

    bottomS = segment;

    colorT = cg3::Color(rand()%256, rand()%256, rand()%256);

    Trapezoid top = Trapezoid(topS, bottomS, colorT);

    // bottom trapezoid

    topS = segment;

    bottomS = cg3::Segment2d(cg3::Point2d(segment.p1().x(), -boundingBox),
                             cg3::Point2d(segment.p2().x(), -boundingBox));;

    colorT = cg3::Color(rand()%256, rand()%256, rand()%256);
    Trapezoid bottom = Trapezoid(topS, bottomS, colorT);

//     right trapezoid

    topS = cg3::Segment2d(cg3::Point2d(segment.p2().x(),boundingBox),
                          cg3::Point2d(boundingBox,boundingBox));

    bottomS = cg3::Segment2d(cg3::Point2d(segment.p2().x(),-boundingBox),
                             cg3::Point2d(boundingBox,-boundingBox));
    colorT = cg3::Color(rand()%256, rand()%256, rand()%256);

    Trapezoid right = Trapezoid(topS, bottomS, colorT);

    // add new trapezoids
    trapezoids.push_back(left);
    trapezoids.push_back(top);
    trapezoids.push_back(bottom);
    trapezoids.push_back(right);



}
