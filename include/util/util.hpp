#ifndef _CGA_T3_UTIL_HPP_
#define _CGA_T3_UTIL_HPP_

#include "OgreFramework.hpp"

namespace util
{
    inline bool isVisible(const Ogre::Vector3 &origin, const Ogre::Vector3 &destination, const Ogre::String &scene_mgr_name, unsigned long mask, const Ogre::Vector3 &offset = Ogre::Vector3::ZERO)
    {
        Ogre::Ray ray(origin+offset,destination+offset);
        Ogre::RaySceneQuery *rsq = Ogre::Root::getSingletonPtr()->getSceneManager(scene_mgr_name)->createRayQuery(ray,mask);
        rsq->setSortByDistance(true,1);

        bool visible = false;
        if(rsq->execute().empty())
            visible = true;
        Ogre::Root::getSingletonPtr()->getSceneManager(scene_mgr_name)->destroyQuery(rsq);
        return visible;
    }

} //end of namespace util.
#endif
