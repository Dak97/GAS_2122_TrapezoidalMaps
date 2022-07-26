#include "algorithm.h"

namespace Algorithm
{
    void buildTrapMapDag(Dag& dag, TrapMap& trapMap, const cg3::Segment2d& segment){
        cg3::Segment2d segment_oriented = segment;
        std::pair<DagNode*, DagNode*> trapsNodeQuery;
        std::vector<Trapezoid*> trapforDag, newTrapsForDag;
        //bool leftToRight = true;

        // check if the segment is left to right orinted
        if (segment_oriented.p2().x() < segment_oriented.p1().x()) { // the segment is not left to right
            segment_oriented = cg3::Segment2d(segment_oriented.p2(), segment_oriented.p1());    // create a new segment left to right oriented
        }

        // find dag node of trapezoids to change
        trapsNodeQuery.first = query(dag, segment_oriented.p1());
        trapsNodeQuery.second = query(dag, segment_oriented.p2());

         // check if the end-points are inside the same trapezoid
         if (((Trapezoid*)trapsNodeQuery.first->getData().objj) == ((Trapezoid*)trapsNodeQuery.second->getData().objj)){

             // create 4 new trapezoids that are inside one trapezoid
             trapforDag = trapMap.addFourTrapezoids(segment_oriented,trapsNodeQuery.first);

             int bbId = ((Trapezoid*)trapsNodeQuery.first->getData().objj)->getId();

              //update the dag with the new created trapezoids
              dag.updateDag(trapforDag, trapsNodeQuery.first, segment_oriented);

             // delete trapezoid
             trapMap.deleteTrapezoidWithId(bbId);
         }else{
             // use the follow segment algorithm to find all the trapezoids that intersect the new segment
             trapforDag = dag.followSegment(segment_oriented, trapsNodeQuery.first);

             newTrapsForDag = trapMap.newTrapezoids(segment_oriented, trapforDag);

             //aggiornamento della dag con i nuovi trapezoidi
             dag.updateDag2(newTrapsForDag, trapforDag, segment_oriented);

             for (Trapezoid* t : trapforDag){
                     trapMap.deleteTrapezoidWithId(t->getId());
             }
         }
//         printDag(dag);
//       printNeigh(trapMap);

    }
    DagNode* query(Dag& dag, const cg3::Point2d& point){
        DagNode* tmp = dag.getRoot();
        cg3::Point2d *p1, *q1;
        cg3::Segment2d* s1;
        cg3::Point2d queryPoint = point;

        while(true){
            switch(tmp->getData().type){
                case DagNode::TypeNode::Trapezoid:
                    return tmp;

                case DagNode::TypeNode::Left:
                    p1 = (cg3::Point2d*)tmp->getData().objj;
                    if ( queryPoint.x() > p1->x()){

                        tmp = tmp->right;
                    }else{

                        tmp = tmp->left;
                    }
                    break;

                case DagNode::TypeNode::Right:
                    q1 = (cg3::Point2d*)tmp->getData().objj;
                    if (queryPoint.x() > q1->x()){

                        tmp = tmp->right;
                    }else{

                        tmp = tmp->left;
                    }
                    break;

                case DagNode::TypeNode::Segment:
                    s1 = (cg3::Segment2d*)tmp->getData().objj;

                    if (Algorithm::pointIsAboveSegment(*s1,queryPoint)){
                        tmp = tmp->left;
                    }else{
                        tmp = tmp->right;
                    }
                    break;
                default:
                 std::cout << "Query - default case" << std::endl;
                    break;

            }
            std::cout << "LOOP" << std::endl;
        }
    }
    std::pair<DagNode*, DagNode*> queryPair(Dag& dag, const cg3::Segment2d& segment){
        DagNode* tmp = dag.getRoot();
        cg3::Point2d *p1, *q1;
        cg3::Segment2d* s1;
        bool isFirstPoint = true;
        cg3::Point2d queryPoint = segment.p1();
        std::pair<DagNode*, DagNode*> trapezoids;

        while(true){

            switch(tmp->getData().type){
                case DagNode::TypeNode::Trapezoid:
                    if (isFirstPoint){
                        // inserisco il primo trapezoide nel pair
                        trapezoids.first = tmp;
                        tmp = dag.getRoot();

                        queryPoint = segment.p2();

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

                    if (Algorithm::pointIsAboveSegment(*s1,queryPoint)){
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
    }

    bool pointIsAboveSegment(cg3::Segment2d segment, cg3::Point2d point){

        cg3::Point2d v1 = cg3::Point2d(segment.p2().x()-segment.p1().x(),segment.p2().y()-segment.p1().y());
        cg3::Point2d v2 = cg3::Point2d(point.x()-segment.p1().x(),point.y()-segment.p1().y());
        double cross_product = v1.x() * v2.y() - v1.y() * v2.x();

        if (cross_product > 0){
            //il punto si trova sopra il segmento
            return true;
        }else{
            // il punto si trova sotto il segmento o sul
            return false;
        }
    }
    void printNeigh(TrapMap trapMap){
        bool neig = true, points = false;
        std::string s;
        for (Trapezoid t : trapMap.getTrapezoids()){
            if (neig){
                std::cout << "ID: " << t.getId() << std::endl;

                if (t.getUpperLeftNeigh() != nullptr && t.getBottomLeftNeigh() != nullptr)
                    std::cout << "U-L " << t.getUpperLeftNeigh()->getId() << " B-L " << t.getBottomLeftNeigh()->getId();
                else
                    std::cout << "U-L " << "NULL " << " B-L " << "NULL";


                if (t.getUpperRightNeigh() != nullptr && t.getBottomRightNeigh() != nullptr)
                    std::cout << " U-R " << t.getUpperRightNeigh()->getId() << " B-R " << t.getBottomRightNeigh()->getId() << std::endl;
                else
                    std::cout << " U-R " << "NULL" << " B-R " << "NULL" << std::endl;
            }
            if (points){
                std::cout << t.getId() << " - L " << t.getLeftPoint() << " - R " << t.getRightPoint()  << std::endl;
            }

       }
    }
    void printDag(Dag dag){
        DagNode* tmp = dag.getRoot();
        std::list<DagNode*> stack;

        stack.push_front(tmp);

        while (stack.size() != 0) {
            tmp = *stack.begin();
            stack.pop_front();


            if (tmp->left != nullptr){
                stack.push_back(tmp->left);
            }
            if (tmp->right != nullptr){
                stack.push_back(tmp->right);
            }

            switch (tmp->getData().type) {
                case DagNode::TypeNode::Left:

                    std::cout << "PL - " << tmp << std::endl;
                    break;
                case DagNode::TypeNode::Right:
                std::cout << "PR - " << tmp << std::endl;
                    break;
                case DagNode::TypeNode::Segment:
                std::cout << "S - " << tmp << std::endl;
                    break;
                case DagNode::TypeNode::Trapezoid:
                Trapezoid* t = (Trapezoid*)tmp->getData().objj;
                    std::cout << "T" << t->getId() << " - " << tmp << std::endl;
                    break;

            }

        }

    }

}


