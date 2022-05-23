#ifndef DAG_H
#define DAG_H

#include "drawables/drawabletrapmap.h"
#include "data_structures/dagnode.h"
#include <cg3/geometry/segment.h>

class Dag
{
public:
    Dag(DrawableTrapMap& drawableTrapMap, Trapezoid *bb);

    void updateDag(std::vector<Trapezoid*> traps, DagNode* trapNode, const cg3::Segment2d& segment);
    void updateDag2(std::vector<Trapezoid*> newTraps, std::vector<Trapezoid*> trapsToDelete, const cg3::Segment2d& segment);
    bool isLeftNode(DagNode *node);
    DagNode* createNewDagNode(DagNode::TypeNode typeObj, void *object);
    std::pair<DagNode*, DagNode*> query(const cg3::Segment2d segment, bool &leftToRight);
    std::vector<Trapezoid*> followSegment(const cg3::Segment2d &segment, DagNode* trap);
    DagNode* getRoot() const;

private:
    DrawableTrapMap drawableTrapMap;
    DagNode *root;

    bool pointIsAbove(cg3::Segment2d* s, cg3::Point2d p);


};

#endif // DAG_H
