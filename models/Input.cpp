#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <vector>
#include <malloc.h>
#include <time.h>
#include <chrono>
#include <thread>
#include <linux/videodev2.h>
#include <drogon/WebSocketController.h>
#include "models/Input.h"

RobotInput::RobotInput()
{
    
}

RobotInput::~RobotInput()
{

}

bool RobotInput::initDevice(int width,int height,int fps,void (*callback)(void* parent,uint8_t* camera,uint32_t len),void* parent)
{
    bcm_host_init();
    //OMX_Init();

    m_nCameraWidth = width;
    m_nCameraHeight = height;
    m_pProcCall = callback;
    m_pParent = parent;
	printf("camera initialize\n");

	m_pCam = new Camera(width, height, 0, false, 0, false);
	m_pEncoder = new VideoEncode(2*1024, VideoEncode::CodingAVC, true, false);
	m_pCam->SetState(Component::StateIdle, true);
	m_pEncoder->SetState(Component::StateIdle);

	m_pCam->DisableProprietaryTunnels(70);
	m_pCam->SetupTunnelVideo(m_pEncoder);
	m_pCam->AllocateOutputBuffer(70);

	m_pCam->setFramerate(fps);
    m_pEncoder->setIDRPeriod(fps*2);
	printf("camera setting complete\n");
	m_pCam->SetState(Component::StateExecuting, true);
	m_pEncoder->SetState(Component::StateExecuting);

	printf("camera executing mode.\n");
    m_fEncode=true;
    m_fCapture=true;
    pthread_create(&m_hCapture,NULL,RobotInput::CaptureThread,(void*)this);
    pthread_create(&m_hEncode,NULL,RobotInput::EncodeThread,(void*)this);
    m_pCam->SetCapturing( true );

    return true;
}

void RobotInput::releaseDevice()
{
    m_fEncode = false;
    pthread_join(m_hEncode,NULL);
    m_fCapture = false;
    pthread_join(m_hCapture,NULL);
    m_pCam->SetState(Component::StateIdle);
	m_pEncoder->SetState(Component::StateIdle);

	Component::DestroyAll();
    m_pClients.clear();
}

void* RobotInput::CaptureThread(void* param)
{
    RobotInput* parent = (RobotInput*)param;
    uint8_t *SrcPtr = new uint8_t[parent->m_nCameraWidth*parent->m_nCameraHeight*2];
	int32_t Srclen = 0;
    while (parent->m_fCapture)
	{
		Srclen = parent->m_pCam->getOutputData(70, SrcPtr);
        if(parent->m_pParent != NULL && parent->m_pProcCall != NULL && Srclen > 0)
        {
            parent->m_pProcCall(parent->m_pParent,SrcPtr,Srclen);
        }
	}
    return NULL;
}

void* RobotInput::EncodeThread(void* param)
{
    RobotInput* parent = (RobotInput*)param;
    uint8_t *data = NULL; //h264 temporary buffer data.
	uint32_t datalen = 0;	  //h264 temporary buffer size.
	int frame = 0;			   //h264 frame count(reset this value when fps equals this)
    data = new uint8_t[80000];
	memset(data, 0, 80000);
    while(parent->m_fEncode)
    {
        datalen = parent->m_pEncoder->getOutputData(data,true);
        if (datalen > 0) //data is available.
		{
			frame++;
            if(parent->m_pClients.size() > 0)
            {
                for(int i = 0;i<parent->m_pClients.size();i++)
                {
                    if((data[4]&0x1f) != 0x07)
                    {
                        parent->m_pClients[i]->send((const char*)data,datalen,WebSocketMessageType::Binary);
                    }
                    else
                    {
                        int pos = datalen-9;
                        parent->m_pClients[i]->send((const char*)data,pos+1,WebSocketMessageType::Binary);
                        parent->m_pClients[i]->send((const char*)data+pos,8,WebSocketMessageType::Binary);
                    }
                }
            }
        }
    }
    return NULL;
}

void RobotInput::RequestIFrame()
{
    m_pEncoder->RequestIFrame();
}