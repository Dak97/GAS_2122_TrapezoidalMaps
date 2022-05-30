#include "trapmap.h"

#include <cg3/viewer/opengl_objects/opengl_objects2.h>
#include "algorithm.h"

TrapMap::TrapMap() : boundingBox(Trapezoid())
{
    //trapezoids.push_back(boundingBox);
    init();
}

void TrapMap::init(){
    trapezoids.push_back(boundingBox);

}

const std::list<Trapezoid> TrapMap::getTrapezoids() const{
    return trapezoids;
}


Trapezoid* TrapMap::getTrapezoidWithId(const int id){

    for (std::list<Trapezoid>::iterator it=trapezoids.begin(); it != trapezoids.end(); ++it){
        if((*it).getId() == id) return &*it;
    }
}

void TrapMap::deleteTrapezoidByRef(std::vector<Trapezoid*> traps){
//    for (Trapezoid* t : traps){
//        trapezoids.remove(*t);
//    }

}


void TrapMap::deleteTrapezoidWithId(const int id){
    for (std::list<Trapezoid>::iterator i=trapezoids.begin(); i!=trapezoids.end(); i++){
        if ((*i).getId() == id){
            trapezoids.erase(i);
            return;
        }
    }
}

void TrapMap::clear(){
    trapezoids.clear();
}

Trapezoid* TrapMap::addNewTrapezoid(){
    //TODO capire se conviene restituire l'iteratore

    //inserisco un nuovo trapezoide di default
    trapezoids.push_back(Trapezoid());
    // restituisco il puntatore del nuovo trapezoide
   // std::_List_iterator<Trapezoid> s = std::prev(trapezoids.end());
    //std::cout << "id del nuvo trapezoide" << s->getId() << std::endl;
    return  &*std::prev(trapezoids.end());
}

// devo trovare il punto di intersezione tra il segmento e la linea verticale
// rappresentata dalla sua coordinata x
cg3::Point2d TrapMap::findIntersectionVerticalLine(const cg3::Segment2d& s, const cg3::Point2d& px){
    cg3::Point2d p1 = s.p1();
    cg3::Point2d p2 = s.p2();
    cg3::Point2d intersection;

    double a, b, c, y;

    a = p1.y() - p2.y();
    b = p2.x() - p1.x();
    c = ((p1.x() * p2.y()) - (p2.x() * p1.y()));

    y = (-c -(a * px.x())) / b;

    return cg3::Point2d(px.x(), y);
}

void TrapMap::updateNeigh(Trapezoid *a, Trapezoid *b, const cg3::Segment2d& s){
    cg3::Point2d p = a->getRightPoint();

    if (Algorithm::pointIsAboveSegment(s, p)){
        //il punto si trova sopra il segmento
        if (p.x() < s.p1().x()){
            a->setBottomRightNeigh(b);
            b->setBottomLeftNeigh(a);
          }else
        {
            a->setBottomLeftNeigh(b);
            b->setBottomRightNeigh(a);
        }
    }else{
        // il punto si trova sotto il segmento o sul
        if (p.x() < s.p1().x()){
            a->setUpperRightNeigh(b);
            b->setUpperLeftNeigh(a);
        }else if (p.x() > s.p1().x()){
            a->setUpperLeftNeigh(b);
            b->setUpperRightNeigh(a);
        }else{
            // i punti coincidono
            if (b->getSegmentUp().p1() == p){
                a->setBottomRightNeigh(b);
                b->setBottomLeftNeigh(a);
            }else
            {
                a->setUpperRightNeigh(b);
                b->setUpperLeftNeigh(a);
            }
        }
    }



}
std::vector<Trapezoid*> TrapMap::addFourTrapezoids(const cg3::Segment2d& segment,  DagNode* bbNode){
    cg3::Segment2d topS, bottomS;
    cg3::Point2d leftP, rightP, p1, q1;
    cg3::Color colorT;
    Trapezoid *left, *top, *bottom, *right;

    std::vector<Trapezoid*> trapForDag;
    Trapezoid bb = *(Trapezoid*)bbNode->getData().objj;

    int idLastTrap = trapezoids.back().getId();


    p1 = segment.p1();
    q1 = segment.p2();

    // create and add a new default trapezoids in the trapezoid's list
    left = addNewTrapezoid();
    top = addNewTrapezoid();
    bottom = addNewTrapezoid();
    right = addNewTrapezoid();

    ////////////////////
    // LEFT trapezoid //
    ////////////////////

    topS = cg3::Segment2d(bb.getSegmentUp().p1(),
                          findIntersectionVerticalLine(bb.getSegmentUp(), p1));

    bottomS = cg3::Segment2d(bb.getSegmentDown().p1(),
                             findIntersectionVerticalLine(bb.getSegmentDown(), p1));

    leftP = bb.getLeftPoint();
    rightP = p1;


    colorT = cg3::Color(rand()%256, rand()%256, rand()%256);

    left->setTrapezoid(++idLastTrap, topS, bottomS, leftP, rightP, colorT);


    ////////////////////
    // TOP trapezoid //
    ////////////////////
    ///
    topS = cg3::Segment2d(findIntersectionVerticalLine(bb.getSegmentUp(), p1),
                          findIntersectionVerticalLine(bb.getSegmentUp(), q1));
    bottomS = segment;

    leftP = p1;

    rightP = q1;

    colorT = cg3::Color(rand()%256, rand()%256, rand()%256);
    top->setTrapezoid(++idLastTrap, topS, bottomS, leftP, rightP, colorT);

    ////////////////////
    // BOTTOM trapezoid //
    ////////////////////


    topS = segment;

    bottomS = cg3::Segment2d(findIntersectionVerticalLine(bb.getSegmentDown(), p1),
                             findIntersectionVerticalLine(bb.getSegmentDown(), q1));

    leftP = p1;
    rightP = q1;

    colorT = cg3::Color(rand()%256, rand()%256, rand()%256);

    bottom->setTrapezoid(++idLastTrap, topS, bottomS, leftP, rightP, colorT);

    ////////////////////
    // RIGHT trapezoid //
    ////////////////////


    topS = cg3::Segment2d(findIntersectionVerticalLine(bb.getSegmentUp(), q1),
                          bb.getSegmentUp().p2());

    bottomS = cg3::Segment2d(findIntersectionVerticalLine(bb.getSegmentDown(), q1),
                             bb.getSegmentDown().p2());

    leftP = q1;
    rightP = bb.getRightPoint();

    colorT = cg3::Color(rand()%256, rand()%256, rand()%256);

    right->setTrapezoid(++idLastTrap, topS, bottomS, leftP, rightP, colorT);

    ////////////////////
    // SETTING NEIGHBOR //
    ////////////////////

    left->setNeighbor(bb.getUpperLeftNeigh(), bb.getBottomLeftNeigh(), top, bottom);
    if (bb.getUpperLeftNeigh() != nullptr && bb.getBottomLeftNeigh() != nullptr && (bb.getUpperLeftNeigh()->getId() == bb.getBottomLeftNeigh()->getId())){
        if (Algorithm::pointIsAboveSegment(segment, bb.getUpperLeftNeigh()->getRightPoint())){
            bb.getBottomLeftNeigh()->setBottomRightNeigh(left);
        }
        else{

            bb.getUpperLeftNeigh()->setUpperRightNeigh(left);
        }
    }else if(bb.getUpperLeftNeigh() != nullptr && bb.getBottomLeftNeigh() != nullptr && (bb.getUpperLeftNeigh()->getId() != bb.getBottomLeftNeigh()->getId())){
        bb.getUpperLeftNeigh()->setUpperRightNeigh(left);
        bb.getUpperLeftNeigh()->setBottomRightNeigh(left);
        bb.getBottomLeftNeigh()->setUpperRightNeigh(left);
        bb.getBottomLeftNeigh()->setBottomRightNeigh(left);
    }

    top->setNeighbor(left, left, right, right);
    bottom->setNeighbor(left, left, right, right);

    right->setNeighbor(top, bottom, bb.getUpperRightNeigh(), bb.getBottomRightNeigh());
    if (bb.getUpperRightNeigh() != nullptr && bb.getBottomRightNeigh() != nullptr && (bb.getUpperRightNeigh()->getId() == bb.getBottomRightNeigh()->getId())){
        if (Algorithm::pointIsAboveSegment(segment, bb.getUpperLeftNeigh()->getLeftPoint())){
            bb.getBottomRightNeigh()->setBottomLeftNeigh(right);
        }else{
            bb.getUpperRightNeigh()->setUpperLeftNeigh(right);
        }
    }else if (bb.getUpperRightNeigh() != nullptr && bb.getBottomRightNeigh() != nullptr && (bb.getUpperRightNeigh()->getId() != bb.getBottomRightNeigh()->getId())){
        bb.getUpperRightNeigh()->setUpperLeftNeigh(right);
        bb.getUpperRightNeigh()->setBottomLeftNeigh(right);
        bb.getBottomRightNeigh()->setUpperLeftNeigh(right);
        bb.getBottomRightNeigh()->setBottomLeftNeigh(right);
    }

    trapForDag.push_back(left);
    trapForDag.push_back(top);
    trapForDag.push_back(bottom);
    trapForDag.push_back(right);

    return trapForDag;
}
void TrapMap::splitInThreeRight(Trapezoid *d, Trapezoid *e, std::vector<Trapezoid*>& low, std::vector<Trapezoid*>& up,
                       Trapezoid& bb,  const cg3::Segment2d& segment, int id){
    cg3::Point2d p1, q1;
    cg3::Color colorT;

    int idLastTrap = id;

    p1 = segment.p1();
    q1 = segment.p2();

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

}
void TrapMap::splitInThreeLeft(Trapezoid *a, Trapezoid *b, Trapezoid *c_1, std::vector<Trapezoid*>& low, std::vector<Trapezoid*>& up,
                           Trapezoid& bb, const cg3::Segment2d& segment, int id){
    cg3::Point2d p1, q1;
    cg3::Color colorT;

    int idLastTrap = id;

    p1 = segment.p1();
    q1 = segment.p2();

    colorT = cg3::Color(rand()%256, rand()%256, rand()%256);
    a->setTrapezoid(++idLastTrap,
                  cg3::Segment2d(bb.getSegmentUp().p1(),findIntersectionVerticalLine(bb.getSegmentUp(),p1)),
                  cg3::Segment2d(bb.getSegmentDown().p1(),findIntersectionVerticalLine(bb.getSegmentDown(),p1)),
                  bb.getLeftPoint(), p1, colorT);

    colorT = cg3::Color(rand()%256, rand()%256, rand()%256);
    if (Algorithm::pointIsAboveSegment(segment, bb.getRightPoint())){
        b->setTrapezoid(++idLastTrap,
                        cg3::Segment2d(a->getSegmentUp().p2(), findIntersectionVerticalLine(bb.getSegmentUp(), bb.getRightPoint())),
                        cg3::Segment2d(p1,findIntersectionVerticalLine(segment, bb.getRightPoint())),
                        p1, bb.getRightPoint(), colorT);
    }else{
        b->setTrapezoid(++idLastTrap,
                        cg3::Segment2d(p1,findIntersectionVerticalLine(segment,bb.getRightPoint())),
                        cg3::Segment2d(a->getSegmentDown().p2(),bb.getSegmentDown().p2()),
                        p1, bb.getRightPoint(), colorT);

    }

    colorT = cg3::Color(rand()%256, rand()%256, rand()%256);
    if (Algorithm::pointIsAboveSegment(segment, bb.getRightPoint())){
        c_1->setTrapezoid(++idLastTrap,
                        cg3::Segment2d(p1, b->getSegmentDown().p2()),
                        cg3::Segment2d(a->getSegmentDown().p2(), bb.getSegmentDown().p2()),
                        p1, b->getSegmentDown().p2(), colorT);

        low.push_back(c_1);
    }else{
        c_1->setTrapezoid(++idLastTrap,
                        cg3::Segment2d(a->getSegmentUp().p2(), bb.getSegmentUp().p2()),
                        cg3::Segment2d(p1, b->getSegmentUp().p2()),
                        p1, b->getSegmentUp().p2(), colorT);
        up.push_back(c_1);
    }

}
std::vector<Trapezoid*> TrapMap::newTrapezoids(const cg3::Segment2d& segment,  std::vector<Trapezoid*>& traps){
    cg3::Point2d p1, q1;
    Trapezoid t;
    cg3::Color colorT;
    std::vector<Trapezoid*> upV, lowV, newTrapsToReturn;
    Trapezoid *a, *b, *c_1, *d, *e, *low;

    int idLastTrap = trapezoids.back().getId();

    a = addNewTrapezoid();
    b = addNewTrapezoid();
    c_1 = addNewTrapezoid();


     for(size_t i = 0; i <= traps.size() -1; i++){
        // prendo l'i-esimo trapezoide da modificare
        t = *(traps[i]);
        if (i == 0){
            // a
            splitInThreeLeft(a,b,c_1,lowV, upV, t, segment, idLastTrap);


            newTrapsToReturn.push_back(a);

            // b

            newTrapsToReturn.push_back(b);



            // c'
            newTrapsToReturn.push_back(c_1);

            // A neighbor //
            a->setUpperLeftNeigh(t.getUpperLeftNeigh());
            a->setBottomLeftNeigh(t.getBottomLeftNeigh());
            if (Algorithm::pointIsAboveSegment(segment, t.getRightPoint())){
                a->setUpperRightNeigh(b);
                a->setBottomRightNeigh(c_1);
            }else{
                a->setUpperRightNeigh(c_1);
                a->setBottomRightNeigh(b);
            }

            if (t.getBottomLeftNeigh() != nullptr && t.getUpperLeftNeigh() != nullptr){

                if (t.getBottomLeftNeigh()->getId() == t.getUpperLeftNeigh()->getId()){
                    if (Algorithm::pointIsAboveSegment(segment, t.getLeftPoint())){
                        t.getUpperLeftNeigh()->setBottomRightNeigh(a);
                    }else{
                        t.getUpperLeftNeigh()->setUpperRightNeigh(a);
                    }
                }else{
                    t.getUpperLeftNeigh()->setUpperRightNeigh(a);
                    t.getUpperLeftNeigh()->setBottomRightNeigh(a);

                    t.getBottomLeftNeigh()->setUpperRightNeigh(a);
                    t.getBottomLeftNeigh()->setBottomRightNeigh(a);
                }
            }

            //B neighbor
            b->setUpperLeftNeigh(a);
            b->setBottomLeftNeigh(a);
            b->setUpperRightNeigh(t.getUpperRightNeigh());
            b->setBottomRightNeigh(t.getBottomRightNeigh());

            if (t.getUpperRightNeigh()->getId() == t.getBottomRightNeigh()->getId()){
                if (Algorithm::pointIsAboveSegment(segment, t.getLeftPoint())){
                    t.getUpperRightNeigh()->setBottomLeftNeigh(b);
                }else{
                    t.getUpperRightNeigh()->setUpperLeftNeigh(b);
                }
            }else{
                t.getUpperRightNeigh()->setUpperLeftNeigh(b);
                t.getUpperRightNeigh()->setBottomLeftNeigh(b);

                t.getBottomRightNeigh()->setUpperLeftNeigh(b);
                t.getBottomRightNeigh()->setBottomLeftNeigh(b);
            }

            // c neighbor
            c_1->setUpperLeftNeigh(a);
            c_1->setBottomLeftNeigh(a);

            if (Algorithm::pointIsAboveSegment(segment, t.getLeftPoint())){
                c_1->setUpperRightNeigh(t.getBottomRightNeigh());
                c_1->setBottomRightNeigh(t.getBottomRightNeigh());

                //t.getBottomRightNeigh()->setBottomLeftNeigh(c_1);
            }else{
                c_1->setUpperRightNeigh(t.getUpperRightNeigh());
                c_1->setBottomRightNeigh(t.getUpperRightNeigh());

                //t.getUpperRightNeigh()->setUpperLeftNeigh(c_1);
            }

        }else if (i < traps.size() - 1){
            // creo i due nuovi trapezoidi
            colorT = cg3::Color(rand()%256, rand()%256, rand()%256);
            if (Algorithm::pointIsAboveSegment(segment, t.getLeftPoint())){
                lowV.back()->setSegmentUp(cg3::Segment2d(lowV.back()->getSegmentUp().p1(),
                                                           findIntersectionVerticalLine(segment, t.getRightPoint())));

                lowV.back()->setSegmentDown(cg3::Segment2d(lowV.back()->getSegmentDown().p1(),
                                                             findIntersectionVerticalLine(t.getSegmentDown(), t.getRightPoint())));

                // NEIGHBOR
                lowV.back()->setUpperRightNeigh(t.getUpperRightNeigh());
                lowV.back()->setBottomRightNeigh(t.getBottomRightNeigh());

                t.getUpperRightNeigh()->setUpperLeftNeigh(lowV.back());
                t.getUpperRightNeigh()->setBottomLeftNeigh(lowV.back());

                t.getBottomRightNeigh()->setBottomLeftNeigh(lowV.back());
            }else{

                upV.back()->setSegmentUp(cg3::Segment2d(upV.back()->getSegmentUp().p1(),
                                                           findIntersectionVerticalLine(t.getSegmentUp(), t.getRightPoint())));
                upV.back()->setSegmentDown(cg3::Segment2d(upV.back()->getSegmentDown().p1(),
                                                             findIntersectionVerticalLine(segment, t.getRightPoint())));

                // NEIGHBOR
                upV.back()->setUpperRightNeigh(t.getUpperRightNeigh());
                upV.back()->setBottomRightNeigh(t.getBottomRightNeigh());

                t.getUpperRightNeigh()->setUpperLeftNeigh(upV.back());
                t.getUpperRightNeigh()->setBottomLeftNeigh(upV.back());

                t.getBottomRightNeigh()->setUpperLeftNeigh(upV.back());
            }

            colorT = cg3::Color(rand()%256, rand()%256, rand()%256);
            if (Algorithm::pointIsAboveSegment(segment, t.getLeftPoint())){
                low = addNewTrapezoid();
                low->setTrapezoid(++idLastTrap,
                                t.getSegmentUp(),
                                cg3::Segment2d(findIntersectionVerticalLine(segment, t.getLeftPoint()), findIntersectionVerticalLine(segment,t.getRightPoint())),
                                t.getLeftPoint(), t.getRightPoint(),colorT);

                upV.push_back(low);

                // NEIGHBOR
                low->setUpperLeftNeigh(t.getUpperLeftNeigh());
                low->setBottomLeftNeigh(t.getUpperLeftNeigh());
                low->setUpperRightNeigh(t.getUpperRightNeigh());
                low->setBottomRightNeigh(t.getUpperRightNeigh());

                t.getBottomLeftNeigh()->setBottomRightNeigh(low);
                t.getBottomRightNeigh()->setBottomLeftNeigh(low);
            }else{
                low = addNewTrapezoid();
                low->setTrapezoid(++idLastTrap,
                                cg3::Segment2d(findIntersectionVerticalLine(segment, t.getLeftPoint()), findIntersectionVerticalLine(segment,t.getRightPoint())),
                                t.getSegmentDown(),
                                t.getLeftPoint(), t.getRightPoint(),colorT);

                lowV.push_back(low);

                //NEIGHBOR
                low->setUpperLeftNeigh(t.getBottomLeftNeigh());
                low->setBottomLeftNeigh(t.getBottomLeftNeigh());
                low->setUpperRightNeigh(t.getBottomRightNeigh());
                low->setBottomRightNeigh(t.getBottomRightNeigh());

                t.getBottomLeftNeigh()->setUpperRightNeigh(low);
                t.getBottomRightNeigh()->setUpperLeftNeigh(low);

            }

//            if (i == 1)
//                break;
        }else{

            idLastTrap = trapezoids.back().getId();

            d = addNewTrapezoid();
            e = addNewTrapezoid();

            splitInThreeRight(d,e,lowV,upV,t,segment, idLastTrap);

//            if (Algorithm::pointIsAboveSegment(segment, t.getLeftPoint())){
//                newTrapsToReturn.push_back(lowV.back());
//            }else{
//                newTrapsToReturn.push_back(upV.back());
//            }

            newTrapsToReturn.push_back(d);
            newTrapsToReturn.push_back(e);
            // neighbor

            e->setUpperRightNeigh(t.getUpperRightNeigh());
            e->setBottomRightNeigh(t.getBottomRightNeigh());
            if (Algorithm::pointIsAboveSegment(segment, t.getLeftPoint())){
                e->setUpperLeftNeigh(d);
                e->setBottomLeftNeigh(lowV.back());
            }else{
                e->setUpperLeftNeigh(upV.back());
                e->setBottomLeftNeigh(d);
            }

            if (t.getBottomRightNeigh() != nullptr && t.getUpperRightNeigh() != nullptr){
                if (t.getBottomRightNeigh()->getId() == t.getUpperRightNeigh()->getId()){
                    if (Algorithm::pointIsAboveSegment(segment, t.getLeftPoint())){
                        t.getUpperRightNeigh()->setBottomLeftNeigh(e);
                    }else{
                        t.getUpperRightNeigh()->setUpperLeftNeigh(e);
                    }
                }else{
                    t.getUpperRightNeigh()->setUpperLeftNeigh(e);
                    t.getUpperRightNeigh()->setBottomLeftNeigh(e);

                    t.getBottomRightNeigh()->setUpperLeftNeigh(e);
                    t.getBottomRightNeigh()->setBottomLeftNeigh(e);
                }
            }

            //D neighbor
            d->setUpperRightNeigh(e);
            d->setBottomRightNeigh(e);
            if (t.getUpperLeftNeigh()->getId() == t.getBottomLeftNeigh()->getId()){
                if (Algorithm::pointIsAboveSegment(segment, t.getLeftPoint())){
                    d->setUpperLeftNeigh(t.getUpperLeftNeigh());
                    d->setBottomLeftNeigh(t.getBottomLeftNeigh());

                   // t.getBottomLeftNeigh()->setUpperRightNeigh(d);
                    t.getBottomLeftNeigh()->setBottomRightNeigh(d);
                }else{
                    d->setUpperLeftNeigh(t.getUpperLeftNeigh());
                    d->setBottomLeftNeigh(t.getBottomLeftNeigh());

                    t.getUpperLeftNeigh()->setUpperRightNeigh(d);
                    //t.getUpperLeftNeigh()->setBottomRightNeigh(d);

                }
            }else{
                if (Algorithm::pointIsAboveSegment(segment, t.getLeftPoint())){
                    d->setUpperLeftNeigh(t.getUpperLeftNeigh());
                    d->setBottomLeftNeigh(t.getBottomLeftNeigh());

                    t.getBottomLeftNeigh()->setUpperRightNeigh(d);
                    t.getBottomLeftNeigh()->setBottomRightNeigh(d);
                    t.getUpperLeftNeigh()->setUpperRightNeigh(d);
                    t.getUpperLeftNeigh()->setBottomRightNeigh(d);
                }else{
                    d->setUpperLeftNeigh(t.getUpperLeftNeigh());
                    d->setBottomLeftNeigh(t.getBottomLeftNeigh());

                    t.getUpperLeftNeigh()->setUpperRightNeigh(d);
                    t.getUpperLeftNeigh()->setBottomRightNeigh(d);
                    t.getBottomLeftNeigh()->setUpperRightNeigh(d);
                    t.getBottomLeftNeigh()->setBottomRightNeigh(d);

                }
            }


            if (Algorithm::pointIsAboveSegment(segment, t.getLeftPoint())){
                //t.getBottomLeftNeigh()->setBottomRightNeigh(d);

                lowV.back()->setUpperRightNeigh(e);
                lowV.back()->setBottomRightNeigh(e);
            }else{
                //t.getBottomLeftNeigh()->setUpperRightNeigh(d);

                upV.back()->setUpperRightNeigh(e);
                upV.back()->setBottomRightNeigh(e);
            }
        }
    }

    return newTrapsToReturn;

}
