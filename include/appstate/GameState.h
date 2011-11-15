#ifndef CGAT3_GAME_STATE_H_
#define CGAT3_GAME_STATE_H_

#include "AppState.hpp"

namespace appstate
{
    class GameState : public AppState
    {
    public:
        GameState();

        DECLARE_APPSTATE_CLASS(GameState)

        void enter();
        void exit();
        bool pause();
        void resume();
        void update(double timeSinceLastFrame);

        bool keyPressed(const OIS::KeyEvent &keyEventRef);
	    bool keyReleased(const OIS::KeyEvent &keyEventRef);

	    bool mouseMoved(const OIS::MouseEvent &arg);
	    bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	    bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);

        void itemSelected(OgreBites::SelectMenu* menu);

    }; //end of class GameState.

} //end of namespace appstate.

#endif
