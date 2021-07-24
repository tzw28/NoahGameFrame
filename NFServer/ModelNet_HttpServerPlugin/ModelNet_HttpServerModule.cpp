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

#include "ModelNet_HttpServerModule.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"
#include "google/protobuf/util/json_util.h"
#include "zlib.h"
#include "zip.h"
#include <fstream>
#include "gzip/compress.hpp"

bool ModelNet_HttpServerModule::Init()
{
    m_pHttpNetModule = pPluginManager->FindModule<NFIHttpServerModule>();
    m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
    // m_pModelServerModule = pPluginManager->FindModule<NFIModelNet_ServerModule>();
    m_pLogicClassModule = pPluginManager->FindModule<NFIClassModule>();
    m_pElementModule = pPluginManager->FindModule<NFIElementModule>();

    return true;
}
bool ModelNet_HttpServerModule::Shut()
{
    return true;
}

bool ModelNet_HttpServerModule::AfterInit()
{
    //http://127.0.0.1/json
    m_pHttpNetModule->AddRequestHandler("/json", NFHttpType::NF_HTTP_REQ_GET, this, &ModelNet_HttpServerModule::OnCommandQuery);
    m_pHttpNetModule->AddRequestHandler("/model", NFHttpType::NF_HTTP_REQ_GET, this, &ModelNet_HttpServerModule::OnModelRawQuery);
    m_pHttpNetModule->AddRequestHandler("/json", NFHttpType::NF_HTTP_REQ_POST, this, &ModelNet_HttpServerModule::OnCommandQuery);
    m_pHttpNetModule->AddRequestHandler("/json", NFHttpType::NF_HTTP_REQ_DELETE, this, &ModelNet_HttpServerModule::OnCommandQuery);
    m_pHttpNetModule->AddRequestHandler("/json", NFHttpType::NF_HTTP_REQ_PUT, this, &ModelNet_HttpServerModule::OnCommandQuery);

    m_pHttpNetModule->AddNetFilter("/json", this, &ModelNet_HttpServerModule::OnFilter);

    NF_SHARE_PTR<NFIClass> xLogicClass = m_pLogicClassModule->GetElement(NFrame::Server::ThisName());
    if (xLogicClass)
    {
        const std::vector<std::string>& strIdList = xLogicClass->GetIDList();
        for (int i = 0; i < strIdList.size(); ++i)
        {
            const std::string& strId = strIdList[i];

            int nJsonPort = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::WebPort());
            int nWebServerAppID = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::ServerID());

            //webserver only run one instance in each server
            if (pPluginManager->GetAppID() == nWebServerAppID)
            {
                m_pHttpNetModule->InitServer(nJsonPort);

                break;
            }
        }
    }
    m_pOcc = new OCCProcessor();
    loadModelFileNames(m_aModels);
    return true;
}

bool ModelNet_HttpServerModule::Execute()
{
    //m_pHttpNetModule->Execute();
    return true;
}

bool ModelNet_HttpServerModule::OnCommandQuery(NF_SHARE_PTR<NFHttpRequest> req)
{
    std::cout << "url: " << req->url << std::endl;
    std::cout << "path: " << req->path << std::endl;
    std::cout << "type: " << req->type << std::endl;
    std::cout << "body: " << req->body << std::endl;

    std::cout << "params: " << std::endl;

    for (auto item : req->params)
    {
        std::cout << item.first << ":" << item.second << std::endl;
    }

    std::cout << "headers: " << std::endl;

    for (auto item : req->headers)
    {
        std::cout << item.first << ":" << item.second << std::endl;
    }

    // std::string str = m_pModelServerModule->GetServersStatus();
    std::string str = "simple response";
    return m_pHttpNetModule->ResponseMsg(req, str, NFWebStatus::WEB_OK);
}

bool ModelNet_HttpServerModule::OnModelRawQuery(NF_SHARE_PTR<NFHttpRequest> req)
{
    int target, level;
    try
    {
        target = std::stoi(req->params["target"]);
        level = std::stoi(req->params["level"]);
    }
    catch (exception e)
    {
        return m_pHttpNetModule->ResponseMsg(req, e.what(), NFWebStatus::WEB_BAD_REQ);
    }

    std::string aModelFile = "";
    std::stringstream ssout;
    long long t;
    string temp_str = "";
    if (m_aModels.size() > 0)
    {
        aModelFile = m_aModels[target];
        t = GetSystemTime();
        ssout << t << " ";
        m_pOcc->loadModel(aModelFile.c_str(), level, temp_str);
        t = GetSystemTime();
        ssout << t << " ";
    }
    else
    {
        m_pOcc->initializeModel();
    }
    if (temp_str == "")
        m_pOcc->toMeshString(temp_str);
    t = GetSystemTime();
    ssout << t << " ";
    NFMsg::ReqAckModelTarget xMsg;
    xMsg.set_msg(ssout.str());
    NFMsg::ModelSyncUnit* syncUnit = xMsg.mutable_sync_unit();
    syncUnit->set_raw(temp_str);

    std::string str = PBMsgToJsonString(xMsg);
    return m_pHttpNetModule->ResponseMsg(req, str, NFWebStatus::WEB_OK);
}

NFWebStatus ModelNet_HttpServerModule::OnFilter(NF_SHARE_PTR<NFHttpRequest> req)
{
    std::cout << "OnFilter: " << std::endl;

    std::cout << "url: " << req->url << std::endl;
    std::cout << "path: " << req->path << std::endl;
    std::cout << "type: " << req->type << std::endl;
    std::cout << "body: " << req->body << std::endl;

    std::cout << "params: " << std::endl;

    for (auto item : req->params)
    {
        std::cout << item.first << ":" << item.second << std::endl;
    }

    std::cout << "headers: " << std::endl;

    for (auto item : req->headers)
    {
        std::cout << item.first << ":" << item.second << std::endl;
    }

    return NFWebStatus::WEB_OK;
}

std::string ModelNet_HttpServerModule::PBMsgToJsonString(const google::protobuf::Message& xMsg)
{
    std::string msg1 = "";
    // google::protobuf::util::MessageToJsonString(xMsg, &msg1);
    google::protobuf::util::JsonPrintOptions options;
    options.add_whitespace = true;
    options.always_print_primitive_fields = true;
    options.preserve_proto_field_names = true;
    MessageToJsonString(xMsg, &msg1, options);
    // return msg1;

    const char* buf = msg1.c_str();
    const char* pointer = msg1.data();
    std::size_t size = msg1.size();
    std::string compressed_data = gzip::compress(pointer, size);

    return compressed_data;
}
