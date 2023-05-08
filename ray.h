#ifndef RAY_H
#define RAY_H

#include <QPoint>

class Ray {
public:
    Ray(const QPointF& begin, const QPointF& end);
    Ray(const QPointF&, long double);

    void calculateRay();

    QPointF getBegin() const;
    QPointF getEnd() const;
    double getAngle() const;

    void setBegin(const QPointF&);
    void setEnd(const QPointF&);

    Ray rotate(double angle) const;

private:
    QPointF begin;
    QPointF end;
    double angle;
};

#endif // RAY_H
