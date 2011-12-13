#pragma once

#include "dsgame/Enemies/FSM.hpp"
#include "dsgame/Enemies/FSMFactory.hpp"
#include "util/util.hpp"
#include "macros.h"

namespace dsgame { namespace npc {

    /********************************/
    /* ESTADOS PARA O NORMAL OGRE   */
    /********************************/

    class NormalOgreIdleState : public FSMState
    {
    public:
        NormalOgreIdleState(NPCUnit *unit)
            : FSMState(unit, IDLE_STATE)
        {}

        void enter()
        {
            m_pUnit->setAnimState("Idle2");
        }

        void run() 
        {
            Ogre::Vector3 hero_pos = Ogre::Root::getSingletonPtr()->getSceneManager("GameSceneMgr")->getSceneNode("HeroNode")->getPosition();
            Ogre::Vector3 ogre_pos = m_pUnit->getNode()->getPosition();
            if(hero_pos.distance(ogre_pos) <= m_pUnit->getVisibilityRadius() && util::isVisible(ogre_pos, hero_pos))
                setNextState(SEEK_STATE);
        }

        void leave() {}

    }; //end of class NormalOgreIdleState.

    class NormalOgreSeekState : public FSMState
    {
    public:
        NormalOgreSeekState(NPCUnit *unit)
            : FSMState(unit, SEEK_STATE)
        {}

        void enter() 
        {
            m_pUnit->setAnimState("Run");
        }

        void run() 
        {
            Ogre::Vector3 hero_pos = Ogre::Root::getSingletonPtr()->getSceneManager("GameSceneMgr")->getSceneNode("HeroNode")->getPosition();
            Ogre::Entity *hero_entity = Ogre::Root::getSingletonPtr()->getSceneManager("GameSceneMgr")->getEntity("HeroEntity");
            Ogre::Vector3 ogre_pos = m_pUnit->getNode()->getPosition();

            Ogre::Real dist = hero_pos.distance(ogre_pos);
            if(dist > m_pUnit->getVisibilityRadius())
                setNextState(RETURN_STATE);
            else if(!m_pUnit->hasNextDestination())
            {
                if(dist <= hero_entity->getBoundingRadius())
                    setNextState(ATTACK_STATE);
                else
                {
                    Ogre::Vector3 dest = hero_pos;
                    dest += (ogre_pos-hero_pos).normalisedCopy()*hero_entity->getBoundingRadius()*.3f;
                    m_pUnit->clearDestinations();
                    m_pUnit->addDestinations(util::VPGRAPH->pathFindingAStar(ogre_pos, dest));

                    //OgreFramework::getSingletonPtr()->m_pLog->logMessage(Ogre::StringConverter::toString(dest));
                }
            }
        }

        void leave() {}

    }; //end of class NormalOgreSeekState.

    class NormalOgreAttackState : public FSMState
    {
    public:
        NormalOgreAttackState(NPCUnit *unit)
            : FSMState(unit, ATTACK_STATE)
        {}

        void enter()
        {
            m_pUnit->setAnimState("Attack");
        }

        void run() 
        {
            Ogre::Vector3 hero_pos = Ogre::Root::getSingletonPtr()->getSceneManager("GameSceneMgr")->getSceneNode("HeroNode")->getPosition();
            Ogre::Entity *hero_entity = Ogre::Root::getSingletonPtr()->getSceneManager("GameSceneMgr")->getEntity("HeroEntity");
            Ogre::Vector3 ogre_pos = m_pUnit->getNode()->getPosition();

            Ogre::Real dist = hero_pos.distance(ogre_pos);
            if(dist > hero_entity->getBoundingRadius()*.3f)
                setNextState(SEEK_STATE);
        }

        void leave() {}
    }; //end of class NormalOgreAttackState.

    class NormalOgreReturnState : public FSMState
    {
    public:
        NormalOgreReturnState(NPCUnit *unit)
            : FSMState(unit, RETURN_STATE)
        {}

        void enter()
        {
            m_pUnit->setAnimState("Run");
            m_pUnit->clearDestinations();
        }

        void run()
        {
            Ogre::Vector3 hero_pos = Ogre::Root::getSingletonPtr()->getSceneManager("GameSceneMgr")->getSceneNode("HeroNode")->getPosition();
            Ogre::Vector3 ogre_pos = m_pUnit->getNode()->getPosition();
            if(hero_pos.distance(ogre_pos) <= m_pUnit->getVisibilityRadius() && util::isVisible(ogre_pos, hero_pos))
                setNextState(SEEK_STATE);
            else
            {
                if(!m_pUnit->hasNextDestination())
                {
                    m_pUnit->addDestinations(util::VPGRAPH->pathFindingAStar(ogre_pos, m_pUnit->getOrigPos()));
                }
            }

            if(!m_pUnit->shouldMove())
                setNextState(IDLE_STATE);
        }

        void leave() {}

    }; //end of class NormalOgreReturnState.

    class NormalOgreLifeLowState : public FSMState
    {
    public:
        NormalOgreLifeLowState(NPCUnit *unit)
            : FSMState(unit, LIFE_LOW_STATE)
        {}

        void enter()
        {
            m_pUnit->setAnimState("Run");
        }

        void run() {}
        void leave() {}

    }; //end of class NormalOgreLifeLowState.

    /*************** FIM DOS ESTADOS ************************/

    class NormalOgreStateFactory : public FSMFactory
    {
    public:
        
        FSMState *createIdleState(NPCUnit *unit)
        {
            return new NormalOgreIdleState(unit);
        }

        FSMState *createSeekState(NPCUnit *unit)
        {
            return new NormalOgreSeekState(unit);
        }

        FSMState *createAttackState(NPCUnit *unit)
        {
            return new NormalOgreAttackState(unit);
        }
        FSMState *createReturnState(NPCUnit *unit)
        {
            return new NormalOgreReturnState(unit);
        }
        FSMState *createLifeLowState(NPCUnit *unit)
        {
            return new NormalOgreLifeLowState(unit);
        }
    }; //end of class NormalOgreStateFactory.


} //end of namespace dsgame.
} //end of namespace npc.


