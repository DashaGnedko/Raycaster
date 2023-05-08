#include "ray.h"

#include <cmath>
#include <QDebug>

Ray::Ray(const QPointF& begin_, const QPointF& end_) {
    begin = begin_;
    end = end_;
    calculateRay();
}

Ray::Ray(const QPointF& begin_, long double angle_) {
    begin = begin_;
    end = QPointF(begin_.x() + cos(angle_), begin_.y() + sin(angle_));
    angle = angle_;
}

void Ray::calculateRay() {

    long double length = (end.x() - begin.x()) * (end.x() - begin.x());
    length += (end.y() - begin.y()) * (end.y() - begin.y());
    length = std::sqrt(length);

    angle = (end.x() - begin.x()) / length;
    angle = acos(angle);
    if (begin.y() > end.y()) angle = -angle;
}

QPointF Ray::getBegin() const {
    return begin;
}

QPointF Ray::getEnd() const {
    return end;
}
double Ray::getAngle() const {
    return angle;
}

void Ray::setBegin(const QPointF& begin_) {
    begin = begin_;
    calculateRay();
}

void Ray::setEnd(const QPointF& end_) {
    end = end_;
    calculateRay();
}

Ray Ray::rotate(double angle_) const {
    return Ray(begin, angle + angle_);
}
