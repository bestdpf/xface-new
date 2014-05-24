#ifndef _QT_FACE_H
#define _QT_FACE_H
#include<string>
#include<XFaceApp/ApplicationBase.h>
//#include<ScriptProcessorLoader.h>

class QtView;
namespace XFaceApp{
class QtFace:public ApplicationBase{
private:
    QtView* m_pWnd;
    void renderBegin() const;
    void renderEnd() const;
    //create timer maybe useless!!!
    //boost::shared_ptr<XEngine::ITimer> createTimer() const;
    bool yield(){return true;};
protected:
    bool onLoadFDP(const std::string& param1, const std::string& param2);
    bool onLoadFAP(const std::string& param);
    boost::shared_ptr<XFace::FaceBase> createFace() const;
    boost::shared_ptr<XEngine::IRenderer> createRenderer() const;
    boost::shared_ptr<XFace::IFapStream> createFapStream() const;
public:
    bool init();
    bool processTask();
    void newTask(const Task& task);
    void onRenderFrame() const;
    void jumpToFap(unsigned long fapID);
    QtFace(QtView* pWnd);
};
}//end of namespace XFaceApp

#endif
