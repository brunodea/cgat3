#ifndef _CGA_T3_UTIL_HPP_
#define _CGA_T3_UTIL_HPP_

#include "OgreFramework.hpp"
#include "macros.h"

namespace util
{
    inline bool isVisible(const Ogre::Vector3 &origin, const Ogre::Vector3 &destination, 
        const Ogre::String &scene_mgr_name, unsigned long mask, const Ogre::Vector3 &offset = Ogre::Vector3::ZERO)
    {
        Ogre::Ray ray;
        ray.setOrigin(origin);
        ray.setDirection((destination-origin).normalisedCopy());

        Ogre::RaySceneQuery *rsq = Ogre::Root::getSingletonPtr()->getSceneManager(scene_mgr_name)->createRayQuery(ray,mask);
        rsq->setSortByDistance(true);

        Ogre::RaySceneQueryResult &result = rsq->execute();

        bool visible = true;
        Ogre::RaySceneQueryResult::iterator itr;
        for(itr = result.begin(); itr != result.end(); itr++)
        {
            if(itr->movable && itr->distance < origin.distance(destination))
            {
                visible = false;
                break;
            }
        }

        Ogre::Root::getSingletonPtr()->getSceneManager(scene_mgr_name)->destroyQuery(rsq);
        return visible;
    }

    inline bool isVisible(const Ogre::Vector3 &origin, const Ogre::Vector3 &destination)
    {
        return isVisible(origin, destination, "GameSceneMgr", OBSTACLE_MASK, Ogre::Vector3::UNIT_Y);
    }
    
} //end of namespace util.
#endif
