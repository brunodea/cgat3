#pragma once

#include "dsgame/HeroUnit.hpp"

using namespace dsgame;

HeroUnit::HeroUnit(Ogre::Entity *entity, Ogre::Node *hero_node)
: GameUnit("media/initchars/hero.dsg", entity,hero_node), m_pEnemyTarget(0)
{
}

void HeroUnit::adjustAnimationState(double timeSinceLastFrame)
{
    std::string anim_name;
    if(m_isMoving)
        anim_name = "Walk";
    else if(m_pEnemyTarget != 0 && !hasNextDestination())
    {
        anim_name = "Attack1";
    }
    else
        anim_name = "Idle1";

    m_pAnimState = m_pEntity->getAnimationState(anim_name);
    m_pAnimState->setLoop(true);
    m_pAnimState->setEnabled(true);
}

void HeroUnit::getInput(util::ViewPointsGraph *vpg, const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
    if(id == OIS::MB_Left)
    {
        Ogre::MovableObject *obj = clickedOn(arg);
        if(obj != 0)
        {
            bool notarget = true;
            Ogre::uint32 mask = obj->getQueryFlags();
            if(mask == GROUND_MASK)
                handleClickedOnGround(vpg, arg);
            else if(mask == ENEMY_MASK)
            {
                notarget = false;
                m_pEnemyTarget = obj;
                clearDestinations();

                Ogre::Vector3 pos = getNode()->getPosition();
                Ogre::Vector3 dest = obj->getParentNode()->getParent()->getPosition();

                dest += (pos-dest).normalisedCopy()*obj->getBoundingRadius()*.3f;

                addDestinations(vpg->pathFindingAStar(pos,dest));
            }

            if(notarget)
                m_pEnemyTarget = 0;
        }
    }
}

void HeroUnit::handleClickedOnGround(util::ViewPointsGraph *vpg, const OIS::MouseEvent &arg)
{
    Ogre::SceneManager *smgr = Ogre::Root::getSingletonPtr()->getSceneManager("GameSceneMgr");
    Ogre::Camera *cam = smgr->getCamera("HeroCam");

    Ogre::Real screenWidth = Ogre::Root::getSingleton().getAutoCreatedWindow()->getWidth();
    Ogre::Real screenHeight = Ogre::Root::getSingleton().getAutoCreatedWindow()->getHeight();
           
    Ogre::Real offsetX = arg.state.X.abs / screenWidth;
    Ogre::Real offsetY = arg.state.Y.abs / screenHeight;

    Ogre::Ray mouseRay = cam->getCameraToViewportRay(offsetX, offsetY);
    std::pair<bool, Ogre::Real> rayresult = mouseRay.intersects(smgr->getEntity("Chao")->getWorldBoundingBox());
    if(rayresult.first)
    {
        Ogre::RaySceneQuery *rsq = smgr->createRayQuery(mouseRay, OBSTACLE_MASK);
        rsq->setSortByDistance(true,1);

        Ogre::RaySceneQueryResult &result = rsq->execute();
        Ogre::RaySceneQueryResult::iterator itr;

        bool clicked_on_the_ground = true;
        for(itr = result.begin(); itr != result.end(); itr++)
        {
            if(itr->movable)
            {
                clicked_on_the_ground = false;
                break;
            }
        }
            
        if(clicked_on_the_ground)
        {
            Ogre::Vector3 position = mouseRay.getPoint(rayresult.second);
            clearDestinations();
            addDestinations(vpg->pathFindingAStar(getNode()->getPosition(),position));
        }

        smgr->destroyQuery(rsq);
    }
}

Ogre::MovableObject *HeroUnit::clickedOn(const OIS::MouseEvent &arg)
{
    Ogre::SceneManager *smgr = Ogre::Root::getSingletonPtr()->getSceneManager("GameSceneMgr");
    Ogre::Camera *cam = smgr->getCamera("HeroCam");

    Ogre::Real screenWidth = Ogre::Root::getSingleton().getAutoCreatedWindow()->getWidth();
    Ogre::Real screenHeight = Ogre::Root::getSingleton().getAutoCreatedWindow()->getHeight();
           
    Ogre::Real offsetX = arg.state.X.abs / screenWidth;
    Ogre::Real offsetY = arg.state.Y.abs / screenHeight;

    Ogre::Ray mouseRay = cam->getCameraToViewportRay(offsetX, offsetY);

    Ogre::RaySceneQuery *rsq = smgr->createRayQuery(mouseRay, OBSTACLE_MASK | ENEMY_MASK | GROUND_MASK | HERO_MASK);
    rsq->setSortByDistance(true,1);

    Ogre::RaySceneQueryResult &result = rsq->execute();
    Ogre::RaySceneQueryResult::iterator itr;

    Ogre::MovableObject *obj = 0;
    for(itr = result.begin(); itr != result.end(); itr++)
    {
        if(itr->movable)
        {
            obj = itr->movable;
            break;
        }
    }
            
    smgr->destroyQuery(rsq);
    return obj;
}
