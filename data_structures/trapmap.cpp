#include "trapmap.h"

#include <cg3/viewer/opengl_objects/opengl_objects2.h>

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

void TrapMap::updateNeigh(Trapezoid *a, Trapezoid *b, const cg3::Segment2d& s){
    cg3::Point2d p = a->getRightPoint();
    cg3::Point2d v1 = cg3::Point2d(s.p2().x()-s.p1().x(),s.p2().y()-s.p1().y());
    cg3::Point2d v2 = cg3::Point2d(p.x()-s.p1().x(),p.y()-s.p1().y());
    double cross_product = v1.x() * v2.y() - v1.y() * v2.x();

    if (cross_product > 0){
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

        leftP = bottomS.p1();
        rightP = p1;
    }

    colorT = cg3::Color(rand()%256, rand()%256, rand()%256);

    if (!leftToRight){
        topS = cg3::Segment2d(bb.getSegmentUp().p1(),
                             findIntersectionVerticalLine(bb.getSegmentUp(), q1));

        bottomS = cg3::Segment2d(bb.getSegmentDown().p1(),
                               findIntersectionVerticalLine(bb.getSegmentDown(), q1));

        leftP = bottomS.p1();
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
        rightP = bottomS.p2();
    }
    colorT = cg3::Color(rand()%256, rand()%256, rand()%256);

    if(!leftToRight){
        topS = cg3::Segment2d(findIntersectionVerticalLine(bb.getSegmentUp(), p1),
                              bb.getSegmentUp().p2());

        bottomS = cg3::Segment2d(findIntersectionVerticalLine(bb.getSegmentDown(), p1),
                                 bb.getSegmentDown().p2());

        leftP = p1;
        rightP = bottomS.p2();
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
    if (bb.getUpperLeftNeigh() != nullptr){
        bb.getUpperLeftNeigh()->setUpperRightNeigh(l);
    }
    if (bb.getBottomLeftNeigh() != nullptr){
        bb.getBottomLeftNeigh()->setBottomRightNeigh(l);
    }

    t->setNeighbor(l, l, r, r);
    b->setNeighbor(l, l, r, r);

    r->setNeighbor(t, b, bb.getUpperRightNeigh(), bb.getBottomRightNeigh());

    if (bb.getUpperRightNeigh() != nullptr){
        bb.getUpperRightNeigh()->setUpperLeftNeigh(r);
    }
    if (bb.getBottomRightNeigh() != nullptr){
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
    bool twoTraps = false;
    int i = 1;

    int idLastTrap = trapezoids.back().getId();

    if (leftToRight){
        p1 = segment.p1();
        q1 = segment.p2();
    }else{
        q1 = segment.p1();
        p1 = segment.p2();
    }

    if (traps.size() == 2)
        twoTraps = true;

    t = *traps.at(0);
    tL = *traps.back();

    colorT = cg3::Color(rand()%256, rand()%256, rand()%256);
    Trapezoid a = Trapezoid(++idLastTrap,
                            cg3::Segment2d(t.getSegmentUp().p1(),findIntersectionVerticalLine(t.getSegmentUp(),p1)),
                            cg3::Segment2d(t.getSegmentDown().p1(),findIntersectionVerticalLine(t.getSegmentDown(),p1)),
                            t.getLeftPoint(), p1, colorT);

    colorT = cg3::Color(rand()%256, rand()%256, rand()%256);
    Trapezoid b = Trapezoid(++idLastTrap,
                                cg3::Segment2d(p1,findIntersectionVerticalLine(segment,t.getRightPoint())),
                                cg3::Segment2d(a.getSegmentDown().p2(),t.getSegmentDown().p2()),
                                p1, t.getRightPoint(), colorT);



    colorT = cg3::Color(rand()%256, rand()%256, rand()%256);
    Trapezoid c = Trapezoid(++idLastTrap,
                            cg3::Segment2d(a.getSegmentUp().p2(), findIntersectionVerticalLine(tL.getSegmentUp(), q1) ),
                            cg3::Segment2d(p1,q1),
                            p1, q1, colorT);

    if (twoTraps){
        colorT = cg3::Color(rand()%256, rand()%256, rand()%256);
        Trapezoid d = Trapezoid(++idLastTrap,
                                cg3::Segment2d(b.getSegmentUp().p2(), q1),
                                cg3::Segment2d(b.getRightPoint(),findIntersectionVerticalLine(tL.getSegmentDown(),q1)),
                                b.getRightPoint(), findIntersectionVerticalLine(tL.getSegmentDown(),q1), colorT);
    }else{
        t = *traps.at(++i);
        colorT = cg3::Color(rand()%256, rand()%256, rand()%256);
        Trapezoid d = Trapezoid(++idLastTrap,
                                cg3::Segment2d(b.getSegmentUp().p2(), findIntersectionVerticalLine(segment, t.getSegmentDown().p2())),
                                cg3::Segment2d(t.getSegmentDown().p1(), t.getSegmentDown().p2()),
                                t.getSegmentDown().p1(), t.getSegmentDown().p2() , colorT);
    }

//    colorT = cg3::Color(rand()%256, rand()%256, rand()%256);
//    Trapezoid e = Trapezoid(++idLastTrap,
//                            cg3::Segment2d(c.getSegmentUp().p2(),tL.getSegmentUp().p2()),
//                            cg3::Segment2d(d.getSegmentDown().p1(),tL.getSegmentDown().p2()),
//                            q1, tL.getRightPoint(), colorT);





}
