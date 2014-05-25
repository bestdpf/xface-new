#ifndef _QFACE_H
#define _QFACE_H
#include<cstdio>
#include<QtCore/QCoreApplication>
#include<QtGui/QMainWindow>
#include<QtGui/QPushButton>
#include<QLayout>
#include<QtGui>
#include"QtView.h"
namespace QFace{
class QWindow: public QMainWindow{
    Q_OBJECT
public:
    QWindow(QWidget* parent):QMainWindow(parent){
        resize(sizeHint());
        m_button=new QPushButton("test",this);
        m_viewer=new QtView(this);
        m_viewer->resize(m_viewer->sizeHint());
        connect(m_button,SIGNAL(released()),this,SLOT(HandleStart()));
        m_layout=new QHBoxLayout;
        m_layout->addWidget(m_viewer);
        m_layout->addWidget(m_button);
        m_viewer->lower();
        setLayout(m_layout);
        setCentralWidget(m_viewer);
        //m_viewer->show();
        setWindowTitle("QFace");
    }
     QSize minimumSizeHint() const
 {
     return QSize(50, 50);
 }

 QSize sizeHint() const
 {
     return QSize(800, 600);
 }

private slots:
    void HandleStart(){
        printf("handle start button");
        m_viewer->test();
    };
private:
    QPushButton* m_button;
    QtView* m_viewer;
    QLayout *m_layout;
};
}//end of namespace QFace
#endif
