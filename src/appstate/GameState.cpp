#include "appstate/GameState.h"

using namespace appstate;

GameState::GameState()
{
}

void GameState::enter()
{
}

void GameState::exit()
{
}

bool GameState::pause()
{
    return true;
}

void GameState::resume()
{
}

void GameState::update(double timeSinceLastFrame)
{
}

bool GameState::keyPressed(const OIS::KeyEvent &keyEventRef)
{
    return true;
}

bool GameState::keyReleased(const OIS::KeyEvent &keyEventRef)
{
    return true;
}

bool GameState::mouseMoved(const OIS::MouseEvent &arg)
{
    return true;
}


bool GameState::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
    return true;
}

bool GameState::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
    return true;
}

void GameState::itemSelected(OgreBites::SelectMenu* menu)
{
}