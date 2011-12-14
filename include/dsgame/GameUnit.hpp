#ifndef CGA_T3_GAME_UNIT_HPP_
#define CGA_T3_GAME_UNIT_HPP_

#include <OgreVector3.h>
#include <OgreAnimationState.h>
#include <OgreString.h>
#include <OgreNode.h>
#include "OgreFramework.hpp"
#include "util/util.hpp"
#include "macros.h"

#include <fstream>
#include <string>
#include <vector>

namespace dsgame //Dungeon's Secret Game.
{
    /* Unit that moves. */
    class GameUnit
    {
    public:
        GameUnit(Ogre::Entity *entity, Ogre::Node *unit_node);
        GameUnit(const Ogre::Vector3 &dir, const Ogre::Real &speed, Ogre::Entity *entity, Ogre::Node *unit_node);
        GameUnit(const Ogre::String &filename, Ogre::Entity *entity, Ogre::Node *unit_node);

        virtual void update(double timeSinceLastFrame);

        bool isMoving() { return m_isMoving; }
        
        bool isAlive() { return m_Health > 0.f; }
        void loseHealth(Ogre::Real amount) 
        { 
            m_Health -= amount;
            if(m_Health < 0.f)
                m_Health = 0.f;
        }

        Ogre::Real damage() { return m_Damage; }

        void rotate(Ogre::Degree angle);
        void rotate(Ogre::Real degrees);
        void rotateTo(const Ogre::Vector3& dest, Ogre::Degree error=Ogre::Degree(.5f));

        virtual void adjustAnimationState(double timeSinceLastFrame) = 0;

        Ogre::Vector3 getDirection();
        Ogre::Real getSpeed() { return m_Speed; }
        void setSpeed(Ogre::Real speed) { m_Speed = speed; }
        
        Ogre::Entity *getEntity() { return m_pEntity; }
        Ogre::Node *getNode() { return m_pUnitNode; }

        Ogre::AnimationState *getAnimState() { return m_pAnimState; }
        void setAnimState(const Ogre::String &anim_name, bool loop=true)
        {
            m_pAnimState = m_pEntity->getAnimationState(anim_name);
            m_pAnimState->setLoop(loop);
            m_pAnimState->setEnabled(true);
        }

        bool hasNextDestination() { return !m_Destinations.empty(); }
        Ogre::Vector3 getNextDestination() { return m_Destinations.at(0); }
        void addDestination(const Ogre::Vector3 &p) { m_Destinations.push_back(p); }
        void clearDestinations() { m_Destinations.clear(); }
        void addDestinations(const std::vector<Ogre::Vector3> &dests)
        {
            m_Destinations.insert(m_Destinations.end(),dests.begin(),dests.end());
        }

        void adjustDestinations();
        bool shouldMove();

    protected:
        virtual void move(double timeSinceLastFrame);
        void fillGameUnitByFile(const Ogre::String &filename, void(GameUnit::*fill_spec_ptr)(const Ogre::String &, const Ogre::String &) = 0);

    protected:
        bool m_isMoving;
        Ogre::Degree m_DirAngle;
        Ogre::AnimationState *m_pAnimState;
        Ogre::Entity *m_pEntity;

        Ogre::Real m_Health; //quanto de vida tem.
        Ogre::Real m_Damage; //quanto de dano causa.

    private:
        void init(const Ogre::Vector3 &dir, const Ogre::Real &speed, Ogre::Entity *entity, Ogre::Node *unit_node);
        
    private:
        Ogre::Vector3 m_Direction;
        std::vector<Ogre::Vector3> m_Destinations;
        Ogre::Real m_Speed;
        Ogre::Node *m_pUnitNode;

    }; //end of class GameUnit.
} //end of namespace dsgame.

#endif
