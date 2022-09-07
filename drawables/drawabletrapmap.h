#ifndef DRAWABLETRAPMAP_H
#define DRAWABLETRAPMAP_H

#include "data_structures/trapezoidalmap.h"

#include <cg3/viewer/interfaces/drawable_object.h>

#include <cg3/viewer/opengl_objects/opengl_objects2.h>

#include <cg3/geometry/bounding_box2.h>

#include <cg3/utilities/color.h>

class DrawableTrapezoidalMap : public TrapezoidalMap, public cg3::DrawableObject
{
public:

    DrawableTrapezoidalMap();

    void draw() const;
    cg3::Point3d sceneCenter() const;
    double sceneRadius() const;
    void highlightTrapezoid(Trapezoid* t) const;


};

#endif // DRAWABLETRAPMAP_H
