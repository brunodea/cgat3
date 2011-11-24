#ifndef CGAT3_GAME_STATE_H_
#define CGAT3_GAME_STATE_H_

#include "AppState.hpp"
#include "dsgame/HeroUnit.hpp"

namespace appstate
{
    enum
    {
        GROUND_MASK = 0 << 1,
        OBSTACLE_MASK = 1 << 1
    };

    class GameState : public AppState
    {
    public:
        GameState();
        ~GameState();

        DECLARE_APPSTATE_CLASS(GameState)

        /* Virtual Functions */
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

        /***********************************************/

        void createScene();

    private:
        bool m_bQuit;
        bool m_bRMousePressed;

        dsgame::HeroUnit *m_pHero;
    }; //end of class GameState.

} //end of namespace appstate.

#endif
