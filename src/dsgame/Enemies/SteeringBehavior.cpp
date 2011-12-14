#include "dsgame/Enemies/SteeringBehavior.hpp"

using namespace dsgame;
using namespace npc;

Ogre::Vector3 SteeringBehavior::seek(NPCUnit *unit, const Ogre::Vector3 &dest)
{
    Ogre::Vector3 des_vel;
    des_vel = dest-unit->getNode()->getPosition();
    des_vel.normalise();
    des_vel *= unit->getMaxSpeed();

    return des_vel-unit->getVelocity();
}
