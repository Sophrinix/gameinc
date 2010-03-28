#pragma once

#include "nrpChartObject.h"

namespace irr
{

namespace gui
{

class CChartAxis;

class CChartAxisLabel : public CChartObject  
{
	friend CChartAxis;

public:
	void SetText(const TChartString& NewText);
	TChartString GetText() const        { return strLabelText_;    }

	void SetFont(int nPointSize, const TChartString& strFaceName);

	CChartAxisLabel( IGUIEnvironment* env, CChartCtrl* ptrParent, bool horizontal);
	virtual ~CChartAxisLabel();
	void Draw();

private:
	bool horizontal_;      // Specifies if the axis is horizontal or not
	int  fontSize_;
	TChartString strFontName_;
	TChartString strLabelText_;
};

}//namespace gui

}//namespace irr