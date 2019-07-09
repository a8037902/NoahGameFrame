/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2018 NoahFrame(NoahGameFrame)

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

#ifndef NFC_CLIENT_MODULE_H
#define NFC_CLIENT_MODULE_H

#include "NFComm/NFPluginModule/NFIPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFINavigationModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIScheduleModule.h"
//#include "NFComm/NFPluginModule/NFISceneAOIModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFIClassModule.h"
#include "NFComm/NFPluginModule/NFIEventModule.h"
#include "NFComm/NFPluginModule/NFINetModule.h"
#include "NFComm/NFPluginModule/NFINetClientModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"

/*
IN THIS PLUGIN:
YOU WILL KNOW HOW TO USE THE "IScriptModule" TO CALL LUA SCRIPT
*/
enum ClientDataState
{
	IDLE,
};
struct ClientData
{
	ClientData()
	{
		nClientID = 0;
		nLoginID = 0;
		nWorldID = 0;
		nGameID = 0;
		nRoleID = 0;
		strAccount = "";
		strPassword = "";
		eState = ClientDataState::IDLE;
		mnLastActionTime = 0;
	}

	int nClientID;
	int nLoginID;
	int nWorldID;
	int nGameID;
	int nRoleID;
	std::string strAccount;
	std::string strPassword;
	ClientDataState eState;
	NFINT64 mnLastActionTime;
};

class NFIClientModule
	: public NFIModule
{
protected:

	typedef std::function<int(const NFGUID&, const NFEventDefine, const NFDataList&)> OBJECT_EVENT_FUNCTOR;

	typedef NF_SHARE_PTR<OBJECT_EVENT_FUNCTOR> OBJECT_EVENT_FUNCTOR_PTR;//EVENT
public:
	virtual bool DoEvent(const NFGUID& self, const NFEventDefine nEventID, const NFDataList& valueList) = 0;

	virtual bool ExistEventCallBack(const NFEventDefine nEventID) = 0;

	virtual bool RemoveEventCallBack(const NFEventDefine nEventID) = 0;

	template<typename BaseType>
	bool AddEventCallBack(const NFEventDefine nEventID, BaseType* pBase, int (BaseType::*handler)(const NFGUID&, const NFEventDefine, const NFDataList&))
	{
		OBJECT_EVENT_FUNCTOR functor = std::bind(handler, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
		OBJECT_EVENT_FUNCTOR_PTR functorPtr(new OBJECT_EVENT_FUNCTOR(functor));
		return AddEventCallBack(nEventID, functorPtr);
	}

	NFGUID mRoleID;

protected:
	virtual bool AddEventCallBack(const NFEventDefine nEventID, const OBJECT_EVENT_FUNCTOR_PTR cb) = 0;
};

class NFCClientModule
    : public NFIClientModule
{

public:
	NFCClientModule(NFIPluginManager* p)
    {
        pPluginManager = p;
    }

    virtual bool Init();
    virtual bool AfterInit();

    virtual bool Execute();

    virtual bool BeforeShut();
    virtual bool Shut();

	//////////////////////////////////////////////////////////
	virtual bool DoEvent(const NFGUID& self, const NFEventDefine nEventID, const NFDataList& valueList);

	virtual bool ExistEventCallBack(const NFEventDefine nEventID);

	virtual bool RemoveEventCallBack(const NFEventDefine nEventID);

	//NFGUID mSelf;

protected:
	virtual bool AddEventCallBack(const NFEventDefine nEventID, const OBJECT_EVENT_FUNCTOR_PTR cb);

	void OnSocketLoginEvent(const NFSOCK nSockIndex, const NF_NET_EVENT eEvent, NFINet* pNet);
	void OnSocketWorldEvent(const NFSOCK nSockIndex, const NF_NET_EVENT eEvent, NFINet* pNet);

	void OnLoginProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnWorldList(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnConnectWorld(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnConnectKey(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnSelectServer(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnRoleList(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

	int OnEvent(const NFGUID& self, const NFEventDefine event, const NFDataList& arg);
	int OnClassCallBackEvent(const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT event, const NFDataList& arg);
	int OnPropertyCallBackEvent(const NFGUID& self, const std::string& strProperty, const NFData& oldVar, const NFData& newVar);
	int OnRecordBetCallBackEvent(const NFGUID& self, const RECORD_EVENT_DATA& xEventData, const NFData& oldVar, const NFData& newVarr);
	int OnRecordCallBackEvent(const NFGUID& self, const RECORD_EVENT_DATA& xEventData, const NFData& oldVar, const NFData& newVarr);

	void OnWorldPropertyIntProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

	int OnHeartBeat(const NFGUID& self, const std::string& strHeartBeat, const float fTime, const int nCount);

	void Login(NFINet* pNet);
	void LoginPB(int nID, string strAccount, string strPwd, string strKey);
private:
	NFIKernelModule* m_pKernelModule;
	NFIScheduleModule* m_pScheduleModule;
	//NFISceneAOIModule* m_pSceneAOIModule;
	NFIElementModule* m_pElementModule;
	NFIClassModule* m_pClassModule;
	NFIEventModule* m_pEventModule;
	NFINetModule* m_pNetModule;
	NFINetClientModule* m_pNetClientModule;
	NFILogModule* m_pLogModule;

	int64_t mLastTime;

	
	int mServerID;
	string mKey;

	ClientData mClientData;
	// for module
	NFList<NFEventDefine> mModuleRemoveListEx;
	NFMapEx<NFEventDefine, NFList<OBJECT_EVENT_FUNCTOR_PTR>> mModuleEventInfoMapEx;
};

#endif
