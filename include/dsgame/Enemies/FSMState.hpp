#pragma once

#include "dsgame/Enemies/NPCUnit.hpp"

namespace dsgame { namespace npc {
    class NPCUnit;

    class FSMState
    {
    public:
        FSMState(NPCUnit *unit, FSMStateEnum state_type)
            : m_pUnit(unit), m_StateType(state_type), m_NextState(state_type)
        {}

        virtual void enter() = 0;
        virtual void run() = 0;
        virtual void leave() = 0;

        bool shouldChangeState() { return m_NextState != m_StateType; }
        FSMStateEnum nextState() { return m_NextState; }
        void setNextState(FSMStateEnum new_state) { m_NextState = new_state; }
        FSMStateEnum getType() { return m_StateType; }

    protected:
        NPCUnit *m_pUnit;
        FSMStateEnum m_StateType;
        FSMStateEnum m_NextState;
    }; //end of class FSMState.

} //end of namespace npc.
} //end of namespace dsgame.