#include "robot_comm.h"
void robot_comm::handleNewMessage(const WebSocketConnectionPtr& wsConnPtr, std::string &&message, const WebSocketMessageType &type)
{
    //write your application logic here
    if(type == WebSocketMessageType::Text)
        wsConnPtr->send(message);
}
void robot_comm::handleNewConnection(const HttpRequestPtr &req,const WebSocketConnectionPtr& wsConnPtr)
{
    //write your application logic here
}
void robot_comm::handleConnectionClosed(const WebSocketConnectionPtr& wsConnPtr)
{
    //write your application logic here
}
