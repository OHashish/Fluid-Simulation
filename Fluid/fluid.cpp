#define _USE_MATH_DEFINES
#include <fstream>
#include <string>
#include "fluid.h"
#include "string.h"
#include "cstring"
#include "stdlib.h"
#include <algorithm>
#include <math.h>
#include <OpenGL/glu.h>
#include <OpenGL/gl.h>
#include <QWidget>
#include <QGLWidget>
#include <QTimer>
#include <QDebug>
#include <QMouseEvent>
#include <iostream>

double framespeed = 0;
bool toggleViscosity = true;
bool toggleSurfaceTension = true;
bool toggleSS4 = 0;
bool toggleSS5 = 0;

//Default
//glm::dvec2 G(0.0, -10.0);
//double REST_DENSITY = 500.0;
//double DT = 0.007;
//double PARTICLE_RADIUS = 0.09;
//double GAS_CONSTANT = 2000.0;
//double H_KER_RAD = PARTICLE_RADIUS*2.2;
//double VELOCITY_DAMP = 0.5;
//double MASS = 2.5;

//float POLY6 = 4.f / (M_PI * pow(H_KER_RAD, 8.f));
//float EPS = PARTICLE_RADIUS; // boundary epsilon
//float SPIKY_GRAD = -10.f / (M_PI * pow(H_KER_RAD, 5.f));
//float VISC_LAP = 40.f / (M_PI * pow(H_KER_RAD, 5.f));
//double SURFACE_TENSION_CONSTANT = 0.5;
//double VISCOSITY_CONSTANT = 200.0;

//float POLY6 = 4.f / (M_PI * pow(H_KER_RAD, 8.f));
//float POLY6_GRAD = -24.f / (M_PI * pow(H_KER_RAD, 8.f));
//float POLY6_LAP = -24.f / (M_PI * pow(H_KER_RAD, 8.f));
//float SPIKY_GRAD = -10.f / (M_PI * pow(H_KER_RAD, 5.f));
//float VISC_LAP = 40.f / (M_PI * pow(H_KER_RAD, 5.f));
//float SPIKY_GRAD = -10.f / (M_PI * pow(H_KER_RAD, 5.f));
//float VISC_LAP = 40.f / (M_PI * pow(H_KER_RAD, 5.f));

//Play
glm::dvec2 G(0.0, -10.0);
double REST_DENSITY = 700.0;
double DT = 0.3;
double PARTICLE_RADIUS = 0.09;
double GAS_CONSTANT = 800.0; //500 with Poly in pressure
double H_KER_RAD = PARTICLE_RADIUS*2.01;
double VELOCITY_DAMP = 0.5;
double MASS = 2.5;
double SURFACE_TENSION_CONSTANT = 0.9;
double VISCOSITY_CONSTANT = 3000.0;
float EPS = PARTICLE_RADIUS; // boundary epsilon

double POLY6 = 315.0 / (64.0 * M_PI * pow(H_KER_RAD, 9));
double POLY6_GRAD = -945.f / (32.0* M_PI * pow(H_KER_RAD,9.f));
double POLY6_LAP = -945.f / (8.0 * M_PI * pow(H_KER_RAD, 9.f));

double SPIKY_GRAD = -45.f / (M_PI * pow(H_KER_RAD, 6.f));
double VISC_LAP = 45.f / (M_PI * pow(H_KER_RAD, 6.f));


Fluid::Fluid(){
    horizontalPos = 0.f;
    verticalPos = 0.f;
}


Fluid::Fluid(QWidget *parent) : QGLWidget(parent){
    QTimer *ptimer = new QTimer(this);
    ptimer->start(1);
    connect(ptimer, SIGNAL(timeout()),  this, SLOT(updateTime()));

    Clear();
    blobCreator();
}

Fluid::~Fluid(){

}
void Fluid::Clear(){
    particles.clear();

}
void Fluid::initializeGL(){
    glEnable(GL_DEPTH_TEST);
    //    glShadeModel(GL_FLAT);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glClearColor( 0.0, 0.0,  0.0,  0.0);
}

void Fluid::resizeGL(int w, int h){ 
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float aspectRatio = float(w) / float(h);
    float size=5;
    glOrtho((-aspectRatio * size), (aspectRatio * size), -size, size, -size, size);

}

void Fluid::paintGL(){ 
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // set light position first
    GLfloat light_position[] = {0.0f, 0.0f, 1.0f ,0.0};
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    // apply translation for interface control
    glLoadIdentity();

    glTranslatef(-2, -3, 0);
    glPushMatrix();
        //Left
        glBegin(GL_QUADS);
        glColor3f(1.0f,1.0f,1.0f);
        glNormal3f(0,0,1);
        glVertex3f(0,-0.05,0);
        glVertex3f(0,5-0.05,0);
        glVertex3f(-0.1,5-0.05,0);
        glVertex3f(-0.1,0-0.05,0);

        glEnd();

        //Bottom
        glBegin(GL_QUADS);
        glColor3f(1.0f,1.0f,1.0f);
        glNormal3f(0,0,1);
        glVertex3f(-0.1,-0.05,0);
        glVertex3f(5.1,-0.05,0);
        glVertex3f(5.1,-0.15,0);
        glVertex3f(-0.1,-0.15,0);
        glEnd();

        //Right
        glBegin(GL_QUADS);
        glColor3f(1.0f,1.0f,1.0f);
        glNormal3f(0,0,1);
        glVertex3f(5,0-0.05,0);
        glVertex3f(5,5-0.05,0);
        glVertex3f(5.1,5-0.05,0);
        glVertex3f(5.1,0-0.05,0);
        glEnd();

        //Ground
        glBegin(GL_QUADS);
        glColor3f(0.0f,0.0f,1.0f);
        glNormal3f(0,0,1);
        glVertex3f(-20,-0.15,0);
        glVertex3f(20,-0.15,0);
        glVertex3f(20,-0.25,0);
        glVertex3f(-20,-0.25,0);
        glEnd();


        // example of disk
        glPushMatrix();
            glTranslatef(1, 4, 0);
            GLUquadricObj *quadric= gluNewQuadric();
            gluQuadricDrawStyle( quadric, GLU_FILL);
            gluQuadricNormals( quadric, GLU_SMOOTH);
            gluQuadricOrientation( quadric,GLU_OUTSIDE);

//            glColor3f(0.0f,1.0f,0.0f);

            for ( auto& p : particles){
                glColor3f(1.0f,p.color,0.0f);
                glPushMatrix();
                    glTranslatef(p.position.x, p.position.y, 0);
                    gluDisk(quadric, 0, PARTICLE_RADIUS, 20, 10);
                glPopMatrix();
            }
        glPopMatrix();

    glPopMatrix();

}

void Fluid::addParticle(double i, double j){
    Particle p = {
        glm::dvec2(i, j), // Position
        glm::dvec2(0, 0), // Velocity
        glm::dvec2(0, -10.0), // Force
        0.0, // Density
        0.0, // Pressure
        1.0,
    };

    particles.push_back(p);
}

void Fluid::blobCreator(){
    for ( double i = 0 ; i < 2;i+=PARTICLE_RADIUS*2){
        for ( double j = 0 ; j < 3;j+=PARTICLE_RADIUS*2){
            addParticle(i+horizontalPos, j+verticalPos);
        }
    }

//    for (int i = 0; i < 2 / (PARTICLE_RADIUS * 2); ++i) {
//        for (int j = 0; j < 3 / (PARTICLE_RADIUS * 2); ++j) {
//            addParticle(i * PARTICLE_RADIUS * 2, j * PARTICLE_RADIUS * 2 - 3);
//        }
//    }

}

void Fluid::updateTime(){

    double kernelRadiusH = H_KER_RAD;
    double dt = DT;
    double restDensity = REST_DENSITY;
    double gasConstant = GAS_CONSTANT;
    double viscosityConstant = VISCOSITY_CONSTANT;
    double damp =  VELOCITY_DAMP;
    double surfaceConstant = SURFACE_TENSION_CONSTANT;

    simulateFluid(kernelRadiusH,dt,restDensity,gasConstant,viscosityConstant , damp,surfaceConstant);
    this->repaint();

}

void Fluid::posX(double pos){
    Clear();
    blobCreator();
    REST_DENSITY=pos;
    this->repaint();
}

void Fluid::gasConstant(double k){
    Clear();
    blobCreator();
    GAS_CONSTANT=k;
    this->repaint();
}

void Fluid::vCONSTANT(double k){
    Clear();
    blobCreator();
    VELOCITY_DAMP=k;
    this->repaint();
}

void Fluid::kRADIUS(double k){
    Clear();
    blobCreator();
    H_KER_RAD=k;
    this->repaint();
}

void Fluid::surfaceTESNION(double k){
    Clear();
    blobCreator();
    SURFACE_TENSION_CONSTANT=k;
    this->repaint();
}

void Fluid::deltaTIME(double k){
    Clear();
    blobCreator();
    DT=k;
    this->repaint();
}

void Fluid::DAMP(double k){
    Clear();
    blobCreator();
    VELOCITY_DAMP=k;
    this->repaint();
}
void Fluid::SS2(){

    toggleViscosity= !toggleViscosity;
    Clear();
    blobCreator();
}

void Fluid::SS3(){

    toggleSurfaceTension = !toggleSurfaceTension;
    Clear();
    blobCreator();
}

void Fluid::moveUp(){
    if (verticalPos < -0.5)
        verticalPos += 0.2;
    Clear();
    blobCreator();
}

void Fluid::moveDown(){
    if (verticalPos > -3.8)
        verticalPos -= 0.2;
    Clear();
    blobCreator();
}

void Fluid::moveLeft(){
    if (horizontalPos > -0.8)
         horizontalPos -= 0.2;
     Clear();
     blobCreator();
 }

 void Fluid::moveRight(){
     if (horizontalPos < 1.8)
        horizontalPos += 0.2;
     Clear();
     blobCreator();
 }

double Fluid::computePoly6Kernel(double r, double h){
    if (r > h || r < 0)  return 0;

    double q = pow(h, 2) - pow(r, 2);
    return POLY6 * pow(q, 3);
}

void Fluid::calculatePressureDensity(double kernelRadiusH, double contantK, double restDensity){
    for (auto &pi : particles) {
        pi.density = 0.0;
        for (auto &pj : particles) {

            glm::dvec2 rLen(pj.position - pi.position);
            double r = glm::length(rLen);
            pi.density += MASS * computePoly6Kernel(r, kernelRadiusH);

        }
        pi.pressure = contantK * (pi.density - restDensity);

    }
}

void Fluid::calculateForces(double kernelRadiusH, double viscosityConstant, double surfaceConstant){
    int i =0;
    for (auto &pi : particles) {
        glm::dvec2 pressureForce(0,0);
        glm::dvec2 viscosityForce(0,0);
        glm::dvec2 surfaceTensionForce(0,0);
        glm::dvec2 n(0,0);

        for (auto &pj : particles) {
            if (&pi == &pj) continue;

            glm::dvec2 rLen(pj.position - pi.position);
            double r = glm::length(rLen);
            if (r < kernelRadiusH){
                pressureForce +=  ((pj.pressure +  pi.pressure)/(2.0*pj.density)) *
                                    SPIKY_GRAD * pow(kernelRadiusH - r, 2.0) *
                                    // POLY6_GRAD * pow(kernelRadiusH*kernelRadiusH - r*r,2) *
                                    -1.0*glm::normalize(rLen)  *
                                    MASS;

                viscosityForce +=  ((pj.velocity -  pi.velocity)/pj.density) *
                                    VISC_LAP * (kernelRadiusH - r) *
                                    MASS;

                n             +=  ((1.0)/(pj.density)) *
                                    POLY6_GRAD * pow(kernelRadiusH*kernelRadiusH - r*r,2) *
                                    glm::normalize(rLen) *
                                    MASS;

                surfaceTensionForce +=  ((1.0)/(pj.density)) *
                                    POLY6_LAP * (kernelRadiusH*kernelRadiusH - r*r) *
                                    (3.0*kernelRadiusH*kernelRadiusH - 7.0*r*r) *
                                    MASS;
            }
        }

        glm::dvec2 gravityForce(G * MASS);
        viscosityForce *= viscosityConstant;
        surfaceTensionForce *= -1.0*surfaceConstant * (n/glm::length(n)) ;
        if (i==1)
        cout<<glm::length(n)<<endl;
        if (toggleViscosity){
            gravityForce += viscosityForce;
        }
        if (!isnan(surfaceTensionForce.x) && toggleSurfaceTension){
            gravityForce += surfaceTensionForce;
        }
        if(glm::length(n)<0.2)
            pi.color = 1.0;
        else
            pi.color = 0.0;
        pi.force =  (gravityForce + pressureForce) / pi.density  ;
        i++;
    }
}

void Fluid::integrate(double dt, double damp,bool first){

    for (auto &p : particles) {

        p.velocity += 0.5*dt * (p.force / p.density);
        if (first){
            p.position += p.velocity * dt;

            //        if (glm::length(p.velocity) > 1.0){
            //            p.velocity *= damp;
            //        }
            if (p.position.x - EPS < -0.95){
                p.velocity.x *= -damp;
                p.position.x = -0.95 + EPS;
            }
            if (p.position.x + EPS > 3.95){

                p.velocity.x *= -damp;
                p.position.x = 3.95 - EPS;
            }
            if (p.position.y - EPS < -4.0){

                p.velocity.y *= -damp;
                p.position.y = -4.0 + EPS;
            }
            if (p.position.y + EPS > 3.0){

                p.velocity.y *= -damp;
                p.position.y = 3.0 - EPS;
            }
        }

    }
}

void Fluid::simulateFluid(double kernelRadiusH, double dt , double restDensity , double gasConstant , double viscosityConstant , double damp , double surfaceConstant){

    calculatePressureDensity(kernelRadiusH, gasConstant, restDensity);
    calculateForces(kernelRadiusH, viscosityConstant, surfaceConstant);
    integrate(dt, damp,true);
    calculatePressureDensity(kernelRadiusH, gasConstant, restDensity);
    calculateForces(kernelRadiusH, viscosityConstant, surfaceConstant);
    integrate(dt, damp,false);
}
