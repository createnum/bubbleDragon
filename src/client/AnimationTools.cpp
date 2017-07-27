#include "AnimationTools.h"

USING_NS_CC;
USING_NS_CC_EXT;

AnimationTools*AnimationTools::m_inst=NULL;

AnimationTools::AnimationTools(void)
{
}


AnimationTools::~AnimationTools(void)
{
}

AnimationTools*AnimationTools::getInstance()
{
	if(!m_inst)
		m_inst=new AnimationTools();
	return m_inst;
}

CCAnimation*AnimationTools::createAnimationWithPlist(char*plist,char*picturename,int start,int num,float duration)
{
	CCSpriteFrameCache*cache=CCSpriteFrameCache::sharedSpriteFrameCache();
	cache->addSpriteFramesWithFile(plist);

	CCArray* arr=CCArray::createWithCapacity(num+1);
	char str[100]={0};

	for (int i=start;i<num+start;i++)
	{
		sprintf(str,picturename,i);
		CCSpriteFrame*frame=cache->spriteFrameByName(str);
		arr->addObject(frame);
	}

	CCAnimation*animation=CCAnimation::createWithSpriteFrames(arr,duration/num);
	arr->removeAllObjects();
	cache->removeSpriteFramesFromFile(plist);
	return animation;
}

CCParticleSystem* AnimationTools::createParticleAnimation(char*pFileName,CCPoint point)
{
	CCParticleSystem* m_emitter= CCParticleSystemQuad::create(pFileName); 
	m_emitter->setPosition(point.x, point.y);
	return m_emitter;
}

CCArmature*AnimationTools::createAnimationWithExportJson(char*pExportJsonName,char*pArmatureName,CCPoint pos)
{
	CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo(pExportJsonName);
	CCArmature*armature = CCArmature::create(pArmatureName);
	armature->getAnimation()->playWithIndex(0);
	armature->setPosition(pos);
	return armature;
}