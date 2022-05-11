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
cg3::Point2d TrapMap::findIntersectionVerticalLine(const cg3::Segment2d& s, const cg3::Point2d& px, bool dir){
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

std::vector<Trapezoid*> TrapMap::addFourTrapezoids(const cg3::Segment2d &segment,  DagNode* bbNode, bool &leftToRight){
    cg3::Segment2d topS, bottomS;
    cg3::Point2d leftP, rightP, p1, q1;
    cg3::Color colorT;

    std::vector<Trapezoid*> trapForDag;
    Trapezoid bb = *(Trapezoid*)bbNode->getData().objj;

    int idLastTrap = trapezoids.back().getId();
    //bool leftToRight = true;

    p1 = segment.p1();
    q1 = segment.p2();

    // segment direction - right to left
//    if (q1.x() < p1.x()){
//        leftToRight = false;
//    }

    if (leftToRight){
    // left trapezoid
    topS = cg3::Segment2d(bb.getSegmentUp().p1(),
                         findIntersectionVerticalLine(bb.getSegmentUp(), p1, leftToRight));

    bottomS = cg3::Segment2d(bb.getSegmentDown().p1(),
                           findIntersectionVerticalLine(bb.getSegmentDown(), p1, leftToRight));

    leftP = topS.p1();
    rightP = p1;
    }
    colorT = cg3::Color(rand()%256, rand()%256, rand()%256);

    if (!leftToRight){
        topS = cg3::Segment2d(bb.getSegmentUp().p1(),
                             findIntersectionVerticalLine(bb.getSegmentUp(), q1, leftToRight));

        bottomS = cg3::Segment2d(bb.getSegmentDown().p1(),
                               findIntersectionVerticalLine(bb.getSegmentDown(), q1, leftToRight));

        leftP = topS.p1();
        rightP = q1;
    }
    Trapezoid left = Trapezoid(++idLastTrap, topS, bottomS, leftP, rightP, colorT);


    // top trapezoid
    if (leftToRight){
    topS = cg3::Segment2d(findIntersectionVerticalLine(bb.getSegmentUp(), p1, leftToRight),
                          findIntersectionVerticalLine(bb.getSegmentUp(), q1, leftToRight));

    bottomS = segment;

    leftP = p1;
    rightP = q1;
}
    colorT = cg3::Color(rand()%256, rand()%256, rand()%256);

    if (!leftToRight){
        topS = cg3::Segment2d(findIntersectionVerticalLine(bb.getSegmentUp(), q1, leftToRight),
                              findIntersectionVerticalLine(bb.getSegmentUp(), p1, leftToRight));

        bottomS = cg3::Segment2d(q1, p1);

        leftP = q1;
        rightP = p1;
    }
    Trapezoid top = Trapezoid(++idLastTrap, topS, bottomS, leftP, rightP, colorT);

    // bottom trapezoid

    if (leftToRight){
    topS = segment;

    bottomS = cg3::Segment2d(findIntersectionVerticalLine(bb.getSegmentDown(), p1, leftToRight),
                             findIntersectionVerticalLine(bb.getSegmentDown(), q1, leftToRight));

    leftP = p1;
    rightP = q1;
}
    colorT = cg3::Color(rand()%256, rand()%256, rand()%256);

    if (!leftToRight){
        topS = cg3::Segment2d(segment.p2(), segment.p1());;

        bottomS = cg3::Segment2d(findIntersectionVerticalLine(bb.getSegmentDown(), q1, leftToRight),
                                 findIntersectionVerticalLine(bb.getSegmentDown(), p1, leftToRight));

        leftP = q1;
        rightP = p1;
    }
    Trapezoid bottom = Trapezoid(++idLastTrap, topS, bottomS, leftP, rightP, colorT);

//     right trapezoid
if (leftToRight){

    topS = cg3::Segment2d(findIntersectionVerticalLine(bb.getSegmentUp(), q1, leftToRight),
                          bb.getSegmentUp().p2());

    bottomS = cg3::Segment2d(findIntersectionVerticalLine(bb.getSegmentDown(), q1, leftToRight),
                             bb.getSegmentDown().p2());

    leftP = q1;
    rightP = topS.p2();
}
    colorT = cg3::Color(rand()%256, rand()%256, rand()%256);

    if(!leftToRight){
        topS = cg3::Segment2d(findIntersectionVerticalLine(bb.getSegmentUp(), p1, leftToRight),
                              bb.getSegmentUp().p2());

        bottomS = cg3::Segment2d(findIntersectionVerticalLine(bb.getSegmentDown(), p1, leftToRight),
                                 bb.getSegmentDown().p2());

        leftP = p1;
        rightP = topS.p2();
    }
    Trapezoid right = Trapezoid(++idLastTrap, topS, bottomS, leftP, rightP, colorT);

    // elimino il trapezoide che é il bb
    //deleteTrapezoidWithId(bb.getId());

    // add new trapezoids
    trapezoids.push_back(left);
    trapezoids.push_back(top);
    trapezoids.push_back(bottom);
    trapezoids.push_back(right);

    trapForDag.push_back(getTrapezoidWithId(left.getId()));
    trapForDag.push_back(getTrapezoidWithId(top.getId()));
    trapForDag.push_back(getTrapezoidWithId(bottom.getId()));
    trapForDag.push_back(getTrapezoidWithId(right.getId()));

    return trapForDag;
}
