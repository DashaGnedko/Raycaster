#include "mainwindow.h"
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), window(new Raycaster(this)) {

    resize(1500, 700);
    setCentralWidget(window);

    QPushButton* lightButton = new QPushButton;
    QIcon lightIcon(":/icons/light.png");
    lightButton->setIcon(lightIcon);
    lightButton->setToolTip("Light mode");

    QPushButton* polygonButton = new QPushButton;
    QIcon polygonsIcon(":/icons/polygons.png");
    polygonButton->setIcon(polygonsIcon);
    polygonButton->setToolTip("Draw polygon");

    QPushButton* shadowButton = new QPushButton;
    QIcon shadowIcon(":/icons/shadow.png");
    shadowButton->setIcon(shadowIcon);
    shadowButton->setToolTip("Shadow mode");

    QPushButton* clearButton = new QPushButton;
    QIcon clearIcon(":/icons/cleaning.png");
    clearButton->setIcon(clearIcon);
    clearButton->setToolTip("Clear");

    QPushButton* staticLightsButton = new QPushButton;
    QIcon staticLightsIcon(":/icons/street-light.png");
    staticLightsButton->setIcon(staticLightsIcon);
    staticLightsButton->setToolTip("Static light");

    QToolBar* bar = new QToolBar;
    bar->addWidget(lightButton);
    bar->addWidget(polygonButton);
    bar->addWidget(shadowButton);
    bar->addWidget(staticLightsButton);
    bar->addWidget(clearButton);

    addToolBar(bar);

    connect(lightButton, &QPushButton::clicked, window, &Raycaster::lightClicked);
    connect(polygonButton, &QPushButton::clicked, window, &Raycaster::polygonsClicked);
    connect(shadowButton, &QPushButton::clicked, window, &Raycaster::shadowClicked);
    connect(clearButton, &QPushButton::clicked, window, &Raycaster::clearClicked);
    connect(staticLightsButton, &QPushButton::clicked, window, &Raycaster::staticLightsClicked);
}

MainWindow::~MainWindow() {
}

