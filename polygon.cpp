#include "polygon.h"

Polygon::Polygon(const std::vector<QPointF>& vertices_) {
    vertices = vertices_;
}

const QPointF* Polygon::getFirstPoint() const {
    return &vertices[0];
}

QPointF Polygon::operator[](int index) const {
    return vertices[index];
}

int Polygon::getSize() const {
    return vertices.size();
}

void Polygon::addVertex(const QPointF& vertex) {
    vertices.push_back(vertex);
}

void Polygon::deleteVertex() {
    if (vertices.size() > 0) {
        vertices.pop_back();
    }
}

void Polygon::updateLastVertex(const QPointF& newVertex) {
    if (vertices.empty()) {
        vertices.push_back(newVertex);
    } else {
        vertices.back() = newVertex;
    }
}

long double Polygon::calculateSquareDistance(const QPointF& first, const QPointF& second) {
    return (first.x() - second.x()) * (first.x() - second.x()) + (first.y() - second.y()) * (first.y() - second.y());
}

bool Polygon::dotOnLine(const Ray& line, QPointF point) const {
    return (std::min(line.getBegin().x(), line.getEnd().x()) - eps <= point.x()
            && point.x() - eps <= std::max(line.getBegin().x(), line.getEnd().x())
            && std::min(line.getBegin().y(), line.getEnd().y()) - eps <= point.y()
            && point.y() - eps <= std::max(line.getBegin().y(), line.getEnd().y()));
}

int Polygon::sign(long double number) const {
    if (number < 0) {
        return -1;
    } else return (number > 0 ? 1 : 0);
}

bool Polygon::dotOnRay(const Ray& ray, QPointF point) const {
    bool goodX = (sign(ray.getEnd().x() - ray.getBegin().x()) == sign(point.x() - ray.getBegin().x()));
    if (std::abs(point.x() - ray.getBegin().x()) < eps) {
        goodX = true;
    }
    bool goodY = (sign(ray.getEnd().y() - ray.getBegin().y()) == sign(point.y() - ray.getBegin().y()));
    if (std::abs(point.y() - ray.getBegin().y()) < eps) {
        goodY = true;
    }
    return (goodX && goodY);
}

std::optional<QPointF> Polygon::intersection(const Ray& ray, const Ray& line) const {
    long double A1 = ray.getBegin().y() - ray.getEnd().y();
    long double B1 = ray.getEnd().x() - ray.getBegin().x();
    long double C1 = ray.getBegin().x() * ray.getEnd().y() - ray.getEnd().x() * ray.getBegin().y();

    long double A2 = line.getBegin().y() - line.getEnd().y();
    long double B2 = line.getEnd().x() - line.getBegin().x();
    long double C2 = line.getBegin().x() * line.getEnd().y() - line.getEnd().x() * line.getBegin().y();

    if (A1 * B2 - A2 * B1 == 0) {

        if (dotOnLine(line, ray.getBegin())) {
            return ray.getBegin();
        }

        bool isBeginOnRay = (((A1 * line.getBegin().x() + B1 * line.getBegin().y() + C1) < eps) && dotOnRay(ray, line.getBegin()));
        bool isEndOnRay = (((A1 * line.getEnd().x() + B1 * line.getEnd().y() + C1) < eps) && dotOnRay(ray, line.getEnd()));
        long double beginDistance = calculateSquareDistance(ray.getBegin(), line.getBegin());
        long double endDistance = calculateSquareDistance(ray.getBegin(), line.getEnd());

        if (isBeginOnRay && (!isEndOnRay || beginDistance <= endDistance)) {
            return line.getBegin();
        }
        if (isEndOnRay) {
            return line.getEnd();
        }
        return std::nullopt;
    }

    long double x = - (C1 * B2 - C2 * B1) / (A1 * B2 - A2 * B1);
    long double y = - (A1 * C2 - A2 * C1) / (A1 * B2 - A2 * B1);

    if (dotOnLine(line, QPointF(x, y)) && dotOnRay(ray, QPointF(x, y))) {
        return QPointF(x, y);
    } else {
        return std::nullopt;
    }
}

std::optional<QPointF> Polygon::intersectRay(const Ray& ray) const {
    std::pair<long double, QPointF> minPoint;
    bool isUpdate = false;
    for (size_t i = 0; i < vertices.size(); i++) {
        Ray line(vertices[i], vertices[(i + 1) % vertices.size()]);
        std::optional<QPointF> point = intersection(ray, line);
        if (point == std::nullopt) {
            continue;
        }
        long double distance = calculateSquareDistance(ray.getBegin(), point.value());
        if (isUpdate && minPoint.first > distance) {
            minPoint = {distance, point.value()};
        } else if (!isUpdate) {
            isUpdate = true;
            minPoint = {distance, point.value()};
        }
    }
    if (isUpdate) {
        return minPoint.second;
    } else {
        return std::nullopt;
    }
}

std::optional<QPointF> Polygon::intersectionLine(const Ray& ray, const Ray& line) const {
    long double A1 = ray.getBegin().y() - ray.getEnd().y();
    long double B1 = ray.getEnd().x() - ray.getBegin().x();
    long double C1 = ray.getBegin().x() * ray.getEnd().y() - ray.getEnd().x() * ray.getBegin().y();

    long double A2 = line.getBegin().y() - line.getEnd().y();
    long double B2 = line.getEnd().x() - line.getBegin().x();
    long double C2 = line.getBegin().x() * line.getEnd().y() - line.getEnd().x() * line.getBegin().y();

    if (A1 * B2 - A2 * B1 == 0) {
        return std::nullopt;
    }

    long double x = - (C1 * B2 - C2 * B1) / (A1 * B2 - A2 * B1);
    long double y = - (A1 * C2 - A2 * C1) / (A1 * B2 - A2 * B1);

    if (dotOnLine(line, QPointF(x, y)) && dotOnLine(ray, QPointF(x, y))) {
        return QPointF(x, y);
    } else {
        return std::nullopt;
    }
}

void Polygon::rebuild(const Polygon& polygon) {
    std::vector<QPointF> result;
    std::vector<QPointF> intersections;
    for (int i = 0; i < getSize(); i++) {
        intersections.clear();
        Ray segment(vertices[i], vertices[(i + 1) % getSize()]);
        for (int j = 0; j < polygon.getSize(); j++) {
            Ray line(polygon[j], polygon[(j + 1) % polygon.getSize()]);
            std::optional<QPointF> point = intersectionLine(segment, line);
            if (point == std::nullopt) {
                continue;
            }
            intersections.push_back(point.value());
        }

        sort(intersections.begin(), intersections.end(), [&](const QPointF& x, const QPointF& y){
            return (calculateSquareDistance(segment.getBegin(), x) < calculateSquareDistance(segment.getBegin(), y));
        });

        result.push_back(vertices[i]);
        for (const QPointF& point: intersections) {
            result.push_back(point);
        }
    }

    vertices.clear();
    for (const QPointF& point: result) {
        if (vertices.size() > 0 && calculateSquareDistance(vertices.back(), point) < eps) {
            continue;
        }
        vertices.push_back(point);
    }

    while (vertices.size() > 1 && calculateSquareDistance(vertices.back(), vertices[0]) < eps) {
        vertices.pop_back();
    }
}
