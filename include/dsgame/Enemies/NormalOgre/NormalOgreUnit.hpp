#pragma once

#include "dsgame/Enemies/NormalOgre/NormalOgreFSM.hpp"
#include "dsgame/Enemies/FSM.hpp"
#include "dsgame/Enemies/NPCUnit.hpp"

namespace dsgame { namespace npc {

    class NormalOgreUnit : public NPCUnit
    {
    public:
        NormalOgreUnit(Ogre::Entity *entity, Ogre::Node *unit_node)
            : NPCUnit("media/initchars/NormalOgre.dsg", entity, unit_node, new NormalOgreIdleState(this), new NormalOgreStateFactory())
        {
        }

    }; //end of class NPCUnit.

} //end of namespace npc.
} //end of namespace dsgame.



