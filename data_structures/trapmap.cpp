#include "trapmap.h"

#include <cg3/viewer/opengl_objects/opengl_objects2.h>
#include "algorithm.h"
/**
 * @brief TrapMap::TrapMap Default constructor of the trapezoidal map object
 */
TrapMap::TrapMap()
{
    // initializie trapezoidal map
    init();
}
/**
 * @brief TrapMap::init Add the default trapezoid (corrisponding to the bounding box) to the trapezoidal map
 */
void TrapMap::init(){
    addNewTrapezoid();
}

/**
 * @brief TrapMap::getTrapezoids Return the list of the trapezoids in the trapezoidal map
 * @return List of the trapezoids
 */
const std::list<Trapezoid> TrapMap::getTrapezoids() const{
    return trapezoids;
}

/**
 * @brief TrapMap::getFirstTrapezoid Return the first trapezoid in the list of the trapezoids
 * @return The first trapezoid
 */
Trapezoid* TrapMap::getFirstTrapezoid(){
    std::list<Trapezoid>::iterator it=trapezoids.begin();
    return &*it;
}

/**
 * @brief TrapMap::deleteTrapezoidByRef Elimination of the trapezoids in the list of the trapezoids
 * @param traps List of the trapezoids that have to be deleted
 */
void TrapMap::deleteTrapezoidByRef(std::vector<Trapezoid*> traps){
    for (const Trapezoid* t : traps){
        trapezoids.erase(t->getRefToIter());
    }
}
/**
 * @brief TrapMap::clear Elimination of the entire list of the trapezoids
 */
void TrapMap::clear(){
    trapezoids.clear();
}

/**
  * @brief TrapMap::addNewTrapezoid Insertion of a default trapezoid in the list of trapezoids
  * @return The new trapezoid inserted
  */
 Trapezoid* TrapMap::addNewTrapezoid(){
    // add a default trapezoid to the list
    trapezoids.push_back(Trapezoid());

    trapezoids.back().setRefToIter(--(trapezoids.end()));

    return  &*(--(trapezoids.end()));
}
 /**
 * @brief TrapMap::findIntersectionVerticalLine Find the point that intersect a segment and x point coordinate
 * @param s The segment
 * @param px x point coordinate
 * @return The point intersected
 */
cg3::Point2d TrapMap::findIntersectionVerticalLine(const cg3::Segment2d& s, const cg3::Point2d& px){

    double a, b, c, y;

    a = s.p1().y() - s.p2().y();
    b = s.p2().x() - s.p1().x();
    c = ((s.p1().x() * s.p2().y()) - (s.p2().x() * s.p1().y()));

    y = (-c -(a * px.x())) / b;

    return cg3::Point2d(px.x(), y);
}
std::vector<Trapezoid*> TrapMap::newTrapezoidsSingleSplit(const cg3::Segment2d& segment,  Trapezoid* bb1, bool &left_coincident, bool &right_coincident){
    cg3::Segment2d topS, bottomS;
    cg3::Point2d leftP, rightP, p1, q1;
    cg3::Color colorT;
    Trapezoid *left, *top, *bottom, *right;

    std::vector<Trapezoid*> trapForDag;
    Trapezoid bb = *bb1;

    int idLastTrap = trapezoids.back().getId();


    p1 = segment.p1();
    q1 = segment.p2();

    if (p1.operator==(bb.getLeftPoint())){
        left_coincident = true;
    }
    if (q1.operator==(bb.getRightPoint())){
        right_coincident = true;
    }

    // create and add a new default trapezoids in the trapezoid's list
    if (!left_coincident)
        left = addNewTrapezoid();
    top = addNewTrapezoid();
    bottom = addNewTrapezoid();
    if (!right_coincident)
        right = addNewTrapezoid();

    ////////////////////
    // LEFT trapezoid //
    ////////////////////
    if (!left_coincident){
        colorT = cg3::Color(rand()%256, rand()%256, rand()%256);
        left->setTrapezoid(++idLastTrap,
                           cg3::Segment2d(bb.getSegmentUp().p1(),findIntersectionVerticalLine(bb.getSegmentUp(), p1)),
                           cg3::Segment2d(bb.getSegmentDown().p1(),findIntersectionVerticalLine(bb.getSegmentDown(), p1)),
                           bb.getLeftPoint(), p1,
                           colorT);

    }

    ////////////////////
    // TOP trapezoid  //
    ////////////////////
    colorT = cg3::Color(rand()%256, rand()%256, rand()%256);
    top->setTrapezoid(++idLastTrap,
                      cg3::Segment2d(findIntersectionVerticalLine(bb.getSegmentUp(), p1),findIntersectionVerticalLine(bb.getSegmentUp(), q1)),
                      segment,
                      p1, q1,
                      colorT);

    //////////////////////
    // BOTTOM trapezoid //
    //////////////////////

    colorT = cg3::Color(rand()%256, rand()%256, rand()%256);

    bottom->setTrapezoid(++idLastTrap,
                         segment,
                         cg3::Segment2d(findIntersectionVerticalLine(bb.getSegmentDown(), p1),findIntersectionVerticalLine(bb.getSegmentDown(), q1)),
                         p1, q1,
                         colorT);

    /////////////////////
    // RIGHT trapezoid //
    /////////////////////

    if (!right_coincident){
        colorT = cg3::Color(rand()%256, rand()%256, rand()%256);
        right->setTrapezoid(++idLastTrap,
                            cg3::Segment2d(findIntersectionVerticalLine(bb.getSegmentUp(), q1), bb.getSegmentUp().p2()),
                            cg3::Segment2d(findIntersectionVerticalLine(bb.getSegmentDown(), q1), bb.getSegmentDown().p2()),
                            q1, bb.getRightPoint(),
                            colorT);
    }

    //////////////////////
    // SETTING NEIGHBOR //
    //////////////////////

    if (!left_coincident && !right_coincident){
        left->setNeighbor(bb.getUpperLeftNeigh(), bb.getBottomLeftNeigh(), top, bottom);
        if (bb.getUpperLeftNeigh() != nullptr && bb.getBottomLeftNeigh() != nullptr
                && (bb.getUpperLeftNeigh()->getId() == bb.getBottomLeftNeigh()->getId())){
            // bb ha gli stessi left neighbors
            if (bb.getUpperLeftNeigh()->sameRightNeighbor()){
                // il left neighbor di bb ha gli stessi right neighbors
                bb.getUpperLeftNeigh()->setUpperRightNeigh(left);
                bb.getUpperLeftNeigh()->setBottomRightNeigh(left);
            }else{
                // il left neighbor di bb ha diversi right neighbors
                if (bb.getId() == bb.getUpperLeftNeigh()->getBottomRightNeigh()->getId()){
                    // il nuovo segment si trova sotto
                    bb.getBottomLeftNeigh()->setBottomRightNeigh(left);
                }
                else{
                    // il nuovo segmento si trova sopra
                    bb.getUpperLeftNeigh()->setUpperRightNeigh(left);
                }
            }
        }else if(bb.getUpperLeftNeigh() != nullptr && bb.getBottomLeftNeigh() != nullptr
                 && (bb.getUpperLeftNeigh()->getId() != bb.getBottomLeftNeigh()->getId())){
            bb.getUpperLeftNeigh()->setUpperRightNeigh(left);
            bb.getUpperLeftNeigh()->setBottomRightNeigh(left);
            bb.getBottomLeftNeigh()->setUpperRightNeigh(left);
            bb.getBottomLeftNeigh()->setBottomRightNeigh(left);
        }

        top->setNeighbor(left, left, right, right);
        bottom->setNeighbor(left, left, right, right);

        right->setNeighbor(top, bottom, bb.getUpperRightNeigh(), bb.getBottomRightNeigh());
        if (bb.getUpperRightNeigh() != nullptr && bb.getBottomRightNeigh() != nullptr &&
                (bb.getUpperRightNeigh()->getId() == bb.getBottomRightNeigh()->getId())){
            if (bb.getUpperRightNeigh()->sameLeftNeighbor()){
                bb.getUpperRightNeigh()->setUpperLeftNeigh(right);
                bb.getUpperRightNeigh()->setBottomLeftNeigh(right);
            }else{
                if (bb.getId() == bb.getUpperRightNeigh()->getBottomLeftNeigh()->getId()){
                    bb.getBottomRightNeigh()->setBottomLeftNeigh(right);
                }else{
                    bb.getUpperRightNeigh()->setUpperLeftNeigh(right);
                }
            }
        }else if (bb.getUpperRightNeigh() != nullptr && bb.getBottomRightNeigh() != nullptr

                  && (bb.getUpperRightNeigh()->getId() != bb.getBottomRightNeigh()->getId())){
            bb.getUpperRightNeigh()->setUpperLeftNeigh(right);
            bb.getUpperRightNeigh()->setBottomLeftNeigh(right);
            bb.getBottomRightNeigh()->setUpperLeftNeigh(right);
            bb.getBottomRightNeigh()->setBottomLeftNeigh(right);
        }

        trapForDag.push_back(left);
        trapForDag.push_back(top);
        trapForDag.push_back(bottom);
        trapForDag.push_back(right);

    }else if (left_coincident && right_coincident){
        // sia right che left coincidence
        if (top->isLeftDegenerate() && bottom->isRightDegenerate()){
            top->setNeighbor(nullptr, nullptr, bb.getUpperRightNeigh(), bb.getUpperRightNeigh());
            bottom->setNeighbor(bb.getUpperLeftNeigh(), bb.getUpperLeftNeigh(), nullptr, nullptr);

            if (bb.getUpperLeftNeigh()->sameRightNeighbor()){
                bb.getUpperLeftNeigh()->setUpperRightNeigh(bottom);
                bb.getUpperLeftNeigh()->setBottomRightNeigh(bottom);
            }else
                bb.getUpperLeftNeigh()->setBottomRightNeigh(bottom);

            if (bb.getUpperRightNeigh()->sameLeftNeighbor()){
                bb.getUpperRightNeigh()->setUpperLeftNeigh(top);
                bb.getUpperRightNeigh()->setBottomLeftNeigh(top);
            }else
                bb.getUpperRightNeigh()->setUpperLeftNeigh(top);



        }else if (top->isRightDegenerate() && bottom->isLeftDegenerate()){
            top->setNeighbor(bb.getUpperLeftNeigh(), bb.getUpperLeftNeigh(), nullptr, nullptr);
            bottom->setNeighbor(nullptr, nullptr, bb.getUpperRightNeigh(), bb.getUpperRightNeigh());

            if (bb.getUpperLeftNeigh()->sameRightNeighbor()){
                bb.getUpperLeftNeigh()->setUpperRightNeigh(top);
                bb.getUpperLeftNeigh()->setBottomRightNeigh(top);
            }else
                bb.getUpperLeftNeigh()->setUpperRightNeigh(top);

            if (bb.getUpperRightNeigh()->sameLeftNeighbor()){
                bb.getUpperRightNeigh()->setUpperLeftNeigh(bottom);
                bb.getUpperRightNeigh()->setBottomLeftNeigh(bottom);
            }else
                bb.getUpperRightNeigh()->setBottomLeftNeigh(bottom);

        } else if (top->isLeftDegenerate() && bottom->isLeftDegenerate()){
            top->setNeighbor(nullptr, nullptr, bb.getUpperRightNeigh(), bb.getUpperRightNeigh());
            bottom->setNeighbor(nullptr, nullptr, bb.getBottomRightNeigh(), bb.getBottomRightNeigh());

            bb.getUpperRightNeigh()->setUpperLeftNeigh(top);
            bb.getUpperRightNeigh()->setBottomLeftNeigh(top);

            bb.getBottomRightNeigh()->setUpperLeftNeigh(bottom);
            bb.getBottomRightNeigh()->setBottomLeftNeigh(bottom);

        }else if (top->isRightDegenerate() && bottom->isRightDegenerate()){
            top->setNeighbor(bb.getUpperLeftNeigh(), bb.getUpperLeftNeigh(), nullptr, nullptr);
            bottom->setNeighbor(bb.getBottomLeftNeigh(), bb.getBottomLeftNeigh(), nullptr, nullptr);

            bb.getUpperLeftNeigh()->setUpperRightNeigh(top);
            bb.getUpperLeftNeigh()->setBottomRightNeigh(top);

            bb.getBottomLeftNeigh()->setUpperRightNeigh(bottom);
            bb.getBottomLeftNeigh()->setBottomRightNeigh(bottom);
        }

        else if (top->isLeftDegenerate() || top->isRightDegenerate()){

            if (top->isLeftDegenerate()){
                top->setNeighbor(nullptr, nullptr, bb.getUpperRightNeigh(), bb.getUpperRightNeigh());
                bb.getUpperRightNeigh()->setUpperLeftNeigh(top);
                bb.getUpperRightNeigh()->setBottomLeftNeigh(top);
            }
            else{
                top->setNeighbor(bb.getUpperLeftNeigh(), bb.getUpperLeftNeigh(), nullptr, nullptr);
                bb.getUpperLeftNeigh()->setUpperRightNeigh(top);
                bb.getUpperLeftNeigh()->setBottomRightNeigh(top);
            }

            bottom->setNeighbor(bb.getBottomLeftNeigh(), bb.getBottomLeftNeigh(), bb.getBottomRightNeigh(), bb.getBottomRightNeigh());

            // update left neighbors
            if (Algorithm::pointsAreEquals(bb.getLeftPoint(), bb.getSegmentUp().p1())){
                // ho un solo left neighbor
                if (bb.getUpperLeftNeigh()->sameRightNeighbor()){
                     bb.getUpperLeftNeigh()->setUpperRightNeigh(bottom);
                    bb.getUpperLeftNeigh()->setBottomRightNeigh(bottom);
                }else
                    bb.getUpperLeftNeigh()->setBottomRightNeigh(bottom);
            }else{
                // ho diversi left neignors
                bb.getBottomLeftNeigh()->setUpperRightNeigh(bottom);
                bb.getBottomLeftNeigh()->setBottomRightNeigh(bottom);
            }
            // update right neighbors
            if (Algorithm::pointsAreEquals(bb.getRightPoint(), bb.getSegmentUp().p2())){
                // ho un solo right neighbor

                if (bb.getUpperRightNeigh()->sameLeftNeighbor()){
                    bb.getUpperRightNeigh()->setUpperLeftNeigh(bottom);
                    bb.getUpperRightNeigh()->setBottomLeftNeigh(bottom);
                }else
                    bb.getUpperRightNeigh()->setBottomLeftNeigh(bottom);
            }else{
                // ho diversi right neignors
                bb.getBottomRightNeigh()->setUpperLeftNeigh(bottom);
                bb.getBottomRightNeigh()->setBottomLeftNeigh(bottom);
            }

        }else if (bottom->isLeftDegenerate() || bottom->isRightDegenerate()){

             top->setNeighbor(bb.getUpperLeftNeigh(), bb.getUpperLeftNeigh(), bb.getUpperRightNeigh(), bb.getUpperRightNeigh());

            if (bottom->isLeftDegenerate()){
                bottom->setNeighbor(nullptr, nullptr, bb.getBottomRightNeigh(), bb.getBottomRightNeigh());
                bb.getBottomRightNeigh()->setUpperLeftNeigh(bottom);
                bb.getBottomRightNeigh()->setBottomLeftNeigh(bottom);
            }
            else{
                bottom->setNeighbor(bb.getBottomLeftNeigh(), bb.getBottomLeftNeigh(), nullptr, nullptr);
                bb.getBottomLeftNeigh()->setUpperRightNeigh(bottom);
                bb.getBottomLeftNeigh()->setBottomRightNeigh(bottom);
            }

            // update left neighbors
            if (Algorithm::pointsAreEquals(bb.getLeftPoint(), bb.getSegmentDown().p1())){
                // ho un solo left neighbor

                if (bb.getUpperLeftNeigh()->sameRightNeighbor()){
                    bb.getUpperLeftNeigh()->setUpperRightNeigh(top);
                    bb.getUpperLeftNeigh()->setBottomRightNeigh(top);
                }else
                    bb.getUpperLeftNeigh()->setUpperRightNeigh(top);
            }else{
                // ho diversi left neignors
                bb.getUpperLeftNeigh()->setUpperRightNeigh(top);
                bb.getUpperLeftNeigh()->setBottomRightNeigh(top);
            }
            // update right neighbors
            if (Algorithm::pointsAreEquals(bb.getRightPoint(), bb.getSegmentDown().p2())){
                // ho un solo right neighbor
                if (bb.getUpperRightNeigh()->sameLeftNeighbor()){
                    bb.getUpperRightNeigh()->setUpperLeftNeigh(top);
                    bb.getUpperRightNeigh()->setBottomLeftNeigh(top);
                }else
                    bb.getUpperRightNeigh()->setUpperLeftNeigh(top);
            }else{
                // ho diversi right neignors
                bb.getUpperRightNeigh()->setUpperLeftNeigh(top);
                bb.getUpperRightNeigh()->setBottomLeftNeigh(top);
            }
        }else{
            // nessuno dei due è degenere
            top->setNeighbor(bb.getUpperLeftNeigh(), bb.getUpperLeftNeigh(), bb.getUpperRightNeigh(), bb.getUpperRightNeigh());
            bottom->setNeighbor(bb.getBottomLeftNeigh(), bb.getBottomLeftNeigh(), bb.getBottomRightNeigh(), bb.getBottomRightNeigh());

            bb.getUpperLeftNeigh()->setUpperRightNeigh(top);
            bb.getUpperLeftNeigh()->setBottomRightNeigh(top);
            bb.getUpperRightNeigh()->setUpperLeftNeigh(top);
            bb.getUpperRightNeigh()->setBottomLeftNeigh(top);

            bb.getBottomLeftNeigh()->setUpperRightNeigh(bottom);
            bb.getBottomLeftNeigh()->setBottomRightNeigh(bottom);
            bb.getBottomRightNeigh()->setUpperLeftNeigh(bottom);
            bb.getBottomRightNeigh()->setBottomLeftNeigh(bottom);
        }


        trapForDag.push_back(top);
        trapForDag.push_back(bottom);
    }else if (left_coincident){
       right->setNeighbor(top, bottom, bb.getUpperRightNeigh(), bb.getBottomRightNeigh());

       if (bb.getUpperRightNeigh() != nullptr && bb.getBottomRightNeigh() != nullptr){
           if (bb.sameRightNeighbor()){
               // same right neighbors
               if (!bb.getUpperRightNeigh()->sameLeftNeighbor()){
                   // right neighbor has different left neighbors
                   if (bb.getId() == bb.getUpperRightNeigh()->getBottomLeftNeigh()->getId()){
                       bb.getUpperRightNeigh()->setBottomLeftNeigh(right);
                   }else{
                       bb.getUpperRightNeigh()->setUpperLeftNeigh(right);
                   }
               }else{
                   bb.getUpperRightNeigh()->setUpperLeftNeigh(right);
                   bb.getUpperRightNeigh()->setBottomLeftNeigh(right);
               }
           }else{

               // different right neighbor
               bb.getUpperRightNeigh()->setUpperLeftNeigh(right);
               bb.getUpperRightNeigh()->setBottomLeftNeigh(right);

               bb.getBottomRightNeigh()->setUpperLeftNeigh(right);
               bb.getBottomRightNeigh()->setBottomLeftNeigh(right);
           }
       }

           if (bb.sameLeftNeighbor()){
               // should be a degenerate case
               if (bb.getUpperLeftNeigh() != nullptr && bb.getBottomLeftNeigh() != nullptr){
               if (!bb.getUpperLeftNeigh()->sameRightNeighbor()){
                   if (bb.getId() == bb.getUpperLeftNeigh()->getBottomRightNeigh()->getId()){
                       top->setNeighbor(nullptr, nullptr, right, right);
                       bottom->setNeighbor(bb.getUpperLeftNeigh(), bb.getBottomLeftNeigh(), right, right);

                       bb.getUpperLeftNeigh()->setBottomRightNeigh(bottom);
                   }else{
                       bottom->setNeighbor(nullptr, nullptr, right, right);
                       top->setNeighbor(bb.getUpperLeftNeigh(), bb.getBottomLeftNeigh(), right, right);

                       bb.getUpperLeftNeigh()->setUpperRightNeigh(top);
                   }
               }else{
                   // left neigh has same right neighbors
                   if (top->isLeftDegenerate()){
                       // segment is below
                       top->setNeighbor(nullptr, nullptr, right, right);
                       bottom->setNeighbor(bb.getUpperLeftNeigh(), bb.getBottomLeftNeigh(), right, right);

                       bb.getUpperLeftNeigh()->setUpperRightNeigh(bottom);
                       bb.getUpperLeftNeigh()->setBottomRightNeigh(bottom);
                   }else{
                       // segment is above
                       top->setNeighbor(bb.getUpperLeftNeigh(), bb.getBottomLeftNeigh(), right, right);
                       bottom->setNeighbor(nullptr, nullptr, right, right);

                       bb.getUpperLeftNeigh()->setUpperRightNeigh(top);
                       bb.getUpperLeftNeigh()->setBottomRightNeigh(top);
                   }
               }
               }else{
                   top->setNeighbor(nullptr, nullptr, right, right);
                   bottom->setNeighbor(nullptr, nullptr, right, right);
               }

           }else{
               // no degenerate case
               top->setNeighbor(bb.getUpperLeftNeigh(), bb.getUpperLeftNeigh(), right, right);
               bottom->setNeighbor(bb.getBottomLeftNeigh(), bb.getBottomLeftNeigh(), right, right);

               bb.getUpperLeftNeigh()->setUpperRightNeigh(top);
               bb.getUpperLeftNeigh()->setBottomRightNeigh(top);

               bb.getBottomLeftNeigh()->setUpperRightNeigh(bottom);
               bb.getBottomLeftNeigh()->setBottomRightNeigh(bottom);
           }


        trapForDag.push_back(top);
        trapForDag.push_back(bottom);
        trapForDag.push_back(right);

    }else if (right_coincident){
        left->setNeighbor(bb.getUpperLeftNeigh(), bb.getBottomLeftNeigh(), top, bottom);

        if (bb.getUpperLeftNeigh() != nullptr && bb.getBottomLeftNeigh() != nullptr){
            if (bb.sameLeftNeighbor())
            {
                // same left neighbor
                if (!bb.getUpperLeftNeigh()->sameRightNeighbor()){
                    //left neigh has different right neighbors
                    if (bb.getId() == bb.getUpperLeftNeigh()->getBottomRightNeigh()->getId()){
                        // il segmento si trova nel bottom right neigh del left neigh di bb
                        bb.getUpperLeftNeigh()->setBottomRightNeigh(left);
                    }else{
                        bb.getUpperLeftNeigh()->setUpperRightNeigh(left);
                    }
                }else{
                    //left neigh has the same right neighbors
                    bb.getUpperLeftNeigh()->setUpperRightNeigh(left);
                    bb.getUpperLeftNeigh()->setBottomRightNeigh(left);

                }

            }else{
                // different left neighbor
                bb.getUpperLeftNeigh()->setUpperRightNeigh(left);
                bb.getUpperLeftNeigh()->setBottomRightNeigh(left);

                bb.getBottomLeftNeigh()->setUpperRightNeigh(left);
                bb.getBottomLeftNeigh()->setBottomRightNeigh(left);
            }
        }

        if (bb.sameRightNeighbor()){
            // should be a degenerate case
            if (bb.getUpperRightNeigh() != nullptr && bb.getBottomRightNeigh() != nullptr){
                if (!bb.getUpperRightNeigh()->sameLeftNeighbor()){
                    if (bb.getId() == bb.getUpperRightNeigh()->getBottomLeftNeigh()->getId()){
                        top->setNeighbor(left, left, nullptr, nullptr);
                        bottom->setNeighbor(left, left, bb.getUpperRightNeigh(), bb.getBottomRightNeigh());

                        bb.getUpperRightNeigh()->setBottomLeftNeigh(bottom);
                    }else{
                        bottom->setNeighbor(left, left, nullptr, nullptr);
                        top->setNeighbor(left, left, bb.getUpperRightNeigh(), bb.getBottomRightNeigh());

                        bb.getUpperRightNeigh()->setUpperLeftNeigh(top);
                    }
                }else{
                    // right neigh has same left neighbors
                    if (top->isRightDegenerate()){
                        top->setNeighbor(left, left, nullptr, nullptr);
                        bottom->setNeighbor(left, left, bb.getUpperRightNeigh(), bb.getBottomRightNeigh());

                        bb.getUpperRightNeigh()->setUpperLeftNeigh(bottom);
                        bb.getUpperRightNeigh()->setBottomLeftNeigh(bottom);
                    }else{
                        bottom->setNeighbor(left, left, nullptr, nullptr);
                        top->setNeighbor(left, left, bb.getUpperRightNeigh(), bb.getBottomRightNeigh());

                        bb.getUpperRightNeigh()->setUpperLeftNeigh(top);
                        bb.getUpperRightNeigh()->setBottomLeftNeigh(top);
                    }
                }
            }else{
                // sia top che bottom sono degeneri
                top->setNeighbor(left, left, nullptr, nullptr);
                bottom->setNeighbor(left, left, nullptr, nullptr);
            }
        }else{
            // no degenerate case
            top->setNeighbor(left, left, bb.getUpperRightNeigh(), bb.getUpperRightNeigh());
            bottom->setNeighbor(left, left, bb.getBottomRightNeigh(), bb.getBottomRightNeigh());

            bb.getUpperRightNeigh()->setUpperLeftNeigh(top);
            bb.getUpperRightNeigh()->setBottomLeftNeigh(top);

            bb.getBottomRightNeigh()->setUpperLeftNeigh(bottom);
            bb.getBottomRightNeigh()->setBottomLeftNeigh(bottom);
        }


        trapForDag.push_back(left);
        trapForDag.push_back(top);
        trapForDag.push_back(bottom);

    }

    return trapForDag;
}

void TrapMap::splitInThreeRight(Trapezoid *d, Trapezoid *e, std::vector<Trapezoid*>& low, std::vector<Trapezoid*>& up,
                       Trapezoid& bb,  const cg3::Segment2d& segment, int id){
    cg3::Point2d p1, q1;
    cg3::Color colorT;

    int idLastTrap = id;

    p1 = segment.p1();
    q1 = segment.p2();
    if (e != nullptr){
        colorT = cg3::Color(rand()%256, rand()%256, rand()%256);
        if (Algorithm::pointIsAboveSegment(segment, bb.getLeftPoint())){
            // merging
            low.back()->setSegmentUp(cg3::Segment2d(low.back()->getSegmentUp().p1(), q1));
            low.back()->setSegmentDown(cg3::Segment2d(low.back()->getSegmentDown().p1(), findIntersectionVerticalLine(bb.getSegmentDown(),q1)));
            low.back()->setRightPoint(q1);

        }else{
            // merging
            up.back()->setSegmentUp(cg3::Segment2d(up.back()->getSegmentUp().p1(), findIntersectionVerticalLine(bb.getSegmentUp(),q1)));
            up.back()->setSegmentDown(cg3::Segment2d(up.back()->getSegmentDown().p1(), q1));
            up.back()->setRightPoint(q1);
        }


        colorT = cg3::Color(rand()%256, rand()%256, rand()%256);
        if (Algorithm::pointIsAboveSegment(segment, bb.getLeftPoint())){
            d->setTrapezoid(++idLastTrap,
                            cg3::Segment2d(bb.getSegmentUp().p1(),findIntersectionVerticalLine(bb.getSegmentUp(), q1)),
                            cg3::Segment2d(findIntersectionVerticalLine(segment, bb.getLeftPoint()), q1),
                            bb.getLeftPoint(), q1, colorT);

        }else{
            d->setTrapezoid(++idLastTrap,
                            cg3::Segment2d(findIntersectionVerticalLine(segment, bb.getLeftPoint()),q1),
                            cg3::Segment2d(bb.getSegmentDown().p1(),findIntersectionVerticalLine(bb.getSegmentDown(), q1)),
                            bb.getLeftPoint(), q1, colorT);

        }


        colorT = cg3::Color(rand()%256, rand()%256, rand()%256);
        if (Algorithm::pointIsAboveSegment(segment, bb.getLeftPoint())){
            e->setTrapezoid(++idLastTrap,
                            cg3::Segment2d(d->getSegmentUp().p2(), bb.getSegmentUp().p2()),
                            cg3::Segment2d(findIntersectionVerticalLine(bb.getSegmentDown(), q1), bb.getSegmentDown().p2()),
                            q1, bb.getRightPoint(), colorT);

        }else{
            e->setTrapezoid(++idLastTrap,
                            cg3::Segment2d(findIntersectionVerticalLine(bb.getSegmentUp(), q1), bb.getSegmentUp().p2()),
                            cg3::Segment2d(d->getSegmentDown().p2(), bb.getSegmentDown().p2()),
                            q1, bb.getRightPoint(), colorT);
        }
    }else{
        // right coincident
        colorT = cg3::Color(rand()%256, rand()%256, rand()%256);
        if (Algorithm::pointIsAboveSegment(segment, bb.getLeftPoint())){
            // merging
            low.back()->setSegmentUp(cg3::Segment2d(low.back()->getSegmentUp().p1(), q1));
            low.back()->setSegmentDown(cg3::Segment2d(low.back()->getSegmentDown().p1(), bb.getSegmentDown().p2()));
            low.back()->setRightPoint(q1);

        }else{
            // merging
            up.back()->setSegmentUp(cg3::Segment2d(up.back()->getSegmentUp().p1(), bb.getSegmentUp().p2()));
            up.back()->setSegmentDown(cg3::Segment2d(up.back()->getSegmentDown().p1(), q1));
            up.back()->setRightPoint(q1);
        }


        colorT = cg3::Color(rand()%256, rand()%256, rand()%256);
        if (Algorithm::pointIsAboveSegment(segment, bb.getLeftPoint())){
            d->setTrapezoid(++idLastTrap,
                            cg3::Segment2d(bb.getSegmentUp().p1(),findIntersectionVerticalLine(bb.getSegmentUp(), q1)),
                            cg3::Segment2d(findIntersectionVerticalLine(segment, bb.getLeftPoint()), q1),
                            bb.getLeftPoint(), q1, colorT);

        }else{
            d->setTrapezoid(++idLastTrap,
                            cg3::Segment2d(findIntersectionVerticalLine(segment, bb.getLeftPoint()),q1),
                            cg3::Segment2d(bb.getSegmentDown().p1(),findIntersectionVerticalLine(bb.getSegmentDown(), q1)),
                            bb.getLeftPoint(), q1, colorT);

        }
    }
}
void TrapMap::splitInThreeLeft(Trapezoid *a, Trapezoid *b, Trapezoid *c_1, std::vector<Trapezoid*>& low, std::vector<Trapezoid*>& up,
                           Trapezoid& bb, const cg3::Segment2d& segment, int id, bool& right_above_segment){
    cg3::Point2d p1, q1;
    cg3::Color colorT;

    int idLastTrap = id;

    p1 = segment.p1();
    q1 = segment.p2();

    if (a != nullptr){
        colorT = cg3::Color(rand()%256, rand()%256, rand()%256);
        a->setTrapezoid(++idLastTrap,
                        cg3::Segment2d(bb.getSegmentUp().p1(),findIntersectionVerticalLine(bb.getSegmentUp(),p1)),
                        cg3::Segment2d(bb.getSegmentDown().p1(),findIntersectionVerticalLine(bb.getSegmentDown(),p1)),
                        bb.getLeftPoint(), p1, colorT);

        if (right_above_segment){
            colorT = cg3::Color(rand()%256, rand()%256, rand()%256);
            b->setTrapezoid(++idLastTrap,
                            cg3::Segment2d(a->getSegmentUp().p2(), findIntersectionVerticalLine(bb.getSegmentUp(), bb.getRightPoint())),
                            cg3::Segment2d(p1,findIntersectionVerticalLine(segment, bb.getRightPoint())),
                            p1, bb.getRightPoint(), colorT);
            up.push_back(b);

            colorT = cg3::Color(rand()%256, rand()%256, rand()%256);
            c_1->setTrapezoid(++idLastTrap,
                              cg3::Segment2d(p1, b->getSegmentDown().p2()),
                              cg3::Segment2d(a->getSegmentDown().p2(), bb.getSegmentDown().p2()),
                              p1, b->getSegmentDown().p2(), colorT);

            low.push_back(c_1);
        }else{
            colorT = cg3::Color(rand()%256, rand()%256, rand()%256);
            b->setTrapezoid(++idLastTrap,
                            cg3::Segment2d(p1,findIntersectionVerticalLine(segment,bb.getRightPoint())),
                            cg3::Segment2d(a->getSegmentDown().p2(),bb.getSegmentDown().p2()),
                            p1, bb.getRightPoint(), colorT);
            low.push_back(b);

            colorT = cg3::Color(rand()%256, rand()%256, rand()%256);
            c_1->setTrapezoid(++idLastTrap,
                              cg3::Segment2d(a->getSegmentUp().p2(), bb.getSegmentUp().p2()),
                              cg3::Segment2d(p1, b->getSegmentUp().p2()),
                              p1, b->getSegmentUp().p2(), colorT);
            up.push_back(c_1);
        }
    }else{
        // left coincident
        if (right_above_segment){
            colorT = cg3::Color(rand()%256, rand()%256, rand()%256);
            b->setTrapezoid(++idLastTrap,
                           bb.getSegmentUp(),
                            cg3::Segment2d(p1,findIntersectionVerticalLine(segment, bb.getRightPoint())),
                            p1, bb.getRightPoint(), colorT);
            up.push_back(b);

            colorT = cg3::Color(rand()%256, rand()%256, rand()%256);
            c_1->setTrapezoid(++idLastTrap,
                              cg3::Segment2d(p1, b->getSegmentDown().p2()),
                              cg3::Segment2d(bb.getSegmentDown().p1(), bb.getSegmentDown().p2()),
                              p1, b->getSegmentDown().p2(), colorT);

            low.push_back(c_1);
        }else{
            colorT = cg3::Color(rand()%256, rand()%256, rand()%256);
            b->setTrapezoid(++idLastTrap,
                            cg3::Segment2d(p1,findIntersectionVerticalLine(segment,bb.getRightPoint())),
                            bb.getSegmentDown(),
                            p1, bb.getRightPoint(), colorT);
            low.push_back(b);
            colorT = cg3::Color(rand()%256, rand()%256, rand()%256);
            c_1->setTrapezoid(++idLastTrap,
                              cg3::Segment2d(bb.getSegmentUp().p1(), bb.getSegmentUp().p2()),
                              cg3::Segment2d(p1, b->getSegmentUp().p2()),
                              p1, b->getSegmentUp().p2(), colorT);
            up.push_back(c_1);
        }
    }

}
void TrapMap::assignNeighborsLeftSplit(Trapezoid t, Trapezoid *left_t, Trapezoid *no_merge_t, Trapezoid *merge_t, bool& left_coincident, bool& right_above_segment){
    //////////////////////
    // NEIGHBOR SETTING //
    //      LEFT_T      //
    //////////////////////

    if (!left_coincident){
        // assign new trapezoid's neighbor
        if (right_above_segment){
            left_t->setUpperRightNeigh(no_merge_t);
            left_t->setBottomRightNeigh(merge_t);
        }else{
            left_t->setUpperRightNeigh(merge_t);
            left_t->setBottomRightNeigh(no_merge_t);
        }

        // assign old neighbor inherited from current trapezoid t
        left_t->setUpperLeftNeigh(t.getUpperLeftNeigh());
        left_t->setBottomLeftNeigh(t.getBottomLeftNeigh());

        // update other trapezoids affected by left_t
        if (t.getBottomLeftNeigh() != nullptr && t.getUpperLeftNeigh() != nullptr){
            if (t.getBottomLeftNeigh()->getId() == t.getUpperLeftNeigh()->getId()){
                // upper and lower neighbor are the same
                if (t.getUpperLeftNeigh()->getUpperRightNeigh()->getId() == t.getUpperLeftNeigh()->getBottomRightNeigh()->getId()){
                    // left neighbor has the same right neighbors
                    t.getUpperLeftNeigh()->setUpperRightNeigh(left_t);
                    t.getUpperLeftNeigh()->setBottomRightNeigh(left_t);
                }else{
                    if (t.getId() == t.getUpperLeftNeigh()->getUpperRightNeigh()->getId()){
                        t.getUpperLeftNeigh()->setUpperRightNeigh(left_t);
                    }else{
                        t.getUpperLeftNeigh()->setBottomRightNeigh(left_t);
                    }
                }
            }else{
                // upper and lower neighbor are different trapezoids
                // i have to change both neighbor
                t.getUpperLeftNeigh()->setUpperRightNeigh(left_t);
                t.getUpperLeftNeigh()->setBottomRightNeigh(left_t);

                t.getBottomLeftNeigh()->setUpperRightNeigh(left_t);
                t.getBottomLeftNeigh()->setBottomRightNeigh(left_t);
            }
        }
    }
    //////////////////////
    // NEIGHBOR SETTING //
    //    NO_MERGE_T    //
    //////////////////////

    if(!left_coincident){
        // assign new trapezoid's neighbor
        no_merge_t->setUpperLeftNeigh(left_t);
        no_merge_t->setBottomLeftNeigh(left_t);

        // assign old neighbor inherited from current trapezoid t
        no_merge_t->setUpperRightNeigh(t.getUpperRightNeigh());
        no_merge_t->setBottomRightNeigh(t.getBottomRightNeigh());

        // update other trapezoids affected by no_merge_t
        if (t.getUpperRightNeigh()->getId() != t.getBottomRightNeigh()->getId()){
            if (right_above_segment){
                // update trapezoid that won't be modified
                t.getUpperRightNeigh()->setUpperLeftNeigh(no_merge_t);
                t.getUpperRightNeigh()->setBottomLeftNeigh(no_merge_t);
            }else{
                t.getBottomRightNeigh()->setUpperLeftNeigh(no_merge_t);
                t.getBottomRightNeigh()->setBottomLeftNeigh(no_merge_t);
            }
        }
    }else{
        //left coincident

        if (no_merge_t->isLeftDegenerate()){
            // è left degenerate

            no_merge_t->setNeighbor(nullptr, nullptr,
                                    t.getUpperRightNeigh(), t.getBottomRightNeigh());
            if (right_above_segment){
                if (!Algorithm::pointsAreEquals(t.getRightPoint(), t.getSegmentUp().p2())){
                    t.getUpperRightNeigh()->setUpperLeftNeigh(no_merge_t);
                    t.getUpperRightNeigh()->setBottomLeftNeigh(no_merge_t);
                }
            }else{
                if (!Algorithm::pointsAreEquals(t.getRightPoint(), t.getSegmentDown().p2())){
                    t.getBottomRightNeigh()->setUpperLeftNeigh(no_merge_t);
                    t.getBottomRightNeigh()->setBottomLeftNeigh(no_merge_t);
                }
            }
        }else{
            // non è left degenerate

            // UPDATE LEFT NEIGHBORS
            if (right_above_segment){
                // il right point si trova sopra il segmento

                if (Algorithm::pointsAreEquals(t.getLeftPoint(), t.getSegmentDown().p1())){
                    no_merge_t->setNeighbor(t.getUpperLeftNeigh(), t.getBottomLeftNeigh(),
                                            t.getUpperRightNeigh(), t.getBottomRightNeigh());
                    if (t.getUpperLeftNeigh()->sameRightNeighbor()){
                        t.getUpperLeftNeigh()->setUpperRightNeigh(no_merge_t);
                        t.getUpperLeftNeigh()->setBottomRightNeigh(no_merge_t);
                    }else
                        t.getUpperLeftNeigh()->setUpperRightNeigh(no_merge_t);
                }else{
                    no_merge_t->setNeighbor(t.getUpperLeftNeigh(), t.getUpperLeftNeigh(),
                                            t.getUpperRightNeigh(), t.getBottomRightNeigh());
                    t.getUpperLeftNeigh()->setUpperRightNeigh(no_merge_t);
                    t.getUpperLeftNeigh()->setBottomRightNeigh(no_merge_t);
                }

                if (!Algorithm::pointsAreEquals(t.getRightPoint(), t.getSegmentUp().p2())){
                    t.getUpperRightNeigh()->setUpperLeftNeigh(no_merge_t);
                    t.getUpperRightNeigh()->setBottomLeftNeigh(no_merge_t);
                }


            }else{
                // il right point si trova sotto il segmento
                if (Algorithm::pointsAreEquals(t.getLeftPoint(), t.getSegmentUp().p1())){
                    // i left neighbors sono uguali
                    no_merge_t->setNeighbor(t.getUpperLeftNeigh(), t.getBottomLeftNeigh(),
                                            t.getUpperRightNeigh(), t.getBottomRightNeigh());

                    if (t.getUpperLeftNeigh()->sameRightNeighbor()){
                        t.getUpperLeftNeigh()->setUpperRightNeigh(no_merge_t);
                        t.getUpperLeftNeigh()->setBottomRightNeigh(no_merge_t);
                    }else
                        t.getUpperLeftNeigh()->setBottomRightNeigh(no_merge_t);
                }else{
                    // i left neighbors sono diversi
                    no_merge_t->setNeighbor(t.getBottomLeftNeigh(), t.getBottomLeftNeigh(),
                                            t.getUpperRightNeigh(), t.getBottomRightNeigh());

                    t.getBottomLeftNeigh()->setUpperRightNeigh(no_merge_t);
                    t.getBottomLeftNeigh()->setBottomRightNeigh(no_merge_t);
                }

                if (!Algorithm::pointsAreEquals(t.getRightPoint(), t.getSegmentDown().p2())){
                    t.getBottomRightNeigh()->setUpperLeftNeigh(no_merge_t);
                    t.getBottomRightNeigh()->setBottomLeftNeigh(no_merge_t);
                }

            }
        }
    }

    //////////////////////
    // NEIGHBOR SETTING //
    //    MERGE_T       //
    //////////////////////

    if (!left_coincident){
        // assign new trapezoid's neighbor
        merge_t->setUpperLeftNeigh(left_t);
        merge_t->setBottomLeftNeigh(left_t);

        // assign old neighbor inherited from current trapezoid t
        if (right_above_segment){
            merge_t->setUpperRightNeigh(t.getBottomRightNeigh());
            merge_t->setBottomRightNeigh(t.getBottomRightNeigh());
        }else{
            merge_t->setUpperRightNeigh(t.getUpperRightNeigh());
            merge_t->setBottomRightNeigh(t.getUpperRightNeigh());
        }
    }else{
        // left coincident
        if (t.getUpperLeftNeigh() != nullptr && t.getBottomLeftNeigh() != nullptr){
            if (t.getUpperLeftNeigh()->getId() == t.getBottomLeftNeigh()->getId()){
                if (!merge_t->isLeftDegenerate()){
                    merge_t->setNeighbor(t.getUpperLeftNeigh(),t.getBottomLeftNeigh(),
                                         t.getUpperRightNeigh(), t.getBottomRightNeigh());
                    // gestire il caso in cui il vicino abbia gli stessi right neigh
                    if (t.getUpperLeftNeigh()->sameRightNeighbor()){
                        t.getUpperLeftNeigh()->setUpperRightNeigh(merge_t);
                        t.getUpperLeftNeigh()->setBottomRightNeigh(merge_t);
                    }else{
                        if(right_above_segment){
                            t.getUpperLeftNeigh()->setBottomRightNeigh(merge_t);
                        }else{
                            t.getUpperLeftNeigh()->setUpperRightNeigh(merge_t);
                        }
                    }
                }else{
                    merge_t->setNeighbor(nullptr, nullptr,
                                         t.getUpperRightNeigh(), t.getBottomRightNeigh());
                }
            }else{
                //current trapezoid has different left neighbor
                if (right_above_segment){
                    merge_t->setNeighbor(t.getBottomLeftNeigh(),t.getBottomLeftNeigh(),
                                         t.getBottomRightNeigh(), t.getBottomRightNeigh());

                    t.getBottomLeftNeigh()->setUpperRightNeigh(merge_t);
                    t.getBottomLeftNeigh()->setBottomRightNeigh(merge_t);

                }else{
                    merge_t->setNeighbor(t.getUpperLeftNeigh(),t.getUpperLeftNeigh(),
                                         t.getUpperRightNeigh(), t.getUpperRightNeigh());

                    t.getUpperLeftNeigh()->setUpperRightNeigh(merge_t);
                    t.getUpperLeftNeigh()->setBottomRightNeigh(merge_t);
                }
            }
        }

    }
}
void TrapMap::assignNeighborsRightSplit(Trapezoid t, Trapezoid *right, Trapezoid *no_merge_right_t, std::vector<Trapezoid*>& up_merging,
                                        std::vector<Trapezoid*>& low_merging, const cg3::Segment2d segment,  bool& right_coincident){
    //////////////////////
    // NEIGHBOR SETTING //
    //       RIGHT      //
    //////////////////////
    if (!right_coincident){
        right->setUpperRightNeigh(t.getUpperRightNeigh());
        right->setBottomRightNeigh(t.getBottomRightNeigh());
        if (Algorithm::pointIsAboveSegment(segment, t.getLeftPoint())){
            right->setUpperLeftNeigh(no_merge_right_t);
            right->setBottomLeftNeigh(low_merging.back());
        }else{
            right->setUpperLeftNeigh(up_merging.back());
            right->setBottomLeftNeigh(no_merge_right_t);
        }

        if (t.getBottomRightNeigh() != nullptr && t.getUpperRightNeigh() != nullptr){
            if (t.sameRightNeighbor()){
                if (Algorithm::pointsAreEquals(t.getUpperRightNeigh()->getLeftPoint(),
                                               t.getUpperRightNeigh()->getSegmentDown().p1()) ||
                        Algorithm::pointsAreEquals(t.getUpperRightNeigh()->getLeftPoint(),
                                                   t.getUpperRightNeigh()->getSegmentUp().p1()) ){
                    t.getUpperRightNeigh()->setUpperLeftNeigh(right);
                    t.getUpperRightNeigh()->setBottomLeftNeigh(right);
                }else{
                    if (t.getId() == t.getUpperRightNeigh()->getUpperLeftNeigh()->getId()){
                        t.getUpperRightNeigh()->setUpperLeftNeigh(right);
                    }else{
                        t.getUpperRightNeigh()->setBottomLeftNeigh(right);
                    }
                }
            }else{
                t.getUpperRightNeigh()->setUpperLeftNeigh(right);
                t.getUpperRightNeigh()->setBottomLeftNeigh(right);

                t.getBottomRightNeigh()->setUpperLeftNeigh(right);
                t.getBottomRightNeigh()->setBottomLeftNeigh(right);
            }
        }
    }
    //////////////////////
    // NEIGHBOR SETTING //
    //  NO_MERGE_RIGHT  //
    //////////////////////

    if (!right_coincident){
        no_merge_right_t->setUpperRightNeigh(right);
        no_merge_right_t->setBottomRightNeigh(right);

        if (Algorithm::pointIsAboveSegment(segment, t.getLeftPoint())){
            //il no merge si trova sopra
            if (Algorithm::pointsAreEquals(t.getLeftPoint(), t.getSegmentUp().p1())){
                // il trapezoide corrente non ha neighbor diversi

                no_merge_right_t->setUpperLeftNeigh(up_merging.back());
                no_merge_right_t->setBottomLeftNeigh(up_merging.back());

                if (Algorithm::pointsAreEquals(t.getUpperLeftNeigh()->getRightPoint(), t.getUpperLeftNeigh()->getSegmentUp().p2())){
                    // t.getupperleft ha lo stesso neighbor a destra
                    up_merging.back()->setUpperRightNeigh(no_merge_right_t);
                    up_merging.back()->setBottomRightNeigh(no_merge_right_t);
                }else
                    // t.getupperleft ha neigbors diversi a destra
                    up_merging.back()->setBottomRightNeigh(no_merge_right_t);
            }else{
                // neighbors diversi
                no_merge_right_t->setUpperLeftNeigh(t.getUpperLeftNeigh());
                no_merge_right_t->setBottomLeftNeigh(up_merging.back());

                up_merging.back()->setUpperRightNeigh(no_merge_right_t);
                up_merging.back()->setBottomRightNeigh(no_merge_right_t);

                // non sto considerando se t.getupperleft abbia neighbors diversi....possibile?
                t.getUpperLeftNeigh()->setUpperRightNeigh(no_merge_right_t);
                t.getUpperLeftNeigh()->setBottomRightNeigh(no_merge_right_t);
            }
        }else{
            // il no merge si trova sotto
            if (Algorithm::pointsAreEquals(t.getLeftPoint(), t.getSegmentDown().p1())){
                // t ha un solo neighbor
                no_merge_right_t->setUpperLeftNeigh(low_merging.back());
                no_merge_right_t->setBottomLeftNeigh(low_merging.back());

                if(Algorithm::pointsAreEquals(t.getUpperLeftNeigh()->getRightPoint(), t.getUpperLeftNeigh()->getSegmentDown().p2())){
                    low_merging.back()->setUpperRightNeigh(no_merge_right_t);
                    low_merging.back()->setBottomRightNeigh(no_merge_right_t);
                } else{
                    low_merging.back()->setUpperRightNeigh(no_merge_right_t);
                }

            }else{
                no_merge_right_t->setUpperLeftNeigh(low_merging.back());
                no_merge_right_t->setBottomLeftNeigh(t.getBottomLeftNeigh());

                low_merging.back()->setUpperRightNeigh(no_merge_right_t);
                low_merging.back()->setBottomRightNeigh(no_merge_right_t);

                t.getBottomLeftNeigh()->setUpperRightNeigh(no_merge_right_t);
                t.getBottomLeftNeigh()->setBottomRightNeigh(no_merge_right_t);
            }
        }
    }else{
        // right coincident
        if (Algorithm::pointIsAboveSegment(segment, t.getLeftPoint())){
            // il merge avviene sotto
            if (Algorithm::pointsAreEquals(t.getLeftPoint(), t.getSegmentUp().p1())){
                //il trapezoide non ha neighbors diversi
                if (no_merge_right_t->isRightDegenerate()){
                    // no merge é right degenerate
                    no_merge_right_t->setNeighbor(up_merging.back(), up_merging.back(),
                                                  nullptr, nullptr);

                    if (Algorithm::pointsAreEquals(up_merging.back()->getRightPoint(), up_merging.back()->getSegmentUp().p2())){
                        // up_merging ha gli stessi right neighbors
                        up_merging.back()->setUpperRightNeigh(no_merge_right_t);
                        up_merging.back()->setBottomRightNeigh(no_merge_right_t);
                    }else{
                        // up_merging ha right neighbors diversi
                        up_merging.back()->setBottomRightNeigh(no_merge_right_t);
                    }

                }else{
                    // no merge NON é right degenerate
                    no_merge_right_t->setNeighbor(up_merging.back(), up_merging.back(),
                                                  t.getUpperRightNeigh(), t.getUpperRightNeigh());

                    if (Algorithm::pointsAreEquals(up_merging.back()->getRightPoint(), up_merging.back()->getSegmentUp().p2())){
                        // up_merging ha gli stessi right neighbors
                        up_merging.back()->setUpperRightNeigh(no_merge_right_t);
                        up_merging.back()->setBottomRightNeigh(no_merge_right_t);
                    }else{
                        // up_merging ha right neighbors diversi
                        up_merging.back()->setBottomRightNeigh(no_merge_right_t);
                    }

                    if (t.getUpperRightNeigh()->sameLeftNeighbor()){
                        t.getUpperRightNeigh()->setUpperLeftNeigh(no_merge_right_t);
                        t.getUpperRightNeigh()->setBottomLeftNeigh(no_merge_right_t);
                    }else{
                        t.getUpperRightNeigh()->setUpperLeftNeigh(no_merge_right_t);
                    }

                }
            }else{
                // il trapezoide ha neighbors diversi
                if (no_merge_right_t->isRightDegenerate()){
                    // no merge é degenere
                    no_merge_right_t->setNeighbor(t.getUpperLeftNeigh(), up_merging.back(),
                                                  nullptr, nullptr);

                    t.getUpperLeftNeigh()->setUpperRightNeigh(no_merge_right_t);
                    t.getUpperLeftNeigh()->setBottomRightNeigh(no_merge_right_t);

                    up_merging.back()->setUpperRightNeigh(no_merge_right_t);
                    up_merging.back()->setBottomRightNeigh(no_merge_right_t);
                }else{
                    // no merge non é degenere
                    no_merge_right_t->setNeighbor(t.getUpperLeftNeigh(), up_merging.back(),
                                                  t.getUpperRightNeigh(), t.getUpperRightNeigh());

                    t.getUpperLeftNeigh()->setUpperRightNeigh(no_merge_right_t);
                    t.getUpperLeftNeigh()->setBottomRightNeigh(no_merge_right_t);

                    up_merging.back()->setUpperRightNeigh(no_merge_right_t);
                    up_merging.back()->setBottomRightNeigh(no_merge_right_t);



                    if (!Algorithm::pointsAreEquals(t.getRightPoint(), t.getSegmentDown().p2())){
                        // ho diversi right neighbors
                        t.getUpperRightNeigh()->setUpperLeftNeigh(no_merge_right_t);
                        t.getUpperRightNeigh()->setBottomLeftNeigh(no_merge_right_t);
                    }else{
                        // ho gli syessi right neighbors
                        if (t.getUpperRightNeigh()->sameLeftNeighbor()){
                            // il right neighbor ha gli stessi left neighbors
                            t.getUpperRightNeigh()->setUpperLeftNeigh(no_merge_right_t);
                            t.getUpperRightNeigh()->setBottomLeftNeigh(no_merge_right_t);
                        }else{
                            t.getUpperRightNeigh()->setUpperLeftNeigh(no_merge_right_t);
                        }
                    }

                }
            }

        }else{
            // il merge avvviene sopra
            if (Algorithm::pointsAreEquals(t.getLeftPoint(), t.getSegmentDown().p1())){
                if (no_merge_right_t->isRightDegenerate()){
                    // il no merge è degenere
                    no_merge_right_t->setNeighbor(low_merging.back(), low_merging.back(),
                                                  nullptr, nullptr);

                    if (Algorithm::pointsAreEquals(low_merging.back()->getRightPoint(), low_merging.back()->getSegmentDown().p2())){

                        low_merging.back()->setUpperRightNeigh(no_merge_right_t);
                        low_merging.back()->setBottomRightNeigh(no_merge_right_t);
                    }else{
                        low_merging.back()->setUpperRightNeigh(no_merge_right_t);
                    }

                }else{
                    // il no merge NON è degenere
                    no_merge_right_t->setNeighbor(low_merging.back(), low_merging.back(),
                                                  t.getBottomRightNeigh(), t.getBottomRightNeigh());

                    if (Algorithm::pointsAreEquals(low_merging.back()->getRightPoint(), low_merging.back()->getSegmentDown().p2())){
                        // il low merging ha gli stessi right neighbors
                        low_merging.back()->setUpperRightNeigh(no_merge_right_t);
                        low_merging.back()->setBottomRightNeigh(no_merge_right_t);
                    }else{
                        // il low merging ha diversi right neighbors
                        low_merging.back()->setUpperRightNeigh(no_merge_right_t);
                    }

                    if (t.getBottomRightNeigh()->sameLeftNeighbor()){
                        t.getBottomRightNeigh()->setUpperLeftNeigh(no_merge_right_t);
                        t.getBottomRightNeigh()->setBottomLeftNeigh(no_merge_right_t);
                    }else{
                        t.getBottomRightNeigh()->setBottomLeftNeigh(no_merge_right_t);
                    }

                }
            }else{
                if (no_merge_right_t->isRightDegenerate()){
                    no_merge_right_t->setNeighbor(low_merging.back(), t.getBottomLeftNeigh(),
                                                  nullptr, nullptr);

                    low_merging.back()->setUpperRightNeigh(no_merge_right_t);
                    low_merging.back()->setBottomRightNeigh(no_merge_right_t);

                    t.getBottomLeftNeigh()->setUpperRightNeigh(no_merge_right_t);
                    t.getBottomLeftNeigh()->setBottomRightNeigh(no_merge_right_t);
                }else{
                    no_merge_right_t->setNeighbor(low_merging.back(), t.getBottomLeftNeigh(),
                                                  t.getBottomRightNeigh(), t.getBottomRightNeigh());

                    low_merging.back()->setUpperRightNeigh(no_merge_right_t);
                    low_merging.back()->setBottomRightNeigh(no_merge_right_t);

                    t.getBottomLeftNeigh()->setUpperRightNeigh(no_merge_right_t);
                    t.getBottomLeftNeigh()->setBottomRightNeigh(no_merge_right_t);

                    if (!Algorithm::pointsAreEquals(t.getRightPoint(), t.getSegmentUp().p2())){
                        // ho diversi right neighbors
                        t.getBottomRightNeigh()->setUpperLeftNeigh(no_merge_right_t);
                        t.getBottomRightNeigh()->setBottomLeftNeigh(no_merge_right_t);
                    }else{
                        // ho gli stessi right neighbors
                        if (t.getBottomRightNeigh()->sameLeftNeighbor()){
                            // il right neighbors ha gli stessi left neighbors
                            t.getBottomRightNeigh()->setUpperLeftNeigh(no_merge_right_t);
                            t.getBottomRightNeigh()->setBottomLeftNeigh(no_merge_right_t);
                        }else{
                            t.getBottomRightNeigh()->setBottomLeftNeigh(no_merge_right_t);
                        }
                    }
                }
            }
        }
    }

    if (!right_coincident){
        if (Algorithm::pointIsAboveSegment(segment, t.getLeftPoint())){
            low_merging.back()->setUpperRightNeigh(right);
            low_merging.back()->setBottomRightNeigh(right);
        }else{
            up_merging.back()->setUpperRightNeigh(right);
            up_merging.back()->setBottomRightNeigh(right);
        }
    }else{
        // right coincident

        if (Algorithm::pointIsAboveSegment(segment, t.getLeftPoint())){
            // il merge avviene giu
            if (low_merging.back()->isRightDegenerate()){
                // il merge é degenere
                low_merging.back()->setUpperRightNeigh(nullptr);
                low_merging.back()->setBottomRightNeigh(nullptr);
            }else{
                //il merge non é degenere
                if (Algorithm::pointsAreEquals(t.getRightPoint(), t.getSegmentUp().p2())){
                    // il right neighbor ha due neighbors diversi
                    low_merging.back()->setUpperRightNeigh(t.getUpperRightNeigh());
                    low_merging.back()->setBottomRightNeigh(t.getUpperRightNeigh());

                    if (t.getUpperRightNeigh()->sameLeftNeighbor()){
                        t.getUpperRightNeigh()->setUpperLeftNeigh(low_merging.back());
                        t.getUpperRightNeigh()->setBottomLeftNeigh(low_merging.back());
                    }else
                        t.getUpperRightNeigh()->setBottomLeftNeigh(low_merging.back());
                }else{
                    // il right neighbor ha lo stesso left neighbor
                    low_merging.back()->setUpperRightNeigh(t.getBottomRightNeigh());
                    low_merging.back()->setBottomRightNeigh(t.getBottomRightNeigh());

                    t.getBottomRightNeigh()->setUpperLeftNeigh(low_merging.back());
                    t.getBottomRightNeigh()->setBottomLeftNeigh(low_merging.back());
                }
            }
        }else{
            // il merge avviene su
            if (up_merging.back()->isRightDegenerate()){
                // il merge é degenere
                up_merging.back()->setUpperRightNeigh(nullptr);
                up_merging.back()->setBottomRightNeigh(nullptr);
            }else{
                //il merge non é degenere
                if (Algorithm::pointsAreEquals(t.getRightPoint(), t.getSegmentDown().p2())){
                    //il right neighbor ha diversi left neighbors
                    up_merging.back()->setUpperRightNeigh(t.getUpperRightNeigh());
                    up_merging.back()->setBottomRightNeigh(t.getUpperRightNeigh());

                    if (t.getUpperRightNeigh()->sameLeftNeighbor()){
                        t.getUpperRightNeigh()->setUpperLeftNeigh(up_merging.back());
                        t.getUpperRightNeigh()->setBottomLeftNeigh(up_merging.back());
                    }else
                        t.getUpperRightNeigh()->setUpperLeftNeigh(up_merging.back());
                }else{
                    // il right neighbor ha lo stesso left neighbor
                    up_merging.back()->setUpperRightNeigh(t.getUpperRightNeigh());
                    up_merging.back()->setBottomRightNeigh(t.getUpperRightNeigh());

                    t.getUpperRightNeigh()->setUpperLeftNeigh(up_merging.back());
                    t.getUpperRightNeigh()->setBottomLeftNeigh(up_merging.back());
                }
            }
        }
    }
}
std::vector<Trapezoid*> TrapMap::newTrapezoidsMultipleSplit(const cg3::Segment2d& segment,  std::vector<Trapezoid*>& traps,
                                                            bool &left_coincident, bool &right_coincident){
    cg3::Point2d p1, q1;
    Trapezoid t;
    cg3::Color colorT;
    std::vector<Trapezoid*> up_merging, low_merging, newTrapsToReturn;
    Trapezoid *left_t = nullptr, *no_merge_t, *merge_t, *no_merge_right_t, *right=nullptr, *mirror_merge_t;
    bool left_above_segment = false, right_above_segment = false;


    int idLastTrap = trapezoids.back().getId();

    if (segment.p1().operator==(traps.front()->getLeftPoint()))
        left_coincident = true;
    if(segment.p2().operator==(traps.back()->getRightPoint()))
        right_coincident = true;

    if (!left_coincident)
        left_t = addNewTrapezoid();
    no_merge_t = addNewTrapezoid();
    merge_t = addNewTrapezoid();


      for(size_t i = 0; i <= traps.size() -1; i++){

        t = *(traps[i]);

        left_above_segment = Algorithm::pointIsAboveSegment(segment, t.getLeftPoint());
        right_above_segment = Algorithm::pointIsAboveSegment(segment, t.getRightPoint());

        if (i == 0){

            splitInThreeLeft(left_t, no_merge_t, merge_t, low_merging, up_merging, t, segment, idLastTrap, right_above_segment);

            if (!left_coincident)
                newTrapsToReturn.push_back(left_t);

            newTrapsToReturn.push_back(no_merge_t);

            newTrapsToReturn.push_back(merge_t);

            assignNeighborsLeftSplit(t, left_t, no_merge_t, merge_t, left_coincident, right_above_segment);

        }else if (i < traps.size() - 1){

            // MERGING TRAPEZOID
            colorT = cg3::Color(rand()%256, rand()%256, rand()%256);
            if (left_above_segment){
                // trapezoid that has to be merged is in low_merging

                // update merging trapezoid's shape
                low_merging.back()->setSegmentUp(cg3::Segment2d(low_merging.back()->getSegmentUp().p1(),
                                                           findIntersectionVerticalLine(segment, t.getRightPoint())));

                low_merging.back()->setSegmentDown(cg3::Segment2d(low_merging.back()->getSegmentDown().p1(),
                                                             findIntersectionVerticalLine(t.getSegmentDown(), t.getRightPoint())));
                // update right point
                low_merging.back()->setRightPoint(t.getRightPoint());


                //////////////////////
                // NEIGHBOR SETTING //
                //////////////////////
                ///

                if (!right_above_segment){
                    // il prossimo merge sarà sopra
                    low_merging.back()->setUpperRightNeigh(t.getUpperRightNeigh()); // successivamente verrà modificato
                    low_merging.back()->setBottomRightNeigh(t.getBottomRightNeigh());

                    if (!Algorithm::pointsAreEquals(t.getRightPoint(), t.getSegmentDown().p2())){
                        t.getBottomRightNeigh()->setUpperLeftNeigh(low_merging.back());
                        t.getBottomRightNeigh()->setBottomLeftNeigh(low_merging.back());
                    }
                }

            }else{
                // trapezoid that has to be merged is in up_merging

                // update merging trapezoid's shape
                up_merging.back()->setSegmentUp(cg3::Segment2d(up_merging.back()->getSegmentUp().p1(),
                                                           findIntersectionVerticalLine(t.getSegmentUp(), t.getRightPoint())));
                up_merging.back()->setSegmentDown(cg3::Segment2d(up_merging.back()->getSegmentDown().p1(),
                                                             findIntersectionVerticalLine(segment, t.getRightPoint())));

                up_merging.back()->setRightPoint(t.getRightPoint());
                //////////////////////
                // NEIGHBOR SETTING //
                //////////////////////

                if (right_above_segment){
                    // il prossimo merge sarà sotto

                    up_merging.back()->setUpperRightNeigh(t.getUpperRightNeigh());
                    up_merging.back()->setBottomRightNeigh(t.getBottomRightNeigh()); // successivamente verrà modificato

                    if(!Algorithm::pointsAreEquals(t.getRightPoint(),t.getSegmentUp().p2())){
                        t.getUpperRightNeigh()->setUpperLeftNeigh(up_merging.back());
                        t.getUpperRightNeigh()->setBottomLeftNeigh(up_merging.back());
                    }
                }
            }


            // creation of trapezoid that won't be merged

            idLastTrap = trapezoids.back().getId();
            colorT = cg3::Color(rand()%256, rand()%256, rand()%256);
            if (left_above_segment){
                mirror_merge_t = addNewTrapezoid();
                mirror_merge_t->setTrapezoid(++idLastTrap,
                                t.getSegmentUp(),
                                cg3::Segment2d(findIntersectionVerticalLine(segment, t.getLeftPoint()), findIntersectionVerticalLine(segment,t.getRightPoint())),
                                t.getLeftPoint(), t.getRightPoint(),colorT);

                newTrapsToReturn.push_back(mirror_merge_t);



                //////////////////////
                // NEIGHBOR SETTING //
                //////////////////////
                if (Algorithm::pointsAreEquals(t.getLeftPoint(), t.getSegmentUp().p1())){
                     // ho solo un left neighbor
                    mirror_merge_t->setNeighbor(up_merging.back(), up_merging.back(),
                                                t.getUpperRightNeigh(), t.getBottomRightNeigh());

                     if(!Algorithm::pointsAreEquals(up_merging.back()->getRightPoint(), up_merging.back()->getSegmentUp().p2())){
                         // l'up merging ha right neighbors diversi
                        up_merging.back()->setBottomRightNeigh(mirror_merge_t);
                     }else{
                         // l'up merging ha right neighbors uguali
                         up_merging.back()->setUpperRightNeigh(mirror_merge_t);
                         up_merging.back()->setBottomRightNeigh(mirror_merge_t);
                     }

                 }else{

                     // ho left neighbors diversi
                     mirror_merge_t->setNeighbor(t.getUpperLeftNeigh(), up_merging.back(),
                                                 t.getUpperRightNeigh(), t.getBottomRightNeigh());


                     t.getUpperLeftNeigh()->setUpperRightNeigh(mirror_merge_t);
                     t.getUpperLeftNeigh()->setBottomRightNeigh(mirror_merge_t);

                     up_merging.back()->setUpperRightNeigh(mirror_merge_t);
                     up_merging.back()->setBottomRightNeigh(mirror_merge_t);

                 }

                 if (right_above_segment &&
                         !Algorithm::pointsAreEquals(t.getRightPoint(), t.getSegmentUp().p2())){
                     // a destra devo aggiornare upper right neighbor
                     t.getUpperRightNeigh()->setUpperLeftNeigh(mirror_merge_t);
                     t.getUpperRightNeigh()->setBottomLeftNeigh(mirror_merge_t);

                 }

                 up_merging.push_back(mirror_merge_t);

            }else{
                mirror_merge_t = addNewTrapezoid();
                mirror_merge_t->setTrapezoid(++idLastTrap,
                                cg3::Segment2d(findIntersectionVerticalLine(segment, t.getLeftPoint()), findIntersectionVerticalLine(segment,t.getRightPoint())),
                                t.getSegmentDown(),
                                t.getLeftPoint(), t.getRightPoint(),colorT);


                newTrapsToReturn.push_back(mirror_merge_t);

                //////////////////////
                // NEIGHBOR SETTING //
                //////////////////////
                if (Algorithm::pointsAreEquals(t.getLeftPoint(), t.getSegmentDown().p1())){
                    // ho gli stessi left neighbors
                    mirror_merge_t->setNeighbor(low_merging.back(), low_merging.back(),
                                                t.getUpperRightNeigh(), t.getBottomRightNeigh());

                    if (!Algorithm::pointsAreEquals(low_merging.back()->getRightPoint(), low_merging.back()->getSegmentDown().p2())){
                        //il low merging ha due right neighbors diversi
                        low_merging.back()->setUpperRightNeigh(mirror_merge_t);
                    }else{
                        // il low merging ha gli stessi right neighbors
                        low_merging.back()->setUpperRightNeigh(mirror_merge_t);
                        low_merging.back()->setBottomRightNeigh(mirror_merge_t);
                    }

                }else{
                    // ho left neighbors diversi
                    mirror_merge_t->setNeighbor(low_merging.back(), t.getBottomLeftNeigh(),
                                                t.getUpperRightNeigh(), t.getBottomRightNeigh());

                    t.getBottomLeftNeigh()->setUpperRightNeigh(mirror_merge_t);
                    t.getBottomLeftNeigh()->setBottomRightNeigh(mirror_merge_t);

                    low_merging.back()->setUpperRightNeigh(mirror_merge_t);
                    low_merging.back()->setBottomRightNeigh(mirror_merge_t);


                }

                if (!right_above_segment &&
                        !Algorithm::pointsAreEquals(t.getRightPoint(), t.getSegmentDown().p2())){
                    // devo aggiornare il bottom right neighbor
                    t.getBottomRightNeigh()->setUpperLeftNeigh(mirror_merge_t);
                    t.getBottomRightNeigh()->setBottomLeftNeigh(mirror_merge_t);
                }
                low_merging.push_back(mirror_merge_t);
            }

        }else{

            idLastTrap = trapezoids.back().getId();

            no_merge_right_t = addNewTrapezoid();
            if (!right_coincident)
                right = addNewTrapezoid();

            splitInThreeRight(no_merge_right_t, right,low_merging, up_merging, t, segment, idLastTrap);


            newTrapsToReturn.push_back(no_merge_right_t);

            if (!right_coincident)
                newTrapsToReturn.push_back(right);

            assignNeighborsRightSplit(t, right, no_merge_right_t, up_merging, low_merging, segment, right_coincident);

        }

    }

    return newTrapsToReturn;

}

bool TrapMap::findID(Trapezoid t){
    for (Trapezoid t1 : trapezoids){
        if (t.getId() == t1.getId())
            return true;
    }

    return false;
}
void TrapMap::compareNeigh(){
    bool neig = true;
    const Trapezoid tr;
    std::string s;
    for (Trapezoid t : getTrapezoids()){
        if (neig){
            s += "ID: ";
            s += std::to_string(t.getId());
            if (t.getUpperLeftNeigh() != nullptr && t.getBottomLeftNeigh() != nullptr){
                s += "U-L ";
                s += std::to_string(t.getUpperLeftNeigh()->getId());
                s += " B-L ";
                s += std::to_string(t.getBottomLeftNeigh()->getId());


                if (!findID(*t.getUpperLeftNeigh())){
                    std::cerr << "t: " << t.getId() << " U-L: " << t.getUpperLeftNeigh()->getId() << " -> NOT FOUND" << std::endl;
                }
                if (!findID(*t.getBottomLeftNeigh())){
                    std::cerr << "t: " << t.getId() << "B-L: " << t.getBottomLeftNeigh()->getId() << " -> NOT FOUND" << std::endl;
                }
            }
            else{
                s += "U-L NULL B-L NULL";
            }

            if (t.getUpperRightNeigh() != nullptr && t.getBottomRightNeigh() != nullptr){

                s += "U-R ";
                s += std::to_string(t.getUpperRightNeigh()->getId());
                s += " B-R ";
                s += std::to_string(t.getBottomRightNeigh()->getId());


                if (!(findID(*t.getUpperRightNeigh()))){
                    std::cerr << "t: " << t.getId() << " U-R: " << t.getUpperRightNeigh()->getId() << " -> NOT FOUND" << std::endl;
                }

                if (!(findID(*t.getBottomRightNeigh()))){
                    std::cerr << "t: " << t.getId() << " B-R: " << t.getBottomRightNeigh()->getId() << " -> NOT FOUND" << std::endl;
                }
            }
            else
                s += "U-R NULL B-R NULL";
        }

   }

    if (prev != ""){
        if (s.compare(prev) == 0){
            std::cout << "i neigh sono uguali" << std::endl;
        }else{
            std::cerr << "i neigh sono DIVERSII!!!!!!" << std::endl;
        }
    }

    prev = s;
//        std::cout << s << std::endl;
}
