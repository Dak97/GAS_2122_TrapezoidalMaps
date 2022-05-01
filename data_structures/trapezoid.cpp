#include "trapezoid.h"

Trapezoid::Trapezoid(){
    segmentUp = cg3::Segment2d(cg3::Point2d(-BB,BB),cg3::Point2d(BB,BB));
    segmentDown = cg3::Segment2d(cg3::Point2d(-BB,-BB),cg3::Point2d(BB,-BB));
    leftPoint = cg3::Point2d(-BB,BB);
    rightPoint = cg3::Point2d(BB,BB);
    color = cg3::Color(255,255,255);
    id = 0;
}

Trapezoid::Trapezoid(int id, cg3::Segment2d &segmentUp, cg3::Segment2d &segmentDown,
                     cg3::Point2d& leftP, cg3::Point2d& rightP,
                     const cg3::Color color) :
    id(id),
    segmentUp(segmentUp),
    segmentDown(segmentDown),
    leftPoint(leftP),
    rightPoint(rightP),
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
