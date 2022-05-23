#ifndef TRAPEZOID_H
#define TRAPEZOID_H

#include <cg3/geometry/segment2.h>

#include <cg3/utilities/color.h>

#include "data_structures/dagnode.h"

#define BB 1e+6

class Trapezoid {

public:
    Trapezoid();

    Trapezoid(int id, cg3::Segment2d segmentUp, cg3::Segment2d segmentDown,
                         cg3::Point2d leftP, cg3::Point2d rightP, const cg3::Color color);

    Trapezoid(int id, cg3::Segment2d &segmentUp, cg3::Segment2d &segmentDown,
              cg3::Point2d& leftP, cg3::Point2d& rightP, Trapezoid* upperLeftN,
              Trapezoid* bottomLeftN, Trapezoid* upperRightN, Trapezoid* bottomRightN,
              const cg3::Color color);

    // setter e getter del segmento alto del trapezoide
    const cg3::Segment2d &getSegmentUp() const;
    void setSegmentUp(const cg3::Segment2d &newSegmentUp);

    // setter e getter del segmento basso del trapezoide
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

    Trapezoid *getUpperLeftNeigh() const;
    void setUpperLeftNeigh(Trapezoid *newUpperLeftNeigh);

    Trapezoid *getBottomLeftNeigh() const;
    void setBottomLeftNeigh(Trapezoid *newBottomLeftNeigh);

    Trapezoid *getUpperRightNeigh() const;
    void setUpperRightNeigh(Trapezoid *newUpperRightNeigh);

    Trapezoid *getBottomRightNeigh() const;
    void setBottomRightNeigh(Trapezoid *newBottomRightNeigh);

    void setNeighbor(Trapezoid *newUpperLeftNeigh, Trapezoid *newBottomLeftNeigh,
                     Trapezoid *newUpperRightNeigh, Trapezoid *newBottomRightNeigh);

    void setTrapezoid(int id, cg3::Segment2d segmentUp, cg3::Segment2d segmentDown,
                         cg3::Point2d leftP, cg3::Point2d rightP, const cg3::Color color);

    DagNode *getRefToDag() const;
    void setRefToDag(DagNode *newRefToDag);

private:
    int id;

    cg3::Segment2d segmentUp;
    cg3::Segment2d segmentDown;
    cg3::Point2d leftPoint;
    cg3::Point2d rightPoint;

    Trapezoid* upperLeftNeigh;
    Trapezoid* bottomLeftNeigh;
    Trapezoid* upperRightNeigh;
    Trapezoid* bottomRightNeigh;
    cg3::Color color;

    DagNode* refToDag;

};

#endif // TRAPEZOID_H
