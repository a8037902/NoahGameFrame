/*
This file is part of:
NoahFrame
https://github.com/ketoo/NoahGameFrame

Copyright 2009 - 2018 NoahFrame(NoahGameFrame)

File creator: lvsheng.huang

NoahFrame is open-source software and you can Mysqltribute it and/or modify
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

#include "NFCPlayerMysqlModule.h"
#include "NFCCommonMysqlModule.h"

NFCPlayerMysqlModule::NFCPlayerMysqlModule(NFIPluginManager * p)
{
	pPluginManager = p;
}

bool NFCPlayerMysqlModule::Init()
{
	m_pLogicClassModule = pPluginManager->FindModule<NFIClassModule>();
	m_pCommonMysqlModule = pPluginManager->FindModule<NFICommonMysqlModule>();
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();
	//m_pMysqlModule = pPluginManager->FindModule<NFIMysqlModule>();

	return true;
}

bool NFCPlayerMysqlModule::Shut()
{
	return true;
}

bool NFCPlayerMysqlModule::Execute()
{
	return true;
}

bool NFCPlayerMysqlModule::AfterInit()
{

	return true;
}

bool NFCPlayerMysqlModule::LoadPlayerData(const NFGUID & self, NFMsg::RoleDataPack& roleData)
{
	NFCCommonMysqlModule* pCommonMysqlModule = (NFCCommonMysqlModule*)(m_pCommonMysqlModule);

	NF_SHARE_PTR<NFIPropertyManager> xPropertyManager = m_pCommonMysqlModule->GetPropertyInfo(self, NFrame::Player::ThisName());
	if (xPropertyManager)
	{
		*(roleData.mutable_property()->mutable_player_id()) = NFINetModule::NFToPB(self);

		pCommonMysqlModule->ConvertPropertyManagerToPB(xPropertyManager, roleData.mutable_property());

		pCommonMysqlModule->GetRecordInfo(self, NFrame::Player::ThisName(), roleData.mutable_record());

		return true;
	}



	/*
	if (xRecordManager)
	{
	NF_SHARE_PTR<NFIRecord> xRecord = xRecordManager->First();
	while (xRecord)
	{
	NFMsg::ObjectRecordBase* pRecord = roleData.mutable_record()->add_record_list();

	pCommonMysqlModule->ConvertRecordToPB(xRecord, pRecord);

	xRecord = xRecordManager->Next();
	}
	}
	*/
	m_pLogModule->LogNormal(NFILogModule::NF_LOG_LEVEL::NLL_ERROR_NORMAL, self, "loaded data false", NFGetTimeMS());

	return false;
}

bool NFCPlayerMysqlModule::SavePlayerData(const NFGUID & self, const NFMsg::RoleDataPack& roleData)
{
	NFCCommonMysqlModule* pCommonMysqlModule = (NFCCommonMysqlModule*)m_pCommonMysqlModule;

	std::string strClassName;
	for (int i = 0; i < roleData.property().property_string_list_size(); ++i)
	{
		const NFMsg::PropertyString& xData = roleData.property().property_string_list(i);

		if (xData.property_name()=="ClassName")
		{
			strClassName = xData.data();
			break;
		}
	}

	NF_SHARE_PTR<NFIPropertyManager> xPropManager = pCommonMysqlModule->NewPropertyManager(/*NFrame::Player::ThisName()*/strClassName);
	if (pCommonMysqlModule->ConvertPBToPropertyManager(roleData.property(), xPropManager))
	{
		m_pCommonMysqlModule->SavePropertyInfo(self, strClassName, xPropManager);
	}

	NF_SHARE_PTR<NFIRecordManager> xReManager = pCommonMysqlModule->NewRecordManager(/*NFrame::Player::ThisName()*/strClassName);
	if (pCommonMysqlModule->ConvertPBToRecordManager(roleData.record(), xReManager))
	{
		m_pCommonMysqlModule->SaveRecordInfo(self, strClassName, xReManager);
	}
	//pCommonMysqlModule->SaveRecordInfo(self, strClassName, roleData.record(), -1);

	return true;
}


const NFGUID NFCPlayerMysqlModule::CreateRole(const std::string& strAccount, const std::string& strName, const int nRace, const int nJob, const int nSex) {
	return NULL;
}
const bool NFCPlayerMysqlModule::DeleteRole(const std::string& strAccount, const NFGUID xID)
{
	return false;
}
const NFGUID NFCPlayerMysqlModule::GetChar(const std::string& strAccount, const std::vector<std::string>& xFieldVec, std::vector<std::string>& xValueVeec)
{
	return NULL;
}

bool NFCPlayerMysqlModule::RegisterAutoSave(const std::string& strClassName)
{
	return false;
}
const bool NFCPlayerMysqlModule::LoadDataFormSql(const NFGUID& self, const std::string& strClassName)
{
	return false;
}
const bool NFCPlayerMysqlModule::SaveDataToSql(const NFGUID& self)
{
	return false;
}

const bool NFCPlayerMysqlModule::LoadDataFormSqlAsy(const NFGUID& self, const std::string& strClassName, const LOADDATA_RETURN_FUNCTOR& xFun, const std::string& strUseData)
{
	return false;
}
