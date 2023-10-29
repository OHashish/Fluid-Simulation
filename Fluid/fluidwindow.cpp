#include "fluidwindow.h"
#include "fluid.h"
#include <QDebug>
#include <iostream>
#include <QToolBar>
#include <QPushButton>
#include <QWindow>
#include <QTextEdit>
#include <QLineEdit>
// constructor / destructor
FluidWindow::FluidWindow(QWidget *parent)
    : QWidget(parent)
{ // constructor

    // create the window layout
    windowLayoutMain = new QHBoxLayout(this);

    windowLayoutUI = new QVBoxLayout(this);

    windowLayouttest= new QVBoxLayout(this);



    Fluid* reader = new Fluid(NULL);


    windowLayoutMain->addWidget(reader);
    windowLayoutMain->addLayout(windowLayoutUI);
    windowLayoutUI->addLayout(windowLayouttest);

    QLabel *label = new QLabel("Rest Density");
    label->setMaximumHeight(10);
    windowLayouttest->addWidget(label);
    QDoubleSpinBox *px = new QDoubleSpinBox;
    px->setMaximumWidth(80);
    px->setMaximumHeight(30);
    px->setMinimum(-360);
    px->setMaximum(360);
    px->setValue(20);
    windowLayouttest->addWidget(px);
    connect(px, SIGNAL(valueChanged(double)),  reader  , SLOT(posX(double)));

    QLabel *g = new QLabel("Gas Constant");
    g->setMaximumHeight(10);
    windowLayouttest->addWidget(g);
    QDoubleSpinBox *k = new QDoubleSpinBox;
    k->setMaximumWidth(80);
    k->setMaximumHeight(30);
    k->setMinimum(-360);
    k->setMaximum(360);
    k->setValue(3);
    windowLayouttest->addWidget(k);
    connect(k, SIGNAL(valueChanged(double)),  reader  , SLOT(gasConstant(double)));

    QLabel *v = new QLabel("Viscosity Constant");
    v->setMaximumHeight(10);
    windowLayouttest->addWidget(v);
    QDoubleSpinBox *vc = new QDoubleSpinBox;
    vc->setMaximumWidth(80);
    vc->setMaximumHeight(30);
    vc->setMinimum(-360);
    vc->setMaximum(360);
    vc->setValue(7);
    windowLayouttest->addWidget(vc);
    connect(vc, SIGNAL(valueChanged(double)),  reader  , SLOT(vCONSTANT(double)));

    QLabel *r = new QLabel("Kernel Radius");
    r->setMaximumHeight(10);
    windowLayouttest->addWidget(r);
    QDoubleSpinBox *kr = new QDoubleSpinBox;
    kr->setMaximumWidth(80);
    kr->setMaximumHeight(30);
    kr->setMinimum(-360);
    kr->setMaximum(360);
    kr->setValue(1.5);
    windowLayouttest->addWidget(kr);
    connect(kr, SIGNAL(valueChanged(double)),  reader  , SLOT(kRADIUS(double)));

    QLabel *s = new QLabel("Surface Tension Constant");
    s->setMaximumHeight(10);
    windowLayouttest->addWidget(s);
    QDoubleSpinBox *st = new QDoubleSpinBox;
    st->setMaximumWidth(80);
    st->setMaximumHeight(30);
    st->setMinimum(-360);
    st->setMaximum(360);
    st->setValue(10);
    windowLayouttest->addWidget(st);
    connect(st, SIGNAL(valueChanged(double)),  reader  , SLOT(surfaceTESNION(double)));

    QLabel *d = new QLabel("Delta Time");
    d->setMaximumHeight(10);
    windowLayouttest->addWidget(d);
    QDoubleSpinBox *dt = new QDoubleSpinBox;
    dt->setMaximumWidth(80);
    dt->setMaximumHeight(30);
    dt->setMinimum(-360);
    dt->setMaximum(360);
    dt->setDecimals(3);
    dt->setSingleStep(0.001);
    dt->setValue(0.01);
    windowLayouttest->addWidget(dt);
    connect(dt, SIGNAL(valueChanged(double)),  reader  , SLOT(deltaTIME(double)));

    QLabel *dl = new QLabel("Damp");
    dl->setMaximumHeight(10);
    windowLayouttest->addWidget(dl);
    QDoubleSpinBox *dd = new QDoubleSpinBox;
    dd->setMaximumWidth(80);
    dd->setMaximumHeight(30);
    dd->setMinimum(-360);
    dd->setMaximum(360);
    dd->setDecimals(2);
    dd->setSingleStep(0.01);
    dd->setValue(0.5);
    windowLayouttest->addWidget(dd);
    connect(dd, SIGNAL(valueChanged(double)),  reader  , SLOT(DAMP(double)));



    QPushButton *S2 = new QPushButton("Simulate 2",this);
    S2->setMaximumWidth(100);
    connect(S2,SIGNAL(released()), reader, SLOT(SS2()));

    windowLayoutUI->addWidget(S2);

    QPushButton *S3 = new QPushButton("Simulate 3",this);
    S3->setMaximumWidth(100);
    connect(S3,SIGNAL(released()), reader, SLOT(SS3()));


    windowLayoutUI->addWidget(S3);

    QPushButton *S4 = new QPushButton("Simulate 4",this);
    S4->setMaximumWidth(100);
    connect(S4,SIGNAL(released()), reader, SLOT(SS4()));

    windowLayoutUI->addWidget(S4);

    QPushButton *S5 = new QPushButton("Simulate 5",this);
    S5->setMaximumWidth(100);
    connect(S5,SIGNAL(released()), reader, SLOT(SS5()));

    windowLayoutUI->addWidget(S5);


} // constructor

FluidWindow::~FluidWindow()
{ // destructor
    delete ptimer;

} // destructor

void FluidWindow::handleButton(){
    //    FluidWindow* window = new FluidWindow(NULL);
    qDebug()<<"BENAS";

}
