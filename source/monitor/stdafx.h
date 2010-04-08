// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

// TODO: reference additional headers your program requires here

#include "Logger.h"
#include "NrpActionType.h"
#include <irrTypes.h>

typedef std::string CLASS_NAME; 
typedef std::string SYSTEM_NAME;
typedef std::string CONFIG_TYPE;

namespace irr
{
	namespace scene
	{
		const int ESNT_TARGET = MAKE_IRR_ID( 't', 'r', 'g', 'n' );
		const int ESNT_SONAR = MAKE_IRR_ID( 's', 'n', 'r', 'n' );
		const int ESNT_REGION = MAKE_IRR_ID( 'r', 'e', 'g', 'n' );
	}
}