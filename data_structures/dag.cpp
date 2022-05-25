#include "dag.h"

#include "algorithm.h"

Dag::Dag(DrawableTrapMap& drawableTrapMap, Trapezoid *bb):
    drawableTrapMap(drawableTrapMap)

{
    // inizializzo la dag con la bounding box
    root = createNewDagNode(DagNode::TypeNode::Trapezoid, bb);
    nodes.push_back(root);

}
void Dag::init(Trapezoid *bb){
    // inizializzo la dag con la bounding box
    root = createNewDagNode(DagNode::TypeNode::Trapezoid, bb);
    nodes.push_back(root);
}
DagNode* Dag::getRoot() const {
    return root;
}

DagNode* Dag::createNewDagNode(DagNode::TypeNode typeObj, void *object){
    DagNode *newNode = new DagNode(typeObj, object);
    return newNode;
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

std::vector<Trapezoid*> Dag::followSegment(const cg3::Segment2d &segment, DagNode* trap){
    Trapezoid t = *(Trapezoid*)trap->getData().objj;
    cg3::Segment2d s;
    std::vector<Trapezoid*> trapezoids;
    trapezoids.push_back((Trapezoid*)trap->getData().objj);

    if (segment.p1() < segment.p2())
        s = segment;
    else{
        s = cg3::Segment2d(segment.p2(), segment.p1());
    }
    // se il punto q1 si trova a destra del right point del trapezoide

    while (s.p2().x() > t.getRightPoint().x()) {

        // se il right point si trova sopra il segmento
        if (pointIsAbove(&s, t.getRightPoint())){
            // prendo il bottom right neighbor
            trapezoids.push_back(t.getBottomRightNeigh());
            t = *t.getBottomRightNeigh();
        }else{
            trapezoids.push_back(t.getUpperRightNeigh());
            t = *t.getUpperRightNeigh();
        }
    }

    return trapezoids;

}
bool Dag::pointIsAbove(cg3::Segment2d *s, cg3::Point2d p){
    //            v1 = {x2-x1, y2-y1}   # Vector 1
    //            v2 = {xA-x1, yA-y1}   # Vector 2
    //            cross_product = v1.x*v2.y - v1.y*v2.x
    //            if cross_product > 0:
    //                print 'pointA is on the counter-clockwise side of line'
    //            elif cross_product < 0:
    //                print 'pointA is on the clockwise side of line'
    //            else:
    //                print 'pointA is exactly on the line'
    cg3::Point2d v1 = cg3::Point2d(s->p2().x()-s->p1().x(),s->p2().y()-s->p1().y());
    cg3::Point2d v2 = cg3::Point2d(p.x()-s->p1().x(),p.y()-s->p1().y());
    double cross_product = v1.x() * v2.y() - v1.y() * v2.x();
    if (cross_product > 0){
        //il punto si trova sopra il segmento
        return true;
    }else{
        // il punto si trova sotto il segmento o sul
        return false;
    }
}
void Dag::updateDag(std::vector<Trapezoid*>& traps, DagNode* trapNode, const cg3::Segment2d& segment){

    //TODO use a list to contain all dag nodes

    cg3::Point2d p1 = segment.p1();
    cg3::Point2d q1 = segment.p2();
    DagNode::DagData data;

    DagNode *tmp = trapNode;

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
}
bool Dag::isLeftNode(DagNode *node){
    Trapezoid *tLeft = (Trapezoid*)node->parent->left->getData().objj;
    Trapezoid *t = (Trapezoid*)node->getData().objj;
    if (tLeft->getId() == t->getId()){
        return true;
    }else{
        return false;
    }
}
void Dag::updateDag2(std::vector<Trapezoid*> newTraps, std::vector<Trapezoid*> trapsToDelete, const cg3::Segment2d& segment){
    DagNode *nodeToSubstitute, *rootSubDag, *tmp, *trapShared;
    int j=0;
    DagNode::DagData data;

    for(size_t i = 0; i <= trapsToDelete.size() -1; i++){
        nodeToSubstitute = trapsToDelete[i]->getRefToDag();

        if (i == 0){
            data.type = DagNode::TypeNode::Left;
            data.objj = new cg3::Point2d(segment.p1());
            nodeToSubstitute->setData(data);

            //rootSubDag = new DagNode(DagNode::TypeNode::Left, new cg3::Point2d(segment.p1()));

            nodeToSubstitute->left = new DagNode(DagNode::TypeNode::Trapezoid, newTraps.at(j));
            newTraps.at(j)->setRefToDag(nodeToSubstitute->left);
            //nodeToSubstitute->left->parent = nodeToSubstitute;
            j++;

            nodeToSubstitute->right = new DagNode(DagNode::TypeNode::Segment, new cg3::Segment2d(segment));
            tmp = nodeToSubstitute->right;

            if (Algorithm::pointIsAboveSegment(segment, newTraps.at(j)->getRightPoint())){
                tmp->left = new DagNode(DagNode::TypeNode::Trapezoid, newTraps.at(j));
                newTraps.at(j)->setRefToDag(tmp->left);
                //tmp->left->parent = tmp;
                j++;

                tmp->right = new DagNode(DagNode::TypeNode::Trapezoid, newTraps.at(j));
                newTraps.at(j++)->setRefToDag(tmp->right);
                trapShared = tmp->right;//prendo il riferimento del trapezoide che sará condiviso nella dag
                //tmp->right->parent = tmp;
            }else{
                tmp->right = new DagNode(DagNode::TypeNode::Trapezoid, newTraps.at(j));
                newTraps.at(j++)->setRefToDag(tmp->right);
                //tmp->right->parent = tmp;


                tmp->left = new DagNode(DagNode::TypeNode::Trapezoid, newTraps.at(j));
                newTraps.at(j++)->setRefToDag(tmp->left);
                //tmp->left->parent = tmp;
                trapShared = tmp->left;

            }

//            if (isLeftNode(nodeToSubstitute))
//                nodeToSubstitute->parent->left = rootSubDag;
//            else
//                nodeToSubstitute->parent->right = rootSubDag;

            //delete nodeToSubstitute;
        }else{
            data.type = DagNode::TypeNode::Right;
            data.objj = new cg3::Point2d(segment.p2());
            nodeToSubstitute->setData(data);

            //nodeToSubstitute = new DagNode(DagNode::TypeNode::Right, new cg3::Point2d(segment.p2()));

            nodeToSubstitute->right = new DagNode(DagNode::TypeNode::Trapezoid, newTraps.back());
            newTraps.back()->setRefToDag(nodeToSubstitute->right);
            //nodeToSubstitute->right->parent = rootSubDag;

            nodeToSubstitute->left = new DagNode(DagNode::TypeNode::Segment, new cg3::Segment2d(segment));
            tmp = nodeToSubstitute->left;

            if (Algorithm::pointIsAboveSegment(segment, newTraps.at(j)->getLeftPoint())){
                tmp->left = new DagNode(DagNode::TypeNode::Trapezoid, newTraps.at(j));
                newTraps.at(j)->setRefToDag(tmp->left);
                //tmp->left->parent = tmp;

                tmp->right = trapShared;//new DagNode(DagNode::TypeNode::Trapezoid, newTraps.at(j-1));
                //newTraps.at(j-1)->setRefToDag(tmp->right);
                //tmp->right->parent = tmp;
            }else{
                tmp->right = new DagNode(DagNode::TypeNode::Trapezoid, newTraps.at(j));
                newTraps.at(j)->setRefToDag(tmp->right);
                //tmp->right->parent = tmp;

                tmp->left = trapShared;//new DagNode(DagNode::TypeNode::Trapezoid, newTraps.at(j-1));
                // newTraps.at(j-1)->setRefToDag(tmp->left);
                //tmp->left->parent = tmp;
            }

//            if (isLeftNode(nodeToSubstitute))
//                nodeToSubstitute->parent->left = rootSubDag;
//            else
//                nodeToSubstitute->parent->right = rootSubDag;

//            delete nodeToSubstitute;

        }

    }
}

