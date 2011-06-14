/********************************************************************
	created:	2009/11/19
	created:	19:11:2009   16:17
	filename: 	ConfigMap.h
	file path:	nerpa_sc
	file base:	ConfigMap
	file ext:	h
	author:		Dalerank
	
	purpose:	Этот класс отвечает за хранение настроек
				отрисовки элементов для ImageGuiSkin
*********************************************************************/

#ifndef CCONFIGMAP_H_
#define CCONFIGMAP_H_

#include <irrMap.h>
#include <irrString.h>
#include <irrTypes.h>
#include <vector2d.h>
#include <vector3d.h>

using namespace irr;

class CConfigMap
{
public:
	/*! получение всей строки конфига для заданного элемента
	 */ 
    core::stringc getConfig( const core::stringc& name );					
	/********************* 
	 функции нигде не используются 
	 были нужны при отладке, отвечают за выдачу строки в виде другого значения 
	***************************/
    s32 getConfigAsInt( const core::stringc& name );						
    f32 getConfigAsFloat( const core::stringc& name );
    core::vector2df getConfigAsVector2df( const core::stringc& name );
    core::vector3df getConfigAsVector3df( const core::stringc& name );
	bool hasConfig( const core::stringc& name );
	/***************************************************/

	//*! размещение нового значения в карте конфига
    void setConfig( const core::stringc& name, const core::stringc& value );	

    const core::array< core::stringc >& GetControls() { return _controls; }

private:
    core::array< core::stringc > _controls;
    core::map<core::stringc,core::stringc> _configs;
};

#endif


