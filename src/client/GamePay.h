#ifndef _GAME_PAY_H_
#define _GAME_PAY_H_

#include "cocos2d.h"
#include <string>
#include "publicDefine.h"

class GamePay
{
public:
	~GamePay(void);
private:
	GamePay(void);
public:
	virtual bool init();
	static GamePay*getInstance();
	virtual void payWithRmb(int targetID);
	virtual void payCallBackSuccess(int rmb, std::string statuesCode);
	virtual void payCallFailSuccess(std::string errorStr);
	int rmbTransitionTargetID(int rmb, PayType payType);
private:
	static GamePay*m_inst;
	PayType m_type;
};


#endif//_UI_STORE_SCENE_H_

