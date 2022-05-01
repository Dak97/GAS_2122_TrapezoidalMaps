#include "drawabletrapmap.h"


DrawableTrapMap::DrawableTrapMap(double boundingBox) :
    TrapMap(),
    bb(boundingBox)
{
    srand(time(NULL));
}

void DrawableTrapMap::draw() const
{

    for (const Trapezoid& t : getTrapezoids()) {

        cg3::opengl::drawQuad2(
                    t.getSegmentUp().p1(),
                    t.getSegmentUp().p2(),
                    t.getSegmentDown().p2(),
                    t.getSegmentDown().p1(),
                    t.getColor(),
                    1, true
                    );
    }
//if (getTrapezoids().size() > 0){

//    for(int i = getTrapezoids().size(); i >=0; i--) {
//        std::cout << i << std::endl;
//        Trapezoid t = getTrapezoids()[i];

//        cg3::opengl::drawQuad2(
//                    t.getSegmentUp().p1(),
//                    t.getSegmentUp().p2(),
//                    t.getSegmentDown().p2(),
//                    t.getSegmentDown().p1(),
//                    t.getColor(),
//                    1, true
//                    );
//     }
//}

//cg3::opengl::drawQuad2(cg3::Point2d(-bb+100000,bb),
//                       cg3::Point2d(bb,bb),
//                       cg3::Point2d(bb,-bb),
//                       cg3::Point2d(-bb+100000,-bb),
//                       cg3::Color(255,0,0), 1, true);




}

cg3::Point3d DrawableTrapMap::sceneCenter() const
{
    const cg3::BoundingBox2& boundingBox = cg3::BoundingBox2();
    return cg3::Point3d(boundingBox.center().x(), boundingBox.center().y(), 0);
}

double DrawableTrapMap::sceneRadius() const
{
    const cg3::BoundingBox2& boundingBox = cg3::BoundingBox2();
    return boundingBox.diag();
}
