#include "dag.h"

DAG::DAG(DrawableTrapMap& drawableTrapMap, Trapezoid *bb):
    drawableTrapMap(drawableTrapMap)

{
    // inizializzo la dag con la bounding box
    root = createNewDagNode(DagNode::TypeNode::Trapezoid, bb);
    Trapezoid* a = (Trapezoid*)root->getData().objj;
    std::cout << "Id del trap (costruttore) " << a->getId() << std::endl;

}
DagNode* DAG::getRoot() const {
    return root;
}

DagNode* DAG::createNewDagNode(DagNode::TypeNode typeObj, void *object){
    DagNode *newNode = new DagNode(typeObj, object);
    return newNode;
}
DagNode* DAG::findTrapToModify(const cg3::Segment2d& segment){
    // se la root contiene un trapezoide sicuramente é il bb
    // lo restituisco per la modifica
    if (root->getData().type == DagNode::TypeNode::Trapezoid) return root;

    CG3_SUPPRESS_WARNING(segment);
}

DagNode* DAG::query(const cg3::Segment2d segment){
    DagNode* tmp = root;
    bool isFirstPoint = true;
    cg3::Point2d queryPoint = segment.p1();

    // TODO sostiutire con uno switch case
    while(isFirstPoint){

        // nodo trapezoide
        if (tmp->getData().type == DagNode::TypeNode::Trapezoid){
                return tmp;
        }

        // punto sinistro
        if (tmp->getData().type == DagNode::TypeNode::Left){
            cg3::Point2d *p1 = (cg3::Point2d*)tmp->getData().objj;
            if ( queryPoint.x() > p1->x()){
                // andare a destra
                tmp = tmp->right;
            }else{
                //andare a sinistra
                tmp = tmp->left;
            }
            continue;
        }

        // punto destro
        if (tmp->getData().type == DagNode::TypeNode::Right){
            cg3::Point2d *q1 = (cg3::Point2d*)tmp->getData().objj;
            if (queryPoint.x() > q1->x()){
                // andare a destra
                tmp = tmp->right;
            }else{
                //andare a sinistra
                tmp = tmp->left;
            }
            continue;
        }

        // segmento

        if (tmp->getData().type == DagNode::TypeNode::Segment){

            cg3::Segment2d* s1 = (cg3::Segment2d*)tmp->getData().objj;

            if (pointIsAbove(s1,queryPoint)){
                //il punto si trova sopra il segmento
                tmp = tmp->left;
            }else{
                // il punto si trova sotto il segmento o sul
                tmp = tmp->right;
            }
            continue;
        }

    }

    // TODO controllare se é presente due volte lo stesso nodo
    //result.pop_back();
    // DA MODIFICARE - NON VA BENE


}

bool DAG::pointIsAbove(cg3::Segment2d *s, cg3::Point2d p){
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
void DAG::updateDag(std::vector<Trapezoid*> traps, DagNode* trapNode, const cg3::Segment2d& segment){
    cg3::Point2d p1 = segment.p1();
    cg3::Point2d q1 = segment.p2();

    DagNode *tmp;


    tmp = new DagNode(DagNode::TypeNode::Left, new cg3::Point2d(segment.p1().x(),segment.p1().y())); // inserisco il punto p1

    tmp->left = new DagNode(DagNode::TypeNode::Trapezoid, traps.at(0)); // inserisco il trapezoide A
    tmp->left->parent = tmp;

    tmp->right = new DagNode(DagNode::TypeNode::Right, new cg3::Point2d(segment.p2().x(),segment.p2().y())); // inserisco il punto q1

    if (root->getData().type == DagNode::TypeNode::Trapezoid){
        root = tmp;
    }else{
        if (trapNode->parent->left == trapNode)
            trapNode->parent->left = tmp;
        else
            trapNode->parent->right = tmp;

        trapNode->parent = nullptr;
    }

    tmp = tmp->right; // mi sposto a destra

    tmp->left = new DagNode(DagNode::TypeNode::Segment, new cg3::Segment2d(p1,q1)); // inserisco il segmento s1

    tmp->right = new DagNode(DagNode::TypeNode::Trapezoid, traps.at(3)); // inserisco il trapezoide D
    tmp->right->parent = tmp;

    tmp = tmp->left;

    tmp->left = new DagNode(DagNode::TypeNode::Trapezoid, traps.at(1)); // inserisco il trapezoide B
    tmp->left->parent = tmp;

    tmp->right = new DagNode(DagNode::TypeNode::Trapezoid, traps.at(2)); // inserisco il trapezoide C
    tmp->right->parent = tmp;

    // dealloco la memoria del nodo da sostituire
    delete trapNode;

}

