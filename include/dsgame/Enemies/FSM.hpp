#pragma once

#include "dsgame/Enemies/NPCUnit.hpp"
#include "dsgame/HeroUnit.hpp"
#include "dsgame/Enemies/FSMFactory.hpp"
#include "dsgame/Enemies/FSMState.hpp"
#include "macros.h"

//class dsgame::npc::NPCUnit;
//MOVIDO PARA NPCUNIT.HPP
namespace dsgame { namespace npc {
    
    //class FSM
    //{
    //public:
    //    FSM(NPCUnit *unit, FSMState *init_state, FSMFactory *factory);
    //    ~FSM();

    //    void run();
    //    void changeStateTo(FSMStateEnum new_state)
    //    {
    //        m_pState->setNextState(new_state);
    //        changeState();
    //    }

    //    FSMState *getCurrState() { return m_pState; }

    //private:
    //    void changeState();

    //private:
    //    FSMState *m_pState;
    //    FSMFactory *m_pFactory;

    //    NPCUnit *m_pUnit;

    //}; //end of class FSM.

} //end of namespace npc.
} //end of namespace dsgame.