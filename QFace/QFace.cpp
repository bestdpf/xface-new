#include"QtView.h"
#include<QtGui/QApplication>
int main(int argc,char** argv){
    QApplication app(argc,argv);
    QtView viewer=QtView(0);
    viewer.resize(640,480);
    viewer.show();
    return app.exec();
}
