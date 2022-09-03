#ifndef DAG_H
#define DAG_H

#include "drawables/drawabletrapmap.h"
#include "data_structures/dagnode.h"
#include <cg3/geometry/segment.h>

class Dag
{
public:
    Dag(Trapezoid *bb);

    void init(Trapezoid *bb);
    DagNode* createNewDagNode(DagNode::TypeNode typeObj, void *object);
    DagNode* getRoot() const;
    void clearDag();

    void updateDagSingleSplit(std::vector<Trapezoid*>& traps, DagNode* trapNode, const cg3::Segment2d& segment, bool& left_coincident, bool& right_coincident);
    void updateDagMultipleSplit(std::vector<Trapezoid*>& newTraps, std::vector<Trapezoid*>& trapsToDelete, const cg3::Segment2d& segment, bool& left_coincident, bool& right_coincident);

private:
    DagNode *root;
    std::list<DagNode*> nodes;
};

#endif // DAG_H
