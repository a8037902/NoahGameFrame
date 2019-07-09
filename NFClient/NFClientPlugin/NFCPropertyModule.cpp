#include "NFCPropertyModule.h"



bool NFCPropertyModule::Init()
{

	return true;
}

bool NFCPropertyModule::AfterInit()
{
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pNetModule = pPluginManager->FindModule<NFINetModule>();
	m_pNetClientModule = pPluginManager->FindModule<NFINetClientModule>();
	m_pClientModule= pPluginManager->FindModule<NFIClientModule>();
	m_pEventModule = pPluginManager->FindModule<NFIEventModule>();

	m_pNetClientModule->AddReceiveCallBack(NF_SERVER_TYPES::NF_ST_WORLD, NFMsg::EGMI_ACK_PROPERTY_INT, this, &NFCPropertyModule::OnWorldPropertyIntProcess);
	m_pNetClientModule->AddReceiveCallBack(NF_SERVER_TYPES::NF_ST_WORLD, NFMsg::EGMI_ACK_OBJECT_PROPERTY_ENTRY, this, &NFCPropertyModule::OnObjectPropertyEntry);

	return true;
}

bool NFCPropertyModule::Execute()
{

	//std::cout << "Hello, world1, Execute" << std::endl;

	return true;
}

bool NFCPropertyModule::BeforeShut()
{

	return true;
}

bool NFCPropertyModule::Shut()
{

	return true;
}

void NFCPropertyModule::OnWorldPropertyIntProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen) {
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgID, msg, nLen, NFMsg::ObjectPropertyInt)
		NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject(m_pNetModule->PBToNF(xMsg.player_id()));
	NF_SHARE_PTR<NFIPropertyManager> xPropertyManager = pObject->GetPropertyManager();

	for (int i = 0; i < xMsg.property_list_size(); i++)
	{
		const NFMsg::PropertyInt &xPropertyInt = xMsg.property_list().Get(i);
		NF_SHARE_PTR<NFIProperty> pProperty = xPropertyManager->GetElement(xPropertyInt.property_name());
		if (NULL == pProperty)
		{
			NFDataList varList;
			varList.AddInt(0);

			pProperty = xPropertyManager->AddProperty(m_pNetModule->PBToNF(xMsg.player_id()), xPropertyInt.property_name(), NFDATA_TYPE::TDATA_INT);
		}

		pProperty->SetInt(xPropertyInt.data());
	}
}

void NFCPropertyModule::OnObjectPropertyEntry(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen) {
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgID, msg, nLen, NFMsg::MultiObjectPropertyList)

	for (int i = 0; i < xMsg.multi_player_property_size(); i++) {
		NFMsg::ObjectPropertyList xPropertyData = xMsg.multi_player_property()[i];
		NF_SHARE_PTR<NFIObject> go = m_pKernelModule->GetObject(m_pNetModule->PBToNF(xPropertyData.player_id()));

		NF_SHARE_PTR<NFIPropertyManager> xPropertyManager = go->GetPropertyManager();

		for (int j = 0; j < xPropertyData.property_int_list_size(); j++)
		{
			string strPropertyName = xPropertyData.property_int_list()[j].property_name();
			NF_SHARE_PTR<NFIProperty> xProperty = xPropertyManager->GetElement(strPropertyName);
			if (nullptr == xProperty)
			{
				NFDataList varList;
				varList.AddInt(0);

				xProperty = xPropertyManager->AddProperty(m_pNetModule->PBToNF(xPropertyData.player_id()),strPropertyName, TDATA_INT);
			}

			xProperty->SetInt(xPropertyData.property_int_list()[j].data());
		}

		for (int j = 0; j < xPropertyData.property_float_list_size(); j++)
		{
			string strPropertyName = xPropertyData.property_float_list()[j].property_name();
			NF_SHARE_PTR<NFIProperty> xProperty = xPropertyManager->GetElement(strPropertyName);
			if (nullptr == xProperty)
			{
				NFDataList varList;
				varList.AddFloat(0);

				xProperty = xPropertyManager->AddProperty(m_pNetModule->PBToNF(xPropertyData.player_id()), strPropertyName, TDATA_FLOAT);
			}

			xProperty->SetFloat(xPropertyData.property_float_list()[j].data());
		}

		for (int j = 0; j < xPropertyData.property_string_list_size(); j++)
		{
			string strPropertyName = xPropertyData.property_string_list()[j].property_name();
			NF_SHARE_PTR<NFIProperty> xProperty = xPropertyManager->GetElement(strPropertyName);
			if (nullptr == xProperty)
			{
				NFDataList varList;
				varList.AddString("");

				xProperty = xPropertyManager->AddProperty(m_pNetModule->PBToNF(xPropertyData.player_id()), strPropertyName, TDATA_STRING);
			}

			xProperty->SetString(xPropertyData.property_string_list()[j].data());
		}

		for (int j = 0; j < xPropertyData.property_object_list_size(); j++)
		{
			string strPropertyName = xPropertyData.property_object_list()[j].property_name();
			NF_SHARE_PTR<NFIProperty> xProperty = xPropertyManager->GetElement(strPropertyName);
			if (nullptr == xProperty)
			{
				NFDataList varList;
				varList.AddObject(NFGUID());

				xProperty = xPropertyManager->AddProperty(m_pNetModule->PBToNF(xPropertyData.player_id()), strPropertyName, TDATA_OBJECT);
			}

			xProperty->SetObject(m_pNetModule->PBToNF(xPropertyData.property_object_list()[j].data()));
		}
	}
}