#ifndef __COMMON_HEADER_H__
#define __COMMON_HEADER_H__

//Header made to contain various defines, functions and variables that will be needed
#define USING_BOX2D 1

#define uint					unsigned int
#define VISIBLESIZE				CCDirector::sharedDirector()->getVisibleSize()
#define ORIGIN					CCDirector::sharedDirector()->getVisibleOrigin()

#if USING_BOX2D

#include "Box2D\Box2D.h"

#define PTM_RATIO				(VISIBLESIZE.width / 33.4f)
#endif

// Each class needs these
#include "cocos2d.h"
#include "FilenameDefines.h"

// Other files that are commanly used
#include "Player.h"


#endif // __COMMON_HEADER_H__