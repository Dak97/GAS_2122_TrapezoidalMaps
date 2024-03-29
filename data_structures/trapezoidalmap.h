#ifndef TRAPEZOIDALMAP_H
#define TRAPEZOIDALMAP_H

#include <cg3/geometry/segment2.h>
#include <cg3/geometry/point2.h>
#include "data_structures/trapezoid.h"
#include "data_structures/dagnode.h"
#include <cg3/utilities/color.h>
//#include "algoritms/algoritms.h"
class TrapezoidalMap
{
public:
    TrapezoidalMap();

    std::vector<Trapezoid*> newTrapezoidsSingleSplit(const cg3::Segment2d &segment, Trapezoid* bb,
                                                     bool &left_coincident, bool &right_coincident);
    std::vector<Trapezoid*> newTrapezoidsMultipleSplit(const cg3::Segment2d &segment,  std::vector<Trapezoid*>& traps,
                                                       bool &left_coincident, bool &right_coincident);
    void splitInThreeLeft(Trapezoid *a, Trapezoid *b, Trapezoid *c_1, std::vector<Trapezoid*>& low, std::vector<Trapezoid*>& up,
                      Trapezoid& bb,  const cg3::Segment2d& segment, int id, bool& right_above_segment);
    void splitInThreeRight(Trapezoid *b, Trapezoid *c_1, std::vector<Trapezoid*>& low, std::vector<Trapezoid*>& up,
                      Trapezoid& bb,  const cg3::Segment2d& segment, int id, bool& left_above_segment);
    void assignNeighborsLeftSplit(Trapezoid t, Trapezoid *left_t, Trapezoid *no_merge_t, Trapezoid *merge_t, bool& left_coincident, bool& right_above_segment);
    void assignNeighborsRightSplit(Trapezoid t, Trapezoid *right, Trapezoid *no_merge_right_t, std::vector<Trapezoid*> &up_merging,
                                   std::vector<Trapezoid*> &low_merging, bool& right_coincident, bool& left_above_segment);
    void splitInTwo(Trapezoid t,  std::vector<Trapezoid*>& up_merging, std::vector<Trapezoid*>& low_merging, Trapezoid *mirror_merge_t,
                             std::vector<Trapezoid*>& newTrapsToReturnconst, const cg3::Segment2d& segment, bool& right_above_segment,
                    bool& left_above_segment);
    const std::list<Trapezoid> getTrapezoids() const;

    Trapezoid* getFirstTrapezoid();
    int getIndexWithId(const int id);
    void deleteTrapezoidByRef(std::vector<Trapezoid*> traps);
    Trapezoid* addNewTrapezoid();
    void init();
    void clear();

private:

    std::list<Trapezoid> trapezoids;

    cg3::Point2d findIntersectionVerticalLine(const cg3::Segment2d& s, const cg3::Point2d& px);

};

#endif // TRAPEZOIDALMAP_H
