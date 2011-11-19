#include "appstate/GameState.h"
#include "DotSceneLoader.hpp"

using namespace appstate;

GameState::GameState()
    : m_bQuit(false), m_pHero(0)
{
}

GameState::~GameState()
{
    delete m_pHero;
}

void GameState::enter()
{
    m_pSceneMgr = OgreFramework::getSingletonPtr()->m_pRoot->createSceneManager(Ogre::ST_GENERIC, "GameSceneMgr");
    m_pSceneMgr->setAmbientLight(Ogre::ColourValue(0.7f, 0.7f, 0.7f));
    m_pSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);

    OgreFramework::getSingletonPtr()->m_pTrayMgr->hideCursor();

    createScene();
}

void GameState::exit()
{
    if(m_pCamera)
        m_pSceneMgr->destroyCamera(m_pCamera);
    if(m_pSceneMgr)
        OgreFramework::getSingletonPtr()->m_pRoot->destroySceneManager(m_pSceneMgr);
}

bool GameState::pause()
{
    return true;
}

void GameState::resume()
{
    OgreFramework::getSingletonPtr()->m_pViewport->setCamera(m_pCamera);
    m_bQuit = false;
}

void GameState::update(double timeSinceLastFrame)
{
    m_FrameEvent.timeSinceLastFrame = timeSinceLastFrame;
    OgreFramework::getSingletonPtr()->m_pTrayMgr->frameRenderingQueued(m_FrameEvent);

    if(m_bQuit)
    {
        popAppState();
        return;
    }

    m_pHero->getInput();
    m_pHero->update(timeSinceLastFrame);

    if(m_pHero->isMoving())
        m_pCamera->move(m_pHero->getDirection()*m_pHero->getSpeed()*timeSinceLastFrame);
}

bool GameState::keyPressed(const OIS::KeyEvent &keyEventRef)
{
    OgreFramework::getSingletonPtr()->keyPressed(keyEventRef);

    if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_ESCAPE))
    {
        m_bQuit = true;
    }


    return true;
}

bool GameState::keyReleased(const OIS::KeyEvent &keyEventRef)
{
    OgreFramework::getSingletonPtr()->keyPressed(keyEventRef);
    return true;
}

bool GameState::mouseMoved(const OIS::MouseEvent &arg)
{
    if(OgreFramework::getSingletonPtr()->m_pTrayMgr->injectMouseMove(arg)) return true;

    return true;
}


bool GameState::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
    if(OgreFramework::getSingletonPtr()->m_pTrayMgr->injectMouseDown(arg, id)) return true;

    return true;
}

bool GameState::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
    if(OgreFramework::getSingletonPtr()->m_pTrayMgr->injectMouseUp(arg, id)) return true;

    return true;
}

void GameState::itemSelected(OgreBites::SelectMenu* menu)
{
}

void GameState::createScene()
{
    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Creating Game Scene...");

    OgreFramework::getSingletonPtr()->m_pViewport->setShadowsEnabled(true);
    
    DotSceneLoader* dsl = new DotSceneLoader();
    dsl->parseDotScene("Level1.xml", "General", m_pSceneMgr, m_pSceneMgr->getRootSceneNode());
    delete dsl;

    m_pCamera = m_pSceneMgr->getCamera("HeroCam");
    OgreFramework::getSingletonPtr()->m_pViewport->setCamera(m_pCamera);

    m_pHero = new dsgame::HeroUnit(m_pSceneMgr->getEntity("HeroEntity"));

    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Game Scene Created...");
}