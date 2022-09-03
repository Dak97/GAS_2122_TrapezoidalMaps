#include "algorithm.h"
#include "cg3/geometry/utils2.h"
namespace Algorithm
{
    void buildTrapMapDag(Dag& dag, TrapMap& trapMap, const cg3::Segment2d& segment){
        cg3::Segment2d segment_oriented = segment;
        DagNode* trapNodeQuery;
        std::vector<Trapezoid*> trapsCrossed, trapsCreated;
        bool left_coincident = false, right_coincident = false;

        // check if the segment is left to right orinted
        if (segment_oriented.p2().x() < segment_oriented.p1().x()) { // the segment is not left to right
            segment_oriented = cg3::Segment2d(segment_oriented.p2(), segment_oriented.p1());    // create a new segment left to right oriented
        }

        // find the trapezoid where p1 is
        trapNodeQuery = query(dag, segment_oriented);

        // use the follow segment algorithm to find all the trapezoids that intersect the new segment
        trapsCrossed = followSegment(segment_oriented, trapNodeQuery);

         // check if the end-points are inside the same trapezoid
         if (trapsCrossed.size() == 1){

             // create 4 new trapezoids that are inside one trapezoid
             trapsCreated = trapMap.newTrapezoidsSingleSplit(segment_oriented, trapsCrossed.front(), left_coincident, right_coincident);

              //update the dag with the new created trapezoids
              dag.updateDagSingleSplit(trapsCreated, trapNodeQuery, segment_oriented, left_coincident, right_coincident);

             // delete trapezoid
             trapMap.deleteTrapezoidByRef(trapsCrossed);
         }else{

             trapsCreated = trapMap.newTrapezoidsMultipleSplit(segment_oriented, trapsCrossed, left_coincident, right_coincident);

             //aggiornamento della dag con i nuovi trapezoidi
             dag.updateDagMultipleSplit(trapsCreated, trapsCrossed, segment_oriented, left_coincident, right_coincident);

             trapMap.deleteTrapezoidByRef(trapsCrossed);
         }
 //         printDag(dag);
 //         printNeigh(trapMap);
 //         validateNeighbors(trapMap);

    }
    DagNode* query(Dag& dag, const cg3::Segment2d& segment){
        DagNode* tmp = dag.getRoot();
        cg3::Point2d *p1, *q1;
        cg3::Segment2d* s1;
        cg3::Point2d queryPoint = segment.p1();
        double m_current_segment, m_new_segment;

        while(true){
            switch(tmp->getData().type){
                case DagNode::TypeNode::Trapezoid:
                    return tmp;

                case DagNode::TypeNode::Left:
                    p1 = (cg3::Point2d*)tmp->getData().objj;
                    if ( queryPoint.x() >= p1->x()){

                        tmp = tmp->right;
                    }else{

                        tmp = tmp->left;
                    }
                    break;

                case DagNode::TypeNode::Right:
                    q1 = (cg3::Point2d*)tmp->getData().objj;
                    if (queryPoint.x() >= q1->x()){

                        tmp = tmp->right;
                    }else{

                        tmp = tmp->left;
                    }
                    break;

                case DagNode::TypeNode::Segment:
                    s1 = (cg3::Segment2d*)tmp->getData().objj;

                    if (Algorithm::pointIsAboveSegment(*s1,queryPoint)){
                        tmp = tmp->left;
                    }else if (cg3::isPointAtRight(*s1, queryPoint)){
                        tmp = tmp->right;
                    }
                    else{
                        m_current_segment = (s1->p2().y() - s1->p1().y())/(s1->p2().x() - s1->p1().x());
                        m_new_segment = (segment.p2().y() - segment.p1().y())/(segment.p2().x() - segment.p1().x());

                        if (m_new_segment > m_current_segment){
                            tmp = tmp->left;
                        }else{
                            tmp = tmp->right;
                      }
                    }
                    break;
                default:
                 std::cout << "Query - default case" << std::endl;
                    break;

            }
        }
    }
    std::vector<Trapezoid*> followSegment(const cg3::Segment2d &segment, DagNode* trap){
        Trapezoid *t = (Trapezoid*)trap->getData().objj;
        std::vector<Trapezoid*> trapezoids;

        trapezoids.push_back((Trapezoid*)trap->getData().objj);

        // se il punto q1 si trova a destra del right point del trapezoide
        while (segment.p2().x() > t->getRightPoint().x()) {

            // se il right point si trova sopra il segmento
            if (Algorithm::pointIsAboveSegment(segment, t->getRightPoint())){
                // prendo il bottom right neighbor
                trapezoids.push_back(t->getBottomRightNeigh());
                t = t->getBottomRightNeigh();
            }else{
                trapezoids.push_back(t->getUpperRightNeigh());
                t = t->getUpperRightNeigh();
            }
        }

        return trapezoids;

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

    bool pointsAreEquals(const cg3::Point2d& p1, const cg3::Point2d& p2){
        double diff_x = p1.x() - p2.x();
        double diff_y = p1.y() - p2.y();

        if (diff_x < 0)
            diff_x *= -1;
        if (diff_y < 0)
            diff_y *= -1;

        if (diff_x < 1.5 && diff_y < 1.5)
            return true;
        else
            return false;
    }

    void validateNeighbors(TrapMap trapMap){
//        std::cout << "Validazione in corso..." << std::endl;
        for (Trapezoid t: trapMap.getTrapezoids()){

            // validate left neighbors
            if (t.getUpperLeftNeigh() != nullptr && t.getBottomLeftNeigh() != nullptr){
                if (!trapMap.findID(*t.getUpperLeftNeigh())){
                    std::cerr << "t: " << t.getId() << " U-L: " << t.getUpperLeftNeigh()->getId() << " -> NOT FOUND" << std::endl;
                }
                if (!trapMap.findID(*t.getBottomLeftNeigh())){
                    std::cerr << "t: " << t.getId() << "B-L: " << t.getBottomLeftNeigh()->getId() << " -> NOT FOUND" << std::endl;
                }
                // controllo neighbors sx
                if (t.sameLeftNeighbor()){
                    // t ha gli stessi left neighbors
                    if (t.getUpperLeftNeigh()->sameRightNeighbor()){
                        // left neighbor ha gli stessi right neighbors

                        if (!(t.getUpperLeftNeigh()->getUpperRightNeigh()->getId() == t.getId()
                                && t.getUpperLeftNeigh()->getBottomRightNeigh()->getId() == t.getId())){
                            std::cerr << "ERRORE: " << t.getUpperLeftNeigh()->getId()<< "- current "<< t.getId() << " right neighbors errati!" << std::endl;
                        }
                    }else{
                        // left neighobr ha diversi right neighbors
                        if (pointsAreEquals(t.getLeftPoint(), t.getSegmentDown().p1())){
                            // t è l'upper right neighbors di t.getupperleft
                            if (t.getId() != t.getUpperLeftNeigh()->getUpperRightNeigh()->getId()){
                                std::cerr << "ERRORE: " << t.getUpperLeftNeigh()->getId()<< "- current "<< t.getId() << " U-R errato!" << std::endl;
                            }
                        }else{
                            // t è il bottom right neighbors di t.getupperleft
                            if (t.getId() != t.getUpperLeftNeigh()->getBottomRightNeigh()->getId()){
                                std::cerr << "ERRORE: " << t.getUpperLeftNeigh()->getId()<< "- current "<< t.getId() << " B-R errato!" << std::endl;
                            }
                        }

                    }
                }else{
                    // t ha diversi left neighbors
                    if (t.getUpperLeftNeigh()->getUpperRightNeigh()->getId() != t.getId() ||
                            t.getUpperLeftNeigh()->getBottomRightNeigh()->getId() != t.getId()){
                         std::cerr << "ERRORE: " << t.getUpperLeftNeigh()->getId() << "- current "<< t.getId() <<" U right errati!" << std::endl;
                    }
                    if (t.getBottomLeftNeigh()->getUpperRightNeigh()->getId() != t.getId() ||
                            t.getBottomLeftNeigh()->getBottomRightNeigh()->getId() != t.getId()){
                         std::cerr << "ERRORE: " << t.getBottomLeftNeigh()->getId()<< "- current "<< t.getId() << " B right errati!" << std::endl;
                    }
                }
            }

//            if (t.getId()==69)
//                std::cout << "trovato" << std::endl;

            // validate right neighbors
            if (t.getUpperRightNeigh() != nullptr && t.getBottomRightNeigh() != nullptr){
                if (!trapMap.findID(*t.getUpperRightNeigh())){
                    std::cerr << "t: " << t.getId() << " U-L: " << t.getUpperRightNeigh()->getId() << " -> NOT FOUND" << std::endl;
                }
                if (!trapMap.findID(*t.getBottomRightNeigh())){
                    std::cerr << "t: " << t.getId() << "B-L: " << t.getBottomLeftNeigh()->getId() << " -> NOT FOUND" << std::endl;
                }
                // controllo neighbors sx
                if (t.sameRightNeighbor()){
                    // t ha gli stessi left neighbors
                    if (t.getUpperRightNeigh()->sameLeftNeighbor()){
                        // left neighbor ha gli stessi right neighbors

                        if (!(t.getUpperRightNeigh()->getUpperLeftNeigh()->getId() == t.getId()
                                && t.getUpperRightNeigh()->getBottomLeftNeigh()->getId() == t.getId())){
                            std::cerr << "ERRORE: " << t.getUpperRightNeigh()->getId()<< "- current "<< t.getId() << " left neighbors errati!" << std::endl;
                        }
                    }else{
                        // left neighobr ha diversi right neighbors
                        if (pointsAreEquals(t.getRightPoint(), t.getSegmentDown().p2())){
                            // t è l'upper right neighbors di t.getupperleft
                            if (t.getId() != t.getUpperRightNeigh()->getUpperLeftNeigh()->getId()){
                                std::cerr << "ERRORE: " << t.getUpperRightNeigh()->getId()<< "- current "<< t.getId() << " U-L errato!" << std::endl;
                            }
                        }else{
                            // t è il bottom right neighbors di t.getupperleft
                            if (t.getId() != t.getUpperRightNeigh()->getBottomLeftNeigh()->getId()){
                                std::cerr << "ERRORE: " << t.getUpperRightNeigh()->getId()<< "- current "<< t.getId() << " B-L errato!" << std::endl;
                            }
                        }

                    }
                }else{
                    // t ha diversi left neighbors
                    if (t.getUpperRightNeigh()->getUpperLeftNeigh()->getId() != t.getId() ||
                            t.getUpperRightNeigh()->getBottomLeftNeigh()->getId() != t.getId()){
                         std::cerr << "ERRORE: " << t.getUpperRightNeigh()->getId() << "- current "<< t.getId() <<" U left errati!" << std::endl;
                    }
                    if (t.getBottomRightNeigh()->getUpperLeftNeigh()->getId() != t.getId() ||
                            t.getBottomRightNeigh()->getBottomLeftNeigh()->getId() != t.getId()){
                         std::cerr << "ERRORE: " << t.getBottomRightNeigh()->getId()<< "- current "<< t.getId() << " B left errati!" << std::endl;
                    }
                }
            }

        }
    }

}


