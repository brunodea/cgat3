#pragma once

#include "dsgame/GameUnit.hpp"
#include "dsgame/Enemies/FSM.hpp"
#include "dsgame/Enemies/FSMFactory.hpp"
#include "dsgame/Enemies/FSMState.hpp"

namespace dsgame { namespace npc {
    class NPCUnit;

    class FSM
    {
    public:
        FSM(NPCUnit *unit, FSMState *init_state, FSMFactory *factory);
        ~FSM();

        void run();
        void changeStateTo(FSMStateEnum new_state)
        {
            m_pState->setNextState(new_state);
            changeState();
        }

        FSMState *getCurrState() { return m_pState; }

    private:
        void changeState();

    private:
        FSMState *m_pState;
        FSMFactory *m_pFactory;

        NPCUnit *m_pUnit;

    }; //end of class FSM.
    
    class NPCUnit : public GameUnit
    {
    public:
        NPCUnit(const Ogre::String &filename, Ogre::Entity *entity, Ogre::Node *unit_node, FSMState *init_state, FSMFactory *factory);

        ~NPCUnit();

        void adjustAnimationState(double timeSinceLastFrame) {}
        void behave() 
        {
            m_FSM->run();
        }

        Ogre::Vector3 getOrigPos() { return m_OrigPos; }
        Ogre::Real getVisibilityRadius() { return m_VisibilityRadius; }

    protected:
        FSM *m_FSM;
        Ogre::Vector3 m_OrigPos;
        Ogre::Real m_VisibilityRadius;
    }; //end of class NPCUnit.
} //end of namespace npc.
} //end of namespace dsgame.