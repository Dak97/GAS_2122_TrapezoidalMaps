#ifndef TRAPMAP_H
#define TRAPMAP_H

#include <cg3/geometry/segment2.h>
#include <cg3/geometry/point2.h>
#include "data_structures/trapezoid.h"
#include "data_structures/dagnode.h"
#include <cg3/utilities/color.h>
class TrapMap
{
public:
    TrapMap();

    std::vector<Trapezoid*> addFourTrapezoids(const cg3::Segment2d &segment, DagNode* bbNode, bool &leftToRight);

    // restituisco una lista di trapezoidi
    const std::list<Trapezoid> getTrapezoids() const;

    Trapezoid* getTrapezoidWithId(const int id);
    int getIndexWithId(const int id);

    void deleteTrapezoidWithId(const int id);



    void clear();

private:

    Trapezoid boundingBox;
    // vettore di trapezoidi
    std::list<Trapezoid> trapezoids;


    cg3::Point2d findIntersectionVerticalLine(const cg3::Segment2d& s, const cg3::Point2d& px, bool dir);


};

#endif // TRAPMAP_H
