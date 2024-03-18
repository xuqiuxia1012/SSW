/*
 * China Entry Infotainment Project
 * Copyright (c) 2023 FAW-VW, P-VC & MOSI-Tech.
 * All Rights Reserved.
 */

#ifndef COMMONAPI_FDBUS_FDBUSSERVER_HPP_
#define COMMONAPI_FDBUS_FDBUSSERVER_HPP_

#include<fdbus/fdbus.h>
#include <CommonAPI/FDBus/FDBusAddress.hpp>

using namespace ipc::fdbus;

namespace CommonAPI {
namespace FDBus {

class FDBusStubAdapter;
class FDBusConnection;

class FDBusServer : public ipc::fdbus::CBaseServer
{
public:
    FDBusServer(const FDBusAddress& fdbusAddress, CBaseWorker * worker = 0);

    void setAdapter(FDBusStubAdapter* adapter);
    void setConnection(FDBusConnection* connection);
    bool replyClient(std::shared_ptr<ipc::fdbus::CFdbMessage> msg);

    bool broadcastMessage(std::shared_ptr<ipc::fdbus::CFdbMessage> msg, const char* topic = 0);
    void doBind();
    void doUnbind();
    
protected:
    void onOnline(const CFdbOnlineInfo &info);
    void onOffline(const CFdbOnlineInfo &info);
    void onInvoke(CBaseJob::Ptr &msg_ref);
    void onSubscribe(CBaseJob::Ptr &msg_ref);

private:
    FDBusConnection*  connection_;
    FDBusAddress mAddress;
};

}
}
#endif
