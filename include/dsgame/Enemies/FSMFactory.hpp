#pragma once

#include "dsgame/Enemies/FSM.hpp"
#include "dsgame/Enemies/NPCUnit.hpp"
#include "dsgame/Enemies/FSMState.hpp"
#include "macros.h"


namespace dsgame { namespace npc {
    class NPCUnit;

    class FSMFactory
    {
    public:
        FSMState *createState(NPCUnit *unit, FSMStateEnum state_type)
        {
            FSMState *state = 0;
            switch(state_type)
            {
            case IDLE_STATE:
                state = createIdleState(unit);
                break;
            case SEEK_STATE:
                state = createSeekState(unit);
                break;
            case ATTACK_STATE:
                state = createAttackState(unit);
                break;
            case RETURN_STATE:
                state = createReturnState(unit);
                break;
            case LIFE_LOW_STATE:
                state = createLifeLowState(unit);
                break;
            }

            return state;
        }

        virtual class FSMState *createIdleState(NPCUnit *unit) = 0;
        virtual class FSMState *createSeekState(NPCUnit *unit) = 0;
        virtual class FSMState *createAttackState(NPCUnit *unit) = 0;
        virtual class FSMState *createReturnState(NPCUnit *unit) = 0;
        virtual class FSMState *createLifeLowState(NPCUnit *unit) = 0;

    }; //end of class FSMFactory.

} //end of namespace npc.
} //end of namespace dsgame.
