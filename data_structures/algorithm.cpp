#include "algorithm.h"
#include "cg3/geometry/utils2.h"
namespace Algorithm
{
    /**
     * @brief build the trapezoidal map and its associated dag for the inserted segment
     * @param dag DAG data structure
     * @param trapMap trapezoidal map data structure
     * @param segment segment that has to be inserted
     */
    void buildTrapMapDag(Dag& dag, TrapMap& trapMap, const cg3::Segment2d& segment){
        cg3::Segment2d segment_oriented = segment;
        DagNode* trapNodeQuery;
        std::vector<Trapezoid*> trapsCrossed, trapsCreated;
        bool left_coincident = false, right_coincident = false;

        // check if the segment is left to right oriented
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
         // the segment intersect more trapedoids

             // create new trapezoids
             trapsCreated = trapMap.newTrapezoidsMultipleSplit(segment_oriented, trapsCrossed, left_coincident, right_coincident);

             // update the dag with the new created trapezoids
             dag.updateDagMultipleSplit(trapsCreated, trapsCrossed, segment_oriented, left_coincident, right_coincident);

             // delete trapezoids crossed by the segment
             trapMap.deleteTrapezoidByRef(trapsCrossed);
         }
 //         printDag(dag);
 //         printNeigh(trapMap);
//          validateNeighbors(trapMap);

    }
    /**
     * @brief Execute the query point on the DAG data structure
     * @param dag DAG data structure
     * @param segment segment inserted
     * @return Dag node containing trapezoid information found
     */
    DagNode* query(Dag& dag, const cg3::Segment2d& segment){
        DagNode* tmp = dag.getRoot();
        cg3::Point2d *p1, *q1;
        cg3::Segment2d* s;
        cg3::Point2d queryPoint = segment.p1();
        double m_current_segment, m_new_segment;

        while(true){
            switch(tmp->getData().type){
                case DagNode::TypeNode::Trapezoid:
                    return tmp;

                case DagNode::TypeNode::Left:
                    p1 = (cg3::Point2d*)tmp->getData().obj;
                    if ( queryPoint.x() >= p1->x()){
                        tmp = tmp->right;
                    }else{
                        tmp = tmp->left;
                    }
                    break;

                case DagNode::TypeNode::Right:
                    q1 = (cg3::Point2d*)tmp->getData().obj;
                    if (queryPoint.x() >= q1->x()){
                        tmp = tmp->right;
                    }else{
                        tmp = tmp->left;
                    }
                    break;

                case DagNode::TypeNode::Segment:
                    s = (cg3::Segment2d*)tmp->getData().obj;
                    if (Algorithm::pointIsAboveSegment(*s,queryPoint)){
                        tmp = tmp->left;
                    }else if (cg3::isPointAtRight(*s, queryPoint)){
                        tmp = tmp->right;
                    }
                    else{
                        m_current_segment = (s->p2().y() - s->p1().y())/(s->p2().x() - s->p1().x());
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
    /**
     * @brief Execute the follow segment algorithm
     * @param segment segment inserted
     * @param trap first trapezoid crossed by segment
     * @return Vector of the trapezoids crossed by the segment
     */
    std::vector<Trapezoid*> followSegment(const cg3::Segment2d &segment, DagNode* trap){
        Trapezoid *t = (Trapezoid*)trap->getData().obj;
        std::vector<Trapezoid*> trapezoids;

        trapezoids.push_back((Trapezoid*)trap->getData().obj);

        // run until the second end-point is on the right with respect to the right point of the trapezoid
        while (segment.p2().x() > t->getRightPoint().x()) {

            if (Algorithm::pointIsAboveSegment(segment, t->getRightPoint())){
                trapezoids.push_back(t->getBottomRightNeigh());
                t = t->getBottomRightNeigh();
            }else{
                trapezoids.push_back(t->getTopRightNeigh());
                t = t->getTopRightNeigh();
            }
        }

        return trapezoids;

    }
    /**
     * @brief Check if a point is above a segment
     * @param segment the segment
     * @param point the point
     * @return true if the point is above the segment, otherwise
     */
    bool pointIsAboveSegment(cg3::Segment2d segment, cg3::Point2d point){

        cg3::Point2d v1 = cg3::Point2d(segment.p2().x()-segment.p1().x(),segment.p2().y()-segment.p1().y());
        cg3::Point2d v2 = cg3::Point2d(point.x()-segment.p1().x(),point.y()-segment.p1().y());
        double cross_product = v1.x() * v2.y() - v1.y() * v2.x();

        if (cross_product > 0){
            // the point is above
            return true;
        }else{
            // the point is below or lie on
            return false;
        }
    }

    /**
     * @brief Check if two points are equal
     * @param p1 First point
     * @param p2 Second point
     * @return true if the points are equal, otherwise
     */
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

    void printNeigh(TrapMap trapMap){
        bool neig = true, points = false;
        std::string s;
        for (Trapezoid t : trapMap.getTrapezoids()){
            if (neig){
                std::cout << "ID: " << t.getId() << std::endl;

                if (t.getTopLeftNeigh() != nullptr && t.getBottomLeftNeigh() != nullptr)
                    std::cout << "U-L " << t.getTopLeftNeigh()->getId() << " B-L " << t.getBottomLeftNeigh()->getId();
                else
                    std::cout << "U-L " << "NULL " << " B-L " << "NULL";


                if (t.getTopRightNeigh() != nullptr && t.getBottomRightNeigh() != nullptr)
                    std::cout << " U-R " << t.getTopRightNeigh()->getId() << " B-R " << t.getBottomRightNeigh()->getId() << std::endl;
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
                Trapezoid* t = (Trapezoid*)tmp->getData().obj;
                    std::cout << "T" << t->getId() << " - " << tmp << std::endl;
                    break;

            }

        }

    }
    void validateNeighbors(TrapMap trapMap){
//        std::cout << "Validazione in corso..." << std::endl;
        for (Trapezoid t: trapMap.getTrapezoids()){

            // validate left neighbors
            if (t.getTopLeftNeigh() != nullptr && t.getBottomLeftNeigh() != nullptr){
                if (!trapMap.findID(*t.getTopLeftNeigh())){
                    std::cerr << "t: " << t.getId() << " U-L: " << t.getTopLeftNeigh()->getId() << " -> NOT FOUND" << std::endl;
                }
                if (!trapMap.findID(*t.getBottomLeftNeigh())){
                    std::cerr << "t: " << t.getId() << "B-L: " << t.getBottomLeftNeigh()->getId() << " -> NOT FOUND" << std::endl;
                }
                // controllo neighbors sx
                if (t.sameLeftNeighbor()){
                    // t ha gli stessi left neighbors
                    if (t.getTopLeftNeigh()->sameRightNeighbor()){
                        // left neighbor ha gli stessi right neighbors

                        if (!(t.getTopLeftNeigh()->getTopRightNeigh()->getId() == t.getId()
                                && t.getTopLeftNeigh()->getBottomRightNeigh()->getId() == t.getId())){
                            std::cerr << "ERRORE: " << t.getTopLeftNeigh()->getId()<< "- current "<< t.getId() << " right neighbors errati!" << std::endl;
                        }
                    }else{
                        // left neighobr ha diversi right neighbors
                        if (pointsAreEquals(t.getLeftPoint(), t.getSegmentDown().p1())){
                            // t è l'upper right neighbors di t.getupperleft
                            if (t.getId() != t.getTopLeftNeigh()->getTopRightNeigh()->getId()){
                                std::cerr << "ERRORE: " << t.getTopLeftNeigh()->getId()<< "- current "<< t.getId() << " U-R errato!" << std::endl;
                            }
                        }else{
                            // t è il bottom right neighbors di t.getupperleft
                            if (t.getId() != t.getTopLeftNeigh()->getBottomRightNeigh()->getId()){
                                std::cerr << "ERRORE: " << t.getTopLeftNeigh()->getId()<< "- current "<< t.getId() << " B-R errato!" << std::endl;
                            }
                        }

                    }
                }else{
                    // t ha diversi left neighbors
                    if (t.getTopLeftNeigh()->getTopRightNeigh()->getId() != t.getId() ||
                            t.getTopLeftNeigh()->getBottomRightNeigh()->getId() != t.getId()){
                         std::cerr << "ERRORE: " << t.getTopLeftNeigh()->getId() << "- current "<< t.getId() <<" U right errati!" << std::endl;
                    }
                    if (t.getBottomLeftNeigh()->getTopRightNeigh()->getId() != t.getId() ||
                            t.getBottomLeftNeigh()->getBottomRightNeigh()->getId() != t.getId()){
                         std::cerr << "ERRORE: " << t.getBottomLeftNeigh()->getId()<< "- current "<< t.getId() << " B right errati!" << std::endl;
                    }
                }
            }

//            if (t.getId()==69)
//                std::cout << "trovato" << std::endl;

            // validate right neighbors
            if (t.getTopRightNeigh() != nullptr && t.getBottomRightNeigh() != nullptr){
                if (!trapMap.findID(*t.getTopRightNeigh())){
                    std::cerr << "t: " << t.getId() << " U-L: " << t.getTopRightNeigh()->getId() << " -> NOT FOUND" << std::endl;
                }
                if (!trapMap.findID(*t.getBottomRightNeigh())){
                    std::cerr << "t: " << t.getId() << "B-L: " << t.getBottomLeftNeigh()->getId() << " -> NOT FOUND" << std::endl;
                }
                // controllo neighbors sx
                if (t.sameRightNeighbor()){
                    // t ha gli stessi left neighbors
                    if (t.getTopRightNeigh()->sameLeftNeighbor()){
                        // left neighbor ha gli stessi right neighbors

                        if (!(t.getTopRightNeigh()->getTopLeftNeigh()->getId() == t.getId()
                                && t.getTopRightNeigh()->getBottomLeftNeigh()->getId() == t.getId())){
                            std::cerr << "ERRORE: " << t.getTopRightNeigh()->getId()<< "- current "<< t.getId() << " left neighbors errati!" << std::endl;
                        }
                    }else{
                        // left neighobr ha diversi right neighbors
                        if (pointsAreEquals(t.getRightPoint(), t.getSegmentDown().p2())){
                            // t è l'upper right neighbors di t.getupperleft
                            if (t.getId() != t.getTopRightNeigh()->getTopLeftNeigh()->getId()){
                                std::cerr << "ERRORE: " << t.getTopRightNeigh()->getId()<< "- current "<< t.getId() << " U-L errato!" << std::endl;
                            }
                        }else{
                            // t è il bottom right neighbors di t.getupperleft
                            if (t.getId() != t.getTopRightNeigh()->getBottomLeftNeigh()->getId()){
                                std::cerr << "ERRORE: " << t.getTopRightNeigh()->getId()<< "- current "<< t.getId() << " B-L errato!" << std::endl;
                            }
                        }

                    }
                }else{
                    // t ha diversi left neighbors
                    if (t.getTopRightNeigh()->getTopLeftNeigh()->getId() != t.getId() ||
                            t.getTopRightNeigh()->getBottomLeftNeigh()->getId() != t.getId()){
                         std::cerr << "ERRORE: " << t.getTopRightNeigh()->getId() << "- current "<< t.getId() <<" U left errati!" << std::endl;
                    }
                    if (t.getBottomRightNeigh()->getTopLeftNeigh()->getId() != t.getId() ||
                            t.getBottomRightNeigh()->getBottomLeftNeigh()->getId() != t.getId()){
                         std::cerr << "ERRORE: " << t.getBottomRightNeigh()->getId()<< "- current "<< t.getId() << " B left errati!" << std::endl;
                    }
                }
            }

        }
    }
}


