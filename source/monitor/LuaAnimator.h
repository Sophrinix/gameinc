#ifndef _INCLUDE_ILUAANIMATOR_
#define _INCLUDE_ILUAANIMATOR_

#include "ILuaGuiElement.h"
#include "IGuiAnimator.h"

namespace  nrp
{

class CLuaAnimator : public ILuaGuiElement<irr::gui::IGUIAnimator>
{
public:
	DEFINE_PROPERTIES_AND_METHODS(CLuaAnimator)				//������ �������

	CLuaAnimator(lua_State *L, bool);	
	int SetOnRemove( lua_State *L );

	static const char* ClassName();
};

}

#endif //_INCLUDE_ILUAANIMATOR_