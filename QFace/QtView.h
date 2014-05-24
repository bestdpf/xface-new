#ifndef _QT_VIEW_H
#define _QT_VIEW_H

#include<QGLWidget>
#include<QTimer>
#include<QSettings>
#include<XFaceApp/ModelCamera.h>
#include"QtFace.h"

class QtView: public QGLWidget{
        Q_OBJECT
    private:
	std::string m_initialFap, m_initialFdp, m_initialFdpPath;
	XFaceApp::QtFace* m_pApp;
	bool m_init;
	XFaceApp::ModelCamera* m_pCamera;
	QTimer* m_timer;
	QSettings* m_settings;
	unsigned long m_listeningPort;
    public:
        QtView(QWidget* parent=0);
        ~QtView();
    protected:
        void initGL();
        void paintGL();
	void LoadFace();
	void Render();
	void OnTimer();
	void OnIdle();
	void OnPaint();
    public:
	void loadFDP(const std::string& filename, const std::string& path);
	void loadFAP(const std::string& filename);
	void setInitialFap(const std::string& filename) {m_initialFap = filename;}
	void setInitialFdp(const std::string& filename, const std::string path) {m_initialFdp = filename; m_initialFdpPath = path;}
};

#endif
