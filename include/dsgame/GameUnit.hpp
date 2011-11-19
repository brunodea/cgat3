#ifndef CGA_T3_GAME_UNIT_HPP_
#define CGA_T3_GAME_UNIT_HPP_

#include <OgreVector3.h>
#include <OgreAnimationState.h>
#include <OgreString.h>

namespace dsgame //Dungeon's Secret Game.
{
    /* Unit that moves. */
    class GameUnit
    {
    public:
        GameUnit()
            : m_Direction(Ogre::Vector3::ZERO), m_Speed(0.f)
        {}
        
        GameUnit(const Ogre::Vector3 &dir, const Ogre::Real &speed)
            : m_Direction(dir), m_Speed(speed)
        {}

        GameUnit(const Ogre::String &filename)
        {}
        
    private:

        void fillGameUnitByFile(const Ogre::String &filename)
        {
        }

    private:
        Ogre::Vector3 m_Direction;
        Ogre::Real m_Speed;
    }; //end of class GameUnit.
} //end of namespace dsgame.

#endif
