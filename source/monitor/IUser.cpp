#include "StdAfx.h"
#include "IUser.h"
#include "INrpProject.h"
#include "IniFile.h"
#include "NrpCompany.h"
#include "NrpTechnology.h"
#include "NrpGameProject.h"
#include "NrpProjectModule.h"
#include "INrpDevelopProject.h"
#include "NrpInvention.h"
#include "nrpScript.h"
#include "NrpRelation.h"
#include "OpFileSystem.h"
#include "NrpConfigModificator.h"
#include "NrpTime.h"

#include <io.h>
#include <errno.h>
#include <assert.h>

namespace nrp
{
CLASS_NAME CLASS_USER( "IUser" );
CLASS_NAME CLASS_CODER( "coder" );
CLASS_NAME CLASS_DESIGNER( "designer" );
CLASS_NAME CLASS_COMPOSER( "composer" );

CNrpUser::CNrpUser(const NrpText& className, const NrpText& systemName ) : INrpConfig( className.size() ? className : CLASS_USER, systemName )
{
	assert( systemName.size() );
	RegProperty<NrpText>( NAME, systemName ); //���
	RegProperty<int>( WORK_SPEED, 0 );	//�������� ������
	RegProperty<int>( WORK_QUALITY, 0 ); //�������� ������
	RegProperty<int>( WORK_QUALITY_AWARD, 0 );	//��������� ������� �� �������� ������
	RegProperty<int>( KNOWLEDGE_LEVEL, 0 );	//������� ������
	RegProperty<NrpText>( INTERNAL_NAME, systemName );	//���������� ���
	RegProperty<NrpText>( TECHGROUP, className );	//����� ������������
	RegProperty<int>( TALANT, 0 );			//���������� ����������� �������
	RegProperty<int>( STAMINA, 0 );			//������������
	RegProperty<int>( MOOD, 100 );			//����������
	RegProperty<int>( POPULARITY, 0 );		//����������� �������
	RegProperty<int>( SALARY, 0 );			//��������
	RegProperty<int>( STABILITY, 0 );		//����������������
	RegProperty<int>( BALANCE, 0 );			//�������
	RegProperty<int>( CHARACTER, 0 );		//��� ���������
	RegProperty<int>( WANTMONEY, 0 );		//������� ����� ����� �� ���� ������
	RegProperty<int>( CONTRACTMONEY, 0 );		//������� ����� �������� ��� ���������� ���������
	RegProperty<int>( WORKNUMBER, 0 );		//������� � �������
	RegProperty<int>( LAST_AWARD, 0 );			//�������� ������
	RegProperty<NrpText>( USERSTATE, "readyToWork" );		
	RegProperty<NrpText>( ROOMSTATE, "unknown" );
	RegProperty<int>( HANGRY, 100 );			//�����
	RegProperty<PNrpCompany>( PARENTCOMPANY, NULL );	//�������� � ������� ��������
	RegProperty<int>( EXPERIENCE, 0 );			//����� ����
	RegProperty<NrpText>( TEXTURENORMAL, "" );		//����
	RegProperty<int>( ALL_SKILL_SUMM, 0 );		
	RegProperty<int>( ALCOHOL, 0 );				//��������� � �������
}

CNrpUser::~CNrpUser(void)
{
}

void CNrpUser::SetSkill( const NrpText& name, int valuel )
{
	knowledges_[ name ] = valuel;

	CalculateWantSalary_();
}

void CNrpUser::CalculateWantSalary_()
{
	KNOWLEDGE_MAP::Iterator pIter = knowledges_.getIterator();
	float sum = 0;
	float cash = 500;
	for( ; !pIter.atEnd(); pIter++)
	{
		 sum += pIter->getValue() * cash / 100;
		 cash *= (cash > 100 ? 0.9f : 1);
	}

	sum += (int)_self[ WORK_QUALITY ] * log( static_cast< float>( (int)_self[ WORK_QUALITY ] ) );

	_self[ WANTMONEY ] = (int)sum;

	CalculateKnowledgeLevel_();
}

void CNrpUser::CalculateKnowledgeLevel_()
{
	int sum = 0;
	int allKnowledge = 0;
	KNOWLEDGE_MAP::Iterator pIter = knowledges_.getIterator();
	for( ; !pIter.atEnd(); pIter++)
	{
		sum += pIter->getValue();
		sum /= 2;

		allKnowledge += pIter->getValue();
	}

	pIter = genreExperience_.getIterator();
	for( ; !pIter.atEnd(); pIter++ )
		allKnowledge += pIter->getValue();

	pIter = genrePreferences_.getIterator();
	for( ; !pIter.atEnd(); pIter++ )
		allKnowledge += pIter->getValue();

	_self[ ALL_SKILL_SUMM ] = allKnowledge;
	_self[ KNOWLEDGE_LEVEL ] = sum;
}

int CNrpUser::GetSkill( const NrpText& name )
{
	return knowledges_.find( name ) != NULL ? knowledges_[ name ] : 0;
}

NrpText CNrpUser::Save( const NrpText& folderPath )
{
	try
	{
		assert( OpFileSystem::IsExist( folderPath ) );

		NrpText fileName = OpFileSystem::CheckEndSlash( folderPath )+ Param( NAME ).As<NrpText>() + ".user";
		assert( !OpFileSystem::IsExist( fileName ) );

		INrpConfig::Save( fileName );
		
		IniFile sv( fileName );

		sv.Set( SECTION_GENREPREFERENCE, genrePreferences_ );
		sv.Set( SECTION_GENREEXPIRIENCE, genreExperience_ );	
		sv.Set( SECTION_KNOWLEDGES, knowledges_ );

		for( u32 i=0; i < works_.size(); i++ )
		{
			if( CNrpProjectModule* prjModule = dynamic_cast< CNrpProjectModule* >( works_[ i ] )  )
			{
				NrpText projectName = (*prjModule)[ PARENT ].As<INrpDevelopProject*>()->Text( NAME );
				NrpText name = (*prjModule)[NAME];
				sv.Set( SECTION_WORKS, CreateKeyWork( i ), "project" );
				sv.Set( SECTION_WORKS, CreateKeyProject( i ), projectName );
				sv.Set( SECTION_WORKS, CreateKeyModule( i ), name );
			}
			else if( CNrpInvention* invention = dynamic_cast< CNrpInvention* >( works_[ i ] ) )
			{
				NrpText name = (*invention)[NAME];
				sv.Set( SECTION_WORKS, CreateKeyWork( i ), "invention" );
				sv.Set( SECTION_WORKS, CreateKeyInvention( i ), name );
			}
		}

		sv.Save();
		return fileName;
	}
	catch(...)
	{
		return "";	
	}
}

void CNrpUser::Load( const NrpText& fileName )
{
	assert( OpFileSystem::IsExist( fileName ) );

	INrpConfig::Load( fileName );
	assert( ((NrpText)_self[ NAME ]).size() > 0 );

	IniFile rv( fileName );
	rv.Get( SECTION_KNOWLEDGES, knowledges_ );
	rv.Get( SECTION_GENREPREFERENCE, genrePreferences_ );
	rv.Get( SECTION_GENREEXPIRIENCE, genreExperience_ );

	for( int k=0; k < (int)_self[ WORKNUMBER ]; k++ )
	{
		NrpText action = "";
		NrpText workType = rv.Get( SECTION_WORKS, CreateKeyWork( k ), NrpText("") );
		if( workType.size() )
		{
			if( workType.equals_ignore_case( "project" ) )
			{
				NrpText projectName = rv.Get( SECTION_WORKS, CreateKeyProject( k ), NrpText("") );	
				NrpText moduleName = rv.Get( SECTION_WORKS, CreateKeyModule( k ), NrpText("") );
				action = NrpText( "autoscript:AddUserToGameProject(\"" ) + Text( NAME ) + "\", \"" + projectName + "\", \"" + moduleName + "\")";
			}
			else if( workType.equals_ignore_case( "invention" ) )
			{
				NrpText inventionName = rv.Get( SECTION_WORKS, CreateKeyInvention( k ), NrpText("") );	
				action = NrpText( "autoscript:AddUserToInvention(\"" ) + Text( NAME ) + "\" , \"" + inventionName + "\")";	
			}				
			
			CNrpScript::Instance().AddActionToTemporaryScript( AFTER_LOAD_SCRIPT, action );
		}
	}

/*	PropertyArray::iterator paIter = GetProperties().begin();
	for( ; paIter != GetProperties().end(); ++paIter)
	{
		INrpProperty* prop = paIter->second;
		if( prop->GetValueType() == typeid( int ).name() )
			IniFile::Write( PROPERTIES, paIter->first, ((CNrpProperty<int>*)prop)->GetValue(), fileName );

		if( prop->GetValueType() == typeid( NrpText ).name() )
			IniFile::Write( PROPERTIES, paIter->first, ((CNrpProperty<NrpText>*)prop)->GetValue(), fileName );
	}*/

	CalculateKnowledgeLevel_();
}

void CNrpUser::AddWork( IWorkingModule& module, bool toFront )
{
	//������ �� �������� � ����� ������ ��� � ������ ���������������
	if( (float)module[ READYWORKPERCENT ] < 1.f && GetWork( (NrpText)module[ NAME ] ) == NULL )
	{
		if( toFront )
			works_.insert( &module, 0 );
		else
			works_.push_back( &module );

		module.AddUser( *this );
	}

	_self[ WORKNUMBER ] = static_cast< int >( works_.size() );
}

void CNrpUser::RemoveWork( IWorkingModule& techWork )
{
	for( u32 i=0; i < works_.size(); i++ )
		if( works_[ i ] == &techWork )
		{
			techWork.RemoveUser( (NrpText)_self[ NAME ] );
			works_.erase( i );
			_self[ WORKNUMBER ] = static_cast< int >( works_.size() );
			return;
		}
		
	NrpText text = NrpText( "�� ���� ����� ��������� ��� �������� " ) + (NrpText)techWork[ NAME ];
	Log(HW) << text << term;
}

IWorkingModule* CNrpUser::GetWork( u32 index ) const
{
	assert( index < works_.size() );
	return ( index < works_.size()) ? works_[ index ] : NULL;
}

IWorkingModule* CNrpUser::GetWork( const NrpText& name ) const
{
	for( u32 i=0; i < works_.size(); i++ )
		if( (*works_[ i ])[ NAME ] == name )
			return works_[ i ];

	return NULL;
}

void CNrpUser::BeginNewHour( const NrpTime& time )
{
	if( time.RHour() == 9 && Text( USERSTATE ).equals_ignore_case( "readyToWork" ) )
	{
		_self[ USERSTATE ] = NrpText( "work" );
	}

	if( time.RHour() == 18 )
	{
		_self[ USERSTATE ] = NrpText( "readyToWork" );
	}

	if( Text( USERSTATE ).equals_ignore_case( "work" ) )
	{
		if( works_.size() > 0 )
		{
			//��������� ��������� ����������
			if( (*works_[ 0 ] )[ READYWORKPERCENT ] >= 1.f )
				RemoveWork( *works_[ 0 ] );
			else
				works_[ 0 ]->Update( *this, time );			
		}
	}
}

int CNrpUser::GetGenreExperience( const NrpText& name )
{
	KNOWLEDGE_MAP::Node* kIter = genreExperience_.find( name );
	return kIter != NULL ? kIter->getValue() : 0;
}

int CNrpUser::GetGenrePreferences( const NrpText& name )
{
	KNOWLEDGE_MAP::Node* kIter = genrePreferences_.find( name );
	return kIter != NULL ? kIter->getValue() : 0;
}

void CNrpUser::SetGenreExperience( const NrpText& name, int valuel )
{
	genreExperience_[ name ] = valuel;
}

void CNrpUser::SetGenrePreferences( const NrpText& name, int valuel )
{
	genrePreferences_[ name ] = valuel;
}

void CNrpUser::RemoveOldModificators_( NrpTime time )
{
	for( size_t cnt=0; cnt < modificators_.size(); cnt++ )
	{
		if( modificators_[ cnt ]->IsTimeout( time ) )
		{
			delete modificators_[ cnt ];
			modificators_.erase( cnt );
			cnt--;
		}
	}
}

void CNrpUser::BeginNewDay( const NrpTime& time )
{
	_self[ HANGRY ] = 100;
	_self[ MOOD ] = 100; //����� ������� ��� � ���� ���������� ��������

	RemoveOldModificators_( time );
}

void CNrpUser::AddModificator( IModificator* ptrModificator )
{
	assert( ptrModificator );
	if( ptrModificator )
		modificators_.push_back( ptrModificator );
}

void CNrpUser::IncreaseExperience( const NrpText& name, int grow )
{
	genreExperience_[ name ] = ( genreExperience_.find( name ) != NULL ? genreExperience_[ name ] : 0) + grow;
	//����� ���� ���� ������ �� ���� ������ �������
	_self[ EXPERIENCE ] += grow; 
}

void CNrpUser::_CheckModificators()
{
	/*
	MODIFICATOR_LIST::iterator pIter = modificators_.begin();

	for( ; pIter < modificators_.end(); pIter++ )
	{
		if( ((CNrpUserModificator<R>*)*pIter)->GetName() == name )
			paramValue += ((CNrpUserModificator<R>*)*pIter)->GetValue();
	}
	*/
}

CNrpRelation* CNrpUser::GetRelation( const NrpText& name )
{
	if( _relations.find( name ) == NULL )
		_relations[ name ] = new CNrpRelation( name, 0 );
	
	return  _relations[ name ];
}

bool CNrpUser::Equale( const NrpText& name )
{
	return Text( NAME ) == name;
}

NrpText CNrpUser::ClassName()
{
	return CLASS_USER;
}

bool CNrpUser::operator==( const CNrpUser& other )
{
	return (&other == this) || ((NrpText)other[ NAME ] == (NrpText)_self[ NAME ] );
}

NrpText NrpCoder::ClassName()
{
	return CLASS_CODER;
}

NrpCoder::NrpCoder( const NrpText& systemName ) : CNrpUser( CLASS_CODER, systemName )
{
}


NrpDesigner::NrpDesigner( const NrpText& systemName ) : CNrpUser( CLASS_DESIGNER, systemName )
{

}

NrpText NrpDesigner::ClassName()
{
	return CLASS_DESIGNER;
}

NrpComposer::NrpComposer( const NrpText& systemName ) : CNrpUser( CLASS_COMPOSER, systemName )
{

}

NrpText NrpComposer::ClassName()
{
	return CLASS_COMPOSER;
}
}//namespace nrp