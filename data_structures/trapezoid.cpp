#include "trapezoid.h"
#include "algorithm.h"
/**
 * @brief Trapezoid::Trapezoid Default constructor of the trapezoid object (it id the bounding box)
 */
Trapezoid::Trapezoid(){
    id = 0;
    segmentUp = cg3::Segment2d(cg3::Point2d(-BB,BB),cg3::Point2d(BB,BB));
    segmentDown = cg3::Segment2d(cg3::Point2d(-BB,-BB),cg3::Point2d(BB,-BB));
    leftPoint = cg3::Point2d(-BB,-BB);
    rightPoint = cg3::Point2d(BB,-BB);
    color = cg3::Color(255,255,255);
    highlighted = false;
    topLeftNeigh = nullptr;
    bottomLeftNeigh = nullptr;
    topRightNeigh = nullptr;
    bottomRightNeigh = nullptr;
    refToDag = nullptr;
}
/**
 * @brief Trapezoid::Trapezoid Constructor of the trapezoid
 * @param id trapezoid id
 * @param segmentUp segment up of the trapezoid
 * @param segmentDown segment down of the trapezoid
 * @param leftP left point of the trapezoid
 * @param rightP right point of the trapezoid
 * @param color trapezoid color
 */
Trapezoid::Trapezoid(int id, cg3::Segment2d segmentUp, cg3::Segment2d segmentDown,
                     cg3::Point2d leftP, cg3::Point2d rightP, const cg3::Color color) :
    id(id),
    segmentUp(segmentUp),
    segmentDown(segmentDown),
    leftPoint(leftP),
    rightPoint(rightP),
    color(color),
    highlighted(false)

{
}
/**
 * @brief Trapezoid::Trapezoid Constructor of the trapezoid
 * @param id trapezoid id
 * @param segmentUp segment up of the trapezoid
 * @param segmentDown segment down of the trapezoid
 * @param leftP left point of the trapezoid
 * @param rightP right point of the trapezoid
 * @param upperLeftN top left neighbor
 * @param bottomLeftN bottom left neighbor
 * @param upperRightN top right neighbor
 * @param bottomRightN bottom right neighbor
 * @param color trapezoid color
 */
Trapezoid::Trapezoid(int id, cg3::Segment2d &segmentUp, cg3::Segment2d &segmentDown,
                     cg3::Point2d& leftP, cg3::Point2d& rightP, Trapezoid* upperLeftN,
                     Trapezoid* bottomLeftN, Trapezoid* upperRightN, Trapezoid* bottomRightN,
                     const cg3::Color color) :
    id(id),
    segmentUp(segmentUp),
    segmentDown(segmentDown),
    leftPoint(leftP),
    rightPoint(rightP),
    topLeftNeigh(upperLeftN),
    bottomLeftNeigh(bottomLeftN),
    topRightNeigh(upperRightN),
    bottomRightNeigh(bottomRightN),
    color(color),
    highlighted(false)

{

}
/**
 * @brief Trapezoid::getSegmentUp return segment up
 * @return segment up
 */
const cg3::Segment2d &Trapezoid::getSegmentUp() const
{
    return segmentUp;
}
/**
 * @brief Trapezoid::setSegmentUp set the segment up
 * @param newSegmentUp segment up
 */
void Trapezoid::setSegmentUp(const cg3::Segment2d &newSegmentUp)
{
    segmentUp = newSegmentUp;
}
/**
 * @brief Trapezoid::getSegmentDown return segment down
 * @return segment down
 */
const cg3::Segment2d &Trapezoid::getSegmentDown() const
{
    return segmentDown;
}
/**
 * @brief Trapezoid::setSegmentDown set the segment down
 * @param newSegmentDown segment down
 */
void Trapezoid::setSegmentDown(const cg3::Segment2d &newSegmentDown)
{
    segmentDown = newSegmentDown;
}
/**
 * @brief Trapezoid::getLeftPoint return left point trapezoid
 * @return the left point
 */
cg3::Point2d Trapezoid::getLeftPoint() const
{
    return leftPoint;
}
/**
 * @brief Trapezoid::setLeftPoint set the left point trapezoid
 * @param newLeftPoint the left point
 */
void Trapezoid::setLeftPoint(cg3::Point2d newLeftPoint)
{
    leftPoint = newLeftPoint;
}
/**
 * @brief Trapezoid::getRightPoint return right point trapezoid
 * @return the right point
 */
cg3::Point2d Trapezoid::getRightPoint() const
{
    return rightPoint;
}
/**
 * @brief Trapezoid::setRightPoint set the right point trapezoid
 * @param newLeftPoint the right point
 */
void Trapezoid::setRightPoint(cg3::Point2d newRightPoint)
{
    rightPoint = newRightPoint;
}
/**
 * @brief Trapezoid::getColor return trapezoid color
 * @return trapezoid color
 */
const cg3::Color &Trapezoid::getColor() const
{
    return color;
}
/**
 * @brief Trapezoid::setColor set trapezoid color
 * @param newColor trapezoid color
 */
void Trapezoid::setColor(const cg3::Color &newColor)
{
    color = newColor;
}
/**
 * @brief Trapezoid::getId return trapezoid id
 * @return trapezoid id
 */
int Trapezoid::getId() const
{
    return id;
}
/**
 * @brief Trapezoid::setId set trapezoid id
 * @param newId trapezoid id
 */
void Trapezoid::setId(int newId)
{
    id = newId;
}

/**
 * @brief Trapezoid::getUpperLeftNeigh return the top left neighbor
 * @return top left neighbor
 */
Trapezoid *Trapezoid::getTopLeftNeigh() const
{
    return topLeftNeigh;
}
/**
 * @brief Trapezoid::setUpperLeftNeigh set top left neighbor
 * @param newUpperLeftNeigh top left neighbor
 */
void Trapezoid::setTopLeftNeigh(Trapezoid *newUpperLeftNeigh)
{
    topLeftNeigh = newUpperLeftNeigh;
}
/**
 * @brief Trapezoid::getBottomLeftNeigh return the bottom left neighbor
 * @return bottom left neighbor
 */
Trapezoid *Trapezoid::getBottomLeftNeigh() const
{
    return bottomLeftNeigh;
}
/**
 * @brief Trapezoid::setBottomLeftNeigh
 * @param newBottomLeftNeigh
 */
void Trapezoid::setBottomLeftNeigh(Trapezoid *newBottomLeftNeigh)
{
    bottomLeftNeigh = newBottomLeftNeigh;
}
/**
 * @brief Trapezoid::getUpperRightNeigh return the top right neighbor
 * @return top right neighbor
 */
Trapezoid *Trapezoid::getTopRightNeigh() const
{
    return topRightNeigh;
}
/**
 * @brief Trapezoid::setUpperRightNeigh set top right neighbor
 * @param newUpperRightNeigh top right neighbor
 */
void Trapezoid::setTopRightNeigh(Trapezoid *newUpperRightNeigh)
{
    topRightNeigh = newUpperRightNeigh;
}
/**
 * @brief Trapezoid::getBottomRightNeigh return bottom right neighbor
 * @return bottom right neighbor
 */
Trapezoid *Trapezoid::getBottomRightNeigh() const
{
    return bottomRightNeigh;
}
/**
 * @brief Trapezoid::setBottomRightNeigh set bottom right neighbor
 * @param newBottomRightNeigh bottom right neighbor
 */
void Trapezoid::setBottomRightNeigh(Trapezoid *newBottomRightNeigh)
{
    bottomRightNeigh = newBottomRightNeigh;
}
/**
 * @brief Trapezoid::setNeighbor set all neighbors
 * @param newUpperLeftNeigh top left neighbor
 * @param newBottomLeftNeigh bottom left neighbor
 * @param newUpperRightNeigh top right neighbor
 * @param newBottomRightNeigh bottom right neighbor
 */
void Trapezoid::setNeighbor(Trapezoid *newUpperLeftNeigh, Trapezoid *newBottomLeftNeigh,
                            Trapezoid *newUpperRightNeigh, Trapezoid *newBottomRightNeigh)
{
    topLeftNeigh = newUpperLeftNeigh;
    bottomLeftNeigh = newBottomLeftNeigh;
    topRightNeigh = newUpperRightNeigh;
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
/**
 * @brief Trapezoid::getRefToDag return a DAG node where this trapezoid is
 * @return a DAG node
 */
DagNode *Trapezoid::getRefToDag() const
{
    return refToDag;
}
/**
 * @brief Trapezoid::setRefToDag set a reference of a DAG node
 * @param newRefToDag a DAG node
 */
void Trapezoid::setRefToDag(DagNode *newRefToDag)
{
    refToDag = newRefToDag;
}
/**
 * @brief Trapezoid::getHighlighted return if this trapezoid was quered
 * @return true if it was quered, otherwise
 */
bool Trapezoid::getHighlighted() const
{

    return highlighted;
}
/**
 * @brief Trapezoid::setHighlighted set if this trapezoid was quered
 * @param newHighlighted true if this trapezoid was quered
 */
void Trapezoid::setHighlighted(bool newHighlighted)
{
    highlighted = newHighlighted;
}
/**
 * @brief Trapezoid::isLeftDegenerate check if the trapezoid is left degenerate
 * @return true if it is, otherwise
 */
bool Trapezoid::isLeftDegenerate(){
    if (Algorithm::pointsAreEquals(segmentUp.p1(), segmentDown.p1()))
        return true;
    else
        return false;
}
/**
 * @brief Trapezoid::isRightDegenerate check if the trapezoid is right degenerate
 * @return true if it is, otherwise
 */
bool Trapezoid::isRightDegenerate(){
    if (Algorithm::pointsAreEquals(segmentUp.p2(), segmentDown.p2()))
        return true;
    else
        return false;
}
/**
 * @brief Trapezoid::sameLeftNeighbor check if trapezoid has same left neghbors
 * @return true if it has, otherwise
 */
bool Trapezoid::sameLeftNeighbor(){
    if (topLeftNeigh == nullptr && bottomLeftNeigh == nullptr)
        return true;
    else if (topLeftNeigh->getId() == bottomLeftNeigh->getId())
        return true;
    else
        return false;
}
/**
 * @brief Trapezoid::sameRightNeighbor check if trapezoid has same right neighbors
 * @return true if it has, otherwise
 */
bool Trapezoid::sameRightNeighbor(){
    if (topRightNeigh == nullptr && bottomRightNeigh == nullptr)
        return true;

    if (topRightNeigh->getId() == bottomRightNeigh->getId())
        return true;

    return false;
}
/**
 * @brief Trapezoid::getRefToIter return the reference to the trapezoid in the list
 * @return iterator of the trapezoids list
 */
const std::list<Trapezoid>::iterator &Trapezoid::getRefToIter() const
{
    return refToIter;
}
/**
 * @brief Trapezoid::setRefToIter set the reference to the trapezoid in the list
 * @param newRefToIter iterator of the trapezoids list
 */
void Trapezoid::setRefToIter(const std::list<Trapezoid>::iterator &newRefToIter)
{
    refToIter = newRefToIter;
}
bool operator==(Trapezoid& l, Trapezoid& r)  {
    if (l.getId() == r.getId())
        return true;
    else
        return false;
}
bool operator!=(Trapezoid& l, Trapezoid& r){
    if (l.getId() != r.getId())
        return true;
    else
        return false;
}
