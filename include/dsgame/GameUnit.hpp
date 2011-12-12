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

        void rotate(Ogre::Degree angle);
        void rotate(Ogre::Real degrees);

        virtual void adjustAnimationState(double timeSinceLastFrame) = 0;

        Ogre::Vector3 getDirection();
        Ogre::Real getSpeed() { return m_Speed; }
        
        Ogre::Entity *getEntity() { return m_pEntity; }
        Ogre::Node *getNode() { return m_pUnitNode; }

        bool hasNextDestination() { return !m_Destinations.empty(); }
        Ogre::Vector3 getNextDestination() { return m_Destinations.at(0); }
        void addDestination(const Ogre::Vector3 &p) { m_Destinations.push_back(p); }
        void clearDestinations() { m_Destinations.clear(); }
        void addDestinations(const std::vector<Ogre::Vector3> &dests)
        {
            m_Destinations.insert(m_Destinations.end(),dests.begin(),dests.end());
        }

        void adjustDestinations();

    protected:
        
        virtual void move(double timeSinceLastFrame);
        bool shouldMove();

    protected:
        bool m_isMoving;
        Ogre::Degree m_DirAngle;
        Ogre::AnimationState *m_pAnimState;
        Ogre::Entity *m_pEntity;

    private:
        void fillGameUnitByFile(const Ogre::String &filename);
        void init(const Ogre::Vector3 &dir, const Ogre::Real &speed, Ogre::Entity *entity, Ogre::Node *unit_node);
        
    private:
        Ogre::Vector3 m_Direction;
        std::vector<Ogre::Vector3> m_Destinations;
        Ogre::Real m_Speed;
        Ogre::Node *m_pUnitNode;

    }; //end of class GameUnit.
} //end of namespace dsgame.

#endif
