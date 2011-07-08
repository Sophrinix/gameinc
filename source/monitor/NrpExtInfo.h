/************************************************************************/
/* ����� ��� �������� ������ �� ���������� � ���������� ����            */
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

	//���������� ����� ����������
	const STRINGS& GetBoxImages() { return _imagesBoxPath; }
	//���������� ����� ����������
	const STRINGS& GetImages() { return _imagesPath; }

	const STRINGS& GetRecenses() { return _recenses; }

	//���������� ������� ������������ ���� � ����� ������ ����������
	int GetEqualeRating( const CNrpGame& game );

	bool IsMyYear( int year );

	static NrpText ClassName();
private:
	void InitializeOptions_();
	CNrpExtInfo();
	CNrpExtInfo( const CNrpExtInfo& a );

	//������ ��� �������� ����� �������
	STRINGS _imagesPath;
	//������ ��� �������� ����� � �������
	STRINGS _imagesBoxPath;

	//������ ��� �������� ������, ��� ������� �������� ���� ����� �������
	STRINGS _genres;
	STRINGS _recenses;

	//��������� ����� ������ ����� �����������
	void Load( const NrpText& fileName );
	//��������� ������
	NrpText Save( const NrpText& fileName ) { return ""; };

	//������� � ��������� ����� �������� � ��������� ������
	void _LoadImages( STRINGS& imgs, const NrpText& folder, const NrpText& prefix );
};

}//end namespace nrp

#endif