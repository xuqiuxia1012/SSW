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

#ifndef __CFDBEVENTROUTER_H__
#define __CFDBEVENTROUTER_H__

#include <vector>
#include "common_defs.h"

namespace ipc {
namespace fdbus {
class CBaseEndpoint;
class CFdbMessage;
class CEventRouterProxy;
class CFdbSession;

class CFdbEventRouter
{
public:
    CFdbEventRouter(CBaseEndpoint *endpoint)
        : mEndpoint(endpoint)
    {}
    ~CFdbEventRouter();
    void addPeer(const char *peer_name);
    void connectPeers();
    void routeMessage(CFdbSession *session, CFdbMessage *msg);
    void syncEventPool(CFdbSession *session);
    CBaseEndpoint *endpoint() const
    {
        return mEndpoint;
    }

private:
    CBaseEndpoint *mEndpoint;
    typedef std::vector<CEventRouterProxy *> tPeerTbl;
    tPeerTbl mPeerTbl;
};

}
}
#endif
