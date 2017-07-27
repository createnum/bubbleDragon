#ifndef _PUBLIC_DEFINE_H_
#define _PUBLIC_DEFINE_H_

#define GAME_WIDCH 640
#define GAME_HEIGHT 960

#define NUM_OF_ONE_PAGE  20
#define NUM_OF_X_LINE 4
#define NUM_OF_Y_LINE 4
#define STAGE_BUTTON_START_POS ccp(178,706)
#define STAGE_BUTTON_X_SPACEING 140
#define STAGE_BUTTON_Y_SPACEING 140

#define STAGE_SELECT_PAGE_LABEL_STARTPOS ccp(320,166)		//页面标签起始位置
#define STAGE_SELECT_PAGE_LABEL_SPACEING 60						//便签间距
#define SHOW_BUBBLE_MAX_LINE 13

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) 
#define MUSIC_STARTSCENE_BG_MUSIC "sound/farm.mp3"
#define MUSIC_GAMESCENE_BG_MUSIC "sound/music_blue.wav"
#define EFFECT_BUTTON_EFFECT "sound/bubble_burst.wav"
#define EFFECT_READYGO_EFFECT "sound/readygo2.wav"
#define EFFECT_CLEAR1_EFFECT "sound/bubble_pop1.wav"
#define EFFECT_CLEAR2_EFFECT "sound/bubble_pop2.wav"
#define EFFECT_CLEAR3_EFFECT "sound/bubble_pop3.wav"
#define EFFECT_CLEAR4_EFFECT "sound/bubble_pop4.wav"
#define EFFECT_CLEAR5_EFFECT "sound/bubble_pop5.wav"
#define EFFECT_CLEAR6_EFFECT "sound/bubble_pop8.wav"
#define EFFECT_COMBO1_EFFECT "sound/bubble_great_vo.wav"
#define EFFECT_COMBO2_EFFECT "sound/bubble_oops_6_JP.wav"
#define EFFECT_COMBO3_EFFECT "sound/bubble_perfect_11_JP.wav"
#define EFFECT_COMBO4_EFFECT "sound/ye.wav"
#define EFFECT_BOMB_EFFECT "sound/bomb2.wav"
#define EFFECT_HAMMER_EFFECT "sound/chuzihit.wav"
#define EFFECT_LIGHTING_EFFECT "sound/shandian.wav"
#define EFFECT_GOLD_EFFECT "sound/Effect_gold.wav"
#define EFFECT_PASSGAME_EFFECT "sound/bubble_exp.wav"
#define EFFECT_PASSFAILED_EFFECT "sound/failed.wav"
#define EFFECT_BUBBLEHIT_EFFECT "sound/hit_bubble.wav"
#else
#define MUSIC_STARTSCENE_BG_MUSIC "sound/farm.mp3"
#define MUSIC_GAMESCENE_BG_MUSIC "sound/music_blue.mp3"
#define EFFECT_BUTTON_EFFECT "sound/buttonEffect.ogg"
#define EFFECT_READYGO_EFFECT "sound/readygo.ogg"
#define EFFECT_CLEAR1_EFFECT "sound/bubble_pop1.ogg"
#define EFFECT_CLEAR2_EFFECT "sound/bubble_pop2.ogg"
#define EFFECT_CLEAR3_EFFECT "sound/bubble_pop3.ogg"
#define EFFECT_CLEAR4_EFFECT "sound/bubble_pop4.ogg"
#define EFFECT_CLEAR5_EFFECT "sound/bubble_pop5.ogg"
#define EFFECT_CLEAR6_EFFECT "sound/bubble_pop8.ogg"
#define EFFECT_COMBO1_EFFECT "sound/bubble_great_vo.ogg"
#define EFFECT_COMBO2_EFFECT "sound/bubble_oops_6_JP.ogg"
#define EFFECT_COMBO3_EFFECT "sound/bubble_perfect_11_JP.ogg"
#define EFFECT_COMBO4_EFFECT "sound/ye.ogg"
#define EFFECT_BOMB_EFFECT "sound/bomb.ogg"
#define EFFECT_HAMMER_EFFECT "sound/chuzihit.ogg"
#define EFFECT_LIGHTING_EFFECT "sound/shandian.ogg"
#define EFFECT_GOLD_EFFECT "sound/Effect_gold.ogg"
#define EFFECT_PASSGAME_EFFECT "sound/passGameSuccess.ogg"
#define EFFECT_PASSFAILED_EFFECT "sound/failed.ogg"
#define EFFECT_BUBBLEHIT_EFFECT "sound/hit_bubble.ogg"
#endif

enum PayType
{
	PAY_NORMAL=0,
	PAY_RESURRECTION_GIFT,
	PAY_HOT_GIFT,
	PAY_UNLOCK,
	PAY_OWNUSE,
};

enum MOBILE_TYPE{
	CHINA_NONE = -1,
	CHINA_MOBILE,
	CHINA_TELECOM,
	CHINA_UNICOM
};

#endif//_PUBLIC_DEFINE_H_