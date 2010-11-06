#include "StdAfx.h"
#include "SectionNames.h"

namespace nrp
{
std::string CreateKeyWork( int index ) 
{ 
	return "work_" + conv::ToStr( index ); 
}

std::string CreateKeyReklame( int index ) 
{
	return "reklame_" + conv::ToStr( index ); 
}
}