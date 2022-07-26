#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <cg3/geometry/segment2.h>
#include "../data_structures/dag.h"
#include "../data_structures/trapezoid.h"
#include "../data_structures/dagnode.h"

namespace Algorithm
{
    void buildTrapMapDag(Dag& dag, TrapMap& trapMap, const cg3::Segment2d& segment);
    bool pointIsAboveSegment(cg3::Segment2d segment, cg3::Point2d point);
    std::pair<DagNode*, DagNode*> queryPair(Dag& dag, const cg3::Segment2d& segment);
    DagNode* query(Dag& dag, const cg3::Point2d& point);
    void printNeigh(TrapMap trapMap);
    void printDag(Dag dag);
}

#endif // ALGORITHM_H
