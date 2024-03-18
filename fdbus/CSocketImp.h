/*
 * Copyright (C) 2015   Jeremy Chen jeremy_cz@yahoo.com
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _CSOCKETIMP_H_
#define _CSOCKETIMP_H_

#include <stdint.h>
#include <string>
#include "common_defs.h"

namespace ipc {
namespace fdbus {
enum EFdbSocketType
{
    FDB_SOCKET_TCP,
    FDB_SOCKET_UDP,
    FDB_SOCKET_IPC,
    FDB_SOCKET_SVC,
    FDB_SOCKET_MAX
};

struct CFdbSocketAddr
{
    CFdbSocketAddr()
        : mType(FDB_SOCKET_MAX)
        , mAddrDigit(0)
        , mPort(FDB_INET_PORT_INVALID)
        , mSecure(false)
    {}
    void buildUrl();
    std::string mUrl;
    EFdbSocketType mType;
    std::string mAddr;
    tFdbIpV4 mAddrDigit;
    int32_t mPort;
    bool mSecure;
};

struct CFdbSocketCredentials
{
    uint32_t pid;
    uint32_t gid;
    uint32_t uid;
};

struct CFdbSocketConnInfo
{
    std::string mPeerIp;
    tFdbIpV4 mPeerIpDigit;
    int32_t mPeerPort;
    CFdbSocketAddr mSelfAddress;
};

class CBaseSocket
{
public:
    struct CSocketParams
    {
        bool mBlockMode;
        int32_t mKeepAliveInterval;
        int32_t mKeepAliveRetry;
        void *mSSLContext;
        CSocketParams(bool block_mode, int32_t ka_interval, int32_t ka_retry, void *ssl_ctx)
            : mBlockMode(block_mode)
            , mKeepAliveInterval(ka_interval)
            , mKeepAliveRetry(ka_retry)
            , mSSLContext(ssl_ctx)
        {
        }
        CSocketParams()
            : mBlockMode(true)
            , mKeepAliveInterval(0)
            , mKeepAliveRetry(0)
            , mSSLContext(0)
        {}
    };
    CBaseSocket()
    {
        mCred.pid = UINT32_MAX;
        mCred.gid = UINT32_MAX;
        mCred.uid = UINT32_MAX;
        mConn.mPeerPort = FDB_INET_PORT_INVALID;
        mConn.mSelfAddress.mType = FDB_SOCKET_MAX;
        mConn.mSelfAddress.mPort = FDB_INET_PORT_INVALID;
    }

    CBaseSocket(CFdbSocketAddr &addr)
    {
        mCred.pid = UINT32_MAX;
        mCred.gid = UINT32_MAX;
        mCred.uid = UINT32_MAX;
        mConn.mPeerPort = FDB_SOCKET_MAX;
        mConn.mSelfAddress = addr;
    }

    virtual ~CBaseSocket()
    {
    }

    const CFdbSocketCredentials &getPeerCredentials()
    {
        return mCred;
    }
    const CFdbSocketConnInfo &getConnectionInfo()
    {
        return mConn;
    }
    const CFdbSocketAddr &getAddress()
    {
        return mConn.mSelfAddress;
    }

    virtual int getFd()
    {
        return -1;
    }

protected:
    CFdbSocketCredentials mCred;
    CFdbSocketConnInfo mConn;
};

class CSocketImp : public CBaseSocket
{
public:
    CSocketImp()
        : CBaseSocket()
    {
    }

    CSocketImp(CFdbSocketAddr &addr)
        : CBaseSocket(addr)
    {
    }

    virtual int32_t send(const uint8_t *data, int32_t size)
    {
        return -1;
    }

    virtual int32_t send(const uint8_t *data, int32_t size, tFdbIpV4 ip, int32_t port)
    {
        return -1;
    }

    virtual int32_t recv(uint8_t *data, int32_t size)
    {
        return -1;
    }
    virtual int32_t recv(uint8_t *data, int32_t size, tFdbIpV4 &ip, int32_t &port)
    {
        return -1;
    }
    virtual bool createSSL(void *ctx, bool server)
    {
        return false;
    }
    virtual bool isSecure()
    {
        return false;
    }
};

class CClientSocketImp : public CBaseSocket
{
public:
    CClientSocketImp()
        : CBaseSocket()
    {
    }

    CClientSocketImp(CFdbSocketAddr &addr)
        : CBaseSocket(addr)
    {
    }

    virtual CSocketImp *connect(const CSocketParams &params)
    {
        return 0;
    }
};

class CServerSocketImp : public CBaseSocket
{
public:
    CServerSocketImp()
        : CBaseSocket()
    {
    }

    CServerSocketImp(CFdbSocketAddr &addr)
        : CBaseSocket(addr)
    {
    }

    virtual bool bind()
    {
        return false;
    }

    virtual CSocketImp *accept(const CSocketParams &params)
    {
        return 0;
    }
};

class CUDPSocketImp : public CBaseSocket
{
public:
    CUDPSocketImp()
        : CBaseSocket()
    {
    }

    CUDPSocketImp(CFdbSocketAddr &addr)
        : CBaseSocket(addr)
    {
    }

    virtual CSocketImp *bind()
    {
        return 0;
    }
};

}
}
#endif
