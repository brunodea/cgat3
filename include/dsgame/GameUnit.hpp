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
        GameUnit(Ogre::Entity *entity, Ogre::Node *unit_node)
        {
            init(Ogre::Vector3::ZERO,0,entity,unit_node);
        }
        
        GameUnit(const Ogre::Vector3 &dir, const Ogre::Real &speed, Ogre::Entity *entity, Ogre::Node *unit_node)
        {
            init(dir,speed,entity,unit_node);
        }

        GameUnit(const Ogre::String &filename, Ogre::Entity *entity, Ogre::Node *unit_node)
        {
            init(Ogre::Vector3::ZERO,0,entity,unit_node);
            fillGameUnitByFile(filename);
        }

        virtual void update(double timeSinceLastFrame)
        {
            adjustDestinations();
            m_isMoving = shouldMove();
            if(m_isMoving)
                move(timeSinceLastFrame);

            adjustAnimationState(timeSinceLastFrame);
            if(m_pAnimState != 0)
                m_pAnimState->addTime(timeSinceLastFrame / 1000.f);
        }

        bool isMoving() { return m_isMoving; }

        void rotate(Ogre::Degree angle) 
        { 
            m_DirAngle += angle;
            m_pEntity->getParentNode()->yaw(angle);
        }

        void rotate(Ogre::Real degrees)
        {
            rotate(Ogre::Degree(degrees));
        }

        virtual void adjustAnimationState(double timeSinceLastFrame) = 0;

        Ogre::Vector3 getDirection()
        {
            Ogre::Quaternion quat = Ogre::Quaternion(m_DirAngle,Ogre::Vector3::UNIT_Y);
            m_Direction.normalise();
            return quat*m_Direction;
        }

        Ogre::Real getSpeed() { return m_Speed; }
        
        Ogre::Entity *getEntity() { return m_pEntity; }
        Ogre::Node *getNode() { return m_pUnitNode; }

        bool hasNextDestination() { return !m_Destinations.empty(); }
        Ogre::Vector3 getNextDestination() { return m_Destinations.at(0); }
        void addDestination(const Ogre::Vector3 &p) { m_Destinations.push_back(p); }
        void clearDestinations() { m_Destinations.clear(); }

        void adjustDestinations()
        {
            if(m_Destinations.empty())
                return;
            Ogre::Vector3 dest = m_Destinations.at(0);
            if(dest.distance(getNode()->getPosition()) < .5f) //se ja chegou no destino atual.
            {
                m_Destinations.erase(m_Destinations.begin());
            }

            //if(m_Destinations.size() >= 1)
            {
                for(auto &it = m_Destinations.end(); it != m_Destinations.begin(); it--)
                {
                    Ogre::Vector3 dest = *it;
                    if(util::isVisible(getNode()->getPosition(),dest,"GameSceneMgr",OBSTACLE_MASK, Ogre::Vector3::UNIT_Y))
                    {
                        m_Destinations.erase(m_Destinations.begin(),it-1);
                        break;
                    }
                }

                /*for(unsigned int i = m_Destinations.size()-1; i >= 1; i--)
                {
                    Ogre::Vector3 dest = m_Destinations.at(i);
                    
                    if(util::isVisible(getNode()->getPosition(),dest,"GameSceneMgr",OBSTACLE_MASK, Ogre::Vector3::UNIT_Y))
                    {
                        m_Destinations.erase(m_Destinations.begin(),m_Destinations.begin()+i);
                        break;
                    }
                }*/
            }
        }

    protected:
        
        virtual void move(double timeSinceLastFrame)
        {
            if(!hasNextDestination()) 
                return;

            Ogre::Vector3 dest = getNextDestination()-m_pUnitNode->getPosition();
            dest.normalise();
            Ogre::Vector3 dir = getDirection();
            dir.normalise();
            
            if(dir.directionEquals(dest,Ogre::Radian(Ogre::Degree(.5f))) == false)
            {
                //se, após a rotação, o ângulo entre a direção e o destino for maior, significa que o certo
                //era rotacionar para o outro lado, pois este está mais perto.
                Ogre::Real init_angle = dir.angleBetween(dest).valueDegrees();
                
                Ogre::Real rot_angle = dir.angleBetween(dir+dest).valueDegrees()/(dir.distance(dest)*5.f);

                m_DirAngle += Ogre::Degree(rot_angle);
                Ogre::Real after_angle = getDirection().angleBetween(dest).valueDegrees();
                m_DirAngle -= Ogre::Degree(rot_angle);

                if(after_angle > init_angle)
                    rot_angle *= -1;

                rotate(rot_angle);
            }

            m_pUnitNode->translate(getDirection()*getSpeed()*timeSinceLastFrame);
        }

        bool shouldMove()
        {
            bool move = false;
            if(hasNextDestination())
            {
                move = m_pUnitNode->getPosition().distance(getNextDestination()) >= .5f;
            }

            return move;
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

        void init(const Ogre::Vector3 &dir, const Ogre::Real &speed, Ogre::Entity *entity, Ogre::Node *unit_node)
        {
            m_Direction = dir;
            m_Speed = speed;
            m_pEntity = entity;
            m_isMoving = false;
            m_pAnimState = 0;
            m_pUnitNode = unit_node;

            m_DirAngle = Ogre::Degree(0.f);
            m_Destinations.push_back(unit_node->getPosition());
        }
        
    private:
        Ogre::Vector3 m_Direction;
        std::vector<Ogre::Vector3> m_Destinations;
        Ogre::Real m_Speed;
        Ogre::Node *m_pUnitNode;

    }; //end of class GameUnit.
} //end of namespace dsgame.

#endif
