#include "trapmap.h"

#include <cg3/viewer/opengl_objects/opengl_objects2.h>
#include "algorithm.h"

TrapMap::TrapMap() : boundingBox(Trapezoid())
{
    trapezoids.push_back(boundingBox);
}

const std::list<Trapezoid> TrapMap::getTrapezoids() const{
    return trapezoids;
}

//const std::list<bool> TrapMap::getDeletedTrapezoids() const{
//    return deletedTrapezoids;
//}

Trapezoid* TrapMap::getTrapezoidWithId(const int id){

        for (std::list<Trapezoid>::iterator it=trapezoids.begin(); it != trapezoids.end(); ++it){
            if((*it).getId() == id) return &*it;
        }
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

// devo trovare il punto di intersezione tra il segmento e la linea verticale
// rappresentata dalla sua coordinata x
cg3::Point2d TrapMap::findIntersectionVerticalLine(const cg3::Segment2d& s, const cg3::Point2d& px){
    cg3::Point2d p1 = s.p1();
    cg3::Point2d p2 = s.p2();
    cg3::Point2d intersection;

//    if(!dir){
//        p1 = s.p2();
//        p2 = s.p1();
//    }

    // p2(-1,-3)
    // p1(4,4)
    double a, b, c, y;

    a = p1.y() - p2.y();
    b = p2.x() - p1.x();
    c = ((p1.x() * p2.y()) - (p2.x() * p1.y()));

    y = (-c -(a * px.x())) / b;

    return cg3::Point2d(px.x(), y);
}

//bool TrapMap::pointIsAboveSegment(cg3::Segment2d segment, cg3::Point2d point){

//    cg3::Point2d v1 = cg3::Point2d(segment.p2().x()-segment.p1().x(),segment.p2().y()-segment.p1().y());
//    cg3::Point2d v2 = cg3::Point2d(point.x()-segment.p1().x(),point.y()-segment.p1().y());
//    double cross_product = v1.x() * v2.y() - v1.y() * v2.x();

//    if (cross_product > 0){
//        //il punto si trova sopra il segmento
//        return true;
//    }else{
//        // il punto si trova sotto il segmento o sul
//        return false;
//    }
//}
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
std::vector<Trapezoid*> TrapMap::addFourTrapezoids(const cg3::Segment2d &segment,  DagNode* bbNode, bool &leftToRight){
    cg3::Segment2d topS, bottomS;
    cg3::Point2d leftP, rightP, p1, q1;
    cg3::Color colorT;

    std::vector<Trapezoid*> trapForDag;
    Trapezoid bb = *(Trapezoid*)bbNode->getData().objj;

    int idLastTrap = trapezoids.back().getId();


    p1 = segment.p1();
    q1 = segment.p2();

    ////////////////////
    // LEFT trapezoid //
    ////////////////////

    if (leftToRight){

        topS = cg3::Segment2d(bb.getSegmentUp().p1(),
                              findIntersectionVerticalLine(bb.getSegmentUp(), p1));

        bottomS = cg3::Segment2d(bb.getSegmentDown().p1(),
                                 findIntersectionVerticalLine(bb.getSegmentDown(), p1));

        leftP = bb.getLeftPoint();
        rightP = p1;
    }

    colorT = cg3::Color(rand()%256, rand()%256, rand()%256);

    if (!leftToRight){
        topS = cg3::Segment2d(bb.getSegmentUp().p1(),
                             findIntersectionVerticalLine(bb.getSegmentUp(), q1));

        bottomS = cg3::Segment2d(bb.getSegmentDown().p1(),
                               findIntersectionVerticalLine(bb.getSegmentDown(), q1));

        leftP = bb.getLeftPoint();
        rightP = q1;
    }
    Trapezoid left = Trapezoid(++idLastTrap, topS, bottomS, leftP, rightP, colorT);


    ////////////////////
    // TOP trapezoid //
    ////////////////////
    if (leftToRight){
    topS = cg3::Segment2d(findIntersectionVerticalLine(bb.getSegmentUp(), p1),
                          findIntersectionVerticalLine(bb.getSegmentUp(), q1));

    bottomS = segment;

    leftP = p1;
    rightP = q1;
}
    colorT = cg3::Color(rand()%256, rand()%256, rand()%256);

    if (!leftToRight){
        topS = cg3::Segment2d(findIntersectionVerticalLine(bb.getSegmentUp(), q1),
                              findIntersectionVerticalLine(bb.getSegmentUp(), p1));

        bottomS = cg3::Segment2d(q1, p1);

        leftP = q1;
        rightP = p1;
    }
    Trapezoid top = Trapezoid(++idLastTrap, topS, bottomS, leftP, rightP, colorT);

    ////////////////////
    // BOTTOM trapezoid //
    ////////////////////

    if (leftToRight){
        topS = segment;

        bottomS = cg3::Segment2d(findIntersectionVerticalLine(bb.getSegmentDown(), p1),
                                 findIntersectionVerticalLine(bb.getSegmentDown(), q1));

        leftP = p1;
        rightP = q1;
    }
    colorT = cg3::Color(rand()%256, rand()%256, rand()%256);

    if (!leftToRight){
        topS = cg3::Segment2d(segment.p2(), segment.p1());;

        bottomS = cg3::Segment2d(findIntersectionVerticalLine(bb.getSegmentDown(), q1),
                                 findIntersectionVerticalLine(bb.getSegmentDown(), p1));

        leftP = q1;
        rightP = p1;
    }
    Trapezoid bottom = Trapezoid(++idLastTrap, topS, bottomS, leftP, rightP, colorT);

    ////////////////////
    // RIGHT trapezoid //
    ////////////////////
    if (leftToRight){

        topS = cg3::Segment2d(findIntersectionVerticalLine(bb.getSegmentUp(), q1),
                              bb.getSegmentUp().p2());

        bottomS = cg3::Segment2d(findIntersectionVerticalLine(bb.getSegmentDown(), q1),
                                 bb.getSegmentDown().p2());

        leftP = q1;
        rightP = bb.getRightPoint();
    }
    colorT = cg3::Color(rand()%256, rand()%256, rand()%256);

    if(!leftToRight){
        topS = cg3::Segment2d(findIntersectionVerticalLine(bb.getSegmentUp(), p1),
                              bb.getSegmentUp().p2());

        bottomS = cg3::Segment2d(findIntersectionVerticalLine(bb.getSegmentDown(), p1),
                                 bb.getSegmentDown().p2());

        leftP = p1;
        rightP = bb.getRightPoint();
    }

    Trapezoid right = Trapezoid(++idLastTrap, topS, bottomS, leftP, rightP, colorT);



    // add new trapezoids
    trapezoids.push_back(left);
    trapezoids.push_back(top);
    trapezoids.push_back(bottom);
    trapezoids.push_back(right);

    Trapezoid *l = getTrapezoidWithId(left.getId());
    Trapezoid *t = getTrapezoidWithId(top.getId());
    Trapezoid *b = getTrapezoidWithId(bottom.getId());
    Trapezoid *r = getTrapezoidWithId(right.getId());

    l->setNeighbor(bb.getUpperLeftNeigh(), bb.getBottomLeftNeigh(), t, b);
    if (bb.getUpperLeftNeigh() != nullptr && bb.getBottomLeftNeigh() != nullptr && (bb.getUpperLeftNeigh()->getId() == bb.getBottomLeftNeigh()->getId())){
        if (Algorithm::pointIsAboveSegment(segment, bb.getUpperLeftNeigh()->getRightPoint())){
            bb.getBottomLeftNeigh()->setBottomRightNeigh(l);
        }
        else{

            bb.getUpperLeftNeigh()->setUpperRightNeigh(l);
        }
    }else if(bb.getUpperLeftNeigh() != nullptr && bb.getBottomLeftNeigh() != nullptr && (bb.getUpperLeftNeigh()->getId() != bb.getBottomLeftNeigh()->getId())){
        bb.getUpperLeftNeigh()->setUpperRightNeigh(l);
        bb.getUpperLeftNeigh()->setBottomRightNeigh(l);
        bb.getBottomLeftNeigh()->setUpperRightNeigh(l);
        bb.getBottomLeftNeigh()->setBottomRightNeigh(l);
    }

    t->setNeighbor(l, l, r, r);
    b->setNeighbor(l, l, r, r);

    r->setNeighbor(t, b, bb.getUpperRightNeigh(), bb.getBottomRightNeigh());
    if (bb.getUpperRightNeigh() != nullptr && bb.getBottomRightNeigh() != nullptr && (bb.getUpperRightNeigh()->getId() == bb.getBottomRightNeigh()->getId())){
        if (Algorithm::pointIsAboveSegment(segment, bb.getUpperLeftNeigh()->getLeftPoint())){
            bb.getBottomRightNeigh()->setBottomLeftNeigh(r);
        }else{
            bb.getUpperRightNeigh()->setUpperLeftNeigh(r);
        }
    }else if (bb.getUpperRightNeigh() != nullptr && bb.getBottomRightNeigh() != nullptr && (bb.getUpperRightNeigh()->getId() != bb.getBottomRightNeigh()->getId())){
        bb.getUpperRightNeigh()->setUpperLeftNeigh(r);
        bb.getUpperRightNeigh()->setBottomLeftNeigh(r);
        bb.getBottomRightNeigh()->setUpperLeftNeigh(r);
        bb.getBottomRightNeigh()->setBottomLeftNeigh(r);
    }

    trapForDag.push_back(l);
    trapForDag.push_back(t);
    trapForDag.push_back(b);
    trapForDag.push_back(r);

    return trapForDag;
}

std::vector<Trapezoid*> TrapMap::newTrapezoids(const cg3::Segment2d &segment,  std::vector<Trapezoid*> &traps, bool &leftToRight){
    cg3::Point2d p1, q1;
    Trapezoid t, tL;
    cg3::Color colorT;
    std::vector<Trapezoid> newTraps;
    std::vector<Trapezoid*> newTrapsPointer;

    std::vector<Trapezoid> upV, lowV;



    int idLastTrap = trapezoids.back().getId();

    if (leftToRight){
        p1 = segment.p1();
        q1 = segment.p2();
    }else{
        q1 = segment.p1();
        p1 = segment.p2();
    }



    Trapezoid a, b, c_1, c_n, d, e, up, low;

    for(size_t i = 0; i <= traps.size() -1; i++){
        // prendo l'i-esimo trapezoide da modificare
        Trapezoid t = *(traps[i]);
        if (i == 0){
            // a
            colorT = cg3::Color(rand()%256, rand()%256, rand()%256);
            a = Trapezoid(++idLastTrap,
                          cg3::Segment2d(t.getSegmentUp().p1(),findIntersectionVerticalLine(t.getSegmentUp(),p1)),
                          cg3::Segment2d(t.getSegmentDown().p1(),findIntersectionVerticalLine(t.getSegmentDown(),p1)),
                          t.getLeftPoint(), p1, colorT);

            // b
            colorT = cg3::Color(rand()%256, rand()%256, rand()%256);
            if (Algorithm::pointIsAboveSegment(segment, t.getRightPoint())){
                b = Trapezoid(++idLastTrap,
                              cg3::Segment2d(a.getSegmentUp().p2(), findIntersectionVerticalLine(t.getSegmentUp(),t.getRightPoint())),
                              cg3::Segment2d(p1,findIntersectionVerticalLine(segment, t.getRightPoint())),
                              p1, findIntersectionVerticalLine(segment, t.getRightPoint()), colorT);

            }else{
                b = Trapezoid(++idLastTrap,
                              cg3::Segment2d(p1,findIntersectionVerticalLine(segment,t.getRightPoint())),
                              cg3::Segment2d(a.getSegmentDown().p2(),t.getSegmentDown().p2()),
                              p1, t.getRightPoint(), colorT);
            }

            // c'
            colorT = cg3::Color(rand()%256, rand()%256, rand()%256);
            if (Algorithm::pointIsAboveSegment(segment, t.getRightPoint())){
                c_1 = Trapezoid(++idLastTrap,
                                cg3::Segment2d(p1, b.getSegmentDown().p2()),
                                cg3::Segment2d(a.getSegmentDown().p2(), t.getSegmentDown().p2()),
                                p1, b.getSegmentDown().p2(), colorT);

                lowV.push_back(c_1);
            }else{
                c_1 = Trapezoid(++idLastTrap,
                                cg3::Segment2d(a.getSegmentUp().p2(), t.getSegmentUp().p2()),
                                cg3::Segment2d(p1, b.getSegmentUp().p2()),
                                p1, b.getSegmentUp().p2(), colorT);
                upV.push_back(c_1);
            }
            newTraps.push_back(a);
            newTraps.push_back(b);

            //newTraps.push_back(c_1);

        }else if (i < traps.size() - 1){
            // creo i due nuovi trapezoidi
            colorT = cg3::Color(rand()%256, rand()%256, rand()%256);
            if (Algorithm::pointIsAboveSegment(segment, t.getLeftPoint())){
                lowV.back().setSegmentUp(cg3::Segment2d(lowV.back().getSegmentUp().p1(),
                                                           findIntersectionVerticalLine(segment, t.getRightPoint())));

                lowV.back().setSegmentDown(cg3::Segment2d(lowV.back().getSegmentDown().p1(),
                                                             findIntersectionVerticalLine(t.getSegmentDown(), t.getRightPoint())));

            }else{

                upV.back().setSegmentUp(cg3::Segment2d(upV.back().getSegmentUp().p1(),
                                                           findIntersectionVerticalLine(t.getSegmentUp(), t.getRightPoint())));
                upV.back().setSegmentDown(cg3::Segment2d(upV.back().getSegmentDown().p1(),
                                                             findIntersectionVerticalLine(segment, t.getRightPoint())));


            }

            colorT = cg3::Color(rand()%256, rand()%256, rand()%256);
            if (Algorithm::pointIsAboveSegment(segment, t.getLeftPoint())){
                low = Trapezoid(++idLastTrap,
                                t.getSegmentUp(),
                                cg3::Segment2d(findIntersectionVerticalLine(segment, t.getLeftPoint()), findIntersectionVerticalLine(segment,t.getRightPoint())),
                                t.getLeftPoint(), t.getRightPoint(),colorT);

                upV.push_back(low);
            }else{
                low = Trapezoid(++idLastTrap,
                                cg3::Segment2d(findIntersectionVerticalLine(segment, t.getLeftPoint()), findIntersectionVerticalLine(segment,t.getRightPoint())),
                                t.getSegmentDown(),
                                t.getLeftPoint(), t.getRightPoint(),colorT);

                lowV.push_back(low);
            }

            //newTraps.push_back(low);
//            if (i == 1)
//                break;
        }else{


            colorT = cg3::Color(rand()%256, rand()%256, rand()%256);

            if (Algorithm::pointIsAboveSegment(segment, t.getLeftPoint())){
                // merging
                lowV.back().setSegmentUp(cg3::Segment2d(lowV.back().getSegmentUp().p1(), q1));
                lowV.back().setSegmentDown(cg3::Segment2d(lowV.back().getSegmentDown().p1(), findIntersectionVerticalLine(t.getSegmentDown(),q1)));
                lowV.back().setRightPoint(q1);

            }else{
                // merging
                upV.back().setSegmentUp(cg3::Segment2d(upV.back().getSegmentUp().p1(), findIntersectionVerticalLine(t.getSegmentUp(),q1)));
                upV.back().setSegmentDown(cg3::Segment2d(upV.back().getSegmentDown().p1(), q1));
                upV.back().setRightPoint(q1);
            }


            colorT = cg3::Color(rand()%256, rand()%256, rand()%256);
            if (Algorithm::pointIsAboveSegment(segment, t.getLeftPoint())){
                d = Trapezoid(++idLastTrap,
                              cg3::Segment2d(t.getSegmentUp().p1(),findIntersectionVerticalLine(t.getSegmentUp(), q1)),
                              cg3::Segment2d(findIntersectionVerticalLine(segment, t.getLeftPoint()), q1),
                              t.getLeftPoint(), q1, colorT);
            }else{
                d = Trapezoid(++idLastTrap,
                              cg3::Segment2d(findIntersectionVerticalLine(segment, t.getLeftPoint()),q1),
                              cg3::Segment2d(t.getSegmentDown().p1(),findIntersectionVerticalLine(t.getSegmentDown(), q1)),
                              t.getLeftPoint(), q1, colorT);
            }


            colorT = cg3::Color(rand()%256, rand()%256, rand()%256);
            if (Algorithm::pointIsAboveSegment(segment, t.getLeftPoint())){
                e = Trapezoid(++idLastTrap,
                              cg3::Segment2d(d.getSegmentUp().p2(), t.getSegmentUp().p2()),
                              cg3::Segment2d(findIntersectionVerticalLine(t.getSegmentDown(), q1),t.getSegmentDown().p2()),
                              q1, t.getRightPoint(), colorT);
            }else{
                e = Trapezoid(++idLastTrap,
                              cg3::Segment2d(findIntersectionVerticalLine(t.getSegmentUp(), q1),t.getSegmentUp().p2()),
                              cg3::Segment2d(d.getSegmentDown().p2(), t.getSegmentDown().p2()),
                              q1, t.getRightPoint(), colorT);
            }


            newTraps.push_back(d);
            newTraps.push_back(e);


        }
    }

    newTraps.insert(newTraps.end(), upV.begin(), upV.end());
    newTraps.insert(newTraps.end(), lowV.begin(), lowV.end());

    for(Trapezoid t : newTraps){
        trapezoids.push_back(t);
        newTrapsPointer.push_back(getTrapezoidWithId(t.getId()));
    }


    // neighbor
//    a1->setNeighbor(t.getUpperLeftNeigh(),t.getBottomLeftNeigh(), c1, b1);
//    b1->setNeighbor(a1, a1, d1, t.getBottomRightNeigh());
//    c1->setNeighbor(a1, a1, e1, e1);
//    d1->setNeighbor(b1, b1, e1, e1);
//    e1->setNeighbor(c1, d1, tL.getUpperRightNeigh(), tL.getBottomRightNeigh());


    for (Trapezoid* t : traps){
        deleteTrapezoidWithId(t->getId());
    }


    // TODO DA CAMBIARE
    return traps;

}
