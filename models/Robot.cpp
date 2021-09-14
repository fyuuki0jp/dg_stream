#include "models/Robot.h"

RobotInput Robot::m_rInput;
int Robot::m_nLoadCount = 0;

void ProcCall(void* parent, uint8_t* data,uint32_t len)
{
    Robot* p = (Robot*)parent;
    p->Process(data,len);
}

Robot::Robot()
{
    if(m_nLoadCount == 0)
        m_rInput.initDevice(720,480,30,&ProcCall,(void*)this);
    m_nLoadCount++;

}

Robot::~Robot()
{
    m_nLoadCount--;
    if(m_nLoadCount==0)
        m_rInput.releaseDevice();
}

void Robot::Process(uint8_t* data,uint32_t len)
{

}