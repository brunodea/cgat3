#pragma once

#include "util/ViewPointsGraph.h"

using namespace util;

ViewPointsGraph::ViewPointsGraph()
    : m_Nodes(), m_bGraphIsCreated(false)
{
}

ViewPointsGraph::~ViewPointsGraph()
{
    erase();
}

void ViewPointsGraph::createGraph(const std::vector<Ogre::Vector3> &points)
{
    erase();
    for(auto& it1 = points.begin(); it1 != points.end(); it1++)
    {
        Ogre::Vector3 orig = *it1;
        Node *n = new Node();
        n->position = orig;
        n->m_Neighbors = std::vector<Node *>();
        n->m_bVisited = false;
        m_Nodes.push_back(n);
    }

    for(auto& it = m_Nodes.begin(); it != m_Nodes.end(); it++)
    {
        adjustNeighbors(*it);
    }
    m_bGraphIsCreated = true;
}

std::vector<Ogre::Vector3> ViewPointsGraph::pathFindingAStar(const Ogre::Vector3 &curr_pos, const Ogre::Vector3 &dest)
{
    std::vector<Ogre::Vector3> result;
    if(m_Nodes.size() < 2)
        return result;
    Node *closest_visible = 0;
    Ogre::Real cd = 99999.f;
    for(auto& node_it = m_Nodes.begin(); node_it != m_Nodes.end(); node_it++)
    {
        Ogre::Vector3 &pos = (*node_it)->position;
        if(util::isVisible(curr_pos,pos) && pos.distance(curr_pos) < cd)
        {
            cd = pos.distance(curr_pos);
            closest_visible = *node_it;
        }
    }
    //result.push_back(closest_visible->position);
    std::vector<Node *> v = astar(closest_visible, dest);
    unvisitNodes();
    for(auto& it = v.begin(); it != v.end(); it++)
    {
        result.push_back((*it)->position);
    }
    result.push_back(dest);

    return result;
}

void ViewPointsGraph::adjustNeighbors(Node *node)
{
    OgreFramework::getSingletonPtr()->m_pLog->logMessage(Ogre::StringConverter::toString(node->position));
    for(auto& it = m_Nodes.begin(); it != m_Nodes.end(); it++)
    {
        Node *n = *it;
        if(n != node && util::isVisible(node->position,n->position))
        {
            OgreFramework::getSingletonPtr()->m_pLog->logMessage("VEIO");
            node->m_Neighbors.push_back(n);
        }
    }
}

std::vector<ViewPointsGraph::Node *> ViewPointsGraph::astar(Node *n, const Ogre::Vector3 &dest)
{
    n->m_bVisited = true;
    OgreFramework::getSingletonPtr()->m_pLog->logMessage(Ogre::StringConverter::toString(n->position)+ " DEST: "+Ogre::StringConverter::toString(dest));
    if(n == 0)
        return std::vector<Node *>();
    Ogre::Vector3 nodepos = n->position;
    if(util::isVisible(nodepos,dest))
    {
        std::vector<Node *> v;
        v.push_back(n);
        return v;
    }
    else
    {
        Node *result = 0;
        Ogre::Real best_func = 999999.f;
        for(auto& node_it = n->m_Neighbors.begin(); node_it != n->m_Neighbors.end(); node_it++)
        {
            Ogre::Vector3 npos = (*node_it)->position;
            Ogre::Real heuristic = npos.distance(dest);
            Ogre::Real weight = n->position.distance(npos);

            if(weight + heuristic < best_func && (*node_it)->m_bVisited == false)
            {
                best_func = weight + heuristic;
                result = *node_it;
            }
        }
                
        std::vector<Node *> v = astar(result, dest);
        v.insert(v.begin(),n);
        return v;
    }
}

void ViewPointsGraph::erase()
{
    for(auto& it = m_Nodes.begin(); it != m_Nodes.end(); it++)
        delete *it;
    m_bGraphIsCreated = false;
}

void ViewPointsGraph::unvisitNodes()
{
    for(auto& it = m_Nodes.begin(); it != m_Nodes.end(); it++)
    {
        (*it)->m_bVisited = false;
    }
}
