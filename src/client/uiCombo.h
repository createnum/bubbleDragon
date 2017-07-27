#ifndef _UI_COMBO_H_
#define _UI_COMBO_H_

#include "cocos2d.h"
#include "cocos-ext.h"

static std::string m_comboPngName[]=
{
	"ui/myRes/wenzibucuoo.png",
	"ui/myRes/wenzikubile.png",
	"ui/myRes/wenzishuaidaile.png",
	"ui/myRes/wenzizhenbang.png",
	"ui/myRes/wenzizhenlihai.png",
};

class UICombo:public cocos2d::CCNode
{
public:
	UICombo();
	~UICombo();
public:
	virtual bool init(int num);
	static UICombo*create(int num);
	virtual int getScoreByComboNum(int num);
	virtual int getAnRamdonPng();
	virtual void setComboContent(int num);
private:
	cocos2d::gui::UIImageView*m_promptPng;
	cocos2d::gui::UILabelAtlas*m_comboNum;
	cocos2d::gui::UILabelAtlas*m_comboScore;

};

#endif//_UI_HELP_SCENE_H_

