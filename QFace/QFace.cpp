#include"QFace.h"
#include<QtGui/QApplication>
//#include<QtGui/QPushButton>
using namespace QFace;
int main(int argc,char** argv){
    QApplication app(argc,argv);
    /*
    QtView viewer(0);
    viewer.resize(640,480);
    viewer.show();
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(viewer);
    */
    QWindow win(0);
    //win.resize(win.sizeHint());
    win.show();
    return app.exec();
}
