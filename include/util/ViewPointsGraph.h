#ifndef _CGA_T3_VIEW_POINTS_GRAPH_H_
#define _CGA_T3_VIEW_POINTS_GRAPH_H_

#include "OgreFramework.hpp"
#include "util.hpp"
#include "macros.h"
#include <vector>

namespace util
{
    class ViewPointsGraph
    {
    public:
        ViewPointsGraph()
            : m_Nodes()
        {
        }

        ~ViewPointsGraph()
        {
            for(auto& it = m_Nodes.begin(); it != m_Nodes.end(); it++)
                delete *it;
        }

    private:

        struct Node
        {
            Ogre::Vector3 position;
            std::vector<Node *> m_Neighbors;
        }; //end of struct Node.

    private:

        void createGraph(const std::vector<Ogre::Vector3> &points)
        {
            for(auto& it1 = points.begin(); it1 != points.end(); it1++)
            {
                Ogre::Vector3 orig = *it1;
                Node *n = new Node();
                n->position = orig;
                m_Nodes.push_back(n);
            }

            for(auto& it1 = m_Nodes.begin(); it1 != m_Nodes.end(); it1++)
            {
                Node *n = *it1;
                for(auto& it2 = points.begin(); it2 != points.end(); it2++)
                {
                    Ogre::Vector3 dest = *it2;
                    if(n->position != dest)
                    {
                        if(util::isVisible(n->position,dest,"GameSceneMgr",OBSTACLE_MASK))
                        {
                            n->m_Neighbors.push_back(dest);
                        }
                    }
                }
            }
        }

    private:
        std::vector<Node *> m_Nodes;
    }; //end of class ViewPointsGraph.

} //end of namespace util.

#endif
