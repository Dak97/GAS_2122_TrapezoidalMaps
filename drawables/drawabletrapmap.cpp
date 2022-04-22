#include "drawabletrapmap.h"


DrawableTrapMap::DrawableTrapMap(double boundingBox) :
    TrapMap(boundingBox)
{

}

void DrawableTrapMap::draw() const
{
    Trapezoid t1 = getTrapezoid1();
    cg3::Color c = cg3::Color(0, 130, 30);
    cg3::opengl::drawQuad2(
                t1.getSegmentUp().p1(),
                t1.getSegmentUp().p2(),
                t1.getSegmentDown().p2(),
                t1.getSegmentDown().p1(),
                c, 1, true
                );

    Trapezoid t2 = getTrapezoid2();
    c = cg3::Color(50, 150, 50);
    cg3::opengl::drawQuad2(
                t2.getSegmentUp().p1(),
                t2.getSegmentUp().p2(),
                t2.getSegmentDown().p2(),
                t2.getSegmentDown().p1(),
                c, 1, true
                );
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
