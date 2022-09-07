#include "dagnode.h"
/**
 * @brief DagNode::DagNode Default constructor of a DAG node
 * @param type the type of object
 * @param objj a reference of the object
 */
DagNode::DagNode(DagNode::TypeNode type, void *objj)
{
    data.type = type;
    data.obj = objj;
    left = nullptr;
    right = nullptr;
}
/**
 * @brief DagNode::~DagNode Default deconstructor
 */
DagNode::~DagNode(){}
/**
 * @brief DagNode::getData return the information of a node
 * @return Dag Data object
 */
const DagNode::DagData &DagNode::getData() const
{
    return data;
}
/**
 * @brief DagNode::setObject setting a object reference inside a dag node
 * @param obj the object reference
 */
void DagNode::setObject(void* obj){
    data.obj = obj;
}
/**
 * @brief DagNode::setData setting dag data inside a node
 * @param newData the dag data
 */
void DagNode::setData(const DagData &newData)
{
    data = newData;
}
