#include "drawabletrapmap.h"


DrawableTrapMap::DrawableTrapMap(double boundingBox) :
    TrapMap(),
    bb(boundingBox)
{
    srand(time(NULL));
}

void DrawableTrapMap::draw() const
{
    if (getTrapezoids().size() > 1){
//        for (const Trapezoid& t : getTrapezoids()) {
//            cg3::opengl::drawQuad2(
//                        t.getSegmentUp().p1(),
//                        t.getSegmentUp().p2(),
//                        t.getSegmentDown().p2(),
//                        t.getSegmentDown().p1(),
//                        t.getColor(),
//                        1, false
//                        );

//        }
        for (const Trapezoid& t : getTrapezoids()) {
            cg3::opengl::drawQuad2(

                        t.getSegmentDown().p2(),
                        t.getSegmentDown().p1(),
                        t.getSegmentUp().p1(),
                        t.getSegmentUp().p2(),
                        t.getColor(),
                        1, false
                        );
        }
    }
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
