#ifndef RAYCASTER_H
#define RAYCASTER_H

#include <QMainWindow>
#include <QWidget>
#include <QMouseEvent>
#include <QPushButton>
#include <QIcon>
#include <QPainter>
#include <QColorDialog>

#include "controller.h"

enum class ButtonState {
    None,
    LightMode,
    PolygonsMode,
    ShadowMode,
    StaticLightsMode
};

class Raycaster : public QWidget {
    Q_OBJECT

public:
    Raycaster(QWidget *parent = nullptr);

public slots:
    void lightClicked();
    void polygonsClicked();
    void shadowClicked();
    void clearClicked();
    void staticLightsClicked();

private:
    void resizeEvent(QResizeEvent*);
    void mousePressEvent(QMouseEvent*);
    void paintEvent(QPaintEvent*);
    void mouseMoveEvent(QMouseEvent*);

    Controller controller;
    bool isFirstPoint = true;
    ButtonState state = ButtonState::None;
    std::vector<std::pair<QPointF, QColor>> staticLights;
    QColor staticColor = QColor(255, 255, 225);

    const double lightsPositionX[9] = {0, -10, 10, 0, 0, -7, -7, 7, 7};
    const double lightsPositionY[9] = {0, 0, 0, -10, 10, -7, 7, -7, 7};
};

#endif // RAYCASTER_H
