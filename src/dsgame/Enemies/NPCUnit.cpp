#include "dsgame/Enemies/NPCUnit.hpp"
#include "dsgame/Enemies/FSM.hpp"
#include "util/util.hpp"
#include "dsgame/Enemies/SteeringBehavior.hpp"


using namespace dsgame;
using namespace npc;

NPCUnit::NPCUnit(const Ogre::String &filename, Ogre::Entity *entity, Ogre::Node *unit_node, FSMState *init_state, FSMFactory *factory)
    : GameUnit(entity, unit_node), m_FSM(new FSM(this, init_state, factory))
{
    m_OrigPos = unit_node->getPosition();
    m_VisibilityRadius = 0.f;
    m_VisionAngleDegrees = 0.f;
    m_Mass = 0.f;
    m_MaxSpeed = getSpeed();
    m_MaxForce = 0.f;
    m_CurrSteeringType = STEERING_IDLE;

    void(NPCUnit::*fill_ptr)(const Ogre::String&, const Ogre::String&) = &NPCUnit::fillSpecificAttr;
    fillGameUnitByFile(filename, reinterpret_cast<void(GameUnit::*)(const Ogre::String&, const Ogre::String&)>(fill_ptr));

    m_Velocity = getDirection()*getSpeed();
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
    else if(attribute == "mass")
        m_Mass = Ogre::StringConverter::parseReal(value);
    else if(attribute == "max speed")
        m_MaxSpeed = Ogre::StringConverter::parseReal(value);
    else if(attribute == "max force")
        m_MaxForce = Ogre::StringConverter::parseReal(value);
}

void NPCUnit::move(double timeSinceLastFrame)
{
    if(!hasNextDestination())
        return;

    Ogre::Vector3 dir = getDirection().normalisedCopy();
    Ogre::Vector3 steering_force = SteeringBehavior::calculateSteering(this,getNextDestination(),m_CurrSteeringType);
    Ogre::Vector3 acc = steering_force / getMass();

    m_Velocity += acc;
    //m_Velocity.makeCeil(dir*getMaxSpeed());

    rotateTo(m_Velocity.normalisedCopy());

    //getNode()->setPosition(new_pos);
    getNode()->translate(m_Velocity*timeSinceLastFrame);
    //getNode()->translate(getDirection()*getSpeed()*timeSinceLastFrame);
}

bool NPCUnit::isVisible(const Ogre::Vector3& dest)
{
    Ogre::Vector3 unit_pos = getNode()->getPosition();
    Ogre::Real dist = unit_pos.distance(dest);
    if(dist > m_VisibilityRadius) return false;

    //Ogre::Degree angle = Ogre::Degree(getDirection().angleBetween(dest-unit_pos));
    //OgreFramework::getSingletonPtr()->m_pLog->logMessage(Ogre::StringConverter::toString(angle));
    //if(angle > m_VisionAngleDegrees/2.f) return false;

    return util::isVisible(unit_pos, dest);
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

