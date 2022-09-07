#include "dag.h"
#include "algorithms/algorithm.h"

/**
 * @brief Dag::Dag Default constructor for the DAG data structure
 * @param bb First trapezoid inserted in the DAG (root)
 */
Dag::Dag(Trapezoid *bb) : root(createNewDagNode(DagNode::TypeNode::Trapezoid, bb)){
    nodes.push_back(root);
}
/**
 * @brief Dag::init Initialization of the DAG
 * @param bb First trapezoid inserted in the DAG (root)
 */
void Dag::init(Trapezoid *bb){
    root = createNewDagNode(DagNode::TypeNode::Trapezoid, bb);
    nodes.push_back(root);
}
/**
 * @brief Dag::createNewDagNode Creation of a DAG node
 * @param typeObj Type of the object that the node will contain
 * @param object A reference to the object
 * @return DAG node contatining the object
 */
DagNode* Dag::createNewDagNode(DagNode::TypeNode typeObj, void *object){
    DagNode *newNode = new DagNode(typeObj, object);
    return newNode;
}

/**
 * @brief Dag::getRoot Return a reference of the root of the DAG
 * @return  A DAG node
 */
DagNode* Dag::getRoot() const {
    return root;
}

/**
 * @brief Dag::clearDag Free all the objects that are not trapezoid in all nodes
 */
void Dag::clearDag(){
    for(DagNode *n: nodes){
        if (n->getData().type != DagNode::TypeNode::Trapezoid){
            free(n->getData().obj); // free memory object
            free(n); // free dag node
        }
    }
    nodes.clear(); // delete all the nodes
}
/**
 * @brief Dag::updateDagSingleSplit
 * @param traps
 * @param trapNode
 * @param segment
 * @param left_coincident
 * @param right_coincident
 */
void Dag::updateDagSingleSplit(std::vector<Trapezoid*>& traps, DagNode* trapNode, const cg3::Segment2d& segment, bool& left_coincident, bool& right_coincident){

    cg3::Point2d p1 = segment.p1();
    cg3::Point2d q1 = segment.p2();
    DagNode::DagData data;

    DagNode *tmp = trapNode;

    // case when the segment is not left and right coincident
    if (!left_coincident && !right_coincident){
        data.type = DagNode::TypeNode::Left;
        data.obj = new cg3::Point2d(p1);

        tmp->setData(data);

        tmp->left = new DagNode(DagNode::TypeNode::Trapezoid, traps.at(0)); // insert left trapezoid
        traps.at(0)->setRefToDag(tmp->left); // setting ref to dag node
        nodes.push_back(tmp->left);

        tmp->right = new DagNode(DagNode::TypeNode::Right, new cg3::Point2d(q1)); // insert q1
        nodes.push_back(tmp->right);

        tmp = tmp->right;

        tmp->left = new DagNode(DagNode::TypeNode::Segment, new cg3::Segment2d(p1,q1)); // insert segment
        nodes.push_back(tmp->left);

        tmp->right = new DagNode(DagNode::TypeNode::Trapezoid, traps.at(3)); // insert right trapezoid
        traps.at(3)->setRefToDag(tmp->right);
        nodes.push_back(tmp->right);

        tmp = tmp->left;

        tmp->left = new DagNode(DagNode::TypeNode::Trapezoid, traps.at(1)); // insert top trapeozoid
        traps.at(1)->setRefToDag(tmp->left);
        nodes.push_back(tmp->left);


        tmp->right = new DagNode(DagNode::TypeNode::Trapezoid, traps.at(2)); // insert bottom trapezoid
        traps.at(2)->setRefToDag(tmp->right);
        nodes.push_back(tmp->right);

    }else if (left_coincident && right_coincident){ // case when the segment is both left and right coincident
        data.type = DagNode::TypeNode::Segment;
        data.obj = new cg3::Segment2d(p1, q1);
        tmp->setData(data);

        tmp->left = new DagNode(DagNode::TypeNode::Trapezoid, traps.at(0));
        traps.at(0)->setRefToDag(tmp->left);
        nodes.push_back(tmp->left);

        tmp->right = new DagNode(DagNode::TypeNode::Trapezoid, traps.at(1));
        traps.at(1)->setRefToDag(tmp->right);
        nodes.push_back(tmp->right);

    }else if (left_coincident){ // case when it is only left coincident
        data.type = DagNode::TypeNode::Right;
        data.obj = new cg3::Point2d(q1);
        tmp->setData(data);

        tmp->right = new DagNode(DagNode::TypeNode::Trapezoid, traps.at(2));
        traps.at(2)->setRefToDag(tmp->right);
        nodes.push_back(tmp->right);

        tmp->left = new DagNode(DagNode::TypeNode::Segment, new cg3::Segment2d(p1,q1));
        nodes.push_back(tmp->left);

        tmp = tmp->left;

        tmp->left = new DagNode(DagNode::TypeNode::Trapezoid, traps.at(0));
        traps.at(0)->setRefToDag(tmp->left);
        nodes.push_back(tmp->left);

        tmp->right = new DagNode(DagNode::TypeNode::Trapezoid, traps.at(1));
        traps.at(1)->setRefToDag(tmp->right);
        nodes.push_back(tmp->right);

    }else {
        // whene it is only right coincident
        data.type = DagNode::TypeNode::Left;
        data.obj = new cg3::Point2d(p1);
        tmp->setData(data);

        tmp->left = new DagNode(DagNode::TypeNode::Trapezoid, traps.at(0));
        traps.at(0)->setRefToDag(tmp->left);
        nodes.push_back(tmp->left);

        tmp->right = new DagNode(DagNode::TypeNode::Segment, new cg3::Segment2d(p1,q1));
        nodes.push_back(tmp->right);

        tmp = tmp->right;

        tmp->left = new DagNode(DagNode::TypeNode::Trapezoid, traps.at(1));
        traps.at(1)->setRefToDag(tmp->left);
        nodes.push_back(tmp->left);

        tmp->right = new DagNode(DagNode::TypeNode::Trapezoid, traps.at(2));
        traps.at(2)->setRefToDag(tmp->right);
        nodes.push_back(tmp->right);
    }
}
void Dag::updateDagMultipleSplit(std::vector<Trapezoid*>& newTraps, std::vector<Trapezoid*>& trapsToDelete, const cg3::Segment2d& segment, bool& left_coincident, bool& right_coincident){
    DagNode *nodeToSubstitute, *tmp, *trapShared;
    int j=0;
    DagNode::DagData data;
    Trapezoid *oldTrap;

    // for eache trapezoid crossed by the segment
    for(size_t i = 0; i <= trapsToDelete.size() -1; i++){

        // get the dag node information
        nodeToSubstitute = trapsToDelete[i]->getRefToDag();

        if (i == 0){
            // first trapezoid
            oldTrap = (Trapezoid*)nodeToSubstitute->getData().obj;

            if (!left_coincident){
                data.type = DagNode::TypeNode::Left;
                data.obj = new cg3::Point2d(segment.p1());
                nodeToSubstitute->setData(data);


                nodeToSubstitute->left = new DagNode(DagNode::TypeNode::Trapezoid, newTraps.at(j));
                newTraps.at(j)->setRefToDag(nodeToSubstitute->left);
                j++;

                nodeToSubstitute->right = new DagNode(DagNode::TypeNode::Segment, new cg3::Segment2d(segment));
                tmp = nodeToSubstitute->right;

                if (Algorithm::pointIsAboveSegment(segment, oldTrap->getRightPoint())){
                    tmp->left = new DagNode(DagNode::TypeNode::Trapezoid, newTraps.at(j));
                    newTraps.at(j)->setRefToDag(tmp->left);
                    j++;

                    tmp->right = new DagNode(DagNode::TypeNode::Trapezoid, newTraps.at(j));
                    newTraps.at(j)->setRefToDag(tmp->right);
                    trapShared = tmp->right;
                    j++;
                }else{
                    tmp->right = new DagNode(DagNode::TypeNode::Trapezoid, newTraps.at(j));
                    newTraps.at(j)->setRefToDag(tmp->right);
                    j++;

                    tmp->left = new DagNode(DagNode::TypeNode::Trapezoid, newTraps.at(j));
                    newTraps.at(j)->setRefToDag(tmp->left);
                    trapShared = tmp->left;
                    j++;
                }
            }else{
                // left coincident
                data.type = DagNode::TypeNode::Segment;
                data.obj = new cg3::Segment2d(segment);
                nodeToSubstitute->setData(data);

                if (Algorithm::pointIsAboveSegment(segment, oldTrap->getRightPoint())){
                    // merge trapezoid is below
                    nodeToSubstitute->left = new DagNode(DagNode::TypeNode::Trapezoid, newTraps.at(j));
                    newTraps.at(j)->setRefToDag(nodeToSubstitute->left);
                    j++;

                    nodeToSubstitute->right = new DagNode(DagNode::TypeNode::Trapezoid, newTraps.at(j));
                    newTraps.at(j)->setRefToDag(nodeToSubstitute->right);
                    trapShared = nodeToSubstitute->right;
                    j++;
                }else{
                    // merge trapezoid is above
                    nodeToSubstitute->right = new DagNode(DagNode::TypeNode::Trapezoid, newTraps.at(j));
                    newTraps.at(j)->setRefToDag(nodeToSubstitute->right);
                    j++;

                    nodeToSubstitute->left = new DagNode(DagNode::TypeNode::Trapezoid, newTraps.at(j));
                    newTraps.at(j)->setRefToDag(nodeToSubstitute->left);
                    trapShared = nodeToSubstitute->left;
                    j++;
                }
            }
        }else if (i < trapsToDelete.size() -1 ){
            oldTrap = (Trapezoid*)nodeToSubstitute->getData().obj;

            data.type = DagNode::TypeNode::Segment;
            data.obj = new cg3::Segment2d(segment);
            nodeToSubstitute->setData(data);

            if (Algorithm::pointIsAboveSegment(segment, oldTrap->getLeftPoint())){
                nodeToSubstitute->left = new DagNode(DagNode::TypeNode::Trapezoid, newTraps.at(j));
                newTraps.at(j)->setRefToDag(nodeToSubstitute->left);
                j++;

                nodeToSubstitute->right = trapShared;
                newTraps.at(j)->setRefToDag(nodeToSubstitute->right);

                if (Algorithm::pointIsAboveSegment(segment, oldTrap->getRightPoint())){
                    // share trapezoid is to the right
                    trapShared = nodeToSubstitute->right;
                }else{
                    // share trapezoid is to the left
                    trapShared = nodeToSubstitute->left;
                }

            }else{

                nodeToSubstitute->right = new DagNode(DagNode::TypeNode::Trapezoid, newTraps.at(j));
                newTraps.at(j)->setRefToDag(nodeToSubstitute->right);
                j++;

                nodeToSubstitute->left = trapShared;
                newTraps.at(j)->setRefToDag(nodeToSubstitute->left);



                if (Algorithm::pointIsAboveSegment(segment, oldTrap->getRightPoint())){
                    //share trapezoid is to the right
                    trapShared = nodeToSubstitute->right;
                }else{
                    // share trapezoid is to the left
                    trapShared = nodeToSubstitute->left;
                }
            }
        }
        else{
            // last trapeozid
            oldTrap = (Trapezoid*)nodeToSubstitute->getData().obj;

            if (!right_coincident){
                data.type = DagNode::TypeNode::Right;
                data.obj = new cg3::Point2d(segment.p2());
                nodeToSubstitute->setData(data);

                nodeToSubstitute->right = new DagNode(DagNode::TypeNode::Trapezoid, newTraps.back());
                newTraps.back()->setRefToDag(nodeToSubstitute->right);


                nodeToSubstitute->left = new DagNode(DagNode::TypeNode::Segment, new cg3::Segment2d(segment));
                tmp = nodeToSubstitute->left;

                if (Algorithm::pointIsAboveSegment(segment, oldTrap->getLeftPoint())){
                    tmp->left = new DagNode(DagNode::TypeNode::Trapezoid, newTraps.at(j));
                    newTraps.at(j)->setRefToDag(tmp->left);


                    tmp->right = trapShared;

                }else{
                    tmp->right = new DagNode(DagNode::TypeNode::Trapezoid, newTraps.at(j));
                    newTraps.at(j)->setRefToDag(tmp->right);

                    tmp->left = trapShared;

                }
            }else{
                //right coincident
                data.type = DagNode::TypeNode::Segment;
                data.obj = new cg3::Segment2d(segment);
                nodeToSubstitute->setData(data);

                if (Algorithm::pointIsAboveSegment(segment, oldTrap->getLeftPoint())){
                    nodeToSubstitute->left = new DagNode(DagNode::TypeNode::Trapezoid, newTraps.at(j));
                    newTraps.at(j)->setRefToDag(nodeToSubstitute->left);


                    nodeToSubstitute->right = trapShared;

                }else{
                    nodeToSubstitute->right = new DagNode(DagNode::TypeNode::Trapezoid, newTraps.at(j));
                    newTraps.at(j)->setRefToDag(nodeToSubstitute->right);

                    nodeToSubstitute->left = trapShared;

                }
            }
        }

    }

}
