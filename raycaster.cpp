#include "raycaster.h"

Raycaster::Raycaster(QWidget *parent)
    : QWidget(parent) {

    setMouseTracking(true);

    QPalette palSide;
    palSide.setColor(QPalette::Window, QColor(57, 54, 70));
    this->setAutoFillBackground(true);
    this->setPalette(palSide);

    controller.addPolygon(Polygon({QPointF(0, 0), QPointF(0, 0), QPointF(0, 0), QPointF(0, 0)}));
}

void Raycaster::lightClicked() {
    if (state == ButtonState::PolygonsMode) {
        controller.rebuildPolygons();
    }
    state = ButtonState::LightMode;
    update();
}

void Raycaster::polygonsClicked() {
    state = ButtonState::PolygonsMode;
    update();
}

void Raycaster::shadowClicked() {
    if (state == ButtonState::PolygonsMode) {
        controller.rebuildPolygons();
    }
    state = ButtonState::ShadowMode;
    update();
}

void Raycaster::staticLightsClicked() {
    if (state == ButtonState::PolygonsMode) {
        controller.rebuildPolygons();
    }
    state = ButtonState::StaticLightsMode;
    staticColor = QColorDialog::getColor();
    update();
}

void Raycaster::clearClicked() {
    state = ButtonState::None;
    controller.clearAll();
    staticLights.clear();
    controller.addPolygon(Polygon({QPointF(0, 0), QPointF(width() - 1, 0), QPointF(width() - 1, height() - 1), QPointF(0, height() - 1)}));
    update();
}

void Raycaster::resizeEvent(QResizeEvent*) {
    controller.changeFirstPolygon(Polygon({QPointF(0, 0), QPointF(width() - 1, 0), QPointF(width() - 1, height() - 1), QPointF(0, height() - 1)}));
}

void Raycaster::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.save();

    painter.setPen(QColor(255, 255, 255));
    const std::vector<Polygon>& polygons = controller.getPolygons();
    int size = 0;
    for (const Polygon& figure: polygons) {
        size = figure.getSize();
        for(int i = 0; i < size; i++) {
            painter.drawLine(figure[i], figure[(i + 1) % size]);
        }
    }

    for (const auto& point: staticLights) {
        QRadialGradient gradient(point.first, 100);
        painter.setPen(Qt::NoPen);
        gradient.setStops({{0, point.second}, {1, Qt::transparent}});
        painter.setBrush(gradient);
        Polygon lightArea = controller.createLightArea(point.first);
        painter.drawPolygon(lightArea.getFirstPoint(), lightArea.getSize());
    }

    if (state == ButtonState::LightMode) {
        QRadialGradient gradient(controller.getLightSourse(), width());
        painter.setPen(Qt::NoPen);
        gradient.setStops({{0, QColor(179, 182, 191, 170)}, {1, Qt::transparent}});
        painter.setBrush(gradient);
        Polygon lightArea = controller.createLightArea();
        painter.drawPolygon(lightArea.getFirstPoint(), lightArea.getSize());

        painter.setBrush(QColor(255, 255, 255));
        painter.setPen(QColor(255, 255, 255));
        painter.drawEllipse(controller.getLightSourse().x() - 2, controller.getLightSourse().y() - 2, 4, 4);
    }

    if (state == ButtonState::ShadowMode) {
        QPointF lightSource = controller.getLightSourse();
        QRadialGradient gradient(controller.getLightSourse(), width());
        painter.setPen(Qt::NoPen);
        gradient.setStops({{0, QColor(179, 182, 191, 150)}, {1, Qt::transparent}});
        painter.setBrush(gradient);

        for (int i = 0; i < 9; i++) {
            Polygon lightArea = controller.createLightArea(QPointF(lightSource.x() + lightsPositionX[i], lightSource.y() + lightsPositionY[i]));
            painter.drawPolygon(lightArea.getFirstPoint(), lightArea.getSize());
        }


        painter.setBrush(QColor(255, 255, 255));
        painter.setPen(QColor(255, 255, 255));
        for (int i = 0; i < 9; i++) {
            painter.drawEllipse(lightSource.x() + lightsPositionX[i] - 2, lightSource.y() + lightsPositionY[i] - 2, 4, 4);
        }
    }

    if (state == ButtonState::StaticLightsMode) {
        painter.setBrush(QColor(255, 255, 225));
        painter.setPen(QColor(255, 255, 225));
        painter.drawEllipse(controller.getLightSourse().x() - 2, controller.getLightSourse().y() - 2, 4, 4);
    }

    painter.restore();
}

void Raycaster::mousePressEvent(QMouseEvent* event) {
    if (state == ButtonState::PolygonsMode) {
        if (event->button() == Qt::RightButton) {
            if (!isFirstPoint) {
                controller.deleteVertexFromLastPolygon();
                isFirstPoint = true;
            }
            update();
        } else if (event->button() == Qt::LeftButton) {
            if (isFirstPoint) {
                isFirstPoint = false;
                controller.addPolygon(Polygon({QPointF(event->localPos())}));
                if (!staticLights.empty()) {
                    controller.rebuildPolygons();
                }
                controller.addVertexToLastPolygon(QPointF(event->localPos()));
            } else {
                controller.updateLastPolygon(QPointF(event->localPos()));
                if (!staticLights.empty()) {
                    controller.rebuildPolygons();
                }
                controller.addVertexToLastPolygon(QPointF(event->localPos()));
            }

            update();
        }
    }

    if (state == ButtonState::StaticLightsMode) {
        if (event->button() == Qt::LeftButton) {
            staticLights.push_back({event->localPos(), staticColor});
            update();
        }
    }
}

void Raycaster::mouseMoveEvent(QMouseEvent* event) {
    if (state == ButtonState::PolygonsMode) {
        if (!isFirstPoint) {
            controller.updateLastPolygon(QPointF(event->localPos()));
        }
        update();
    }
    if (state == ButtonState::LightMode || state == ButtonState::ShadowMode || state == ButtonState::StaticLightsMode) {
        QPointF mousePosition = event->localPos();
        if (0 <= mousePosition.x() && mousePosition.x() < this->width() && 0 <= mousePosition.y() && mousePosition.y() < this->height()) {
            controller.setLightSourse(mousePosition);
        }
        update();
    }
}
