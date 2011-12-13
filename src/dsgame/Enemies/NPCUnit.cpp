#include "dsgame/Enemies/NPCUnit.hpp"
#include "dsgame/Enemies/FSM.hpp"

using namespace dsgame;
using namespace npc;

NPCUnit::NPCUnit(const Ogre::String &filename, Ogre::Entity *entity, Ogre::Node *unit_node, FSMState *init_state, FSMFactory *factory)
    : GameUnit(entity, unit_node), m_FSM(new FSM(this, init_state, factory))
{
    m_OrigPos = unit_node->getPosition();
    m_VisibilityRadius = 0.f;
    m_VisionAngleDegrees = 0.f;

    void(NPCUnit::*fill_ptr)(const Ogre::String&, const Ogre::String&) = &NPCUnit::fillSpecificAttr;
    fillGameUnitByFile(filename, reinterpret_cast<void(GameUnit::*)(const Ogre::String&, const Ogre::String&)>(fill_ptr));
}

NPCUnit::~NPCUnit()
{
    delete m_FSM;
}

void NPCUnit::fillSpecificAttr(const Ogre::String &attribute, const Ogre::String &value)
{
    if(attribute == "visibility radius")
        m_VisibilityRadius = Ogre::StringConverter::parseReal(value);
    else if(attribute == "vision angle")
        m_VisionAngleDegrees = Ogre::Degree(Ogre::StringConverter::parseAngle(value));
}

FSM::FSM(NPCUnit *unit, FSMState *init_state, FSMFactory *factory)
    : m_pState(init_state), m_pUnit(unit), m_pFactory(factory)
{
    m_pState->enter();
}

FSM::~FSM()
{
    delete m_pState;
    delete m_pFactory;
}

void FSM::run()
{
    m_pState->run();
    changeState();
}

void FSM::changeState()
{
    if(m_pState->shouldChangeState())
    {
        m_pState->leave();
        FSMState *tmp = m_pFactory->createState(m_pUnit, m_pState->nextState());

        delete m_pState;
        m_pState = tmp;
    
        //OgreFramework::getSingletonPtr()->m_pLog->logMessage(Ogre::StringConverter::toString(m_pState->getType()));
        m_pState->enter();
    }
}

