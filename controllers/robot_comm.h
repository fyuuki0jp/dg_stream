#pragma once
#include <drogon/WebSocketController.h>
#include "models/Robot.h"

using namespace drogon;
class robot_comm:public drogon::WebSocketController<robot_comm>,Robot
{
  public:
    virtual void handleNewMessage(const WebSocketConnectionPtr&,
                                  std::string &&,
                                  const WebSocketMessageType &) override;
    virtual void handleNewConnection(const HttpRequestPtr &,
                                     const WebSocketConnectionPtr&)override;
    virtual void handleConnectionClosed(const WebSocketConnectionPtr&)override;
    WS_PATH_LIST_BEGIN
    //list path definitions here;
    WS_PATH_ADD("/robot");
    WS_PATH_LIST_END
};
