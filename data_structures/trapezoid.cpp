#include "trapezoid.h"

Trapezoid::Trapezoid(){
    id = 0;
    segmentUp = cg3::Segment2d(cg3::Point2d(-BB,BB),cg3::Point2d(BB,BB));
    segmentDown = cg3::Segment2d(cg3::Point2d(-BB,-BB),cg3::Point2d(BB,-BB));
    leftPoint = cg3::Point2d(-BB,BB);
    rightPoint = cg3::Point2d(BB,BB);
    color = cg3::Color(255,255,255);
    upperLeftNeigh = nullptr;
    bottomLeftNeigh = nullptr;
    upperRightNeigh = nullptr;
    bottomRightNeigh = nullptr;
    refToDag = nullptr;
}

Trapezoid::Trapezoid(int id, cg3::Segment2d segmentUp, cg3::Segment2d segmentDown,
                     cg3::Point2d leftP, cg3::Point2d rightP, const cg3::Color color) :
    id(id),
    segmentUp(segmentUp),
    segmentDown(segmentDown),
    leftPoint(leftP),
    rightPoint(rightP),
    color(color)

{

}
Trapezoid::Trapezoid(int id, cg3::Segment2d &segmentUp, cg3::Segment2d &segmentDown,
                     cg3::Point2d& leftP, cg3::Point2d& rightP, Trapezoid* upperLeftN,
                     Trapezoid* bottomLeftN, Trapezoid* upperRightN, Trapezoid* bottomRightN,
                     const cg3::Color color) :
    id(id),
    segmentUp(segmentUp),
    segmentDown(segmentDown),
    leftPoint(leftP),
    rightPoint(rightP),
    upperLeftNeigh(upperLeftN),
    bottomLeftNeigh(bottomLeftN),
    upperRightNeigh(upperRightN),
    bottomRightNeigh(bottomRightN),
    color(color)

{

}

const cg3::Segment2d &Trapezoid::getSegmentUp() const
{
    return segmentUp;
}

void Trapezoid::setSegmentUp(const cg3::Segment2d &newSegmentUp)
{
    segmentUp = newSegmentUp;
}

const cg3::Segment2d &Trapezoid::getSegmentDown() const
{
    return segmentDown;
}

void Trapezoid::setSegmentDown(const cg3::Segment2d &newSegmentDown)
{
    segmentDown = newSegmentDown;
}

cg3::Point2d Trapezoid::getLeftPoint() const
{
    return leftPoint;
}

void Trapezoid::setLeftPoint(cg3::Point2d newLeftPoint)
{
    leftPoint = newLeftPoint;
}

cg3::Point2d Trapezoid::getRightPoint() const
{
    return rightPoint;
}

void Trapezoid::setRightPoint(cg3::Point2d newRightPoint)
{
    rightPoint = newRightPoint;
}

const cg3::Color &Trapezoid::getColor() const
{
    return color;
}

void Trapezoid::setColor(const cg3::Color &newColor)
{
    color = newColor;
}

int Trapezoid::getId() const
{
    return id;
}

void Trapezoid::setId(int newId)
{
    id = newId;
}

Trapezoid *Trapezoid::getUpperLeftNeigh() const
{
    return upperLeftNeigh;
}

void Trapezoid::setUpperLeftNeigh(Trapezoid *newUpperLeftNeigh)
{
    upperLeftNeigh = newUpperLeftNeigh;
}

Trapezoid *Trapezoid::getBottomLeftNeigh() const
{
    return bottomLeftNeigh;
}

void Trapezoid::setBottomLeftNeigh(Trapezoid *newBottomLeftNeigh)
{
    bottomLeftNeigh = newBottomLeftNeigh;
}

Trapezoid *Trapezoid::getUpperRightNeigh() const
{
    return upperRightNeigh;
}

void Trapezoid::setUpperRightNeigh(Trapezoid *newUpperRightNeigh)
{
    upperRightNeigh = newUpperRightNeigh;
}

Trapezoid *Trapezoid::getBottomRightNeigh() const
{
    return bottomRightNeigh;
}

void Trapezoid::setBottomRightNeigh(Trapezoid *newBottomRightNeigh)
{
    bottomRightNeigh = newBottomRightNeigh;
}

void Trapezoid::setNeighbor(Trapezoid *newUpperLeftNeigh, Trapezoid *newBottomLeftNeigh,
                            Trapezoid *newUpperRightNeigh, Trapezoid *newBottomRightNeigh)
{
    upperLeftNeigh = newUpperLeftNeigh;
    bottomLeftNeigh = newBottomLeftNeigh;
    upperRightNeigh = newUpperRightNeigh;
    bottomRightNeigh = newBottomRightNeigh;
}

void Trapezoid::setTrapezoid(int _id, cg3::Segment2d _segmentUp, cg3::Segment2d _segmentDown,
                  cg3::Point2d _leftP, cg3::Point2d _rightP, const cg3::Color _color)
{
    id = _id;
    segmentUp = _segmentUp;
    segmentDown = _segmentDown;
    leftPoint = _leftP;
    rightPoint = _rightP;
    color = _color;
}

DagNode *Trapezoid::getRefToDag() const
{
    return refToDag;
}

void Trapezoid::setRefToDag(DagNode *newRefToDag)
{
    refToDag = newRefToDag;
}
