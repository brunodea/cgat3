#include "DungeonsSecretsApp.h"

using namespace APP;

DungeonsSecretsApp::DungeonsSecretsApp()
{
    m_pAppStateManager = 0;
}

DungeonsSecretsApp::~DungeonsSecretsApp()
{
    delete m_pAppStateManager;
    delete OgreFramework::getSingletonPtr();
}

void DungeonsSecretsApp::start()
{
    new OgreFramework();
	if(!OgreFramework::getSingletonPtr()->initOgre("Dungeon's Secrets", 0, 0))
		return;

	OgreFramework::getSingletonPtr()->m_pLog->logMessage("Dungeon's Secrets initialized!");

	m_pAppStateManager = new AppStateManager();
}

