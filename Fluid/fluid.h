#pragma warning(disable: 4018)
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

class  Fluid: public QGLWidget
{

Q_OBJECT
public slots:

void updateTime();
void posX(double);
void SS2();
void SS3();
void SS4();
void SS5();
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
        glm::dvec2 prevPosition;
        glm::dvec2 velocity;
        double density;
        double pressure;
        double viscosity;
        glm::dvec2 force;
    };

    vector<Particle> particles;

    double computePoly6Kernel(double d, double h);

    void simulateFluid(double h, double dt, double restDensity, double stiffness, double viscosityConstant, double damp, double surfaceConstant);
    void blobCreator();

    double computeDensity( int i ,double supportRadius);
    double computeSpikyKernel(double distance, double supportRadius);
    double computeViscosityKernel(double r, double h);
    double computePressure(Particle p, double stiffness, double restDensity);
    double computePoly6KernelGradiant(double r, double h);
    double computeViscosityLaplacian(double r, double h);
    double computeSpikyKernelGradiant(double r, double h);
    double computePoly6KernelLaplacian(double r, double h);

    void calculateDensityPressure();
    void calculatePressureDensity(double supportRadius, double stiffness, double restDensity);
    void calculateForces(double dt, double supportRadius, double viscosityConstant);
    void integrate(double dt, double damp, bool first);
    void createPressure(int i, glm::dvec2 rLen, double supportRadius, glm::dvec2 pressureForce, int j, double distance);
    void createPressure(int i, double supportRadius, int j, glm::dvec2 pressureForce);
    void createPressure(int j, double supportRadius, glm::dvec2 pressureForce, int i, double distance, glm::dvec2 rLen);
    void d(double supportRadius, glm::dvec2 rLen, int j, int i, double distance);
    void Bruh();
    void addParticle(double i, double j);
};



#endif // _FLUID_H_
