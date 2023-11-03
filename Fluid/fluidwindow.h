#ifndef __GL_POLYGON_WINDOW_H__
#define __GL_POLYGON_WINDOW_H__ 1

#include <QGLWidget>
#include <QMenuBar>
#include <QSlider>
#include <QTimer>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QBoxLayout>
#include <QLabel>
#include "fluid.h"
#include "triangularbutton.h"
#include <iostream>
#include <QPushButton>
#include <QWindow>
#include <QTextEdit>
#include <QLineEdit>
#include <QCheckBox>

class FluidWindow: public QWidget{
    Q_OBJECT
public slots:
    void handleButton();
public:


    // constructor / destructor
    FluidWindow(QWidget *parent);
    ~FluidWindow();

    // window layout
    QBoxLayout *windowLayout;

    QHBoxLayout *windowLayoutMain;
    QVBoxLayout *windowLayoutUI;
    QVBoxLayout *windowLayouttest;



    // a timer
    QTimer *ptimer;

    // resets all the interface elements
    void ResetInterface();
};

#endif
