#pragma once
#include <drogon/WebSocketController.h>
#include "models/Robot.h"

using namespace drogon;
class h264_stream:public drogon::WebSocketController<h264_stream>,Robot
{
  public:
    h264_stream();
    ~h264_stream();
    virtual void handleNewMessage(const WebSocketConnectionPtr&,
                                  std::string &&,
                                  const WebSocketMessageType &) override;
    virtual void handleNewConnection(const HttpRequestPtr &,
                                     const WebSocketConnectionPtr&)override;
    virtual void handleConnectionClosed(const WebSocketConnectionPtr&)override;
    WS_PATH_LIST_BEGIN
    //list path definitions here;
    WS_PATH_ADD("/stream");
    WS_PATH_LIST_END
};
