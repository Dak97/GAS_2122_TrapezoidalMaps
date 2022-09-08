#include "drawabletrapmap.h"
#include "algorithms/algorithm.h"
DrawableTrapezoidalMap::DrawableTrapezoidalMap()
{
    srand(time(NULL));
}

void DrawableTrapezoidalMap::draw() const
{

        for (const Trapezoid& t : getTrapezoids()) {
            if (Algorithm::pointsAreEquals(t.getSegmentDown().p1(), t.getSegmentUp().p1())){
                cg3::opengl::drawTriangle2(
                            t.getSegmentDown().p1(),
                            t.getSegmentUp().p2(),
                            t.getSegmentDown().p2(),
                            ((t.getHighlighted()) ? cg3::Color(10,10,10) : t.getColor()),
                            1, true);
            }
            else if (Algorithm::pointsAreEquals(t.getSegmentDown().p2(), t.getSegmentUp().p2())){
                cg3::opengl::drawTriangle2(
                            t.getSegmentDown().p1(),
                            t.getSegmentUp().p1(),
                            t.getSegmentDown().p2(),
                            ((t.getHighlighted()) ? cg3::Color(10,10,10) : t.getColor()),
                            1, true);
            }else{
                cg3::opengl::drawQuad2(
                            t.getSegmentDown().p2(),
                            t.getSegmentDown().p1(),
                            t.getSegmentUp().p1(),
                            t.getSegmentUp().p2(),
                            ((t.getHighlighted()) ? cg3::Color(10,10,10) : t.getColor()),
                            1, true);
            }
        }

}

cg3::Point3d DrawableTrapezoidalMap::sceneCenter() const
{
    const cg3::BoundingBox2& boundingBox = cg3::BoundingBox2();
    return cg3::Point3d(boundingBox.center().x(), boundingBox.center().y(), 0);
}

double DrawableTrapezoidalMap::sceneRadius() const
{
    const cg3::BoundingBox2& boundingBox = cg3::BoundingBox2();
    return boundingBox.diag();
}
void DrawableTrapezoidalMap::highlightTrapezoid(Trapezoid* t) const{
    t->setHighlighted(true);
}
