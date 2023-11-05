#include "fluidwindow.h"

FluidWindow::FluidWindow(QWidget *parent): QWidget(parent){ // constructor

    // create the window layout
    windowLayoutMain = new QHBoxLayout(this);

    windowLayoutUI = new QVBoxLayout(this);

    windowLayoutSpinBox= new QVBoxLayout(this);

    Fluid* reader = new Fluid(NULL);

    windowLayoutMain->addWidget(reader);
    windowLayoutMain->addLayout(windowLayoutUI);
    windowLayoutUI->addLayout(windowLayoutSpinBox);

    QLabel *label = new QLabel("Rest Density");
    label->setMaximumHeight(20);
    windowLayoutSpinBox->addWidget(label);
    QDoubleSpinBox *px = new QDoubleSpinBox;
    px->setMaximumWidth(80);
    px->setMaximumHeight(30);
    px->setMinimum(0);
    px->setMaximum(5000);
    px->setValue(700);
    px->setSingleStep(50);
    windowLayoutSpinBox->addWidget(px);
    connect(px, SIGNAL(valueChanged(double)),  reader  , SLOT(changeRestDensity(double)));

    QLabel *g = new QLabel("Gas Constant");
    g->setMaximumHeight(20);
    windowLayoutSpinBox->addWidget(g);
    QDoubleSpinBox *k = new QDoubleSpinBox;
    k->setMaximumWidth(80);
    k->setMaximumHeight(30);
    k->setMinimum(0);
    k->setMaximum(5000);
    k->setValue(900);
    k->setSingleStep(50);
    windowLayoutSpinBox->addWidget(k);
    connect(k, SIGNAL(valueChanged(double)),  reader  , SLOT(changeGasConstant(double)));

    QLabel *v = new QLabel("Viscosity Constant");
    v->setMaximumHeight(20);
    windowLayoutSpinBox->addWidget(v);
    QDoubleSpinBox *vc = new QDoubleSpinBox;
    vc->setMaximumWidth(80);
    vc->setMaximumHeight(30);
    vc->setMinimum(0);
    vc->setMaximum(10000);
    vc->setValue(3000);
    vc->setSingleStep(100);
    windowLayoutSpinBox->addWidget(vc);
    connect(vc, SIGNAL(valueChanged(double)),  reader  , SLOT(changeViscosityConstant(double)));

//    QLabel *r = new QLabel("Kernel Radius");
//    r->setMaximumHeight(20);
//    windowLayouttest->addWidget(r);
//    QDoubleSpinBox *kr = new QDoubleSpinBox;
//    kr->setMaximumWidth(80);
//    kr->setMaximumHeight(30);
//    kr->setMinimum(-360);
//    kr->setMaximum(5000);
//    kr->setValue(1.5);
//    windowLayouttest->addWidget(kr);
//    connect(kr, SIGNAL(valueChanged(double)),  reader  , SLOT(changeKernelRadius(double)));

    QLabel *s = new QLabel("Surface Tension Constant");
    s->setMaximumHeight(20);
    windowLayoutSpinBox->addWidget(s);
    QDoubleSpinBox *st = new QDoubleSpinBox;
    st->setMaximumWidth(80);
    st->setMaximumHeight(30);
    st->setMinimum(0);
    st->setMaximum(1);
    st->setSingleStep(0.1);
    st->setValue(0.9);
    windowLayoutSpinBox->addWidget(st);
    connect(st, SIGNAL(valueChanged(double)),  reader  , SLOT(changeSurfaceTension(double)));

    QLabel *d = new QLabel("Delta Time");
    d->setMaximumHeight(20);
    windowLayoutSpinBox->addWidget(d);
    QDoubleSpinBox *dt = new QDoubleSpinBox;
    dt->setMaximumWidth(80);
    dt->setMaximumHeight(30);
    dt->setMinimum(-10);
    dt->setMaximum(10.0);
    dt->setDecimals(3);
    dt->setSingleStep(0.05);
    dt->setValue(0.3);
    windowLayoutSpinBox->addWidget(dt);
    connect(dt, SIGNAL(valueChanged(double)),  reader  , SLOT(changeTime(double)));

    QLabel *dl = new QLabel("Damp");
    dl->setMaximumHeight(20);
    windowLayoutSpinBox->addWidget(dl);
    QDoubleSpinBox *dd = new QDoubleSpinBox;
    dd->setMaximumWidth(80);
    dd->setMaximumHeight(30);
    dd->setMinimum(0);
    dd->setMaximum(1);
    dd->setDecimals(2);
    dd->setSingleStep(0.1);
    dd->setValue(0.5);
    windowLayoutSpinBox->addWidget(dd);
    connect(dd, SIGNAL(valueChanged(double)),  reader  , SLOT(changeDamp(double)));

    // Check boxes for toggling
    QCheckBox *S2 = new QCheckBox("Toggle Viscosity", this);
    S2->setMaximumWidth(400);
    S2->setCheckable(true);
    S2->setCheckState(Qt::Checked);
    connect(S2,SIGNAL(released()), reader, SLOT(SS2()));
    windowLayoutUI->addWidget(S2);

    QCheckBox *S3 = new QCheckBox("Toggle Surface Tension", this);
    S3->setMaximumWidth(400);
    S3->setCheckable(true);
    S3->setCheckState(Qt::Checked);
    connect(S3,SIGNAL(released()), reader, SLOT(SS3()));
    windowLayoutUI->addWidget(S3);

    QCheckBox *visSur = new QCheckBox("Visualize Surface",this);
    visSur->setMaximumWidth(400);
    visSur->setCheckable(true);
    visSur->setCheckState(Qt::Unchecked);
    connect(visSur,SIGNAL(released()), reader, SLOT(SS1()));
    windowLayoutUI->addWidget(visSur);


    //Arrow Buttons
    TriangularButton *downArrow = new TriangularButton("",this,0);
    downArrow->setStyleSheet("background-color: rgb(101, 101, 101);");
    downArrow->setMaximumWidth(25);
    downArrow->setMaximumHeight(25);
    connect(downArrow,SIGNAL(released()), reader, SLOT(moveDown()));

    TriangularButton *leftArrow = new TriangularButton("",this,90);
    leftArrow->setStyleSheet("background-color: rgb(101, 101, 101);");
    leftArrow->setMaximumWidth(50);
    leftArrow->setMaximumHeight(25);
    connect(leftArrow,SIGNAL(released()), reader, SLOT(moveLeft()));

    TriangularButton *rightArrow = new TriangularButton("",this,270);
    rightArrow->setStyleSheet("background-color: rgb(101, 101, 101);");
    rightArrow->setMaximumWidth(50);
    rightArrow->setMaximumHeight(25);
    connect(rightArrow,SIGNAL(released()), reader, SLOT(moveRight()));

    TriangularButton *upArrow = new TriangularButton("",this,180);
    upArrow->setStyleSheet("background-color: rgb(101, 101, 101);");
    upArrow->setMaximumWidth(25);
    upArrow->setMaximumHeight(25);
    connect(upArrow,SIGNAL(released()), reader, SLOT(moveUp()));

    QHBoxLayout *upBox = new QHBoxLayout(this);
    upBox->addWidget(upArrow);

    QHBoxLayout *leftRightBox = new QHBoxLayout(this);
    leftRightBox->addWidget(leftArrow);
    leftRightBox->setSpacing(40);
    leftRightBox->addWidget(rightArrow);

    QHBoxLayout *downBox = new QHBoxLayout(this);
    downBox->addWidget(downArrow);

    QLabel *buttonsLabel = new QLabel("Move blob",this);
    buttonsLabel->setMaximumHeight(20);
    buttonsLabel->setAlignment(Qt::AlignCenter);

    QVBoxLayout *everything = new QVBoxLayout(this);
    everything->addWidget(buttonsLabel);
    everything->addLayout(upBox);
    everything->addLayout(leftRightBox);
    everything->addLayout(downBox);

    windowLayoutUI->addLayout(everything);


} // constructor

FluidWindow::~FluidWindow(){ // destructor
    delete ptimer;

} // destructor

void FluidWindow::handleButton(){
}
