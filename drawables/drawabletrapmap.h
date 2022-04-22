#ifndef DRAWABLETRAPMAP_H
#define DRAWABLETRAPMAP_H

#include "data_structures/trapmap.h"

#include <cg3/viewer/interfaces/drawable_object.h>

#include <cg3/viewer/opengl_objects/opengl_objects2.h>

#include <cg3/geometry/bounding_box2.h>

#include <cg3/utilities/color.h>

class DrawableTrapMap : public TrapMap, public cg3::DrawableObject
{
public:

    DrawableTrapMap(double boundingBox);

    // metodi da implementare dell'interfaccia drawableobject
    void draw() const;
    cg3::Point3d sceneCenter() const;
    double sceneRadius() const;



};

#endif // DRAWABLETRAPMAP_H
