#ifndef  _FLUID_H_
#define  _FLUID_H_

#include <vector>
#include <map>
#include <string>
#include <QGLWidget>
#include <QWidget>
#include <QObject>
#include <QFileDialog>
#include "fluidwindow.h"
#include <glm/glm.hpp>

using namespace  std;

class  Fluid: public QGLWidget{

    Q_OBJECT
public slots:

    void updateTime();
    void posX(double);
    void SS1();
    void SS2();
    void SS3();
    void moveUp();
    void moveDown();
    void moveRight();
    void moveLeft();
    void gasConstant(double k);

    void vCONSTANT(double k);
    void kRADIUS(double k);
    void surfaceTESNION(double k);
    void deltaTIME(double k);
    void DAMP(double k);
public:
    Fluid();
    Fluid( QWidget *parent);
    ~Fluid();
    void  Clear();
    // translation in window x,y

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

    float horizontalPos,verticalPos;

    vector<Particle> particles;

    double computePoly6Kernel(double d, double h);

    void simulateFluid(double h, double dt, double restDensity, double stiffness, double viscosityConstant, double damp, double surfaceConstant);
    void blobCreator();

    void calculatePressureDensity(double supportRadius, double stiffness, double restDensity);
    void calculateForces( double supportRadius, double viscosityConstant,  double surfaceConstant);
    void integrate(double dt, double damp, bool first);

    void addParticle(double i, double j);
};



#endif // _FLUID_H_
