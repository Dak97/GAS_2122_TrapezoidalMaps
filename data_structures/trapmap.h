#ifndef TRAPMAP_H
#define TRAPMAP_H

#include <cg3/geometry/segment2.h>
#include <cg3/geometry/point2.h>
#include "data_structures/trapezoid.h"
#include <cg3/utilities/color.h>
class TrapMap
{
public:
    TrapMap(double boundingBox);

    void addFourTrapezoids(const cg3::Segment2d &segment);
    const Trapezoid getTrapezoid2() const;
    const Trapezoid getTrapezoid1() const;

    // restituisco una lista di trapezoidi
    const std::vector<Trapezoid> getTrapezoids() const;

    void clear();

private:
    const double boundingBox;

    // vettore di trapezoidi
    std::vector<Trapezoid> trapezoids;




};

#endif // TRAPMAP_H
