#ifndef TRAPEZOID_H
#define TRAPEZOID_H

#include <cg3/geometry/segment2.h>

class Trapezoid {

public:
    Trapezoid();
    Trapezoid(const cg3::Segment2d &segmentUp, const cg3::Segment2d &segmentDown);

    // setter e getter del segmento alto del trapezoide
    const cg3::Segment2d &getSegmentUp() const;
    void setSegmentUp(const cg3::Segment2d &newSegmentUp);

    // setter e getter del segmento basso del trapezoide
    const cg3::Segment2d &getSegmentDown() const;
    void setSegmentDown(const cg3::Segment2d &newSegmentDown);

    // setter e getter del segmento inserito
    const cg3::Segment2d &getSegmentLeft() const;
    void setSegmentLeft(const cg3::Segment2d &newSegmentLeft);

    const cg3::Segment2d &getSegmentRight() const;
    void setSegmentRight(const cg3::Segment2d &newSegmentRight);

private:
    cg3::Segment2d segmentUp;
    cg3::Segment2d segmentDown;
    cg3::Segment2d segmentLeft;
    cg3::Segment2d segmentRight;

};

#endif // TRAPEZOID_H
