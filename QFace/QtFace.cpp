#include<XFace/FapFile.h>
#include<XEngine/RendererGL.h>
#include<XFace/FaceBase.h>
#include"QtView.h"
#include"QtFace.h"
#include"cstdio"
namespace XFaceApp{
    using namespace XFace;
    using namespace XEngine;
    QtFace::QtFace(QtView* pWnd):m_pWnd(pWnd){
        //pass
    }
    
    bool QtFace::init(){
        return ApplicationBase::init();
    }

/*
boost::shared_ptr<XEngine::ITimer> QtFace::createTimer() const
{
    printf("create timer\n");
    boost::shared_ptr<XEngine::ITimer> ret(new wxFaceTimer);
    return ret;
}
*/
boost::shared_ptr<XFace::FaceBase> QtFace::createFace() const
{
    printf("create face\n");
    boost::shared_ptr<XFace::FaceBase> ret(new XFace::FaceBase);
    return ret;
}

boost::shared_ptr<XEngine::IRenderer> QtFace::createRenderer() const
{
    printf("create render\n");
    boost::shared_ptr<XEngine::IRenderer> ret(new XEngine::RendererGL);
    return ret;
}

boost::shared_ptr<XFace::IFapStream> QtFace::createFapStream() const
{
    printf("create fap stream\n");
    boost::shared_ptr<XFace::IFapStream> ret(new XFace::FAPFile);
    return ret;
}

void QtFace::renderBegin() const
{
    printf("renderbegin\n");
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glMatrixMode( GL_MODELVIEW );
	glPushMatrix();
//		glLoadIdentity( );
//		glTranslatef( 0.0, 20.0, -650.0 );
}

void QtFace::renderEnd() const
{
    printf("render end\n");
	glPopMatrix();
	m_pWnd->swapBuffers();
}

bool QtFace::onLoadFAP(const std::string& param)
{
  return ApplicationBase::onLoadFAP(param);
}

void QtFace::onRenderFrame() const
{
    ApplicationBase::onRenderFrame();
}

void QtFace::jumpToFap(unsigned long fapID)
{
	unsigned long cnt = m_pFapStream->getFAPCount();
	if(cnt < fapID)
		return;

	if(m_pFapStream->getCurrentFAPId() > fapID)
		m_pFapStream->rewind();
	while (m_pFapStream->getCurrentFAPId() != fapID)
		m_pFapStream->next();

	const std::vector<float>& FAPs = m_pFapStream->getCurrentFAP();
	if(!FAPs.empty())
	{
		//m_pSound->jump((float)fapID / (float)cnt);
		m_pFace->update(m_pFapStream);
		m_renderManager.setGlobalTransform(m_pFace->getTransform());
		renderBegin();
		m_renderManager.render();
		renderEnd();		
	}
	
}

bool QtFace::onLoadFDP(const std::string& param1, const std::string& param2){
  return ApplicationBase::onLoadFDP(param1, param2);
}

void QtFace::newTask(const Task& task)
{
    printf("new task\n");
	static int test = 0;
	if(task.getID() != 0)
	{
		printf("new task %d\n",task.getID());
	}
	ApplicationBase::newTask(task);
}

bool QtFace::processTask()
{
    printf("qt face process task\n");
	if (m_taskQueue.size() == 0) {
        printf("task queue is 0\n");
		return false;
	}

	const Task &tsk_c = m_taskQueue.front();
	if(tsk_c.getName() == "SAVE_AVI")
	{
		// get the task out for proper notification handling 
		m_currentTask = m_taskQueue.front();
		//Task tsk = m_taskQueue.front();
		//m_pCurrTask = &tsk;
		m_taskQueue.pop();

		return true;
	}
	
	return ApplicationBase::processTask();
}

}
