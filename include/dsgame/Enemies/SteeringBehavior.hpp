#pragma once

#include "dsgame/Enemies/NPCUnit.hpp"
#include "macros.h"

namespace dsgame { namespace npc {

    class SteeringBehavior
    {
    public:
        static Ogre::Vector3 calculateSteering(NPCUnit *unit, const Ogre::Vector3 &dest, SteeringTypeEnum type)
        {
            switch(type)
            {
            case STEERING_IDLE: return Ogre::Vector3::ZERO;
            case STEERING_SEEK: return seek(unit,dest);
            default: return Ogre::Vector3::ZERO;
            }
        }

    private:
        
        static Ogre::Vector3 seek(NPCUnit *unit, const Ogre::Vector3 &dest);
    };// end of class SteeringBehavior.
} //end of namespace npc.
} //end of namespace dsgame.
