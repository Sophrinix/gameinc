#pragma once

namespace nrp
{

typedef enum 
{ 
	PT_UNKNOWN=0, 
	PT_GAME, PT_GAMEENGINE, PT_GENRE, 
	PT_SCENARIO, PT_LICENSE, PT_SCRIPTS, 
	PT_VIDEOTECH, PT_SOUNDTECH, PT_MINIGAME,
	PT_PHYSIC, PT_ADVTECH, PT_VIDEOQUALITY,
	PT_SOUNDQUALITY, PT_SCENARIOQUALITY,
	PT_ENGINDEEXTEND, PT_GAMEBOXADDON,
	PT_LANGUAGE, PT_PLATFORM,
	PT_COUNT 
} PROJECT_TYPE;

typedef enum { GT_UNKNOWN=300, 
GT_SKILL,			//развитие навыков игрока
GT_SCROLLING,		//скролл-игры
GT_EDUCATION,		//обучающие игры(детские)
GT_TEST,				//4 проверка знаний
GT_CONTACT,			//5 виртуальное общение(возможно сетевая)
GT_HERO,				//6 развитие персонажа
GT_TOURE,			//7 исследование мира
GT_PAZZLE,			//8 головоломка
GT_QUEST,			//9
GT_PLATFORMER,		//10 
GT_STEALTH,			//11
GT_SHOOTER,			//12
GT_ACROBATIC,		//13
GT_ARCADE,			//14
GT_SPORT,			//15
GT_SIMULATOR,		//16
GT_FIGHTING,			//17
GT_RACING,			//18
GT_LIFESIM,			//19
GT_CITYBUILD,		//20
GT_MANAGER,			//21
GT_ECONOMIC,			//22
GT_SLASHER,			//23
GT_ACTION,			//24
GT_CONTROL,			//25
GT_TACTIC,			//26
GT_LOGIC,			//27
GT_WARGAME,			//28
GT_CARDGAME,			//29
GT_FOOTBALL,			//30
GT_COUNT  
} GENRE_TYPE;

}//end namespace nrp