#define _USE_MATH_DEFINES
#include "fluid.h"

bool toggleViscosity = true;
bool toggleSurfaceTension = true;
bool visualizeSurface = false;

float tSpeed = 0;
//Default
//glm::dvec2 G(0.0, -10.0);
//double REST_DENSITY = 500.0;
//double DT = 0.007;
//double PARTICLE_RADIUS = 0.09;
//double GAS_CONSTANT = 2000.0;
//double H_KER_RAD = PARTICLE_RADIUS*2.2;
//double VELOCITY_DAMP = 0.5;
//double MASS = 2.5;
//double SURFACE_TENSION_CONSTANT = 0.5;
//double VISCOSITY_CONSTANT = 200.0;

// 2D kernels
//float POLY6 = 4.f / (M_PI * pow(H_KER_RAD, 8.f));
//float EPS = PARTICLE_RADIUS; // boundary epsilon
//float SPIKY_GRAD = -10.f / (M_PI * pow(H_KER_RAD, 5.f));
//float VISC_LAP = 40.f / (M_PI * pow(H_KER_RAD, 5.f));

//Play
glm::dvec2 G(0.0, -10.0);
double REST_DENSITY = 700.0;
double DT = 0.3;
double PARTICLE_RADIUS = 0.09;
double GAS_CONSTANT = 800.0; // 500 with Poly in pressure
double H_KER_RAD = PARTICLE_RADIUS*2.01;
double VELOCITY_DAMP = 0.5;
double MASS = 2.5;
double SURFACE_TENSION_CONSTANT = 0.9;
double VISCOSITY_CONSTANT = 4000.0;
float EPS = PARTICLE_RADIUS; // boundary epsilon

// 3D kernels
double POLY6 = 315.0 / (64.0 * M_PI * pow(H_KER_RAD, 9));
double POLY6_GRAD = -945.f / (32.0* M_PI * pow(H_KER_RAD,9.f));
double POLY6_LAP = -945.f / (8.0 * M_PI * pow(H_KER_RAD, 9.f));

double SPIKY_GRAD = -45.f / (M_PI * pow(H_KER_RAD, 6.f));
double VISC_LAP = 45.f / (M_PI * pow(H_KER_RAD, 6.f));

//Part Rad --- Rest Desnity -- Gas Const -- DT
//0.06 --- 2300 -- 2300 -- 0.8
//0.08 --- 1000 -- 1250 -- 0.3
//0.09 --- 700 -- 800 -- 0.3

Fluid::Fluid(){
    horizontalPos = 0.f;
    verticalPos = 0.f;
}

Fluid::Fluid(QWidget *parent) : QOpenGLWidget(parent){
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
//        glShadeModel(GL_FLAT);
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
;

    GLfloat light_position[] = {0.0f, 0.0f, 0.7f ,0.0};
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
        glVertex3f(0,7-0.05,0);
        glVertex3f(-0.1,7-0.05,0);
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
        glVertex3f(5,7-0.05,0);
        glVertex3f(5.1,7-0.05,0);
        glVertex3f(5.1,0-0.05,0);
        glEnd();

        //Ground
        glBegin(GL_QUADS);
        glColor3f(0.7f,0.7f,0.0f);
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

            for ( auto& p : particles){

                glColor3f(0.0f,p.color,0.8f);
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
        0.0, // Color
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
//            addParticle(i * PARTICLE_RADIUS * 2, j * PARTICLE_RADIUS * 2);
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
    tSpeed++;

}

void Fluid::changeRestDensity(double k){
    REST_DENSITY = k;
    Clear();
    blobCreator();
}

void Fluid::changeGasConstant(double k){
    GAS_CONSTANT = k;
    Clear();
    blobCreator();
}

void Fluid::changeViscosityConstant(double k){
    VISCOSITY_CONSTANT = k;
    Clear();
    blobCreator();
}

void Fluid::changeKernelRadius(double k){
    H_KER_RAD = k;
    Clear();
    blobCreator();
}

void Fluid::changeSurfaceTension(double k){
    SURFACE_TENSION_CONSTANT = k;
    Clear();
    blobCreator();
}

void Fluid::changeTime(double k){
    DT = k;
    Clear();
    blobCreator();
}

void Fluid::changeDamp(double k){
    VELOCITY_DAMP = k;
    Clear();
    blobCreator();
}
void Fluid::SS1(){
    visualizeSurface = !visualizeSurface;
}


void Fluid::SS2(){

    toggleViscosity = !toggleViscosity;
    Clear();
    blobCreator();
}

void Fluid::SS3(){

    toggleSurfaceTension = !toggleSurfaceTension;
    Clear();
    blobCreator();
}

void Fluid::moveUp(){
//    if (verticalPos < -0.5)
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
        double nMagnitude = glm::length(n);
        surfaceTensionForce *= -1.0 * surfaceConstant * (n/nMagnitude) ;

        if (toggleViscosity){
            gravityForce += viscosityForce;
        }
        if (!isnan(surfaceTensionForce.x) && toggleSurfaceTension){
            gravityForce += surfaceTensionForce;
        }

        //Needs to be decreased if number of particles increase
        float threshold = 0.2;
        if (visualizeSurface && nMagnitude < threshold){
            pi.color = nMagnitude/threshold;
        }else{
            pi.color = 0.0;
        }

        pi.force =  (gravityForce + pressureForce) / pi.density  ;
    }
}

void Fluid::integrate(double dt, double damp,bool first){

    for (auto &p : particles) {

        p.velocity += dt * (p.force / p.density);
        if (first){

            p.position += p.velocity * dt*2.0;

            //Damp high speed particles
//            if (glm::length(p.velocity) > 3.0){
//                p.velocity *= 0.9;
//            }
            //Left Barrier
            if (p.position.x - EPS < -0.95){
                p.velocity.x *= -damp;
                p.position.x = -0.95 + EPS;
            }

            //Right Barrier
            if (p.position.x + EPS > 3.95){

                p.velocity.x *= -damp;
                p.position.x = 3.95 - EPS;
            }

            //Ground
            if (p.position.y - EPS < -4.0){

                p.velocity.y *= -damp;
                p.position.y = -4.0 + EPS;
            }

            //Ceiling
//            if (p.position.y + EPS > 3.0){

//                p.velocity.y *= -damp;
//                p.position.y = 3.0 - EPS;
//            }
//            float jitter = static_cast<float>(rand()) / static_cast<float>(RAND_MAX / 2) - 1.0f;
//            p.position.x += 0.001*jitter;
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
