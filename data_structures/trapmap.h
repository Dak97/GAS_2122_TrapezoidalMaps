#ifndef TRAPMAP_H
#define TRAPMAP_H

#include <cg3/geometry/segment2.h>
#include <cg3/geometry/point2.h>
#include "data_structures/trapezoid.h"
#include "data_structures/dagnode.h"
#include <cg3/utilities/color.h>
//#include "algoritms/algoritms.h"
class TrapMap
{
public:
    TrapMap();

    std::vector<Trapezoid*> addFourTrapezoids(const cg3::Segment2d &segment, DagNode* bbNode);
    std::vector<Trapezoid*> newTrapezoids(const cg3::Segment2d &segment,  std::vector<Trapezoid*>& traps);
    void splitInThreeLeft(Trapezoid *a, Trapezoid *b, Trapezoid *c_1, std::vector<Trapezoid*>& low, std::vector<Trapezoid*>& up,
                      Trapezoid& bb,  const cg3::Segment2d& segment, int id);
    void splitInThreeRight(Trapezoid *b, Trapezoid *c_1, std::vector<Trapezoid*>& low, std::vector<Trapezoid*>& up,
                      Trapezoid& bb,  const cg3::Segment2d& segment, int id);

    // restituisco una lista di trapezoidi
    const std::list<Trapezoid> getTrapezoids() const;

    Trapezoid* getTrapezoidWithId(const int id);
    int getIndexWithId(const int id);

    void deleteTrapezoidWithId(const int id);
    void deleteTrapezoidByRef(std::vector<Trapezoid*> traps);
    void deleteTrapezoidByRef(Trapezoid* trap);
    Trapezoid* addNewTrapezoid();
    void init();

    void clear();

    void compareNeigh();
private:

    Trapezoid boundingBox;
    // vettore di trapezoidi
    std::list<Trapezoid> trapezoids;

    std::string prev;


    cg3::Point2d findIntersectionVerticalLine(const cg3::Segment2d& s, const cg3::Point2d& px);
//    bool pointIsAboveSegment(cg3::Segment2d segment, cg3::Point2d point);
    void updateNeigh(Trapezoid *a, Trapezoid *b, const cg3::Segment2d& s);

};

#endif // TRAPMAP_H
