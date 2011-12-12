#ifndef _CGA_T3_HERO_UNIT_HPP_
#define _CGA_T3_HERO_UNIT_HPP_

#include "dsgame/GameUnit.hpp"
#include "OgreFramework.hpp"
#include "util/ViewPointsGraph.h"

#include <cstdlib>

namespace dsgame
{
    class HeroUnit : public GameUnit
    {
    public:
        HeroUnit(Ogre::Entity *entity, Ogre::Node *hero_node);

        void adjustAnimationState(double timeSinceLastFrame);
        void getInput(util::ViewPointsGraph *vpg, const OIS::MouseEvent &arg, OIS::MouseButtonID id);

    private:
        void handleClickedOnGround(util::ViewPointsGraph *vpg, const OIS::MouseEvent &arg);
        Ogre::MovableObject *clickedOn(const OIS::MouseEvent &arg);

    private:
        Ogre::MovableObject *m_pEnemyTarget;
    }; //end of class HeroUnit.
} //end of namespace dsgame.

#endif
