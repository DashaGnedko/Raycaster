#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QComboBox>
#include <QToolBar>
#include <QLayout>

#include "raycaster.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Raycaster* window = nullptr;
    QComboBox* modes = nullptr;
};
#endif // MAINWINDOW_H
