#ifndef _ANIMATION_TOOLS_
#define _ANIMATION_TOOLS_

#include "cocos2d.h"
#include "cocos-ext.h"

class AnimationTools
{
public:
	AnimationTools(void);
	~AnimationTools(void);
	static AnimationTools*m_inst;
	static AnimationTools*getInstance();
public:
	cocos2d::CCAnimation*createAnimationWithPlist(char*pListName,char*pictureName,int start,int num,float duration);
	cocos2d::CCParticleSystem*createParticleAnimation(char*pFileName, cocos2d::CCPoint point);
	cocos2d::extension::CCArmature*createAnimationWithExportJson(char*pExportJsonName,char*pArmatureName,cocos2d::CCPoint pos);
};

#endif
