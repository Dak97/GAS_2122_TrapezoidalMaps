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
//    for (const Trapezoid* t : traps){
//        trapezoids.remove(*t);
//    }
}
void TrapMap::deleteTrapezoidByRef(Trapezoid* trap){
//    trapezoids.remove(*trap);

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
    if (bb.getUpperRightNeigh() != nullptr && bb.getBottomRightNeigh() != nullptr &&
            (bb.getUpperRightNeigh()->getId() == bb.getBottomRightNeigh()->getId())){
        if (Algorithm::pointIsAboveSegment(segment, bb.getUpperRightNeigh()->getLeftPoint())){
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
        up.push_back(b);
    }else{
        b->setTrapezoid(++idLastTrap,
                        cg3::Segment2d(p1,findIntersectionVerticalLine(segment,bb.getRightPoint())),
                        cg3::Segment2d(a->getSegmentDown().p2(),bb.getSegmentDown().p2()),
                        p1, bb.getRightPoint(), colorT);
        low.push_back(b);
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
    std::vector<Trapezoid*> up_merging, low_merging, newTrapsToReturn;
    Trapezoid *left_t, *no_merge_t, *merge_t, *no_merge_right_t, *right, *mirror_merge_t;


    int idLastTrap = trapezoids.back().getId();

    left_t = addNewTrapezoid();
    no_merge_t = addNewTrapezoid();
    merge_t = addNewTrapezoid();


      for(size_t i = 0; i <= traps.size() -1; i++){

        t = *(traps[i]);
        if (i == 0){

            splitInThreeLeft(left_t, no_merge_t, merge_t, low_merging, up_merging, t, segment, idLastTrap);

            newTrapsToReturn.push_back(left_t);

            newTrapsToReturn.push_back(no_merge_t);

            newTrapsToReturn.push_back(merge_t);

            //////////////////////
            // NEIGHBOR SETTING //
            //      LEFT_T      //
            //////////////////////

            // assign new trapezoid's neighbor
            if (Algorithm::pointIsAboveSegment(segment, t.getRightPoint())){
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
                    if (t.getId() == t.getUpperLeftNeigh()->getUpperRightNeigh()->getId()){
                        t.getUpperLeftNeigh()->setUpperRightNeigh(left_t);
                    }else{
                        t.getUpperLeftNeigh()->setBottomRightNeigh(left_t);
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

            //////////////////////
            // NEIGHBOR SETTING //
            //    NO_MERGE_T    //
            //////////////////////

            // assign new trapezoid's neighbor
            no_merge_t->setUpperLeftNeigh(left_t);
            no_merge_t->setBottomLeftNeigh(left_t);

            // assign old neighbor inherited from current trapezoid t
            no_merge_t->setUpperRightNeigh(t.getUpperRightNeigh());
            no_merge_t->setBottomRightNeigh(t.getBottomRightNeigh());

            // update other trapezoids affected by no_merge_t
            if (t.getUpperRightNeigh()->getId() != t.getBottomRightNeigh()->getId()){
                if (Algorithm::pointIsAboveSegment(segment, t.getRightPoint())){
                    // update trapezoid that won't be modified
                    t.getUpperRightNeigh()->setUpperLeftNeigh(no_merge_t);
                    t.getUpperRightNeigh()->setBottomLeftNeigh(no_merge_t);
                }else{
                    t.getBottomRightNeigh()->setUpperLeftNeigh(no_merge_t);
                    t.getBottomRightNeigh()->setBottomLeftNeigh(no_merge_t);
                }
            }


            //////////////////////
            // NEIGHBOR SETTING //
            //    MERGE_T       //
            //////////////////////

            // assign new trapezoid's neighbor
            merge_t->setUpperLeftNeigh(left_t);
            merge_t->setBottomLeftNeigh(left_t);

            // assign old neighbor inherited from current trapezoid t
            if (Algorithm::pointIsAboveSegment(segment, t.getRightPoint())){
                merge_t->setUpperRightNeigh(t.getBottomRightNeigh());
                merge_t->setBottomRightNeigh(t.getBottomRightNeigh());
            }else{
                merge_t->setUpperRightNeigh(t.getUpperRightNeigh());
                merge_t->setBottomRightNeigh(t.getUpperRightNeigh());
            }




        }else if (i < traps.size() - 1){

            // MERGING TRAPEZOID
            colorT = cg3::Color(rand()%256, rand()%256, rand()%256);
            if (Algorithm::pointIsAboveSegment(segment, t.getLeftPoint())){
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

                if (t.getUpperRightNeigh()->getId() == t.getBottomRightNeigh()->getId()){
                    low_merging.back()->setUpperRightNeigh(t.getUpperRightNeigh());
                    low_merging.back()->setBottomRightNeigh(t.getBottomRightNeigh());

                }else if (Algorithm::pointIsAboveSegment(segment, t.getRightPoint())){
                    low_merging.back()->setUpperRightNeigh(t.getBottomRightNeigh());
                    low_merging.back()->setBottomRightNeigh(t.getBottomRightNeigh());
                }else{

                    low_merging.back()->setUpperRightNeigh(t.getUpperRightNeigh());
                    low_merging.back()->setBottomRightNeigh(t.getBottomRightNeigh());

                    // ci potrebbero essere problemi che t.getBottomRight abbia
                    // due vicini diversi
                    t.getBottomRightNeigh()->setUpperLeftNeigh(low_merging.back());
                    t.getBottomRightNeigh()->setBottomLeftNeigh(low_merging.back());

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

                if (t.getUpperRightNeigh()->getId() == t.getBottomRightNeigh()->getId()){
                    up_merging.back()->setUpperRightNeigh(t.getUpperRightNeigh());
                    up_merging.back()->setBottomRightNeigh(t.getBottomRightNeigh());
                }else if (Algorithm::pointIsAboveSegment(segment, t.getRightPoint())){
                    up_merging.back()->setUpperRightNeigh(t.getUpperRightNeigh());
                    up_merging.back()->setBottomRightNeigh(t.getBottomRightNeigh());
                }else{
                    up_merging.back()->setUpperRightNeigh(t.getUpperRightNeigh());
                    up_merging.back()->setBottomRightNeigh(t.getUpperRightNeigh());

                    // ci potrebbero essere problemi che t.getUpperRight abbia
                    // due vicini diversi
                    t.getUpperRightNeigh()->setUpperLeftNeigh(up_merging.back());
                    t.getUpperRightNeigh()->setBottomLeftNeigh(up_merging.back());
                }
            }


            // creation of trapezoid that won't be merged

            idLastTrap = trapezoids.back().getId();
            colorT = cg3::Color(rand()%256, rand()%256, rand()%256);
            if (Algorithm::pointIsAboveSegment(segment, t.getLeftPoint())){
                mirror_merge_t = addNewTrapezoid();
                mirror_merge_t->setTrapezoid(++idLastTrap,
                                t.getSegmentUp(),
                                cg3::Segment2d(findIntersectionVerticalLine(segment, t.getLeftPoint()), findIntersectionVerticalLine(segment,t.getRightPoint())),
                                t.getLeftPoint(), t.getRightPoint(),colorT);

                newTrapsToReturn.push_back(mirror_merge_t);



                //////////////////////
                // NEIGHBOR SETTING //
                //////////////////////
                //TODO bisogna assegnare il meigh left a mirror, mettere il no_merge nella lista up_merge
                if (t.getUpperLeftNeigh()->getId() == t.getBottomLeftNeigh()->getId()){

                    mirror_merge_t->setUpperLeftNeigh(up_merging.back());
                    mirror_merge_t->setBottomLeftNeigh(up_merging.back());


                    if (t.getUpperRightNeigh()->getId() == t.getBottomRightNeigh()->getId()){
                        mirror_merge_t->setUpperRightNeigh(t.getUpperRightNeigh());
                        mirror_merge_t->setBottomRightNeigh(t.getBottomRightNeigh());

                    }else{

                        if (Algorithm::pointIsAboveSegment(segment, t.getRightPoint())){
                            mirror_merge_t->setUpperRightNeigh(t.getUpperRightNeigh());
                            mirror_merge_t->setBottomRightNeigh(t.getBottomRightNeigh());

                            // devo modificare il neigh in alto a destra
                            t.getUpperRightNeigh()->setUpperLeftNeigh(mirror_merge_t);
                            t.getUpperRightNeigh()->setBottomLeftNeigh(mirror_merge_t);
                        }else{
                            mirror_merge_t->setUpperRightNeigh(t.getUpperRightNeigh());
                            mirror_merge_t->setBottomRightNeigh(t.getUpperRightNeigh());
                        }

                   }

                    up_merging.back()->setBottomRightNeigh(mirror_merge_t);
                }else{
                    mirror_merge_t->setUpperLeftNeigh(t.getUpperLeftNeigh());
                    mirror_merge_t->setBottomLeftNeigh(up_merging.back());
                    mirror_merge_t->setUpperRightNeigh(t.getUpperRightNeigh());
                    mirror_merge_t->setBottomRightNeigh(t.getBottomRightNeigh());

                    up_merging.back()->setUpperRightNeigh(mirror_merge_t);
                    up_merging.back()->setBottomRightNeigh(mirror_merge_t);

                    t.getUpperLeftNeigh()->setUpperRightNeigh(mirror_merge_t);
                    t.getUpperLeftNeigh()->setBottomRightNeigh(mirror_merge_t);

                    if ((t.getUpperRightNeigh() != t.getBottomRightNeigh()) && Algorithm::pointIsAboveSegment(segment, t.getRightPoint())){
                         t.getUpperRightNeigh()->setUpperLeftNeigh(mirror_merge_t);
                         t.getUpperRightNeigh()->setBottomLeftNeigh(mirror_merge_t);
                    }

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
                if (t.getUpperLeftNeigh()->getId() == t.getBottomLeftNeigh()->getId()){
                    mirror_merge_t->setUpperLeftNeigh(low_merging.back());
                    mirror_merge_t->setBottomLeftNeigh(low_merging.back());


                    if (t.getUpperRightNeigh()->getId() == t.getBottomRightNeigh()->getId()){
                        mirror_merge_t->setUpperRightNeigh(t.getUpperRightNeigh());
                        mirror_merge_t->setBottomRightNeigh(t.getBottomRightNeigh());
                    }else{
                        if (Algorithm::pointIsAboveSegment(segment, t.getRightPoint())){
                            mirror_merge_t->setUpperRightNeigh(t.getBottomRightNeigh());
                            mirror_merge_t->setBottomRightNeigh(t.getBottomRightNeigh());
                        }else{
                            mirror_merge_t->setUpperRightNeigh(t.getUpperRightNeigh());
                            mirror_merge_t->setBottomRightNeigh(t.getBottomRightNeigh());

                            t.getBottomRightNeigh()->setUpperLeftNeigh(mirror_merge_t);
                            t.getBottomRightNeigh()->setBottomLeftNeigh(mirror_merge_t);
                        }

                    }

                    low_merging.back()->setUpperRightNeigh(mirror_merge_t);
                }else{
                    mirror_merge_t->setUpperLeftNeigh(low_merging.back());
                    mirror_merge_t->setBottomLeftNeigh(t.getBottomLeftNeigh());
                    mirror_merge_t->setUpperRightNeigh(t.getUpperRightNeigh());
                    mirror_merge_t->setBottomRightNeigh(t.getBottomRightNeigh());

                    low_merging.back()->setUpperRightNeigh(mirror_merge_t);
                    low_merging.back()->setBottomRightNeigh(mirror_merge_t);

                    t.getBottomLeftNeigh()->setUpperRightNeigh(mirror_merge_t);
                    t.getBottomLeftNeigh()->setBottomRightNeigh(mirror_merge_t);

                    if ((t.getUpperRightNeigh() != t.getBottomRightNeigh()) && !Algorithm::pointIsAboveSegment(segment, t.getRightPoint())){
                        t.getBottomRightNeigh()->setUpperLeftNeigh(mirror_merge_t);
                        t.getBottomRightNeigh()->setBottomLeftNeigh(mirror_merge_t);
                    }

                }

                low_merging.push_back(mirror_merge_t);
            }



//            Algorithm::printNeigh(*this);
        }else{

            idLastTrap = trapezoids.back().getId();

            no_merge_right_t = addNewTrapezoid();
            right = addNewTrapezoid();

            splitInThreeRight(no_merge_right_t, right,low_merging, up_merging, t, segment, idLastTrap);


            newTrapsToReturn.push_back(no_merge_right_t);
            newTrapsToReturn.push_back(right);


            //////////////////////
            // NEIGHBOR SETTING //
            //       RIGHT      //
            //////////////////////

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
                if (t.getBottomRightNeigh() == t.getUpperRightNeigh()){
                    if (t.getId() == t.getUpperRightNeigh()->getUpperLeftNeigh()->getId()){
                        t.getUpperRightNeigh()->setUpperLeftNeigh(right);
                    }else{
                        t.getUpperRightNeigh()->setBottomLeftNeigh(right);
                    }
                }else{
                    t.getUpperRightNeigh()->setUpperLeftNeigh(right);
                    t.getUpperRightNeigh()->setBottomLeftNeigh(right);

                    t.getBottomRightNeigh()->setUpperLeftNeigh(right);
                    t.getBottomRightNeigh()->setBottomLeftNeigh(right);
                }
            }

            //////////////////////
            // NEIGHBOR SETTING //
            //  NO_MERGE_RIGHT  //
            //////////////////////
            no_merge_right_t->setUpperRightNeigh(right);
            no_merge_right_t->setBottomRightNeigh(right);

            if (t.getUpperLeftNeigh()->getId() == t.getBottomLeftNeigh()->getId()){
                if (Algorithm::pointIsAboveSegment(segment, t.getLeftPoint())){
                    no_merge_right_t->setUpperLeftNeigh(up_merging.back());
                    no_merge_right_t->setBottomLeftNeigh(up_merging.back());

                    up_merging.back()->setBottomRightNeigh(no_merge_right_t);
                }else{
                    no_merge_right_t->setUpperLeftNeigh(low_merging.back());
                    no_merge_right_t->setBottomLeftNeigh(low_merging.back());

                    low_merging.back()->setUpperRightNeigh(no_merge_right_t);
                }
            }else{
                if (Algorithm::pointIsAboveSegment(segment, t.getLeftPoint())){
                    no_merge_right_t->setUpperLeftNeigh(t.getUpperLeftNeigh());
                    no_merge_right_t->setBottomLeftNeigh(up_merging.back());

                    up_merging.back()->setUpperRightNeigh(no_merge_right_t);
                    up_merging.back()->setBottomRightNeigh(no_merge_right_t);
                    t.getUpperLeftNeigh()->setUpperRightNeigh(no_merge_right_t);
                    t.getUpperLeftNeigh()->setBottomRightNeigh(no_merge_right_t);

                }else{
                    no_merge_right_t->setUpperLeftNeigh(low_merging.back());
                    no_merge_right_t->setBottomLeftNeigh(t.getBottomLeftNeigh());

                    low_merging.back()->setUpperRightNeigh(no_merge_right_t);
                    low_merging.back()->setBottomRightNeigh(no_merge_right_t);
                    t.getBottomLeftNeigh()->setUpperRightNeigh(no_merge_right_t);
                    t.getBottomLeftNeigh()->setBottomRightNeigh(no_merge_right_t);

                }
            }

            if (Algorithm::pointIsAboveSegment(segment, t.getLeftPoint())){
                low_merging.back()->setUpperRightNeigh(right);
                low_merging.back()->setBottomRightNeigh(right);
            }else{
                up_merging.back()->setUpperRightNeigh(right);
                up_merging.back()->setBottomRightNeigh(right);
            }

//            Algorithm::printNeigh(*this);
        }
    }

    return newTrapsToReturn;

}

void TrapMap::compareNeigh(){
    bool neig = true;
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
            }
            else{
             s += "U-L NULL B-L NULL";
            }

            if (t.getUpperRightNeigh() != nullptr && t.getBottomRightNeigh() != nullptr){

            s += "U-R ";
            s += std::to_string(t.getUpperRightNeigh()->getId());
            s += " B-R ";
            s += std::to_string(t.getBottomRightNeigh()->getId());
            }
            else
                s += "U-R NULL B-R NULL";
        }
   }

    if (prev != ""){
        if (s.compare(prev) == 0){
            std::cout << "i neigh sono uguali" << std::endl;
        }else{
            std::cout << "i neigh sono diversi" << std::endl;
        }
    }

    prev = s;
//        std::cout << s << std::endl;
}
