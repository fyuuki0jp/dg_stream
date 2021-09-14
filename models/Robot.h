#include "models/Input.h"

class Robot {
    public:
    Robot();
    ~Robot();
    static RobotInput m_rInput;
    static int m_nLoadCount;
    void Process(uint8_t* camera,uint32_t len);
};