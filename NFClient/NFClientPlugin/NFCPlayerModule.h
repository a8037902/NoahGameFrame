#pragma once

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
#include "NFClient/NFClientPlugin/NFCClientModule.h"

class NFIPlayerModule
	: public NFIModule
{
};

class NFCPlayerModule
	:public NFIPlayerModule
{
public:
	NFCPlayerModule(NFIPluginManager* p)
	{
		pPluginManager = p;
	}

	virtual bool Init();
	virtual bool AfterInit();

	virtual bool Execute();

	virtual bool BeforeShut();
	virtual bool Shut();

protected:
	void OnObjectEntry(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnObjectLeave(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

	void OnLoadDataFinish(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

private:
	NFINetModule* m_pNetModule;
	NFIKernelModule* m_pKernelModule;
	NFINetClientModule* m_pNetClientModule;
	NFIClientModule* m_pClientModule;
	NFIEventModule* m_pEventModule;
};

