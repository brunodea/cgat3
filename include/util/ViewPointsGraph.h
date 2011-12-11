#ifndef _CGA_T3_VIEW_POINTS_GRAPH_H_
#define _CGA_T3_VIEW_POINTS_GRAPH_H_

#include "OgreFramework.hpp"
#include "util/util.hpp"
#include "macros.h"
#include <vector>

namespace util
{
    class ViewPointsGraph
    {
    public:
        ViewPointsGraph()
            : m_Nodes(), m_bGraphIsCreated(false)
        {
        }

        ~ViewPointsGraph()
        {
            erase();
        }

        bool isCreated() { return m_bGraphIsCreated; }
        
        void createGraph(const std::vector<Ogre::Vector3> &points)
        {
            erase();
            for(auto& it1 = points.begin(); it1 != points.end(); it1++)
            {
                Ogre::Vector3 orig = *it1;
                Node *n = new Node();
                n->position = orig;
                n->m_Neighbors = std::vector<Node *>();
                m_Nodes.push_back(n);
            }

            for(auto& it = m_Nodes.begin(); it != m_Nodes.end(); it++)
            {
                adjustNeighbors(*it);
            }
            m_bGraphIsCreated = true;
        }

        std::vector<Ogre::Vector3> pathFindingAStar(const Ogre::Vector3 &curr_pos, const Ogre::Vector3 &dest)
        {
            std::vector<Ogre::Vector3> result;
            if(m_Nodes.size() < 2)
                return result;

            Node *closest_visible = m_Nodes.at(0);
            for(auto& node_it = m_Nodes.begin()+1; node_it != m_Nodes.end(); node_it++)
            {
                Ogre::Vector3 &pos = (*node_it)->position;
                if(util::isVisible(curr_pos,pos,"GameSceneMgr",OBSTACLE_MASK,Ogre::Vector3::UNIT_Y) &&
                   pos.distance(curr_pos) < closest_visible->position.distance(curr_pos))
                {
                    closest_visible = *node_it;
                }
            }

            std::vector<Node *> v = bestNodeAStar(closest_visible,dest);
            for(auto& it = v.begin(); it != v.end(); it++)
            {
                result.push_back((*it)->position);
            }

            return result;
        }

    private:

        struct Node
        {
            Ogre::Vector3 position;
            std::vector<Node *> m_Neighbors;
        }; //end of struct Node.
        
        std::vector<Node *> bestNodeAStar(Node *n, const Ogre::Vector3 &dest)
        {
            Ogre::Vector3 nodepos = n->position;
            if(util::isVisible(nodepos,dest,"GameSceneMgr",OBSTACLE_MASK,Ogre::Vector3::UNIT_Y))
            {
                std::vector<Node *> v;
                v.push_back(n);
                return v;
            }
            else
            {
                Node *result = 0;
                Ogre::Real best_func = 9999.f;
                for(auto& node_it = n->m_Neighbors.begin(); node_it != n->m_Neighbors.end(); node_it++)
                {
                    Ogre::Vector3 npos = (*node_it)->position;
                    Ogre::Real heuristic = npos.distance(dest);
                    Ogre::Real weight = n->position.distance(npos);

                    if(weight + heuristic < best_func)
                    {
                        best_func = weight + heuristic;
                        result = *node_it;
                    }
                }
                
                std::vector<Node *> v = bestNodeAStar(result,dest);
                v.insert(v.begin(),result);
                return v;
            }
        }

        void erase()
        {
            for(auto& it = m_Nodes.begin(); it != m_Nodes.end(); it++)
                delete *it;
        }

    private:
        std::vector<Node *> m_Nodes;
    }; //end of class ViewPointsGraph.

} //end of namespace util.

#endif
