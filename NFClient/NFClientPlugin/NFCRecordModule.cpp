#include "NFCRecordModule.h"

bool NFCRecordModule::Init()
{

	return true;
}

void NFCRecordModule::OnRecordInt(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen) {

	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgID, msg, nLen, NFMsg::ObjectRecordInt)

	NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject(m_pNetModule->PBToNF(xMsg.player_id()));
	NF_SHARE_PTR<NFIRecordManager> recordManager = pObject->GetRecordManager();

	for (int i = 0; i < xMsg.property_list().size(); i++)
	{
		recordManager->SetRecordInt(xMsg.record_name(),xMsg.property_list()[i].row(), xMsg.property_list()[i].col(), (int)xMsg.property_list()[i].data());
	}
}

bool NFCRecordModule::AfterInit()
{
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pNetModule = pPluginManager->FindModule<NFINetModule>();
	m_pNetClientModule = pPluginManager->FindModule<NFINetClientModule>();

	m_pNetClientModule->AddReceiveCallBack(NF_SERVER_TYPES::NF_ST_WORLD, NFMsg::EGMI_ACK_RECORD_INT, this, &NFCRecordModule::OnRecordInt);

	return true;
}

bool NFCRecordModule::Execute()
{

	//std::cout << "Hello, world1, Execute" << std::endl;

	return true;
}

bool NFCRecordModule::BeforeShut()
{

	return true;
}

bool NFCRecordModule::Shut()
{

	return true;
}