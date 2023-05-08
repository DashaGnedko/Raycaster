#ifndef POLYGON_H
#define POLYGON_H

#include <ray.h>
#include <vector>
#include <optional>

const long double eps = 0.0001;

class Polygon
{
public:
    Polygon(const std::vector<QPointF>&);

    const QPointF* getFirstPoint() const;
    QPointF operator[](int) const;
    int getSize() const;

    void addVertex(const QPointF&);
    void deleteVertex();
    void updateLastVertex(const QPointF&);

    static long double calculateSquareDistance(const QPointF&, const QPointF&);
    int sign(long double) const;
    bool dotOnLine(const Ray&, QPointF) const;
    bool dotOnRay(const Ray&, QPointF) const;

    std::optional<QPointF> intersection(const Ray&, const Ray&) const;
    std::optional<QPointF> intersectRay(const Ray&) const;
    std::optional<QPointF> intersectionLine(const Ray&, const Ray&) const;
    void rebuild(const Polygon&);

private:
    std::vector<QPointF> vertices;
};

#endif // POLYGON_H
