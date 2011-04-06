#pragma once
#include "timeHelpers.h"
#include "NrpText.h"
#include "timehelpers.h"
#include "nrpConfig.h"

namespace nrp
{

class CNrpUser;

class IModificator
{
public:
	virtual NrpText& GetParamName() = 0;
	virtual NrpText& GetName() = 0;
	virtual NrpTime& GetTime() = 0;
	virtual void Update( const NrpTime& ) = 0;
};

template< class ValClass > 
class CNrpConfigModificator : public IModificator
{
public:
	CNrpConfigModificator( CNrpUser* ptrUser, NrpTime time, const NrpText& paramName, bool absolute, ValClass offset, const NrpText& modName ) 
		: _offset( NParam( "", offset ) )
		  , _endTime( 0. )
		  , _oldValue( NParam( "", offset ) )
	{
		_offset = NParam( "", offset );
		_oldValue = (*ptrUser)[ paramName ];
		_endTime = time;
		_ptrUser = ptrUser;
		_paramName = paramName;
		_absolute = absolute;
		_isValid = true;
		_modName = modName;

		if( _absolute )
		{
			(*ptrUser)[ paramName ] = _offset;
			_offset = (ValClass)_oldValue - (ValClass)(*ptrUser)[ paramName ];
		}
		else
			(*ptrUser)[ paramName ] += offset;
	}

	virtual ~CNrpConfigModificator()
	{
		(*_ptrUser)[ _paramName ] -= (ValClass)_offset;
	}

	NrpText& GetParamName() { return _paramName; }
	NrpText& GetName() { return _modName; }
	ValClass& GetValue() { return _offset; }
	NrpTime& GetTime() { return _endTime; }
	bool IsValid() { return _isValid; }

	void Update( const NrpTime& curTime )
	{
		_isValid = (curTime > _endTime);
	}

private:
	NParam _offset;
	NrpTime _endTime;
	CNrpUser* _ptrUser;
	NParam _oldValue;
	NrpText _paramName;
	NrpText _modName;
	bool _absolute;
	bool _isValid;
};

}//end of namespace nrp