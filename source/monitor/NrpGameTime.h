#ifndef _INCLUDE_GAMETIME_
#define _INCLUDE_GAMETIME_

namespace nrp
{

class CNrpApplication;

class CNrpGameTime 
{
	typedef enum { SPD_MINUTE=0, SPD_HOUR, SPD_DAY, SPD_MONTH, SPD_COUNT } SPEED;
public:
	CNrpGameTime( CNrpApplication* parent );
	bool Update();
	~CNrpGameTime(void);
private:
	CNrpGameTime() {};
	SPEED speed_;
	int lastTimeUpdate_;
};

}//end namespace nrp

#endif