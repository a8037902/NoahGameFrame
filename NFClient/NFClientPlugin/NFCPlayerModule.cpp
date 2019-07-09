#include "NFCPlayerModule.h"



bool NFCPlayerModule::Init()
{

	return true;
}

void NFCPlayerModule::OnObjectEntry(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen) {
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgID, msg, nLen, NFMsg::AckPlayerEntryList)

	for (int i = 0;i < xMsg.object_list().size();i++) {
		NFMsg::PlayerEntryInfo xInfo = xMsg.object_list()[i];

		//if (m_pNetModule->PBToNF(xInfo.object_guid()) == m_pClientModule->mRoleID) 
		{

			NFDataList var;
			var.AddString("X");
			var.AddFloat(xInfo.x());
			var.AddString("Y");
			var.AddFloat(xInfo.z());
			var.AddString("Z");
			var.AddFloat(xInfo.y());

			NF_SHARE_PTR<NFIObject> xGO = m_pKernelModule->CreateObject(m_pNetModule->PBToNF(xInfo.object_guid()), xInfo.scene_id(), 0, xInfo.class_id(), xInfo.config_id(), var);
			if (NULL == xGO)
			{
				std::cout << "  Type: " << xInfo.class_id() << " ID: " << xInfo.object_guid().Utf8DebugString() << "  ConfigID: " << xInfo.config_id() << std::endl;
				continue;
			}
		}
	}
}

void NFCPlayerModule::OnObjectLeave(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen) {
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgID, msg, nLen, NFMsg::AckPlayerLeaveList)

	for (int i = 0; i < xMsg.object_list().size(); ++i)
	{
		m_pKernelModule->DestroyObject(m_pNetModule->PBToNF(xMsg.object_list()[i]));
	}
}

void NFCPlayerModule::OnLoadDataFinish(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen) {
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgID, msg, nLen, NFMsg::AckPlayerEntryList)

	for (int i = 0; i < xMsg.object_list().size(); i++) {
		NFMsg::PlayerEntryInfo xInfo = xMsg.object_list()[i];

		m_pEventModule->DoEvent(m_pNetModule->PBToNF(xInfo.object_guid()), (NFEventDefine)(NFEventDefine::NFED_ON_GENERAL_MESSAGE+1),NFDataList());
	}
}

bool NFCPlayerModule::AfterInit()
{
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pNetModule = pPluginManager->FindModule<NFINetModule>();
	m_pNetClientModule = pPluginManager->FindModule<NFINetClientModule>();
	m_pClientModule= pPluginManager->FindModule<NFIClientModule>();
	m_pEventModule = pPluginManager->FindModule<NFIEventModule>();

	m_pNetClientModule->AddReceiveCallBack(NF_SERVER_TYPES::NF_ST_WORLD, NFMsg::EGMI_ACK_OBJECT_ENTRY, this, &NFCPlayerModule::OnObjectEntry);
	m_pNetClientModule->AddReceiveCallBack(NF_SERVER_TYPES::NF_ST_WORLD, NFMsg::EGMI_ACK_OBJECT_LEAVE, this, &NFCPlayerModule::OnObjectLeave);

	m_pNetClientModule->AddReceiveCallBack(NF_SERVER_TYPES::NF_ST_WORLD, NFMsg::EGMI_ACK_DATA_FINISHED, this, &NFCPlayerModule::OnLoadDataFinish);

	return true;
}

bool NFCPlayerModule::Execute()
{

	//std::cout << "Hello, world1, Execute" << std::endl;

	return true;
}

bool NFCPlayerModule::BeforeShut()
{

	return true;
}

bool NFCPlayerModule::Shut()
{

	return true;
}