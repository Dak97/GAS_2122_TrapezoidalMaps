#include "trapezoid.h"

Trapezoid::Trapezoid(){}

Trapezoid::Trapezoid(const cg3::Segment2d &segmentUp, const cg3::Segment2d &segmentDown,
                     const cg3::Color color) :
    segmentUp(segmentUp),
    segmentDown(segmentDown),
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

const cg3::Segment2d &Trapezoid::getSegmentLeft() const
{
    return segmentLeft;
}

void Trapezoid::setSegmentLeft(const cg3::Segment2d &newSegmentLeft)
{
    segmentLeft = newSegmentLeft;
}

const cg3::Segment2d &Trapezoid::getSegmentRight() const
{
    return segmentRight;
}

void Trapezoid::setSegmentRight(const cg3::Segment2d &newSegmentRight)
{
    segmentRight = newSegmentRight;
}

const cg3::Color &Trapezoid::getColor() const
{
    return color;
}

void Trapezoid::setColor(const cg3::Color &newColor)
{
    color = newColor;
}
