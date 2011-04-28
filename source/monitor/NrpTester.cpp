#include "StdAfx.h"
#include "NrpTester.h"
#include "OptionNames.h"
#include <math.h>
#include "IWorkingModule.h"
#include "INrpDevelopProject.h"

namespace nrp
{

CLASS_NAME CLASS_NRPTESTER( "tester" );	

NrpTester::NrpTester( const NrpText& systemName)
: CNrpUser( CLASS_NRPTESTER, systemName )
{
}

NrpTester::~NrpTester(void)
{
}

void NrpTester::BeginNewHour( const NrpTime& time )
{

}

void NrpTester::BeginNewDay( const NrpTime& time )
{
	if( works_.size() > 0 )
	{
		IWorkingModule& work = *(works_[ 0 ]);
		
		//���� � ������ ���� ������, �� �������� ������
		if( (int)work[ ERRORNUMBER ] > 0 )
		{
			float qualityKoeff = (int)_self[ WORK_QUALITY ] / 100.f;
			float testingSkill = (std::max)( qualityKoeff * GetSkill( SKILL_TESTING ), 1.f );

			//������� ���������� ��������� ������ �� ����������������� �������� ���
			work[ ERRORNUMBER ] -= rand() % static_cast< int >( testingSkill * 8 );
			//��� ���� ���������� ������ ������ 0
			work[ ERRORNUMBER ] = static_cast< int >( (std::max)( (int)work[ ERRORNUMBER ], 0 ) );

			//������ ���� ������ �� ������ �����, ���� ������ �� ��������
			if( (int)work[ ERRORNUMBER ] == 0 )
				(*works_[ 0 ])[ PARENT ].As<INrpDevelopProject*>()->ModuleTested( *works_[ 0 ] );
		}
		else //���� � ������ �� �������� ������, �� ������ ��� �� ������ ����� �����
			RemoveWork( work );
	}
}

NrpText NrpTester::ClassName()
{
	return CLASS_NRPTESTER;
}

void NrpTester::AddWork( IWorkingModule& module, bool toFront )
{
	//������ �� �������� � ����� ������ ��� � ������ ���������������
	if( (int)module[ ERRORNUMBER ] > 0 && GetWork( (NrpText)module[ NAME ] ) == NULL )
	{
		if( toFront )
			works_.insert( &module, 0 );
		else
			works_.push_back( &module );

		module.AddUser( *this );
	}

	_self[ WORKNUMBER ] = static_cast< int >( works_.size() );
}
}//end namespace nrp