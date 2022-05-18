#include "dag.h"

Dag::Dag(DrawableTrapMap& drawableTrapMap, Trapezoid *bb):
    drawableTrapMap(drawableTrapMap)

{
    // inizializzo la dag con la bounding box
    root = createNewDagNode(DagNode::TypeNode::Trapezoid, bb);
    Trapezoid* a = (Trapezoid*)root->getData().objj;
    std::cout << "Id del trap (costruttore) " << a->getId() << std::endl;

}
DagNode* Dag::getRoot() const {
    return root;
}

DagNode* Dag::createNewDagNode(DagNode::TypeNode typeObj, void *object){
    DagNode *newNode = new DagNode(typeObj, object);
    return newNode;
}

std::vector<Trapezoid*> Dag::followSegment(const cg3::Segment2d &segment, DagNode* trap){
    Trapezoid t = *(Trapezoid*)trap->getData().objj;
    cg3::Segment2d s = segment;
    std::vector<Trapezoid*> trapezoids;
    trapezoids.push_back((Trapezoid*)trap->getData().objj);
    // se il punto q1 si trova a destra del right point del trapezoide

    while (segment.p2().x() > t.getRightPoint().x()) {

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
std::pair<DagNode*, DagNode*> Dag::query(const cg3::Segment2d segment, bool &leftToRight){
    DagNode* tmp = root;
    cg3::Point2d *p1, *q1;
    cg3::Segment2d* s1;
    bool isFirstPoint = true;
    cg3::Point2d queryPoint;
    std::pair<DagNode*, DagNode*> trapezoids;

    if (leftToRight)
        queryPoint = segment.p1();
    else
        queryPoint = segment.p2();

    while(true){

        switch(tmp->getData().type){
            case DagNode::TypeNode::Trapezoid:
                if (isFirstPoint){
                    // inserisco il primo trapezoide nel pair
                    trapezoids.first = tmp;
                    tmp = root;

                    if (leftToRight){
                        queryPoint = segment.p2();
                    }else
                        queryPoint = segment.p1();

                    isFirstPoint = false;
                }else{
                    trapezoids.second = tmp;
                    return trapezoids;
                }
                break;

            case DagNode::TypeNode::Left:
                p1 = (cg3::Point2d*)tmp->getData().objj;
                if ( queryPoint.x() > p1->x()){
                    // andare a destra
                    tmp = tmp->right;
                }else{
                    //andare a sinistra
                    tmp = tmp->left;
                }
                break;

            case DagNode::TypeNode::Right:
                q1 = (cg3::Point2d*)tmp->getData().objj;
                if (queryPoint.x() > q1->x()){
                    // andare a destra
                    tmp = tmp->right;
                }else{
                    //andare a sinistra
                    tmp = tmp->left;
                }
                break;

            case DagNode::TypeNode::Segment:
                s1 = (cg3::Segment2d*)tmp->getData().objj;

                if (pointIsAbove(s1,queryPoint)){
                    //il punto si trova sopra il segmento
                    tmp = tmp->left;
                }else{
                    // il punto si trova sotto il segmento o sul
                    tmp = tmp->right;
                }
                break;

            default:
                break;

        }
    }

    // TODO controllare se é presente due volte lo stesso nodo
    //result.pop_back();
    // DA MODIFICARE - NON VA BENE


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
void Dag::updateDag(std::vector<Trapezoid*> traps, DagNode* trapNode, const cg3::Segment2d& segment){
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

