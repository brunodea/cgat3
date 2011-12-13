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
        static ViewPointsGraph* getInstance();

        ~ViewPointsGraph();

        bool isCreated() { return m_bGraphIsCreated; }
        
        void createGraph(const std::vector<Ogre::Vector3> &points);
        std::vector<Ogre::Vector3> pathFindingAStar(const Ogre::Vector3 &curr_pos, const Ogre::Vector3 &dest);

    private:

        struct Node
        {
            Ogre::Vector3 position;
            std::vector<Node *> m_Neighbors;
            bool m_bVisited;
        }; //end of struct Node.

    private:
        ViewPointsGraph();

        void adjustNeighbors(Node *node);
        std::vector<Node *> astar(Node *n, const Ogre::Vector3 &dest);
        void erase();
        void unvisitNodes();

    private:
        static ViewPointsGraph *m_sInstance;

        std::vector<Node *> m_Nodes;
        bool m_bGraphIsCreated;

    }; //end of class ViewPointsGraph.

} //end of namespace util.

#endif
