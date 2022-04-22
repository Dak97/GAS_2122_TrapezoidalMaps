#ifndef TRAPMAP_H
#define TRAPMAP_H

#include <cg3/geometry/segment2.h>
#include <cg3/geometry/point2.h>
#include "data_structures/trapezoid.h"
class TrapMap
{
public:
    TrapMap(double boundingBox);

    void addFourTrapezoids(const cg3::Segment2d &segment);
    const Trapezoid getTrapezoid2() const;
    const Trapezoid getTrapezoid1() const;

private:
    const double boundingBox;
    // memorizzo solo un trapezoide per il momento
    Trapezoid trapezoid1;
    Trapezoid trapezoid2;
    Trapezoid trapezoid3;
    Trapezoid trapezoid4;




};

#endif // TRAPMAP_H
