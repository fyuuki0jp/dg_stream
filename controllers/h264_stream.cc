#include "h264_stream.h"
#include <iostream>

//RobotInput h264_stream::m_rInput;

h264_stream::h264_stream()
{
}

h264_stream::~h264_stream()
{
}

void h264_stream::handleNewMessage(const WebSocketConnectionPtr& wsConnPtr, std::string &&message, const WebSocketMessageType &type)
{
    //write your application logic here
}

void h264_stream::handleNewConnection(const HttpRequestPtr &req,const WebSocketConnectionPtr& wsConnPtr)
{
    //write your application logic here
    std::cout << "new connection " << m_rInput.m_pClients.size();
    m_rInput.m_pClients.push_back(wsConnPtr);
//    m_rInput.RequestIFrame();
    std::cout << " -> " << m_rInput.m_pClients.size() << std::endl;
    m_rInput.RequestIFrame();
}
void h264_stream::handleConnectionClosed(const WebSocketConnectionPtr& wsConnPtr)
{
    std::cout << "close connection" << m_rInput.m_pClients.size();
    //write your application logic here
    for (auto it = m_rInput.m_pClients.begin(); it != m_rInput.m_pClients.end();) {
        // 条件一致した要素を削除する
        if (*it == wsConnPtr) {
            // 削除された要素の次を指すイテレータが返される。
            it  = m_rInput.m_pClients.erase(it);
        }
        // 要素削除をしない場合に、イテレータを進める
        else {
            ++it;
        }
    }
    std::cout << " -> " << m_rInput.m_pClients.size() << std::endl;
}
