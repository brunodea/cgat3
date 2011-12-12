#include "dsgame/GameUnit.hpp"

using namespace dsgame;

GameUnit::GameUnit(Ogre::Entity *entity, Ogre::Node *unit_node)
{
    init(Ogre::Vector3::ZERO,0,entity,unit_node);
}

GameUnit::GameUnit(const Ogre::Vector3 &dir, const Ogre::Real &speed, Ogre::Entity *entity, Ogre::Node *unit_node)
{
    init(dir,speed,entity,unit_node);
}

GameUnit::GameUnit(const Ogre::String &filename, Ogre::Entity *entity, Ogre::Node *unit_node)
{
    init(Ogre::Vector3::ZERO,0,entity,unit_node);
    fillGameUnitByFile(filename);
}

void GameUnit::init(const Ogre::Vector3 &dir, const Ogre::Real &speed, Ogre::Entity *entity, Ogre::Node *unit_node)
{
    m_Direction = dir;
    m_Speed = speed;
    m_pEntity = entity;
    m_isMoving = false;
    m_pAnimState = 0;
    m_pUnitNode = unit_node;
    m_Health = 0.f;
    m_Damage = 0.f;

    m_DirAngle = Ogre::Degree(0.f);
    m_Destinations.push_back(unit_node->getPosition());
}

void GameUnit::update(double timeSinceLastFrame)
{
    adjustDestinations();
    m_isMoving = shouldMove();
    if(m_isMoving)
        move(timeSinceLastFrame);

    adjustAnimationState(timeSinceLastFrame);
    if(m_pAnimState != 0)
        m_pAnimState->addTime(timeSinceLastFrame / 1000.f);
}

void GameUnit::rotate(Ogre::Degree angle)
{
    m_DirAngle += angle;
    m_pEntity->getParentNode()->yaw(angle);
}

void GameUnit::rotate(Ogre::Real degrees)
{
    rotate(Ogre::Degree(degrees));
}

Ogre::Vector3 GameUnit::getDirection()
{
    Ogre::Quaternion quat = Ogre::Quaternion(m_DirAngle,Ogre::Vector3::UNIT_Y);
    m_Direction.normalise();
    return quat*m_Direction;
}

void GameUnit::adjustDestinations()
{
    if(m_Destinations.empty())
        return;
    Ogre::Vector3 dest = m_Destinations.at(0);
    if(dest.distance(getNode()->getPosition()) < .5f) //se ja chegou no destino atual.
    {
        m_Destinations.erase(m_Destinations.begin());
    }
    if(m_Destinations.size() > 1)
    {
        auto& it = m_Destinations.begin()+1;
        if(util::isVisible(getNode()->getPosition(),*it))
            m_Destinations.erase(m_Destinations.begin());
    }
}

void GameUnit::move(double timeSinceLastFrame)
{
    if(!hasNextDestination()) 
        return;

    Ogre::Vector3 dest = getNextDestination()-m_pUnitNode->getPosition();
    dest.normalise();
    Ogre::Vector3 dir = getDirection();
    dir.normalise();
            
    if(dir.directionEquals(dest,Ogre::Radian(Ogre::Degree(.5f))) == false)
    {
        //se, após a rotação, o ângulo entre a direção e o destino for maior, significa que o certo
        //era rotacionar para o outro lado, pois este está mais perto.
        Ogre::Real init_angle = dir.angleBetween(dest).valueDegrees();
                
        Ogre::Real rot_angle = dir.angleBetween(dir+dest).valueDegrees()/(dir.distance(dest)*5.f);

        m_DirAngle += Ogre::Degree(rot_angle);
        Ogre::Real after_angle = getDirection().angleBetween(dest).valueDegrees();
        m_DirAngle -= Ogre::Degree(rot_angle);

        if(after_angle > init_angle)
            rot_angle *= -1;

        rotate(rot_angle);
    }

    m_pUnitNode->translate(getDirection()*getSpeed()*timeSinceLastFrame);
}

bool GameUnit::shouldMove()
{
    bool move = false;
    if(hasNextDestination())
    {
        move = m_pUnitNode->getPosition().distance(getNextDestination()) >= .5f;
    }

    return move;
}

void GameUnit::fillGameUnitByFile(const Ogre::String &filename)
{
    std::ifstream file(filename);
    std::string line;
    if(file.is_open())
    {
        while(file.good())
        {
            std::getline(file,line);

            Ogre::StringVector sp = Ogre::StringUtil::split(line,":");
                    
            if(sp.size() >= 2)
            {
                Ogre::StringUtil::toLowerCase(sp[0]);
                Ogre::StringUtil::trim(sp[1]);

                if(sp[0] == "direction")
                {
                    m_Direction = Ogre::StringConverter::parseVector3(sp[1]);
                }
                else if(sp[0] == "speed")
                {
                    m_Speed = Ogre::StringConverter::parseReal(sp[1]);
                }
                else if(sp[0] == "health")
                {
                    m_Health = Ogre::StringConverter::parseReal(sp[1]);
                }
                else if(sp[0] == "damage")
                {
                    m_Damage = Ogre::StringConverter::parseReal(sp[1]);
                }
            }
        }
        file.close();
    }
    else
        OgreFramework::getSingletonPtr()->m_pLog->logMessage("Couldn't open the file '" + filename + "'.");
}