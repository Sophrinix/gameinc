#pragma once
#include "IGuiAnimator.h"
#include "NrpText.h"

namespace irr
{

namespace gui
{

class CNrpTimer : public IGUIAnimator
{
public:
	CNrpTimer( IGUIEnvironment* environment, IGUIElement* node, size_t time, int action );
	~CNrpTimer( void );

	virtual void draw();

	void SetLoop( bool loop ) { _loop = loop; }
private:
	size_t _time;
	size_t _startTime;
	bool _active;
	bool _loop;
	int _action;
};

}//namspace gui

}//namespace irr
