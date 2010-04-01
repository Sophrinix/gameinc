#pragma once

#include "nrpChartObject.h"
#include <vector>

namespace irr
{

namespace gui
{

class CChartTitle : public CChartObject  
{
	friend CChartCtrl;

public:
	size_t GetStringCount() const;
	TChartString GetString(size_t Index) const;
	void AddString(const TChartString& NewString);
	void RemoveAll();

	void SetFont( const TChartString& new_name );

	CChartTitle( IGUIEnvironment* env, CChartCtrl* pParent);
	virtual ~CChartTitle();
	
	irr::core::dimension2di GetSize();

	void Draw();

private:
	std::vector<TChartString> strings_;

	TChartString font_name_;
};

}//namespace gui

}//namesapce irr