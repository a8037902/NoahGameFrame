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

#include "NFCClientModule.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"
#include "NFComm/NFKernelPlugin/NFKernelModule.h"

bool NFCClientModule::Init()
{ 
	
    std::cout << "Poke Game, Init" << std::endl;

    return true;
}

bool NFCClientModule::AddEventCallBack(const NFEventDefine nEventID, const OBJECT_EVENT_FUNCTOR_PTR cb)
{
	NF_SHARE_PTR<NFList<OBJECT_EVENT_FUNCTOR_PTR>> xEventListPtr = mModuleEventInfoMapEx.GetElement(nEventID);
	if (!xEventListPtr)
	{
		xEventListPtr = NF_SHARE_PTR<NFList<OBJECT_EVENT_FUNCTOR_PTR>>(NF_NEW NFList<OBJECT_EVENT_FUNCTOR_PTR>());
		mModuleEventInfoMapEx.AddElement(nEventID, xEventListPtr);
	}

	xEventListPtr->Add(cb);

	return false;
}

bool NFCClientModule::ExistEventCallBack(const NFEventDefine nEventID)
{
	return mModuleEventInfoMapEx.ExistElement(nEventID);
}

bool NFCClientModule::RemoveEventCallBack(const NFEventDefine nEventID)
{
	return mModuleEventInfoMapEx.RemoveElement(nEventID);
}

bool NFCClientModule::DoEvent(const NFGUID& self, const NFEventDefine nEventID, const NFDataList & valueList)
{
	bool bRet = false;

	NF_SHARE_PTR<NFList<OBJECT_EVENT_FUNCTOR_PTR>> xEventListPtr = mModuleEventInfoMapEx.GetElement(nEventID);
	if (xEventListPtr)
	{
		OBJECT_EVENT_FUNCTOR_PTR pFunPtr;
		bool bRet = xEventListPtr->First(pFunPtr);
		while (bRet)
		{
			OBJECT_EVENT_FUNCTOR* pFunc = pFunPtr.get();
			pFunc->operator()(self, nEventID, valueList);

			bRet = xEventListPtr->Next(pFunPtr);
		}

		bRet = true;
	}

	return bRet;
}

int NFCClientModule::OnEvent(const NFGUID& self, const NFEventDefine event, const NFDataList& arg)
{

	std::cout << "OnEvent EventID: " << event << " self: " << self.nData64 << " argList: " << arg.Int(0) << " " << " " << arg.String(1) << std::endl;
	DoEvent(self, event, arg);
	return 0;
}

int NFCClientModule::OnHeartBeat(const NFGUID& self, const std::string& strHeartBeat, const float fTime, const int nCount)
{

	int64_t unNowTime = NFGetTimeMS();

	std::cout << "strHeartBeat: " << fTime << " Count: " << nCount << "  TimeDis: " << unNowTime - mLastTime << std::endl;

	mLastTime = unNowTime;

	NFDataList xList;
	

	return 0;
}

int NFCClientModule::OnRecordBetCallBackEvent(const NFGUID& self, const RECORD_EVENT_DATA& xEventData, const NFData& oldVar, const NFData& newVarr)
{
	NF_SHARE_PTR<NFIRecord> xRecord = m_pKernelModule->FindRecord(self, xEventData.strRecordName);
	if (nullptr == xRecord)
	{
		return 0;
	}

	std::cout << "OnRecordCallBackEvent Name: " << xEventData.strRecordName << " Event: " << xEventData.nOpType << " Col: " << xEventData.nCol << " Row: " << xEventData.nRow << std::endl;
	switch (xEventData.nOpType)
	{
	case RECORD_EVENT_DATA::Add:
	case RECORD_EVENT_DATA::Cover:
	{
		NFDataList xDataList;
		bool bRet = xRecord->QueryRow(xEventData.nRow, xDataList);
		if (bRet)
		{
			
		}
	}
	break;
	default:
		break;
	}
	//std::cout << "OnPropertyCallBackEvent Property: " << strProperty << " OldValue: " << oldVar.GetInt() << " NewValue: " << newVar.GetInt() << std::endl;

	return 0;
}

int NFCClientModule::OnClassCallBackEvent(const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT event, const NFDataList& arg)
{

	std::cout << "OnClassCallBackEvent ClassName: " << strClassName << " ID: " << self.nData64 << " Event: " << event << std::endl;

	if (event == COE_CREATE_HASDATA)
	{
		
		
	}

	return 0;
}

int NFCClientModule::OnPropertyCallBackEvent(const NFGUID& self, const std::string& strProperty, const NFData& oldVar, const NFData& newVar)
{

	std::cout << "OnPropertyCallBackEvent Property: " << strProperty << " OldValue: " << oldVar.GetInt() << " NewValue: " << newVar.GetInt() << std::endl;

	return 0;
}

int NFCClientModule::OnRecordCallBackEvent(const NFGUID& self, const RECORD_EVENT_DATA& xEventData, const NFData& oldVar, const NFData& newVarr)
{
	NF_SHARE_PTR<NFIRecord> xRecord = m_pKernelModule->FindRecord(self, xEventData.strRecordName);
	if (nullptr == xRecord)
	{
		return 0;
	}

	std::cout << "OnRecordCallBackEvent Name: "<< xEventData.strRecordName << " Event: " << xEventData.nOpType << " Col: " << xEventData.nCol << " Row: " << xEventData.nRow << std::endl;
	switch (xEventData.nOpType) 
	{
		case RECORD_EVENT_DATA::Add:
		{
			NFDataList xDataList;
			bool bRet = xRecord->QueryRow(xEventData.nRow, xDataList);
			if (bRet)
			{
				
			}
		}
		break;
		case RECORD_EVENT_DATA::Del:
		{
			
		}
		break;
		case RECORD_EVENT_DATA::Swap:
		{
		}
		break;
		case RECORD_EVENT_DATA::Create:
			break;
		case RECORD_EVENT_DATA::Update:
		{
		}
		break;
		case RECORD_EVENT_DATA::Cleared:
			break;
		case RECORD_EVENT_DATA::Sort:
			break;
		default:
			break;
	}
	//std::cout << "OnPropertyCallBackEvent Property: " << strProperty << " OldValue: " << oldVar.GetInt() << " NewValue: " << newVar.GetInt() << std::endl;

	return 0;
}

void NFCClientModule::LoginPB(int nID, string strAccount, string strPwd, string strKey)
{
	std::cout <<"LoginPB:" << strAccount<<std::endl;
	NFMsg::ReqAccountLogin xData;
	xData.set_account(strAccount);
	xData.set_password(strPwd);
	xData.set_security_code(strKey);
	xData.set_signbuff("");
	xData.set_clientversion(1);
	xData.set_loginmode(NFMsg::ELoginMode::ELM_LOGIN);
	xData.set_clientip(0);
	xData.set_clientmac(0);
	xData.set_device_info("");
	xData.set_extra_info("");

	m_pNetClientModule->SendToServerByPB(nID, NFMsg::EGameMsgID::EGMI_REQ_LOGIN, xData);
}

void NFCClientModule::Login(NFINet* pNet) {
	NF_SHARE_PTR<ConnectData> pServerData = m_pNetClientModule->GetServerNetInfo(pNet);
	if (pServerData)
	{
		int nTargetID = pServerData->nGameID;
		//LoginPB(nTargetID, "test1", "123456", "");
		LoginPB(nTargetID, mClientData.strAccount, mClientData.strPassword, "");
	}
}

void NFCClientModule::OnLoginProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen) 
{
	NFMsg::ReqServerList xData;
	xData.set_type(NFMsg::ReqServerListType::RSLT_WORLD_SERVER);

	m_pNetClientModule->SendToServerByPB(NF_SERVER_TYPES::NF_ST_LOGIN, NFMsg::EGameMsgID::EGMI_REQ_WORLD_LIST, xData);
}

void NFCClientModule::OnWorldList(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgID, msg, nLen, NFMsg::AckServerList)
	if (NFMsg::ReqServerListType::RSLT_WORLD_SERVER == xMsg.type()) {
		NFMsg::ServerInfo xInfo;// = xMsg.info()[0];
		for (int i = 0;i < xMsg.info().size();i++) {
			if (xMsg.info()[i].server_id() == mClientData.nWorldID) {
				xInfo = xMsg.info()[i];
				break;
			}
		}

		NFMsg::ReqConnectWorld xData;
		xData.set_world_id(xInfo.server_id());

		m_pNetClientModule->SendToServerByPB(NF_SERVER_TYPES::NF_ST_LOGIN, NFMsg::EGameMsgID::EGMI_REQ_CONNECT_WORLD, xData);
	}
	else if (NFMsg::ReqServerListType::RSLT_GAMES_ERVER == xMsg.type()) {
		NFMsg::ServerInfo xInfo;// = xMsg.info()[1];
		for (int i = 0;i < xMsg.info().size();i++) {
			if (xMsg.info()[i].server_id() == mClientData.nGameID) {
				xInfo = xMsg.info()[i];
				break;
			}
		}

		NFMsg::ReqConnectWorld xData;
		xData.set_world_id(xInfo.server_id());
		mServerID = xInfo.server_id();
		m_pNetClientModule->SendToServerByPB(NF_SERVER_TYPES::NF_ST_WORLD, NFMsg::EGameMsgID::EGMI_REQ_SELECT_SERVER, xData);
	}
}

void NFCClientModule::OnConnectWorld(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgID, msg, nLen, NFMsg::AckConnectWorldResult)

	mKey = xMsg.world_key();

	NF_SHARE_PTR<NFIClass> xLogicClass = m_pClassModule->GetElement(NFrame::Server::ThisName());
	if (xLogicClass)
	{
		const std::vector<std::string>& strIdList = xLogicClass->GetIDList();
		for (int i = 0; i < strIdList.size(); ++i)
		{
			const std::string& strId = strIdList[i];

			const int nServerType = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::Type());
			const int nServerID = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::ServerID());
			if (nServerID == xMsg.world_id())
			{
				const int nPort = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::Port());
				//const int nMaxConnect = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::MaxOnline());
				//const int nCpus = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::CpuCount());
				const std::string& strName = m_pElementModule->GetPropertyString(strId, NFrame::Server::Name());
				const std::string& strIP = m_pElementModule->GetPropertyString(strId, NFrame::Server::IP());

				ConnectData xServerData;

				xServerData.nGameID = nServerID;
				xServerData.eServerType = (NF_SERVER_TYPES)nServerType;
				xServerData.strIP = xMsg.world_ip();
				xServerData.nPort = xMsg.world_port();
				xServerData.strName = strName;

				m_pNetClientModule->AddServer(xServerData);

				break;
			}
		}
	}

	/*ConnectData xServerData;

	xServerData.nGameID = xMsg.world_id();
	xServerData.eServerType = (NF_SERVER_TYPES::NF_ST_PROXY);
	xServerData.strIP = xMsg.world_ip();
	xServerData.nPort = xMsg.world_port();
	xServerData.strName = "";

	m_pNetClientModule->AddServer(xServerData);*/
}

void NFCClientModule::OnConnectKey(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgID, msg, nLen, NFMsg::AckEventResult)

	if (xMsg.event_code() == NFMsg::EGEC_VERIFY_KEY_SUCCESS) {
		std::cout << "VerifyKey SUCCESS" << std::endl;

		NFMsg::ReqServerList xData;;
		xData.set_type(NFMsg::ReqServerListType::RSLT_GAMES_ERVER);

		m_pNetClientModule->SendToServerByPB(NF_SERVER_TYPES::NF_ST_WORLD, NFMsg::EGameMsgID::EGMI_REQ_WORLD_LIST, xData);
	}
}

void NFCClientModule::OnSelectServer(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgID, msg, nLen, NFMsg::AckEventResult)

	if (xMsg.event_code() == NFMsg::EGEC_SELECTSERVER_SUCCESS)
	{
		std::cout << "SelectGame SUCCESS" << std::endl;

		NFMsg::ReqRoleList xData;
		xData.set_game_id(mServerID);
		xData.set_account(mClientData.strAccount);

		m_pNetClientModule->SendToServerByPB(NF_SERVER_TYPES::NF_ST_WORLD, NFMsg::EGameMsgID::EGMI_REQ_ROLE_LIST, xData);
	}
}

void NFCClientModule::OnRoleList(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgID, msg, nLen, NFMsg::AckRoleLiteInfoList)

	std::cout << "QueryRoleList SUCCESS" << std::endl;

	if (xMsg.char_data().size() == 0) {
		//´´½¨role
		NFMsg::ReqCreateRole xData;
		xData.set_career(1);
		xData.set_sex(1);
		xData.set_noob_name(mClientData.strAccount +"_Role");
		xData.set_account(mClientData.strAccount);
		xData.set_race(0);
		xData.set_game_id(mServerID);

		m_pNetClientModule->SendToServerByPB(NF_SERVER_TYPES::NF_ST_WORLD, NFMsg::EGameMsgID::EGMI_REQ_CREATE_ROLE, xData);
	}
	else {

		NFMsg::RoleLiteInfo info = xMsg.char_data()[mClientData.nRoleID];

		NFMsg::ReqEnterGameServer xData;
		xData.set_name(info.noob_name());
		xData.set_account(mClientData.strAccount);
		xData.set_game_id(mServerID);
		xData.mutable_id()->CopyFrom(info.id());

		mRoleID = m_pNetModule->PBToNF(info.id());
		/*mNetModule.mOwnerID = mRoleID;*/

		NFDataList xDataList;
		xDataList.AddObject(m_pNetModule->PBToNF(info.id()));
		m_pEventModule->DoEvent(NFEventDefine::NFED_ON_GENERAL_MESSAGE, xDataList);

		m_pNetClientModule->SendToServerByPB(NF_SERVER_TYPES::NF_ST_WORLD, NFMsg::EGameMsgID::EGMI_REQ_ENTER_GAME, xData);
	}
}

void NFCClientModule::OnSocketLoginEvent(const NFSOCK nSockIndex, const NF_NET_EVENT eEvent, NFINet* pNet)
{
	if (eEvent & NF_NET_EVENT_EOF)
	{
		m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(0, nSockIndex), "NF_NET_EVENT_EOF", "Connection closed", __FUNCTION__, __LINE__);
	}
	else if (eEvent & NF_NET_EVENT_ERROR)
	{
		m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(0, nSockIndex), "NF_NET_EVENT_ERROR", "Got an error on the connection", __FUNCTION__, __LINE__);
	}
	else if (eEvent & NF_NET_EVENT_TIMEOUT)
	{
		m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(0, nSockIndex), "NF_NET_EVENT_TIMEOUT", "read timeout", __FUNCTION__, __LINE__);
	}
	else  if (eEvent & NF_NET_EVENT_CONNECTED)
	{
		m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(0, nSockIndex), "NF_NET_EVENT_CONNECTED", "connected success", __FUNCTION__, __LINE__);
		Login(pNet);
	}
}

void NFCClientModule::OnSocketWorldEvent(const NFSOCK nSockIndex, const NF_NET_EVENT eEvent, NFINet* pNet)
{
	if (eEvent & NF_NET_EVENT_EOF)
	{
		m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(0, nSockIndex), "NF_NET_EVENT_EOF", "Connection closed", __FUNCTION__, __LINE__);
	}
	else if (eEvent & NF_NET_EVENT_ERROR)
	{
		m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(0, nSockIndex), "NF_NET_EVENT_ERROR", "Got an error on the connection", __FUNCTION__, __LINE__);
	}
	else if (eEvent & NF_NET_EVENT_TIMEOUT)
	{
		m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(0, nSockIndex), "NF_NET_EVENT_TIMEOUT", "read timeout", __FUNCTION__, __LINE__);
	}
	else  if (eEvent & NF_NET_EVENT_CONNECTED)
	{
		m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(0, nSockIndex), "NF_NET_EVENT_CONNECTED", "connected success", __FUNCTION__, __LINE__);

		NFMsg::ReqAccountLogin xData;
		xData.set_account(mClientData.strAccount);
		xData.set_password(mClientData.strPassword);
		xData.set_security_code(mKey);
		xData.set_signbuff("");
		xData.set_clientversion(1);
		xData.set_loginmode(NFMsg::ELoginMode::ELM_LOGIN);
		xData.set_clientip(0);
		xData.set_clientmac(0);
		xData.set_device_info("");
		xData.set_extra_info("");

		m_pNetClientModule->SendToServerByPB(NF_SERVER_TYPES::NF_ST_WORLD, NFMsg::EGameMsgID::EGMI_REQ_CONNECT_KEY, xData);
	}
}

void NFCClientModule::OnWorldPropertyIntProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen) {
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgID, msg, nLen, NFMsg::ObjectPropertyInt)
	NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject(m_pNetModule->PBToNF(xMsg.player_id()));
	NF_SHARE_PTR<NFIPropertyManager> xPropertyManager = pObject->GetPropertyManager();

	for (int i = 0; i < xMsg.property_list_size(); i++)
	{
		const NFMsg::PropertyInt &xPropertyInt = xMsg.property_list().Get(i);
		NF_SHARE_PTR<NFIProperty> pProperty = xPropertyManager->GetElement(xPropertyInt.property_name());
		if (NULL==pProperty)
		{
			NFDataList varList;
			varList.AddInt(0);

			pProperty = xPropertyManager->AddProperty(m_pNetModule->PBToNF(xMsg.player_id()),xPropertyInt.property_name(), NFDATA_TYPE::TDATA_INT);
		}

		pProperty->SetInt(xPropertyInt.data());
	}
}

bool NFCClientModule::AfterInit()
{
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pScheduleModule = pPluginManager->FindModule<NFIScheduleModule>();
	//m_pSceneAOIModule = pPluginManager->FindModule<NFISceneAOIModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pClassModule = pPluginManager->FindModule<NFIClassModule>();
	m_pEventModule = pPluginManager->FindModule<NFIEventModule>();
	m_pNetModule = pPluginManager->FindModule<NFINetModule>();
	m_pNetClientModule = pPluginManager->FindModule<NFINetClientModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();

	m_pNetClientModule->AddReceiveCallBack(NF_SERVER_TYPES::NF_ST_LOGIN, NFMsg::EGMI_ACK_LOGIN, this, &NFCClientModule::OnLoginProcess);
	m_pNetClientModule->AddReceiveCallBack(NF_SERVER_TYPES::NF_ST_LOGIN, NFMsg::EGMI_ACK_WORLD_LIST, this, &NFCClientModule::OnWorldList);
	m_pNetClientModule->AddReceiveCallBack(NF_SERVER_TYPES::NF_ST_LOGIN, NFMsg::EGMI_ACK_CONNECT_WORLD, this, &NFCClientModule::OnConnectWorld);
	m_pNetClientModule->AddReceiveCallBack(NF_SERVER_TYPES::NF_ST_WORLD, NFMsg::EGMI_ACK_CONNECT_KEY, this, &NFCClientModule::OnConnectKey);
	m_pNetClientModule->AddReceiveCallBack(NF_SERVER_TYPES::NF_ST_WORLD, NFMsg::EGMI_ACK_WORLD_LIST, this, &NFCClientModule::OnWorldList);
	m_pNetClientModule->AddReceiveCallBack(NF_SERVER_TYPES::NF_ST_WORLD, NFMsg::EGMI_ACK_SELECT_SERVER, this, &NFCClientModule::OnSelectServer);
	m_pNetClientModule->AddReceiveCallBack(NF_SERVER_TYPES::NF_ST_WORLD, NFMsg::EGMI_ACK_ROLE_LIST, this, &NFCClientModule::OnRoleList);

	//m_pNetClientModule->AddReceiveCallBack(NF_SERVER_TYPES::NF_ST_WORLD, NFMsg::EGMI_ACK_PROPERTY_INT, this, &NFCClientModule::OnWorldPropertyIntProcess);

	m_pNetClientModule->AddEventCallBack(NF_SERVER_TYPES::NF_ST_LOGIN, this, &NFCClientModule::OnSocketLoginEvent);
	m_pNetClientModule->AddEventCallBack(NF_SERVER_TYPES::NF_ST_WORLD, this, &NFCClientModule::OnSocketWorldEvent);

	m_pNetClientModule->ExpandBufferSize();

	NF_SHARE_PTR<NFIClass> xLogicClass = m_pClassModule->GetElement(NFrame::Client::ThisName());
	if (xLogicClass)
	{
		const std::vector<std::string>& strIdList = xLogicClass->GetIDList();
		for (int i = 0; i < strIdList.size(); ++i)
		{
			const std::string& strId = strIdList[i];

			const int nServerID = m_pElementModule->GetPropertyInt32(strId, NFrame::Client::ClientServerID());
			if (pPluginManager->GetAppID() == nServerID)
			{
				mClientData.nClientID = nServerID;
				mClientData.nLoginID = m_pElementModule->GetPropertyInt32(strId, NFrame::Client::LoginServerID());
				mClientData.nWorldID = m_pElementModule->GetPropertyInt32(strId, NFrame::Client::WorldServerID());
				mClientData.nGameID = m_pElementModule->GetPropertyInt32(strId, NFrame::Client::GameServerID());
				mClientData.nRoleID = m_pElementModule->GetPropertyInt32(strId, NFrame::Client::RoleID());
				mClientData.strAccount = m_pElementModule->GetPropertyString(strId, NFrame::Client::Account());
				mClientData.strPassword = m_pElementModule->GetPropertyString(strId, NFrame::Client::Password());

				//const int nMaxConnect = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::MaxOnline());
				//const int nCpus = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::CpuCount());

				break;
			}
		}
	}

	xLogicClass = m_pClassModule->GetElement(NFrame::Server::ThisName());
	if (xLogicClass)
	{
		const std::vector<std::string>& strIdList = xLogicClass->GetIDList();
		for (int i = 0; i < strIdList.size(); ++i)
		{
			const std::string& strId = strIdList[i];

			const int nServerType = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::Type());
			const int nServerID = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::ServerID());
			if (nServerType == NF_SERVER_TYPES::NF_ST_LOGIN && nServerID== mClientData.nLoginID)
			{
				const int nPort = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::Port());
				//const int nMaxConnect = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::MaxOnline());
				//const int nCpus = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::CpuCount());
				const std::string& strName = m_pElementModule->GetPropertyString(strId, NFrame::Server::Name());
				const std::string& strIP = m_pElementModule->GetPropertyString(strId, NFrame::Server::IP());

				ConnectData xServerData;

				xServerData.nGameID = nServerID;
				xServerData.eServerType = (NF_SERVER_TYPES)nServerType;
				xServerData.strIP = strIP;
				xServerData.nPort = nPort;
				xServerData.strName = strName;

				m_pNetClientModule->AddServer(xServerData);
			}
		}
	}

    return true;
}

bool NFCClientModule::Execute()
{
    return true;
}

bool NFCClientModule::BeforeShut()
{
    
    std::cout << "Hello, world2, BeforeShut" << std::endl;

    return true;
}

bool NFCClientModule::Shut()
{
    
    std::cout << "Hello, world2, Shut" << std::endl;

    return true;
}