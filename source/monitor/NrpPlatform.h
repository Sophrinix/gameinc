#pragma once
#include "INrpProject.h"
#include "nrpArrays.h"

//Класс описывает игровую платформу
namespace nrp
{

OPTION_NAME MAXWIDTH("rwidth");
OPTION_NAME MAXHEIGHT("rheight");
OPTION_NAME SELLDEVICE("selldevice");

class CNrpPlatform : public INrpProject
{
public:
	CNrpPlatform( const NrpText& fileName );
	~CNrpPlatform(void);

	NrpText Save( const NrpText& pathTo );
	void Load( const NrpText& pathTo );
	const TECHS& GetTechsList() { return _techs; }
	CNrpTechnology* GetTech( const NrpText& name );

	static NrpText ClassName();
private:
	CNrpPlatform(void);
	CNrpPlatform( const CNrpPlatform& p );
	void _InitialyzeOptions();

	TECHS _techs;
};

}//end namespace nrp