#include "StdAfx.h"
#include "NrpTester.h"
#include "OptionNames.h"
#include <math.h>
#include "IWorkingModule.h"

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
		
		//если в модуле есть ошибки, то потестим модуль
		if( (int)work[ ERRORNUMBER ] > 0 )
		{
			float qualityKoeff = (int)_self[ WORK_QUALITY ] / 100.f;
			float testingSkill = (std::max)( qualityKoeff * GetSkill( SKILL_TESTING ), 1.f );

			work[ ERRORNUMBER ] -= rand() % static_cast< int >( testingSkill );
			//это если количество ошибок меньше 0
			work[ ERRORNUMBER ] = static_cast< int >( (std::max)( (int)work[ ERRORNUMBER ], 0 ) );
		}
		
		//уберем этот модуль из списка задач, если ошибок не осталось
		if( (int)work[ ERRORNUMBER ] == 0 )
			RemoveWork( works_[ 0 ] );
	}
}

NrpText NrpTester::ClassName()
{
	return CLASS_NRPTESTER;
}

}//end namespace nrp