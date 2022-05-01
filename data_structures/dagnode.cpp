#include "dagnode.h"

DagNode::DagNode(DagNode::TypeNode type, void *objj)
{
    data.type = type;
    data.objj = objj;
    left = nullptr;
    right = nullptr;
    parent = nullptr;
}

const DagNode::DagData &DagNode::getData() const
{
    return data;
}

void DagNode::setData(const DagData &newData)
{
    data = newData;
}
