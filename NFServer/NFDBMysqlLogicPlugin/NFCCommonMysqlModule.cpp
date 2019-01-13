/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2018 NoahFrame(NoahGameFrame)

   File creator: chuanbo.guo
   
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

#include "NFCCommonMysqlModule.h"
#include "NFComm/NFCore/NFCPropertyManager.h"
#include "NFComm/NFCore/NFCRecordManager.h"
#include "NFComm/NFPluginModule/NFINetModule.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"


NFCCommonMysqlModule::NFCCommonMysqlModule(NFIPluginManager * p)
{
	pPluginManager = p;
}

std::string NFCCommonMysqlModule::GetPropertyCacheKey(const NFGUID& self)
{
    return self.ToString()/* + "_ObjectProperty"*/;
}

std::string NFCCommonMysqlModule::GetRecordCacheKey(const NFGUID& self)
{
    return self.ToString()/* + "_ObjectRecord"*/;
}

std::string NFCCommonMysqlModule::GetAccountCacheKey(const std::string & strAccount)
{
	return strAccount + "_AccountInfo";
}

std::string NFCCommonMysqlModule::GetTileCacheKey(const int & nSceneID)
{
	return lexical_cast<std::string>(nSceneID) + "_TileInfo";
}

bool NFCCommonMysqlModule::AfterInit()
{
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pLogicClassModule = pPluginManager->FindModule<NFIClassModule>();
    //m_pNoSqlModule = pPluginManager->FindModule<NFINoSqlModule>();
	m_pMysqlModule = pPluginManager->FindModule<NFIMysqlModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();

	NF_SHARE_PTR<NFIPropertyManager> pPropertyManager = NewPropertyManager(NFrame::IObject::ThisName());
	if (pPropertyManager)
	{
		NF_SHARE_PTR<NFIProperty> xProperty = pPropertyManager->First();
		while (xProperty)
		{
			if (xProperty->GetCache() || xProperty->GetSave())
			{
				mIObjectKeyMap[xProperty->GetKey()]=true;
			}

			xProperty = pPropertyManager->Next();
		}
	}

    return true;
}

NF_SHARE_PTR<NFIPropertyManager> NFCCommonMysqlModule::NewPropertyManager(const std::string& strClassName)
{
    NF_SHARE_PTR<NFIPropertyManager> pStaticClassPropertyManager = m_pLogicClassModule->GetClassPropertyManager(strClassName);
    if (pStaticClassPropertyManager)
    {
        NFGUID ident;
        NF_SHARE_PTR<NFIPropertyManager> pPropertyManager(NF_NEW NFCPropertyManager(ident));

        NF_SHARE_PTR<NFIProperty> pStaticConfigPropertyInfo = pStaticClassPropertyManager->First();
        while (pStaticConfigPropertyInfo)
        {
			if (pStaticConfigPropertyInfo->GetSave() || pStaticConfigPropertyInfo->GetCache())
			{
				NF_SHARE_PTR<NFIProperty> xProperty = pPropertyManager->AddProperty(ident, pStaticConfigPropertyInfo->GetKey(), pStaticConfigPropertyInfo->GetType());

				xProperty->SetPublic(pStaticConfigPropertyInfo->GetPublic());
				xProperty->SetPrivate(pStaticConfigPropertyInfo->GetPrivate());
				xProperty->SetSave(pStaticConfigPropertyInfo->GetSave());
				xProperty->SetCache(pStaticConfigPropertyInfo->GetCache());
				xProperty->SetRef(pStaticConfigPropertyInfo->GetRef());
			}

            pStaticConfigPropertyInfo = pStaticClassPropertyManager->Next();
        }

        return pPropertyManager;
    }

    return NF_SHARE_PTR<NFIPropertyManager>(NULL);
}

NF_SHARE_PTR<NFIRecordManager> NFCCommonMysqlModule::NewRecordManager(const std::string& strClassName)
{
    NF_SHARE_PTR<NFIRecordManager> pStaticClassRecordManager = m_pLogicClassModule->GetClassRecordManager(strClassName);
    if (pStaticClassRecordManager)
    {
        NFGUID ident;
        NF_SHARE_PTR<NFIRecordManager> pRecordManager(NF_NEW NFCRecordManager(ident));

        NF_SHARE_PTR<NFIRecord> pConfigRecordInfo = pStaticClassRecordManager->First();
        while (pConfigRecordInfo)
        {
			if (pConfigRecordInfo->GetSave() || pConfigRecordInfo->GetCache())
			{
				NF_SHARE_PTR<NFIRecord> xRecord = pRecordManager->AddRecord(ident,
					pConfigRecordInfo->GetName(),
					pConfigRecordInfo->GetInitData(),
					pConfigRecordInfo->GetTag(),
					pConfigRecordInfo->GetRows());

				xRecord->SetPublic(pConfigRecordInfo->GetPublic());
				xRecord->SetPrivate(pConfigRecordInfo->GetPrivate());
				xRecord->SetSave(pConfigRecordInfo->GetSave());
				xRecord->SetCache(pConfigRecordInfo->GetCache());

			}

            pConfigRecordInfo = pStaticClassRecordManager->Next();
        }

        return pRecordManager;
    }

    return NF_SHARE_PTR<NFIRecordManager>(NULL);
}

NF_SHARE_PTR<NFIPropertyManager> NFCCommonMysqlModule::GetPropertyInfo(const NFGUID& self, const std::string& strClassName, std::vector<std::string>& vKeyCacheList, std::vector<std::string>& vValueCacheList)
{
	//TODO optimize
    NF_SHARE_PTR<NFIPropertyManager> pPropertyManager = NewPropertyManager(strClassName);
    if (!pPropertyManager)
    {
        return nullptr;
    }

	/*NF_SHARE_PTR<NFIMysqlClient> pDriver = m_pMysqlModule->GetDriverBySuit(self.ToString());
    if (!pDriver)
    {
        return nullptr;
    }*/

	//TODO
	//just run this function one time
	NF_SHARE_PTR<NFIProperty> xProperty = pPropertyManager->First();
	while (xProperty)
	{
		if (xProperty->GetCache() || xProperty->GetSave())
		{
			if (!mIObjectKeyMap[xProperty->GetKey()]) {
				vKeyCacheList.push_back(xProperty->GetKey());
			}
		}

		xProperty = pPropertyManager->Next();
	}

	//cache
	std::string strCacheKey = GetPropertyCacheKey(self);
	if (!m_pMysqlModule->Query(strClassName, strCacheKey, vKeyCacheList, vValueCacheList)) 
	{
		return nullptr;
	}
    /*if (!pDriver->HMGET(strCacheKey, vKeyCacheList, vValueCacheList))
    {
        return nullptr;
    }*/

	if (vKeyCacheList.size() == vValueCacheList.size())
	{
		ConvertVectorToPropertyManager(vKeyCacheList, vValueCacheList, pPropertyManager);

		return pPropertyManager;
	}

	return nullptr;
}

NF_SHARE_PTR<NFIRecordManager> NFCCommonMysqlModule::GetRecordInfo(const NFGUID& self, const std::string& strClassName, std::vector<std::string>& vKeyCacheList, std::vector<std::string>& vValueCacheList)
{
    NF_SHARE_PTR<NFIRecordManager> pRecordManager = NewRecordManager(strClassName);
    if (!pRecordManager.get())
    {
        return nullptr;
    }

	/*NF_SHARE_PTR<NFIMysqlClient> pDriver = m_pNoSqlModule->GetDriverBySuit(self.ToString());
    if (!pDriver)
    {
        return nullptr;
    }*/

	
	//TODO
	//just run this function one time
	NF_SHARE_PTR<NFIRecord> xRecord = pRecordManager->First();
	while (xRecord)
	{
		if (xRecord->GetCache() || xRecord->GetSave())
		{
			vKeyCacheList.clear();
			//vKeyCacheList.push_back(xRecord->GetName());
			//根据不同的record，从不同的表格中读取数据
			for (int iCol = 0; iCol < xRecord->GetCols();iCol++) {
				vKeyCacheList.push_back(xRecord->GetColTag(iCol));
			}
			for (int iRow = 0; iRow < xRecord->GetRows(); iRow++) {
				if (m_pMysqlModule->Query(strClassName+xRecord->GetName(), self.ToString()+"-"+std::to_string(iRow), vKeyCacheList, vValueCacheList))
				{
					//这一行数据存在
					xRecord->SetUsed(iRow, true);
					xRecord->PreAllocMemoryForRow(iRow);
					for (int iCol = 0; iCol < xRecord->GetCols();iCol++) {
						const int nType = xRecord->GetColType(iCol);
						switch (nType)
						{
						case TDATA_INT:
						{
							xRecord->SetInt(iRow, iCol, _atoi64(vValueCacheList[iCol].c_str()));
						}
						break;
						case TDATA_FLOAT:
						{
							xRecord->SetFloat(iRow, iCol, atof(vValueCacheList[iCol].c_str()));
						}
						break;
						case TDATA_STRING:
						{
							xRecord->SetString(iRow, iCol, vValueCacheList[iCol]);
						}
						break;
						case TDATA_OBJECT:
						{
							xRecord->SetObject(iRow, iCol, NFGUID(vValueCacheList[iCol]));
						}
						break;
						case TDATA_VECTOR2:
						{
							xRecord->SetVector2(iRow, iCol, NFVector2(vValueCacheList[iCol]));
						}
						break;
						case TDATA_VECTOR3:
						{
							xRecord->SetVector3(iRow, iCol, NFVector3(vValueCacheList[iCol]));
						}
						break;
						default:
							break;
						}
					}
				}
			}
		}

		xRecord = pRecordManager->Next();
	}

	//cache
	/*std::string strCacheKey = GetRecordCacheKey(self);
	if (!m_pMysqlModule->Query(strClassName, strCacheKey, vKeyCacheList, vValueCacheList))
	{
		return nullptr;
	}*/
	/*if (!pDriver->HMGET(strCacheKey, vKeyCacheList, vValueCacheList))
	{
		return nullptr;
	}*/

	/*if (vKeyCacheList.size() == vValueCacheList.size())
	{
		ConvertVectorToRecordManager(vKeyCacheList, vValueCacheList, pRecordManager);
		return pRecordManager;
	}*/

	return pRecordManager;
}

NF_SHARE_PTR<NFIPropertyManager> NFCCommonMysqlModule::GetPropertyInfo(const NFGUID & self, const std::string & strClassName)
{
	std::vector<std::string> vKeyCacheList;
	std::vector<std::string> vValueCacheList;

	return GetPropertyInfo(self, strClassName, vKeyCacheList, vValueCacheList);
}

NF_SHARE_PTR<NFIRecordManager> NFCCommonMysqlModule::GetRecordInfo(const NFGUID & self, const std::string & strClassName)
{
	std::vector<std::string> vKeyCacheList;
	std::vector<std::string> vValueCacheList;

	return GetRecordInfo(self, strClassName, vKeyCacheList, vValueCacheList);
}

bool NFCCommonMysqlModule::GetRecordInfo(const NFGUID & self, const std::string & strClassName, NFMsg::ObjectRecordList * pRecordDataList)
{
	*(pRecordDataList->mutable_player_id()) = NFINetModule::NFToPB(self);

	std::vector<std::string> vKeyCacheList;
	std::vector<std::string> vValueCacheList;
	NF_SHARE_PTR<NFIRecordManager> xRecordManager = GetRecordInfo(self, strClassName, vKeyCacheList, vValueCacheList);
	if (xRecordManager /*&& vKeyCacheList.size() == vValueCacheList.size()*/)
	{
		if (ConvertRecordManagerToPB(xRecordManager, pRecordDataList))
		{
			return true;
		}
	}

	return false;
}

bool NFCCommonMysqlModule::SavePropertyInfo(const NFGUID& self, const std::string& strClassName, NF_SHARE_PTR<NFIPropertyManager> pPropertyManager, const int nExpireSecond)
{
    if (self.IsNull())
    {
        return false;
    }

    if (!pPropertyManager)
    {
        return false;
    }

	/*NF_SHARE_PTR<NFIMysqlClient> pDriver = m_pNoSqlModule->GetDriverBySuit(self.ToString());
    if (!pDriver)
    {
        return false;
    }*/

	std::vector<std::string> vKeyList;
	std::vector<std::string> vValueList;
    if (!ConvertPropertyManagerToVector(pPropertyManager, vKeyList, vValueList))
    {
        return false;
    }

	if (vKeyList.size() != vValueList.size())
	{
		return false;
	}

	std::string strKey= GetPropertyCacheKey(self);
	if (!m_pMysqlModule->Updata(strClassName, strKey, vKeyList, vValueList))
	{
		return false;
	}
    /*if (!pDriver->HMSET(strKey, vKeyList, vValueList))
    {
        return false;
    }*/

	if (nExpireSecond > 0)
	{
		//pDriver->EXPIRE(strKey, nExpireSecond);
	}

    return true;
}

bool NFCCommonMysqlModule::SaveRecordInfo(const NFGUID& self, const std::string& strClassName, NF_SHARE_PTR<NFIRecordManager> pRecordManager, const int nExpireSecond)
{
    if (self.IsNull())
    {
        return false;
    }

    if (!pRecordManager)
    {
        return false;
    }

	std::vector<std::string> vKeyList;
	std::vector<std::string> vValueList;
	for (NF_SHARE_PTR<NFIRecord> pRecord = pRecordManager->First(); pRecord != NULL; pRecord = pRecordManager->Next())
	{
		if (!pRecord->GetCache() && !pRecord->GetSave())
		{
			continue;
		}
		

		//不同的record存储到不同的表
		for (int iRow = 0; iRow < pRecord->GetRows(); iRow++)
		{
			if (!pRecord->IsUsed(iRow))
			{
				m_pMysqlModule->Delete(strClassName + pRecord->GetName(), self.ToString() + "-" + std::to_string(iRow));
				continue;
			}

			vKeyList.push_back("Row");
			vValueList.push_back(std::to_string(iRow));

			//需要存储的一条数据
			for (int iCol = 0; iCol < pRecord->GetCols();iCol++) {
				vKeyList.push_back(pRecord->GetColTag(iCol));

				const int nType = pRecord->GetColType(iCol);
				switch (nType)
				{
				case TDATA_INT:
				{
					vValueList.push_back(std::to_string(pRecord->GetInt(iRow, iCol)));
				}
				break;
				case TDATA_FLOAT:
				{
					vValueList.push_back(std::to_string(pRecord->GetFloat(iRow, iCol)));
				}
				break;
				case TDATA_STRING:
				{
					vValueList.push_back(pRecord->GetString(iRow, iCol));
				}
				break;
				case TDATA_OBJECT:
				{
					vValueList.push_back(pRecord->GetObject(iRow, iCol).ToString());
				}
				break;
				case TDATA_VECTOR2:
				{
					vValueList.push_back(pRecord->GetVector2(iRow, iCol).ToString());
				}
				break;
				case TDATA_VECTOR3:
				{
					vValueList.push_back(pRecord->GetVector3(iRow, iCol).ToString());
				}
				break;
				default:
					break;
				}
			}

			//生成外键
			vKeyList.push_back(strClassName+"ID");
			vValueList.push_back(self.ToString());
			if (!m_pMysqlModule->Updata(strClassName+pRecord->GetName(), self.ToString()+"-"+std::to_string(iRow), vKeyList, vValueList))
			{
				return false;
			}
			vKeyList.clear();
			vValueList.clear();

		}
	}

	/*NFMsg::ObjectRecordList xRecordList;
	if (ConvertRecordManagerToPB(pRecordManager, &xRecordList))
	{
		return SaveRecordInfo(self, strClassName, xRecordList, nExpireSecond);
	}*/

    return true;
}

bool NFCCommonMysqlModule::SaveRecordInfo(const NFGUID & self, const std::string& strClassName, const NFMsg::ObjectRecordList& xRecordData, const int nExpireSecond)
{
	if (self.IsNull())
	{
		return false;
	}

	/*NF_SHARE_PTR<NFIMysqlClient> pDriver = m_pNoSqlModule->GetDriverBySuit(self.ToString());
	if (!pDriver)
	{
		return false;
	}*/

	std::vector<std::string> vKeyList;
	std::vector<std::string> vValueList;
	
	for (int i = 0; i < xRecordData.record_list_size(); ++i)
	{
		const NFMsg::ObjectRecordBase& xRecord = xRecordData.record_list(i);

		//不同的record存到不同的表格
		/*for (int iStuct = 0; iStuct < xRecord.row_struct_size(); iStuct++)
		{
			const NFMsg::RecordAddRowStruct& xAddRowStruct = xRecord.row_struct(iStuct);

			for (int i = 0; i < xAddRowStruct.record_int_list_size(); i++)
			{
				const NFMsg::RecordInt& xPropertyData = xAddRowStruct.record_int_list(i);
				const int nRow = xPropertyData.row();
				const int nCol = xPropertyData.col();
				vKeyList.push_back(xPropertyData.GetTypeName());
				vValueList.push_back(std::to_string(xPropertyData.data()));
			}
		}*/

		std::string strValue;
		if (!xRecord.SerializeToString(&strValue))
		{
			continue;
		}

		vKeyList.push_back(xRecord.record_name());
		vValueList.push_back(strValue);
	}


	if (vKeyList.size() != vValueList.size())
	{
		return false;
	}

	std::string strKey = GetRecordCacheKey(self);
	if (!m_pMysqlModule->Updata(strClassName, strKey, vKeyList, vValueList))
	{
		return false;
	}
	/*if (!pDriver->HMSET(strKey, vKeyList, vValueList))
	{
		return false;
	}*/

	if (nExpireSecond > 0)
	{
		//pDriver->EXPIRE(strKey, nExpireSecond);
	}

	return true;
}

bool NFCCommonMysqlModule::ConvertVectorToPropertyManager(std::vector<std::string>& vKeyList, std::vector<std::string>& vValueList, NF_SHARE_PTR<NFIPropertyManager> pPropertyManager)
{
	if (vKeyList.size() == vValueList.size())
	{
		for (int i = 0; i < vKeyList.size(); ++i)
		{
			const std::string& strKey = vKeyList[i];
			const std::string& strValue = vValueList[i];
			if (strValue.empty())
			{
				continue;
			}

			NF_SHARE_PTR<NFIProperty> pProperty = pPropertyManager->GetElement(strKey);
			if (!pProperty->GetCache() && !pProperty->GetSave())
			{
				continue;
			}

			if(!pProperty->FromString(strValue))
			{
				//TODO
				//error
			}
		}
	}

    return true;
}

bool NFCCommonMysqlModule::ConvertVectorToRecordManager(std::vector<std::string>& vKeyList, std::vector<std::string>& vValueList, NF_SHARE_PTR<NFIRecordManager> pRecordManager)
{
	if (vKeyList.size() == vValueList.size())
	{
		for (int i = 0; i < vKeyList.size(); ++i)
		{
			const std::string& strKey = vKeyList[i];
			const std::string& strValue = vValueList[i];
			if (strValue.empty())
			{
				continue;
			}

			NF_SHARE_PTR<NFIRecord> pRecord = pRecordManager->GetElement(strKey);
			if (!pRecord->GetCache() && !pRecord->GetSave())
			{
				continue;
			}

			NFMsg::ObjectRecordBase xRecordData;
			if (xRecordData.ParseFromString(strValue))
			{
				ConvertPBToRecord(xRecordData, pRecord);
			}
			else
			{
				//TODO
				//error
			}
		}
	}

    return true;
}

bool NFCCommonMysqlModule::ConvertPropertyManagerToVector(NF_SHARE_PTR<NFIPropertyManager> pPropertyManager, std::vector<std::string>& vKeyList, std::vector<std::string>& vValueList)
{
	for (NF_SHARE_PTR<NFIProperty> pProperty = pPropertyManager->First(); pProperty != NULL; pProperty = pPropertyManager->Next())
	{
		const int nType = pProperty->GetType();
		if (!pProperty->GetCache() && !pProperty->GetSave())
		{
			continue;
		}

		const std::string& strPropertyName = pProperty->GetKey();
		const std::string& strPropertyValue = pProperty->ToString();

		if (!mIObjectKeyMap[pProperty->GetKey()]) {
			vKeyList.push_back(strPropertyName);
			vValueList.push_back(strPropertyValue);
		}
	}

	return true;
}

bool NFCCommonMysqlModule::ConvertRecordManagerToVector(NF_SHARE_PTR<NFIRecordManager> pRecordManager, std::vector<std::string>& vKeyList, std::vector<std::string>& vValueList)
{
	for (NF_SHARE_PTR<NFIRecord> pRecord = pRecordManager->First(); pRecord != NULL; pRecord = pRecordManager->Next())
	{
		if (!pRecord->GetCache() && !pRecord->GetSave())
		{
			continue;
		}

		NFMsg::ObjectRecordBase xRecordData;

		ConvertRecordToPB(pRecord, &xRecordData);

		////
		std::string strValue;
		if (!xRecordData.SerializeToString(&strValue))
		{
			continue;
		}

		vKeyList.push_back(xRecordData.record_name());
		vValueList.push_back(strValue);
	}

	return true;
}

bool NFCCommonMysqlModule::ConvertRecordToPB(const NF_SHARE_PTR<NFIRecord>& pRecord, NFMsg::ObjectRecordBase * pRecordData)
{
	pRecordData->set_record_name(pRecord->GetName());

	for (int iRow = 0; iRow < pRecord->GetRows(); iRow++)
	{
		if (!pRecord->IsUsed(iRow))
		{
			continue;
		}

		NFMsg::RecordAddRowStruct* pRowData = pRecordData->add_row_struct();
		if (!pRowData)
		{
			continue;
		}

		pRowData->set_row(iRow);

		for (int iCol = 0; iCol < pRecord->GetCols(); iCol++)
		{
			const int nType = pRecord->GetColType(iCol);
			switch (nType)
			{
			case TDATA_INT:
			{
				NFMsg::RecordInt* pPropertyData = pRowData->add_record_int_list();
				const NFINT64 xPropertyValue = pRecord->GetInt(iRow, iCol);

				if (pPropertyData)
				{
					pPropertyData->set_col(iCol);
					pPropertyData->set_row(iRow);
					pPropertyData->set_data(xPropertyValue);
				}
			}
			break;
			case TDATA_FLOAT:
			{
				NFMsg::RecordFloat* pPropertyData = pRowData->add_record_float_list();
				const double xPropertyValue = pRecord->GetFloat(iRow, iCol);

				if (pPropertyData)
				{
					pPropertyData->set_col(iCol);
					pPropertyData->set_row(iRow);
					pPropertyData->set_data(xPropertyValue);
				}
			}
			break;
			case TDATA_STRING:
			{
				NFMsg::RecordString* pPropertyData = pRowData->add_record_string_list();
				const std::string& xPropertyValue = pRecord->GetString(iRow, iCol);

				if (pPropertyData)
				{
					pPropertyData->set_col(iCol);
					pPropertyData->set_row(iRow);
					pPropertyData->set_data(xPropertyValue);

				}
			}
			break;
			case TDATA_OBJECT:
			{
				NFMsg::RecordObject* pPropertyData = pRowData->add_record_object_list();
				const NFGUID xPropertyValue = pRecord->GetObject(iRow, iCol);

				if (pPropertyData)
				{
					pPropertyData->set_col(iCol);
					pPropertyData->set_row(iRow);
					*pPropertyData->mutable_data() = NFINetModule::NFToPB(xPropertyValue);
				}
			}
			break;
			case TDATA_VECTOR2:
			{
				NFMsg::RecordVector2* pPropertyData = pRowData->add_record_vector2_list();
				const NFVector2 xPropertyValue = pRecord->GetVector2(iRow, iCol);

				if (pPropertyData)
				{
					pPropertyData->set_col(iCol);
					pPropertyData->set_row(iRow);

					NFMsg::Vector2* pVec = pPropertyData->mutable_data();
					pVec->set_x(xPropertyValue.X());
					pVec->set_y(xPropertyValue.Y());
				}
			}
			break;
			case TDATA_VECTOR3:
			{
				NFMsg::RecordVector3* pPropertyData = pRowData->add_record_vector3_list();
				const NFVector3 xPropertyValue = pRecord->GetVector3(iRow, iCol);

				if (pPropertyData)
				{
					pPropertyData->set_col(iCol);
					pPropertyData->set_row(iRow);

					NFMsg::Vector3* pVec = pPropertyData->mutable_data();
					pVec->set_x(xPropertyValue.X());
					pVec->set_y(xPropertyValue.Y());
					pVec->set_z(xPropertyValue.Z());
				}
			}
			break;
			default:
				break;
			}
		}
	}

	return true;
}

bool NFCCommonMysqlModule::ConvertPBToRecord(const NFMsg::ObjectRecordBase& pRecordData, NF_SHARE_PTR<NFIRecord> pRecord)
{
	for (int iStuct = 0; iStuct < pRecordData.row_struct_size(); iStuct++)
	{
		const NFMsg::RecordAddRowStruct& xAddRowStruct = pRecordData.row_struct(iStuct);

		const int nCommonRow = xAddRowStruct.row();
		pRecord->SetUsed(nCommonRow, true);
		pRecord->PreAllocMemoryForRow(nCommonRow);

		for (int i = 0; i < xAddRowStruct.record_int_list_size(); i++)
		{
			const NFMsg::RecordInt& xPropertyData = xAddRowStruct.record_int_list(i);
			const int nRow = xPropertyData.row();
			const int nCol = xPropertyData.col();
			const NFINT64 xPropertyValue = xPropertyData.data();

			pRecord->SetInt(nRow, nCol, xPropertyValue);
		}

		for (int i = 0; i < xAddRowStruct.record_float_list_size(); i++)
		{
			const NFMsg::RecordFloat& xPropertyData = xAddRowStruct.record_float_list(i);
			const int nRow = xPropertyData.row();
			const int nCol = xPropertyData.col();
			const float xPropertyValue = xPropertyData.data();

			pRecord->SetFloat(nRow, nCol, xPropertyValue);
		}

		for (int i = 0; i < xAddRowStruct.record_string_list_size(); i++)
		{
			const NFMsg::RecordString& xPropertyData = xAddRowStruct.record_string_list(i);
			const int nRow = xPropertyData.row();
			const int nCol = xPropertyData.col();
			const std::string& xPropertyValue = xPropertyData.data();

			pRecord->SetString(nRow, nCol, xPropertyValue.c_str());
		}

		for (int i = 0; i < xAddRowStruct.record_object_list_size(); i++)
		{
			const NFMsg::RecordObject& xPropertyData = xAddRowStruct.record_object_list(i);
			const int nRow = xPropertyData.row();
			const int nCol = xPropertyData.col();
			const NFGUID xPropertyValue = NFINetModule::PBToNF(xPropertyData.data());

			pRecord->SetObject(nRow, nCol, xPropertyValue);
		}

		for (int i = 0; i < xAddRowStruct.record_vector2_list_size(); i++)
		{
			const NFMsg::RecordVector2& xPropertyData = xAddRowStruct.record_vector2_list(i);
			const int nRow = xPropertyData.row();
			const int nCol = xPropertyData.col();
			const NFVector2 v = NFINetModule::PBToNF(xPropertyData.data());

			pRecord->SetVector2(nRow, nCol, v);
		}

		for (int i = 0; i < xAddRowStruct.record_vector3_list_size(); i++)
		{
			const NFMsg::RecordVector3& xPropertyData = xAddRowStruct.record_vector3_list(i);
			const int nRow = xPropertyData.row();
			const int nCol = xPropertyData.col();
			const NFVector3 v = NFINetModule::PBToNF(xPropertyData.data());

			pRecord->SetVector3(nRow, nCol, v);
		}
	}

	return false;
}

bool NFCCommonMysqlModule::ConvertRecordManagerToPB(const NF_SHARE_PTR<NFIRecordManager>& pRecordManager, NFMsg::ObjectRecordList * pRecordDataList)
{
	if (pRecordDataList == nullptr )
	{
		return false;
	}

	for (NF_SHARE_PTR<NFIRecord> pRecord = pRecordManager->First(); pRecord != NULL; pRecord = pRecordManager->Next())
	{
		if (!pRecord->GetCache() && !pRecord->GetSave())
		{
			continue;
		}

		NFMsg::ObjectRecordBase* pRecordData = pRecordDataList->add_record_list();
		if (!pRecordData)
		{
			continue;
		}

		ConvertRecordToPB(pRecord, pRecordData);
	}

	return true;
}

bool NFCCommonMysqlModule::ConvertPBToRecordManager(const NFMsg::ObjectRecordList & pRecordData, NF_SHARE_PTR<NFIRecordManager> pRecord)
{
	if (pRecord == nullptr)
	{
		return false;
	}

	for (int i = 0; i < pRecordData.record_list_size(); ++i)
	{
		const NFMsg::ObjectRecordBase& xRecordBase = pRecordData.record_list(i);
		NF_SHARE_PTR<NFIRecord> xRecord = pRecord->GetElement(xRecordBase.record_name());
		if (xRecord)
		{
			ConvertPBToRecord(xRecordBase, xRecord);
		}
	}

	return true;
}

bool NFCCommonMysqlModule::ConvertPropertyManagerToPB(const NF_SHARE_PTR<NFIPropertyManager>& pProps, NFMsg::ObjectPropertyList * pPropertyData)
{
	if (pProps)
	{
		//NF_SHARE_PTR<NFIProperty> xPropert = pProps->First();
		//while (xPropert)
		for (NF_SHARE_PTR<NFIProperty> xPropert = pProps->First(); xPropert != NULL; xPropert = pProps->Next())
		{
			if (xPropert->GetCache() || xPropert->GetSave())
			{
				if (!mIObjectKeyMap[xPropert->GetKey()]) {
					switch (xPropert->GetType())
					{
					case NFDATA_TYPE::TDATA_INT:
					{
						NFMsg::PropertyInt* pData = pPropertyData->add_property_int_list();
						pData->set_property_name(xPropert->GetKey());
						pData->set_data(xPropert->GetInt());
					}
					break;

					case NFDATA_TYPE::TDATA_FLOAT:
					{
						NFMsg::PropertyFloat* pData = pPropertyData->add_property_float_list();
						pData->set_property_name(xPropert->GetKey());
						pData->set_data(xPropert->GetFloat());
					}
					break;

					case NFDATA_TYPE::TDATA_OBJECT:
					{
						NFMsg::PropertyObject* pData = pPropertyData->add_property_object_list();
						pData->set_property_name(xPropert->GetKey());
						*(pData->mutable_data()) = NFINetModule::NFToPB(xPropert->GetObject());
					}
					break;

					case NFDATA_TYPE::TDATA_STRING:
					{
						NFMsg::PropertyString* pData = pPropertyData->add_property_string_list();
						pData->set_property_name(xPropert->GetKey());
						pData->set_data(xPropert->GetString());
					}
					break;

					case NFDATA_TYPE::TDATA_VECTOR2:
					{
						NFMsg::PropertyVector2* pData = pPropertyData->add_property_vector2_list();
						pData->set_property_name(xPropert->GetKey());
						*(pData->mutable_data()) = NFINetModule::NFToPB(xPropert->GetVector2());
					}
					break;

					case NFDATA_TYPE::TDATA_VECTOR3:
					{
						NFMsg::PropertyVector3* pData = pPropertyData->add_property_vector3_list();
						pData->set_property_name(xPropert->GetKey());
						*(pData->mutable_data()) = NFINetModule::NFToPB(xPropert->GetVector3());
					}
					break;
					default:
						break;
					}
				}
			}
			//xPropert = pProps->Next();
		}
	}

	return false;
}

bool NFCCommonMysqlModule::ConvertPBToPropertyManager(const NFMsg::ObjectPropertyList& pPropertyData, NF_SHARE_PTR<NFIPropertyManager> pProps)
{
	if (pProps)
	{
		for (int i = 0; i < pPropertyData.property_int_list_size(); ++i)
		{
			const NFMsg::PropertyInt& xData = pPropertyData.property_int_list(i);

			if (pProps->ExistElement(xData.property_name()))
			{
				pProps->AddProperty(pProps->Self(), xData.property_name(), NFDATA_TYPE::TDATA_INT);
			}

			pProps->SetPropertyInt(xData.property_name(), xData.data());
		}

		for (int i = 0; i < pPropertyData.property_float_list_size(); ++i)
		{
			const NFMsg::PropertyFloat& xData = pPropertyData.property_float_list(i);

			if (pProps->ExistElement(xData.property_name()))
			{
				pProps->AddProperty(pProps->Self(), xData.property_name(), NFDATA_TYPE::TDATA_FLOAT);
			}

			pProps->SetPropertyFloat(xData.property_name(), xData.data());
		}

		for (int i = 0; i < pPropertyData.property_string_list_size(); ++i)
		{
			const NFMsg::PropertyString& xData = pPropertyData.property_string_list(i);

			if (pProps->ExistElement(xData.property_name()))
			{
				pProps->AddProperty(pProps->Self(), xData.property_name(), NFDATA_TYPE::TDATA_STRING);
			}

			pProps->SetPropertyString(xData.property_name(), xData.data());
		}

		for (int i = 0; i < pPropertyData.property_object_list_size(); ++i)
		{
			const NFMsg::PropertyObject& xData = pPropertyData.property_object_list(i);

			if (pProps->ExistElement(xData.property_name()))
			{
				pProps->AddProperty(pProps->Self(), xData.property_name(), NFDATA_TYPE::TDATA_OBJECT);
			}

			pProps->SetPropertyObject(xData.property_name(), NFGUID(xData.data().svrid(), xData.data().index()));
		}

		for (int i = 0; i < pPropertyData.property_vector2_list_size(); ++i)
		{
			const NFMsg::PropertyVector2& xData = pPropertyData.property_vector2_list(i);

			if (pProps->ExistElement(xData.property_name()))
			{
				pProps->AddProperty(pProps->Self(), xData.property_name(), NFDATA_TYPE::TDATA_VECTOR2);
			}

			pProps->SetPropertyVector2(xData.property_name(), NFVector2(xData.data().x(), xData.data().y()));
		}

		for (int i = 0; i < pPropertyData.property_vector3_list_size(); ++i)
		{
			const NFMsg::PropertyVector3& xData = pPropertyData.property_vector3_list(i);

			if (pProps->ExistElement(xData.property_name()))
			{
				pProps->AddProperty(pProps->Self(), xData.property_name(), NFDATA_TYPE::TDATA_VECTOR3);
			}

			pProps->SetPropertyVector3(xData.property_name(), NFVector3(xData.data().x(), xData.data().y(), xData.data().z()));
		}
	}

	return true;
}

bool NFCCommonMysqlModule::LoadObjData(const NFGUID& self, const std::string& strClassName, NFMsg::RoleDataPack& roleData)
{

	NF_SHARE_PTR<NFIPropertyManager> xPropertyManager = GetPropertyInfo(self, strClassName);
	if (xPropertyManager)
	{
		*(roleData.mutable_property()->mutable_player_id()) = NFINetModule::NFToPB(self);

		ConvertPropertyManagerToPB(xPropertyManager, roleData.mutable_property());

		GetRecordInfo(self, strClassName, roleData.mutable_record());

		return true;
	}

	m_pLogModule->LogNormal(NFILogModule::NF_LOG_LEVEL::NLL_ERROR_NORMAL, self, "loaded data false", NFGetTimeMS());

	return false;
}

bool NFCCommonMysqlModule::SaveObjData(const NFGUID& self, const std::string& strClassName, const NFMsg::RoleDataPack& roleData)
{

	NF_SHARE_PTR<NFIPropertyManager> xPropManager = NewPropertyManager(/*NFrame::Player::ThisName()*/strClassName);
	if (ConvertPBToPropertyManager(roleData.property(), xPropManager))
	{
		SavePropertyInfo(self, strClassName, xPropManager);
	}

	NF_SHARE_PTR<NFIRecordManager> xReManager = NewRecordManager(/*NFrame::Player::ThisName()*/strClassName);
	if (ConvertPBToRecordManager(roleData.record(), xReManager))
	{
		SaveRecordInfo(self, strClassName, xReManager);
	}
	//pCommonMysqlModule->SaveRecordInfo(self, strClassName, roleData.record(), -1);

	return true;
}
