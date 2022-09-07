#ifndef TRAPEZOID_H
#define TRAPEZOID_H

#include <cg3/geometry/segment2.h>

#include <cg3/utilities/color.h>

#include "data_structures/dagnode.h"

#define BB 1e+6

class Trapezoid {

public:
    Trapezoid();

    const cg3::Segment2d &getSegmentUp() const;
    void setSegmentUp(const cg3::Segment2d &newSegmentUp);

    const cg3::Segment2d &getSegmentDown() const;
    void setSegmentDown(const cg3::Segment2d &newSegmentDown);

    cg3::Point2d getLeftPoint() const;
    void setLeftPoint(cg3::Point2d newLeftPoint);

    cg3::Point2d getRightPoint() const;
    void setRightPoint(cg3::Point2d newRightPoint);

    const cg3::Color &getColor() const;
    void setColor(const cg3::Color &newColor);

    int getId() const;
    void setId(int newId);

    Trapezoid *getTopLeftNeigh() const;
    void setTopLeftNeigh(Trapezoid *newUpperLeftNeigh);

    Trapezoid *getBottomLeftNeigh() const;
    void setBottomLeftNeigh(Trapezoid *newBottomLeftNeigh);

    Trapezoid *getTopRightNeigh() const;
    void setTopRightNeigh(Trapezoid *newUpperRightNeigh);

    Trapezoid *getBottomRightNeigh() const;
    void setBottomRightNeigh(Trapezoid *newBottomRightNeigh);

    void setNeighbor(Trapezoid *newUpperLeftNeigh, Trapezoid *newBottomLeftNeigh,
                     Trapezoid *newUpperRightNeigh, Trapezoid *newBottomRightNeigh);

    void setTrapezoid(int id, cg3::Segment2d segmentUp, cg3::Segment2d segmentDown,
                         cg3::Point2d leftP, cg3::Point2d rightP, const cg3::Color color);

    DagNode *getRefToDag() const;
    void setRefToDag(DagNode *newRefToDag);

    bool getHighlighted() const;
    void setHighlighted(bool newHighlighted);

    bool isLeftDegenerate();
    bool isRightDegenerate();

    bool sameLeftNeighbor();
    bool sameRightNeighbor();

    const std::list<Trapezoid>::iterator &getRefToIter() const;
    void setRefToIter(const std::list<Trapezoid>::iterator &newRefToIter);

private:
    int id;

    cg3::Segment2d segmentUp;
    cg3::Segment2d segmentDown;
    cg3::Point2d leftPoint;
    cg3::Point2d rightPoint;

    Trapezoid* topLeftNeigh;
    Trapezoid* bottomLeftNeigh;
    Trapezoid* topRightNeigh;
    Trapezoid* bottomRightNeigh;

    cg3::Color color;
    bool highlighted;

    DagNode* refToDag; // reference to the DAG node
    std::list<Trapezoid>::iterator refToIter; // reference to the iterator list

};

#endif // TRAPEZOID_H
