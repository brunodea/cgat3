#pragma once

#include "dsgame/GameUnit.hpp"
#include "dsgame/Enemies/FSM.hpp"
#include "dsgame/Enemies/FSMFactory.hpp"
#include "dsgame/Enemies/FSMState.hpp"
#include "macros.h"

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
        bool isVisible(const Ogre::Vector3& dest);

        Ogre::Real getMaxSpeed() { return m_MaxSpeed; }
        Ogre::Real getMass() { return m_Mass; }
        Ogre::Real getMaxForce() { return m_MaxForce; }
        Ogre::Vector3 getVelocity() { return m_Velocity; }

        void move(double timeSinceLastFrame);
        void setSteeringType(SteeringTypeEnum steering_type) { m_CurrSteeringType = steering_type; }

    protected:
        FSM *m_FSM;
        Ogre::Vector3 m_OrigPos;
        Ogre::Real m_VisibilityRadius;
        Ogre::Degree m_VisionAngleDegrees;
        Ogre::Real m_Mass;
        Ogre::Real m_MaxSpeed;
        Ogre::Real m_MaxForce;
        Ogre::Vector3 m_Velocity;

        SteeringTypeEnum m_CurrSteeringType;

    private:
        void fillSpecificAttr(const Ogre::String &attribute,  const Ogre::String &value);
    }; //end of class NPCUnit.
} //end of namespace npc.
} //end of namespace dsgame.