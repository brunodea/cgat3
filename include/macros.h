#ifndef _CGA_T3_MACROS_H_
#define _CGA_T3_MACROS_H_

#define VPGRAPH ViewPointsGraph::getInstance()

typedef enum
{
    GROUND_MASK = 1 << 1,
    OBSTACLE_MASK = 2 << 1,
    DOOR_MASK = 3 << 1,
    HERO_MASK = 4 << 1,
    ENEMY_MASK = 5 << 1,
} MaskEnum;

typedef enum
{
    IDLE_STATE = 0,
    SEEK_STATE,
    ATTACK_STATE,
    RETURN_STATE,
    LIFE_LOW_STATE
} FSMStateEnum;

#endif

