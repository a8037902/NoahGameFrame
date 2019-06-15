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


#ifndef NFC_PLAYER_MYSQL_MODULE_H
#define NFC_PLAYER_MYSQL_MODULE_H

#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"
#include "NFComm/NFCore/NFDateTime.hpp"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFIClassModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFIPlayerMysqlModule.h"
#include "NFComm/NFPluginModule/NFICommonMysqlModule.h"
#include "NFComm/NFPluginModule/NFINetModule.h"

class NFCPlayerMysqlModule : public NFIPlayerMysqlModule
{
public:

	NFCPlayerMysqlModule(NFIPluginManager* p);

	virtual bool Init();
	virtual bool Shut();
	virtual bool Execute();

	virtual bool AfterInit();


	virtual bool LoadPlayerData(const NFGUID& self, NFMsg::RoleDataPack& roleData);
	virtual bool SavePlayerData(const NFGUID& self, const NFMsg::RoleDataPack& roleData);


	virtual const NFGUID CreateRole(const std::string& strAccount, const std::string& strName, const int nRace, const int nJob, const int nSex);
	virtual const bool DeleteRole(const std::string& strAccount, const NFGUID xID);
	virtual const NFGUID GetChar(const std::string& strAccount, const std::vector<std::string>& xFieldVec, std::vector<std::string>& xValueVeec);

	virtual bool RegisterAutoSave(const std::string& strClassName);
	virtual const bool LoadDataFormSql(const NFGUID& self, const std::string& strClassName);
	virtual const bool SaveDataToSql(const NFGUID& self);

protected:

private:
	const bool LoadDataFormSqlAsy(const NFGUID& self, const std::string& strClassName, const LOADDATA_RETURN_FUNCTOR& xFun, const std::string& strUseData);

private:
	NFIClassModule* m_pLogicClassModule;
	NFICommonMysqlModule* m_pCommonMysqlModule;
	NFIKernelModule* m_pKernelModule;
	NFILogModule* m_pLogModule;
	//NFIMysqlModule* m_pMysqlModule;
};


#endif
