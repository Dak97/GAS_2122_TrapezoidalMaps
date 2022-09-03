#include "dag.h"
#include "algorithm.h"

Dag::Dag(Trapezoid *bb) : root(createNewDagNode(DagNode::TypeNode::Trapezoid, bb)){
    nodes.push_back(root);
}

void Dag::init(Trapezoid *bb){
    root = createNewDagNode(DagNode::TypeNode::Trapezoid, bb);
    nodes.push_back(root);
}

DagNode* Dag::createNewDagNode(DagNode::TypeNode typeObj, void *object){
    DagNode *newNode = new DagNode(typeObj, object);
    return newNode;
}

DagNode* Dag::getRoot() const {
    return root;
}

void Dag::clearDag(){
    for(DagNode *n: nodes){
        if (n->getData().type != DagNode::TypeNode::Trapezoid){
            free(n->getData().objj);
            free(n);
        }
    }
    nodes.clear();
}

void Dag::updateDagSingleSplit(std::vector<Trapezoid*>& traps, DagNode* trapNode, const cg3::Segment2d& segment, bool& left_coincident, bool& right_coincident){

    cg3::Point2d p1 = segment.p1();
    cg3::Point2d q1 = segment.p2();
    DagNode::DagData data;

    DagNode *tmp = trapNode;

    if (!left_coincident && !right_coincident){
        data.type = DagNode::TypeNode::Left;
        data.objj = new cg3::Point2d(p1);

        tmp->setData(data);

        tmp->left = new DagNode(DagNode::TypeNode::Trapezoid, traps.at(0));                             // insert first trapezoid
        traps.at(0)->setRefToDag(tmp->left);                                                            // setting ref to dag node
        nodes.push_back(tmp->left);

        tmp->right = new DagNode(DagNode::TypeNode::Right, new cg3::Point2d(q1));                       // insert q1
        nodes.push_back(tmp->right);

        tmp = tmp->right; // mi sposto a destra

        tmp->left = new DagNode(DagNode::TypeNode::Segment, new cg3::Segment2d(p1,q1)); // inserisco il segmento s1
        nodes.push_back(tmp->left);

        tmp->right = new DagNode(DagNode::TypeNode::Trapezoid, traps.at(3)); // inserisco il trapezoide D
        traps.at(3)->setRefToDag(tmp->right);
        nodes.push_back(tmp->right);

        tmp = tmp->left;

        tmp->left = new DagNode(DagNode::TypeNode::Trapezoid, traps.at(1)); // inserisco il trapezoide B
        traps.at(1)->setRefToDag(tmp->left);
        nodes.push_back(tmp->left);


        tmp->right = new DagNode(DagNode::TypeNode::Trapezoid, traps.at(2)); // inserisco il trapezoide C
        traps.at(2)->setRefToDag(tmp->right);
        nodes.push_back(tmp->right);

    }else if (left_coincident && right_coincident){
        data.type = DagNode::TypeNode::Segment;
        data.objj = new cg3::Segment2d(p1, q1);
        tmp->setData(data);

        tmp->left = new DagNode(DagNode::TypeNode::Trapezoid, traps.at(0));
        traps.at(0)->setRefToDag(tmp->left);
        nodes.push_back(tmp->left);

        tmp->right = new DagNode(DagNode::TypeNode::Trapezoid, traps.at(1));
        traps.at(1)->setRefToDag(tmp->right);
        nodes.push_back(tmp->right);
    }else if (left_coincident){
        data.type = DagNode::TypeNode::Right;
        data.objj = new cg3::Point2d(q1);
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
        data.type = DagNode::TypeNode::Left;
        data.objj = new cg3::Point2d(p1);
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

    for(size_t i = 0; i <= trapsToDelete.size() -1; i++){

        nodeToSubstitute = trapsToDelete[i]->getRefToDag();

        if (i == 0){
            oldTrap = (Trapezoid*)nodeToSubstitute->getData().objj;

            if (!left_coincident){
                data.type = DagNode::TypeNode::Left;
                data.objj = new cg3::Point2d(segment.p1());
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
                    trapShared = tmp->right;//prendo il riferimento del trapezoide che sará condiviso nella dag
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
                data.objj = new cg3::Segment2d(segment);
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
            oldTrap = (Trapezoid*)nodeToSubstitute->getData().objj;

            // creo il nodo segmento
            data.type = DagNode::TypeNode::Segment;
            data.objj = new cg3::Segment2d(segment);
            nodeToSubstitute->setData(data);

            if (Algorithm::pointIsAboveSegment(segment, oldTrap->getLeftPoint())){
                // questo trap va a sinistra
                // il trap shared va a destra

                nodeToSubstitute->left = new DagNode(DagNode::TypeNode::Trapezoid, newTraps.at(j));
                newTraps.at(j)->setRefToDag(nodeToSubstitute->left);
                j++;

                nodeToSubstitute->right = trapShared;
                newTraps.at(j)->setRefToDag(nodeToSubstitute->right);



                if (Algorithm::pointIsAboveSegment(segment, oldTrap->getRightPoint())){
                    // quello che sarà condiviso si trova a destra
                    trapShared = nodeToSubstitute->right;
                }else{
                    // si trova a sinistra
                    trapShared = nodeToSubstitute->left;
                }
            }else{
                // questo a destra
                // il trap shared va a sinistra

                nodeToSubstitute->right = new DagNode(DagNode::TypeNode::Trapezoid, newTraps.at(j));
                newTraps.at(j)->setRefToDag(nodeToSubstitute->right);
                j++;

                nodeToSubstitute->left = trapShared;
                newTraps.at(j)->setRefToDag(nodeToSubstitute->left);



                if (Algorithm::pointIsAboveSegment(segment, oldTrap->getRightPoint())){
                    // quello che sarà condiviso si trova a destra
                    trapShared = nodeToSubstitute->right;
                }else{
                    // si trova a sinistra
                    trapShared = nodeToSubstitute->left;
                }
            }
        }
        else{
            oldTrap = (Trapezoid*)nodeToSubstitute->getData().objj;

            if (!right_coincident){
                data.type = DagNode::TypeNode::Right;
                data.objj = new cg3::Point2d(segment.p2());
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
                data.objj = new cg3::Segment2d(segment);
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
