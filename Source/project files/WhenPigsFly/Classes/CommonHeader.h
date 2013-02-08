#ifndef __COMMON_HEADER__
#define __COMMON_HEADER__

//Header made to contain various defines, functions and variables that will be needed
#define USING_BOX2D 1

#if USING_BOX2D
#include "Box2D\Box2D.h"

#define PTM_RATIO				(PLAYER_HEIGHT / 3.34f)
#endif

#include "cocos2d.h"
#include "Player.h"

#define SMOKE_FILENAME			"Particles/smoke.png"


#endif