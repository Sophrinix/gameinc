/************************************************************************/
/* Класс для хранения данных об скриншотах и сплешшотах игры            */
/************************************************************************/

#ifndef _INCLUDE_NRPSCREENSHOT_
#define _INCLUDE_NRPSCREENSHOT_

#include "nrpConfig.h"
#include "nrpArrays.h"

namespace nrp
{
OPTION_NAME IMAGESNUMBER( "imagesNumber" );
OPTION_NAME IMAGESBOXNUMBER( "imagesBoxNumber" );

class CNrpGame;

class CNrpExtInfo : public INrpConfig
{

public:
	CNrpExtInfo( const NrpText& fileName );
	~CNrpExtInfo(void);

	//Возвращает набор сплешшотов
	const STRINGS& GetBoxImages() { return _imagesBoxPath; }
	//Возвращает набор скриншотов
	const STRINGS& GetImages() { return _imagesPath; }

	const STRINGS& GetRecenses() { return _recenses; }

	//Возвращает уровень соответствия игры и этого набора скриншотов
	int GetEqualeRating( const CNrpGame& game );

	bool IsMyYear( int year );

	static NrpText ClassName();
private:
	void InitializeOptions_();
	CNrpExtInfo();
	CNrpExtInfo( const CNrpExtInfo& a );

	//массив для хранения путей скринов
	STRINGS _imagesPath;
	//массив для хранения путей к сплэнам
	STRINGS _imagesBoxPath;

	//массив для хранения жанров, для которых подходит этот набор скринов
	STRINGS _genres;
	STRINGS _recenses;

	//Загружать можно только через конструктор
	void Load( const NrpText& fileName );
	//Сохранять нельзя
	NrpText Save( const NrpText& fileName ) { return ""; };

	//Находит в указанной папке картинки и формирует массив
	void _LoadImages( STRINGS& imgs, const NrpText& folder, const NrpText& prefix );
};

}//end namespace nrp

#endif