#include "controller.h"

Controller::Controller() {
}

const std::vector<Polygon>& Controller::getPolygons() {
    return polygons;
}

void Controller::addPolygon(const Polygon& polygon_) {
    polygons.push_back(polygon_);
}

void Controller::addVertexToLastPolygon(const QPointF& newVertex) {
    polygons.back().addVertex(newVertex);
}

void Controller::changeFirstPolygon(const Polygon& polygon_) {
    polygons[0] = polygon_;
}

void Controller::deleteVertexFromLastPolygon() {
    polygons.back().deleteVertex();
}

void Controller::updateLastPolygon(const QPointF& newVertex) {
    polygons.back().updateLastVertex(newVertex);
}

void Controller::setLightSourse(const QPointF& lightSource_) {
    lightSource = lightSource_;
}

QPointF Controller::getLightSourse() {
    return lightSource;
}

void Controller::clearAll() {
    polygons.clear();
}

void Controller::rebuildPolygons() {
    for (size_t i = 0; i < polygons.size(); i++) {
        for (size_t j = 0; j < polygons.size(); j++) {
            polygons[i].rebuild(polygons[j]);
        }
    }
}

std::vector<Ray> Controller::castRays(const QPointF& light) const {
    std::vector<Ray> result;
    for (const Polygon& polygon: polygons) {
        for (int i = 0; i < polygon.getSize(); i++) {
            Ray ray(light, polygon[i]);
            result.push_back(ray);
            result.push_back(ray.rotate(0.0001));
            result.push_back(ray.rotate(-0.0001));
        }
    }
    return result;
}

void Controller::intersectRays(std::vector<Ray>* rays) const {
    std::optional<QPointF> point;
    bool isChanged = false;
    for (auto ray = rays->begin(); ray != rays->end(); ray++) {
        isChanged = false;
        for (const Polygon& polygon: polygons) {
            point = polygon.intersectRay(*ray);
            if (point == std::nullopt) {
                continue;
            }
            if (!polygon.dotOnRay(*ray, point.value())) {
                continue;
            }
            if (!isChanged || polygon.calculateSquareDistance(ray->getBegin(), point.value()) < polygon.calculateSquareDistance(ray->getBegin(), ray->getEnd())) {
                ray->setEnd(point.value());
                isChanged = true;
            }
        }
    }
}

void Controller::removeAdjacentRays(std::vector<Ray>& rays) const {
    sort(rays.begin(), rays.end(), [&](Ray first, Ray second){
        return (first.getAngle() < second.getAngle());
    });

    std::vector<Ray> result;
    for (const Ray& ray: rays) {
        if (!result.empty() && std::abs(result.back().getAngle() - ray.getAngle()) < 0.000001) {
            continue;
        }
        result.push_back(ray);
    }

    rays = result;
}

Polygon Controller::createLightArea() const {
    std::vector<Ray> rays = castRays(lightSource);
    intersectRays(&rays);
    removeAdjacentRays(rays);
    std::vector<QPointF> result;
    for(const Ray& line: rays) {
        result.push_back(line.getEnd());
    }
    return Polygon(result);
}


Polygon Controller::createLightArea(const QPointF& light) const {
    std::vector<Ray> rays = castRays(light);
    intersectRays(&rays);
    removeAdjacentRays(rays);
    std::vector<QPointF> result;
    for(const Ray& line: rays) {
        result.push_back(line.getEnd());
    }
    return Polygon(result);
}
