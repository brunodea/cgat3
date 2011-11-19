#ifndef CGA_T3_GAME_UNIT_HPP_
#define CGA_T3_GAME_UNIT_HPP_

#include <OgreVector3.h>
#include <OgreAnimationState.h>
#include <OgreString.h>
#include <OgreNode.h>
#include "OgreFramework.hpp"

#include <fstream>
#include <string>
#include <vector>

namespace dsgame //Dungeon's Secret Game.
{
    /* Unit that moves. */
    class GameUnit
    {
    public:
        GameUnit(Ogre::Entity *entity)
            : m_Direction(Ogre::Vector3::ZERO), m_Speed(0.f),
              m_isMoving(false), m_DirAngle(), m_pAnimState(0), m_pEntity(entity)
        {
        }
        
        GameUnit(const Ogre::Vector3 &dir, const Ogre::Real &speed, Ogre::Entity *entity)
            : m_Direction(dir), m_Speed(speed), m_isMoving(false),
              m_DirAngle(), m_pAnimState(0), m_pEntity(entity)
        {}

        GameUnit(const Ogre::String &filename, Ogre::Entity *entity)
            : m_isMoving(false), m_DirAngle(), m_pAnimState(0), m_pEntity(entity)
        {
            fillGameUnitByFile(filename);
        }

        virtual void update(double timeSinceLastFrame)
        {
            if(m_isMoving)
                move(timeSinceLastFrame);

            adjustAnimationState(timeSinceLastFrame);
            if(m_pAnimState != 0)
                m_pAnimState->addTime(timeSinceLastFrame / 1000.f);
        }
        
        void setMoving(bool moving) { m_isMoving = moving; }
        void rotate(Ogre::Degree angle) 
        { 
            m_DirAngle += angle;
            m_pEntity->getParentNode()->yaw(angle);
        }

        virtual void adjustAnimationState(double timeSinceLastFrame) = 0;

        Ogre::Vector3 getDirection()
        {
            Ogre::Quaternion quat = Ogre::Quaternion(Ogre::Degree(m_DirAngle),Ogre::Vector3::UNIT_Y);
            m_Direction.normalise();
            return quat*m_Direction;
        }

        Ogre::Real getSpeed() { return m_Speed; }

        bool isMoving() { return m_isMoving; }

    protected:
        
        virtual void move(double timeSinceLastFrame)
        {            
            m_pEntity->getParentNode()->translate(getDirection()*getSpeed()*timeSinceLastFrame);
        }

    protected:
        bool m_isMoving;
        Ogre::Degree m_DirAngle;
        Ogre::AnimationState *m_pAnimState;
        Ogre::Entity *m_pEntity;

    private:
        void fillGameUnitByFile(const Ogre::String &filename)
        {
            std::ifstream file(filename);
            std::string line;
            if(file.is_open())
            {
                while(file.good())
                {
                    std::getline(file,line);

                    Ogre::StringVector sp = Ogre::StringUtil::split(line,":");
                    
                    if(sp.size() >= 2)
                    {
                        Ogre::StringUtil::toLowerCase(sp[0]);
                        Ogre::StringUtil::trim(sp[1]);

                        if(sp[0] == "direction")
                        {
                            m_Direction = Ogre::StringConverter::parseVector3(sp[1]);
                        }
                        else if(sp[0] == "speed")
                        {
                            m_Speed = Ogre::StringConverter::parseReal(sp[1]);
                        }
                    }
                }
                file.close();
            }
            else
                OgreFramework::getSingletonPtr()->m_pLog->logMessage("Couldn't open the file '" + filename + "'.");
        }

    private:
        Ogre::Vector3 m_Direction;
        Ogre::Real m_Speed;

    }; //end of class GameUnit.
} //end of namespace dsgame.

#endif
