#include "StdAfx.h"
#include "IUser.h"
#include "INrpProject.h"

namespace nrp
{

IUser::IUser(const char* className, const char* systemName ) : INrpConfig( className, systemName )
{
	options_[ NAME ] = new std::string( "" );
	options_[ CODE_SPEED ] = new int( 0 );
	options_[ CODE_QUALITY ] = new int( 0 );
	options_[ KNOWLEDGE_LEVEL ] = new int( 0 );
	options_[ TALANT ] = new int( 0 );
	options_[ STAMINA ] = new int( 0 );
	options_[ MOOD ] = new int( 0 );
	options_[ POPULARITY ] = new int( 0 );
	options_[ SALARY ] = new int( 0 );
	options_[ STABILITY ] = new int( 0 );
	options_[ BALANCE ] = new int( 0 );
	options_[ CHARACTER ] = new int( 0 );
	options_[ WANTMONEY ] = new int( 0 );
}

IUser::~IUser(void)
{
}

void IUser::SetSkill( int typen, int valuel )
{
	knowledges_[ typen ] = valuel;

	CalculateWantSalary_();
}


void IUser::CalculateWantSalary_()
{
	KNOWLEDGE_LIST::iterator pIter = knowledges_.begin();
	int sum = 0;
	int cash = 500;
	for( ; pIter != knowledges_.end(); ++pIter)
	{
		 sum += pIter->second * cash / 100;
		 cash *= (cash > 100 ? 0.9 : 1);
	}

	SetOption<int>( WANTMONEY, sum );
}

int IUser::GetExperience( int skillType )
{
	int sum = 0;
	switch( skillType )
	{
	case SKILL_MIDDLE:
		{
			KNOWLEDGE_LIST::iterator pIter = knowledges_.begin();
			for( ; pIter != knowledges_.end(); ++pIter)
			{
				sum += pIter->second;
				sum /= 2;
			}
		}
	break;
		 
	}

	return sum;
}
}//namespace nrp