#ifndef  _FLUID_H_
#define  _FLUID_H_

#include <vector>
#include <string>
//#include <QGLWidget>
#include <QWidget>
#include <QObject>
#include "fluidwindow.h"
#include <glm/glm.hpp>
#include "string.h"
#include "cstring"
#include "stdlib.h"
#include <algorithm>
#include <math.h>
#include <OpenGL/glu.h>
#include <OpenGL/gl.h>
#include <QWidget>
#include <QTimer>
#include <QDebug>
#include <QMouseEvent>
#include <iostream>
#include <fstream>
#include <string>
#include <QOpenGLWidget>
using namespace  std;

class  Fluid: public QOpenGLWidget{

    Q_OBJECT
public slots:

    void updateTime();

    void SS1();
    void SS2();
    void SS3();

    void moveUp();
    void moveDown();
    void moveRight();
    void moveLeft();

    void changeViscosityConstant(double k);
    void changeSurfaceTension(double k);
    void changeKernelRadius(double k);
    void changeRestDensity(double);
    void changeGasConstant(double k);
    void changeTime(double k);
    void changeDamp(double k);

public:
    Fluid();
    Fluid( QWidget *parent);
    ~Fluid();
    void  Clear();

    // called when OpenGL context is set up
    void initializeGL();
    // called every time the widget is resized
    void resizeGL(int w, int h);
    // called every time the widget needs painting
    void paintGL();

    struct Particle{
        glm::dvec2 position;
        glm::dvec2 velocity;
        glm::dvec2 force;
        double density;
        double pressure;
        double color;
    };

    vector<Particle> particles;
    void addParticle(double i, double j);

    void simulateFluid(double h, double dt, double restDensity, double stiffness, double viscosityConstant, double damp, double surfaceConstant);
    void calculateForces( double supportRadius, double viscosityConstant,  double surfaceConstant);
    void calculatePressureDensity(double supportRadius, double stiffness, double restDensity);
    void integrate(double dt, double damp, bool first);
    double computePoly6Kernel(double d, double h);
    void blobCreator();

    float horizontalPos,verticalPos;

};



#endif // _FLUID_H_
