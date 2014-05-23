#include<QtGui>
#include<QtOpenGL>
#include<QSettings>
#include"QtView.h"
#include<GL/glu.h>
#include<XFaceApp/Task.h>
#include<XFaceApp/XMLUtils.h>
using XFaceApp::Task;
using XFaceApp::TaskDictionary;
using XFaceApp::XMLUtils;

QtView::QtView(QWidget* parent):QGLWidget(QGLFormat(QGL::SampleBuffers), parent){
    QSettings settings(QString("QFace.ini"),QSettings::IniFormat);
    QString dummy;
    dummy=settings.value("MPEG-4/Initial-FDP",QString("alice.fdp"));
    m_initialFdp = dummy.toStdString();
    dummy=settings.value("MPEG-4/Initial-FAP",QString("../Faps/joy.fap"));
    m_initialFap = dummy.toStdString();
    m_initialFdpPath = "../Faces/alice/";
    m_timer = new QTimer(this);
    connect(m_timer,SIGNAL(timeout()),this,SLOT(OnTimer()));
    m_timer->start(1000);
}
QtView::~QtView(){
    delete m_pCamera;
    delete m_pApp;
    delete m_timer;
}

void QtView::OnTimer(){
  if(m_pApp)m_pApp->processTask();
}

void QtView::initGL(){
    QColor qtPurple = QColor::fromCmykF(0.39, 0.39, 0.0, 0.0);
    qglClearColor(qtPurple.dark());
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glShadeModel(GL_SMOOTH);
    glMatrixMode(GL_MODELVIEW);
    swapBuffers();
}

void QtView::paintGL(){
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
}

