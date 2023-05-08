#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <polygon.h>

class Controller
{
public:
    Controller();

    const std::vector<Polygon>& getPolygons();
    void addPolygon(const Polygon&);
    void addVertexToLastPolygon(const QPointF&);
    void changeFirstPolygon(const Polygon&);
    void deleteVertexFromLastPolygon();
    void updateLastPolygon(const QPointF&);

    void setLightSourse(const QPointF&);
    QPointF getLightSourse();

    void clearAll();

    void rebuildPolygons();

    std::vector<Ray> castRays(const QPointF&) const;
    void intersectRays(std::vector<Ray>*) const;
    void removeAdjacentRays(std::vector<Ray>&) const;
    Polygon createLightArea() const;
    Polygon createLightArea(const QPointF&) const;

private:
    std::vector<Polygon> polygons;
    QPointF lightSource;
};

#endif // CONTROLLER_H
