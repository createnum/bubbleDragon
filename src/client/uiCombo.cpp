#include "uiCombo.h"

#define UI_COMBO_JSON "ui/UICombo_1.json"
#define UI_COMBO_WORD_PNG_NAME "promptWord"
#define UI_COMBO_COUNT_NUM_NAME "comboNum"
#define UI_COMBO_SCORE_NAME "comboScore"


USING_NS_CC;
using namespace gui;

UICombo::UICombo()
{

}

UICombo::~UICombo()
{

}

bool UICombo::init(int num)
{
	if(!CCNode::init())
	{
		return false;
	}

	UIWidget*pWidget=extension::GUIReader::shareReader()->widgetFromJsonFile(UI_COMBO_JSON);
	this->addChild(pWidget);

	m_promptPng=dynamic_cast<UIImageView*>(pWidget->getChildByName(UI_COMBO_WORD_PNG_NAME));
	m_comboNum=dynamic_cast<UILabelAtlas*>(pWidget->getChildByName(UI_COMBO_COUNT_NUM_NAME));
	m_comboScore=dynamic_cast<UILabelAtlas*>(pWidget->getChildByName(UI_COMBO_SCORE_NAME));
	if(!m_promptPng||!m_comboNum||!m_comboScore)
		return false;

	cc_timeval psv;
	CCTime::gettimeofdayCocos2d(&psv, NULL);
	unsigned long int rand_seed = psv.tv_sec*1000 + psv.tv_usec/1000;    
	srand(rand_seed);

	this->setComboContent(num);
	return true;
}

UICombo*UICombo::create(int num)
{
	UICombo*ptr=new UICombo();
	if (ptr&&ptr->init(num))
	{
		ptr->autorelease();
	}
	else
	{
		delete ptr;
		ptr=NULL;
	}
	return ptr;
}

int UICombo::getScoreByComboNum(int num)
{
	return num*300;
}

int UICombo::getAnRamdonPng()
{
	int temp=rand()%5;
	return temp;
}

void UICombo::setComboContent(int num)
{
	if(!m_promptPng||!m_comboNum||!m_comboScore)
		return;
	m_promptPng->loadTexture(m_comboPngName[getAnRamdonPng()].c_str());
	m_comboNum->setStringValue(CCString::createWithFormat("%d",num)->getCString());
	m_comboScore->setStringValue(CCString::createWithFormat("%d",getScoreByComboNum(num))->getCString());
}
