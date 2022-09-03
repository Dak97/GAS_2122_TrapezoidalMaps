#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <cg3/geometry/segment2.h>
#include "../data_structures/dag.h"
#include "../data_structures/trapezoid.h"
#include "../data_structures/dagnode.h"

namespace Algorithm
{
    void buildTrapMapDag(Dag& dag, TrapMap& trapMap, const cg3::Segment2d& segment);
    DagNode* query(Dag& dag, const cg3::Segment2d& segment);
    std::vector<Trapezoid*> followSegment(const cg3::Segment2d &segment, DagNode* trap);

    bool pointIsAboveSegment(cg3::Segment2d segment, cg3::Point2d point);
    bool pointsAreEquals(const cg3::Point2d& p1, const cg3::Point2d& p2);

    void printNeigh(TrapMap trapMap);
    void printDag(Dag dag);
    void validateNeighbors(TrapMap trapMap);
}

#endif // ALGORITHM_H
