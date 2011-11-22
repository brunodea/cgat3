#ifndef _CGA_T3_HERO_UNIT_HPP_
#define _CGA_T3_HERO_UNIT_HPP_

#include "dsgame/GameUnit.hpp"
#include "OgreFramework.hpp"

#include <cstdlib>

namespace dsgame
{
    class HeroUnit : public GameUnit
    {
    public:
        HeroUnit(Ogre::Entity *entity, Ogre::Node *hero_node)
            : GameUnit("media/initchars/hero.dsg", entity,hero_node)
        {
        }

        void adjustAnimationState(double timeSinceLastFrame)
        {
            std::string anim_name;
            if(m_isMoving)
                anim_name = "Walk";
            else
                anim_name = "Idle1";
                        
            m_pAnimState = m_pEntity->getAnimationState(anim_name);
            m_pAnimState->setLoop(true);
            m_pAnimState->setEnabled(true);
        }

        void getInput()
        {
            /*
            if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_W))
            {
                Ogre::Vector3 olddir = getDirection();
                rotate(20.f);
                moveTo(m_pEntity->getParentNode()->getPosition()*2 + (olddir*5));
            }
                m_isMoving = true;
            else
                m_isMoving = false;

            if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_A))
                rotate(Ogre::Degree(1.f));
            else if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_D))
                rotate(Ogre::Degree(-1.f));
            */
        }
    }; //end of class HeroUnit.
} //end of namespace dsgame.

#endif
