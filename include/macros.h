#ifndef _CGA_T3_MACROS_H_
#define _CGA_T3_MACROS_H_


typedef enum
{
    GROUND_MASK = 1 << 1,
    OBSTACLE_MASK = 2 << 1,
    DOOR_MASK = 3 << 1,
    HERO_MASK = 4 << 1,
    ENEMY_MASK = 5 << 1,
} MaskEnum;


#endif

