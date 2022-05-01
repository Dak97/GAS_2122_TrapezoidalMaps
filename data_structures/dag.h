#ifndef DAG_H
#define DAG_H

#include "drawables/drawabletrapmap.h"
#include "data_structures/dagnode.h"
#include <cg3/geometry/segment.h>

class DAG
{
public:
    DAG(DrawableTrapMap& drawableTrapMap, Trapezoid *bb);

    void updateDag(std::vector<Trapezoid> traps, std::vector<DagNode*> trapNodes, const cg3::Segment2d& segment);
    DagNode* findTrapToModify(const cg3::Segment2d& segment);
    DagNode* createNewDagNode(DagNode::TypeNode typeObj, void *object);
    std::vector<DagNode*> query(const cg3::Segment2d segment);

    DagNode* getRoot() const;

private:
    DrawableTrapMap drawableTrapMap;
    DagNode *root;

    bool pointIsAbove(cg3::Segment2d* s, cg3::Point2d p);


};

#endif // DAG_H
