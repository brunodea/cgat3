#include "dsgame/Enemies/FSM.hpp"
#include "dsgame/Enemies/FSMFactory.hpp"

using namespace dsgame;
using namespace npc;

//FSM::FSM(NPCUnit *unit, FSMState *init_state, FSMFactory *factory)
//    : m_pState(init_state), m_pUnit(unit), m_pFactory(factory)
//{
//    m_pState->enter();
//}
//
//FSM::~FSM()
//{
//    delete m_pState;
//    delete m_pFactory;
//}
//
//void FSM::run()
//{
//    m_pState->run();
//    changeState();
//}
//
//void FSM::changeState()
//{
//    if(m_pState->shouldChangeState())
//    {
//        m_pState->leave();
//        FSMState *tmp = m_pFactory->createState(m_pUnit, m_pState->nextState());
//
//        delete m_pState;
//        m_pState = tmp;
//    
//        m_pState->enter();
//    }
//}
