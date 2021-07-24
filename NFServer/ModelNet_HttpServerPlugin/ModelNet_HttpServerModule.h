/*
            This file is part of:
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2021 NoahFrame(NoahGameFrame)

   File creator: lvsheng.huang

   NoahFrame is open-source software and you can redistribute it and/or modify
   it under the terms of the License; besides, anyone who use this file/software must include this copyright announcement.

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#ifndef MODELNET_HTTP_MODULE_H
#define MODELNET_HTTP_MODULE_H

#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFPluginModule/NFPlatform.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFIHttpServerModule.h"
#include "NFComm/NFPluginModule/NFIModelNet_ServerModule.h"
#include "NFComm/NFPluginModule/NFIModelNet_HttpServerModule.h"
#include "NFServer/NFGameServerNet_ServerPlugin/OCCProcessor.h"
#include "NFServer/NFGameServerNet_ServerPlugin/OCCUtil.h"

class ModelNet_HttpServerModule
    : public NFIModelNet_HttpServerModule
{
public:
    ModelNet_HttpServerModule(NFIPluginManager* p)
    {
        m_bIsExecute = true;
        pPluginManager = p;
    }

    virtual bool Init();
    virtual bool Shut();

    virtual bool AfterInit();
    virtual bool Execute();

protected:
    bool OnCommandQuery(NF_SHARE_PTR<NFHttpRequest> req);
    bool OnModelRawQuery(NF_SHARE_PTR<NFHttpRequest> req);

    NFWebStatus OnFilter(NF_SHARE_PTR<NFHttpRequest> req);

private:
    std::string PBMsgToJsonString(const google::protobuf::Message& xMsg);

    NFIKernelModule* m_pKernelModule;
    NFIHttpServerModule* m_pHttpNetModule;
    NFIModelNet_ServerModule* m_pModelServerModule;
    NFIClassModule* m_pLogicClassModule;
    NFIElementModule* m_pElementModule;

    OCCProcessor* m_pOcc;
    std::vector<std::string> m_aModels;
};

#endif
