#include <vector>
#include <pthread.h>
#include <memory>
#include <drogon/WebSocketController.h>
#include "models/Camera.h"
#include "models/VideoEncode.h"
#include "models/VideoRender.h"

using namespace IL;
using namespace drogon;

class RobotInput {
    private:
        Camera* m_pCam;
        VideoEncode* m_pEncoder;
        pthread_t m_hCapture;
        pthread_t m_hEncode;
        bool m_fCapture;
        bool m_fEncode;
        int m_nCameraWidth;
        int m_nCameraHeight;
        void (*m_pProcCall)(void* parent,uint8_t* camera,uint32_t len);
        void* m_pParent;
        static void* CaptureThread(void* param);
        static void* EncodeThread(void* param);
    public:
        std::vector<WebSocketConnectionPtr> m_pClients;
        RobotInput();
        ~RobotInput();
        bool initDevice(int width=1280,int height=720,int fps=30,void (*callback)(void* parent,uint8_t* camera,uint32_t len)=NULL,void* parent=NULL);
        void releaseDevice();
        void RequestIFrame();
};