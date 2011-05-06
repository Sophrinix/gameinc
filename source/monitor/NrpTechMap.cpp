#include "stdafx.h"
#include "NrpTechMap.h"
#include "NrpTechnology.h"

#include <assert.h>

#define ARROW_PAD 15

using namespace nrp;

namespace irr
{

namespace gui
{

//! constructor
CNrpTechMap::CNrpTechMap(IGUIEnvironment* environment, IGUIElement* parent,
						s32 id, const core::rect<s32>& rectangle, bool clip,
						bool drawBack, bool moveOverSelect)
: IGUITable(environment, parent, id, rectangle), Font(0),
	VerticalScrollBar(0), HorizontalScrollBar(0),
	Clip(clip), _drawBack(drawBack), MoveOverSelect(moveOverSelect),
	Selecting(false), CurrentResizedColumn(-1), ResizeStart(0), ResizableColumns(true),
	ItemHeight(0), overItemHeight_(0), TotalItemHeight(0), TotalItemWidth(0), _selected( core::position2di(-1, -1) ),
	CellHeightPadding(2), CellWidthPadding(5), ActiveTab(-1),
	CurrentOrdering(EGOM_NONE), DrawFlags(EGTDF_ROWS | EGTDF_COLUMNS | EGTDF_ACTIVE_ROW ),
	_rMouseDown( false )
{
	#ifdef _DEBUG
	setDebugName("CNrpTechMap");
	#endif

	VerticalScrollBar = Environment->addScrollBar(false, core::rect<s32>(0, 0, 100, 100), this, -1);
	if (VerticalScrollBar)
	{
		VerticalScrollBar->grab();
		VerticalScrollBar->setNotClipped(false);
		VerticalScrollBar->setSubElement(true);
	}

	HorizontalScrollBar = Environment->addScrollBar(true, core::rect<s32>(0, 0, 100, 100), this, -1);
	if ( HorizontalScrollBar )
	{
		HorizontalScrollBar->grab();
		HorizontalScrollBar->setNotClipped(false);
		HorizontalScrollBar->setSubElement(true);
	}

	refreshControls();
}


//! destructor
CNrpTechMap::~CNrpTechMap()
{
	if (VerticalScrollBar)
		VerticalScrollBar->drop();
	if ( HorizontalScrollBar )
		HorizontalScrollBar->drop();

	if (Font)
		Font->drop();
}

void CNrpTechMap::addColumn(const wchar_t* caption, s32 columnIndex)
{
	Column tabHeader;
	tabHeader.Name = caption;
	tabHeader.Width = Font->getDimension(caption).Width + (CellWidthPadding * 2) + ARROW_PAD;
	tabHeader.OrderingMode = EGCO_NONE;

	IGUISkin* skin = Environment->getSkin();
	if (skin)
	{
		tabHeader.TextColor = skin->getColor(EGDC_BUTTON_TEXT);
	}

	if ( columnIndex < 0 || columnIndex >= (s32)Columns.size() )
	{
		Columns.push_back(tabHeader);
		for ( u32 i=0; i < Rows.size(); ++i )
		{
			Cell cell;
			Rows[i].Items.push_back(cell);
		}
	}
	else
	{
		Columns.insert(tabHeader, columnIndex);
		for ( u32 i=0; i < Rows.size(); ++i )
		{
			Cell cell;
			Rows[i].Items.insert(cell, columnIndex);
		}
	}

	if (ActiveTab == -1)
		ActiveTab = 0;

	recalculateWidths();
}

//! remove a column from the table
void CNrpTechMap::removeColumn(u32 columnIndex)
{
	if ( columnIndex < Columns.size() )
	{
		Columns.erase(columnIndex);
		for ( u32 i=0; i < Rows.size(); ++i )
		{
			Rows[i].Items.erase(columnIndex);
		}
	}
	if ( (s32)columnIndex <= ActiveTab )
		ActiveTab = Columns.size() ? 0 : -1;

	recalculateWidths();
}


s32 CNrpTechMap::getColumnCount() const
{
	return Columns.size();
}


s32 CNrpTechMap::getRowCount() const
{
	return Rows.size();
}


bool CNrpTechMap::setActiveColumn(s32 idx, bool doOrder )
{
	if (idx < 0 || idx >= (s32)Columns.size())
		return false;

	bool changed = (ActiveTab != idx);

	ActiveTab = idx;
	if ( ActiveTab < 0 )
		return false;

	if ( doOrder )
	{
		switch ( Columns[idx].OrderingMode )
		{
			case EGCO_NONE:
				CurrentOrdering = EGOM_NONE;
				break;

			case EGCO_CUSTOM:
				CurrentOrdering = EGOM_NONE;
				if (Parent)
				{
					SEvent event;
					event.EventType = EET_GUI_EVENT;
					event.GUIEvent.Caller = this;
					event.GUIEvent.EventType = EGET_TABLE_HEADER_CHANGED;
					Parent->OnEvent(event);
				}

				break;

			case EGCO_ASCENDING:
				CurrentOrdering = EGOM_ASCENDING;
				break;

			case EGCO_DESCENDING:
				CurrentOrdering = EGOM_DESCENDING;
				break;

			case EGCO_FLIP_ASCENDING_DESCENDING:
				CurrentOrdering = EGOM_ASCENDING == CurrentOrdering ? EGOM_DESCENDING : EGOM_ASCENDING;
				break;
			default:
				CurrentOrdering = EGOM_NONE;
		}

		orderRows(getActiveColumn(), CurrentOrdering);
	}

	if (changed)
	{
		SEvent event;
		event.EventType = EET_GUI_EVENT;
		event.GUIEvent.Caller = this;
		event.GUIEvent.EventType = EGET_TABLE_HEADER_CHANGED;
		Parent->OnEvent(event);
	}

	return true;
}


s32 CNrpTechMap::getActiveColumn() const
{
	return ActiveTab;
}


EGUI_ORDERING_MODE CNrpTechMap::getActiveColumnOrdering() const
{
	return CurrentOrdering;
}


void CNrpTechMap::setColumnWidth(u32 width)
{
	_ColumnWidth = width;
	for( size_t columnIndex=0; columnIndex < Columns.size(); columnIndex++ )
		if ( columnIndex < Columns.size() )
		{
			const u32 MIN_WIDTH = Font->getDimension(Columns[columnIndex].Name.c_str() ).Width + (CellWidthPadding * 2);
			if ( width < MIN_WIDTH )
				width = MIN_WIDTH;

			Columns[columnIndex].Width = width;

			for ( u32 i=0; i < Rows.size(); ++i )
			{
				breakText( Rows[i].Items[columnIndex].Text, Rows[i].Items[columnIndex].BrokenText, Columns[columnIndex].Width );
			}
		}
	recalculateWidths();
}


void CNrpTechMap::setResizableColumns(bool resizable)
{
	ResizableColumns = resizable;
}


bool CNrpTechMap::hasResizableColumns() const
{
	return ResizableColumns;
}


u32 CNrpTechMap::addRow(u32 rowIndex)
{
	if ( rowIndex > Rows.size() )
	{
		rowIndex = Rows.size();
	}

	Row row;

	if ( rowIndex == Rows.size() )
		Rows.push_back(row);
	else
		Rows.insert(row, rowIndex);

	Rows[rowIndex].Items.reallocate(Columns.size());
	for ( u32 i = 0 ; i < Columns.size() ; ++i )
	{
		Rows[rowIndex].Items.push_back(Cell());
	}

	recalculateHeights();
	return rowIndex;
}


void CNrpTechMap::removeRow(u32 rowIndex)
{
	if ( rowIndex > Rows.size() )
		return;

	Rows.erase( rowIndex );

	if ( !(_selected.Y < s32(Rows.size())) )
		_selected.Y = Rows.size() - 1;

	recalculateHeights();
}


//! adds an list item, returns id of item
void CNrpTechMap::setCellText(u32 rowIndex, u32 columnIndex, const core::stringw& text)
{
	if ( rowIndex < Rows.size() && columnIndex < Columns.size() )
	{
		Rows[rowIndex].Items[columnIndex].Text = text;
		breakText( Rows[rowIndex].Items[columnIndex].Text, Rows[rowIndex].Items[columnIndex].BrokenText, Columns[columnIndex].Width );

		IGUISkin* skin = Environment->getSkin();
		if ( skin )
			Rows[rowIndex].Items[columnIndex].Color = skin->getColor(EGDC_BUTTON_TEXT);
	}
}

void CNrpTechMap::setCellText(u32 rowIndex, u32 columnIndex, const core::stringw& text, video::SColor color)
{
	if ( rowIndex < Rows.size() && columnIndex < Columns.size() )
	{
		Rows[rowIndex].Items[columnIndex].Text = text;
		breakText( Rows[rowIndex].Items[columnIndex].Text, Rows[rowIndex].Items[columnIndex].BrokenText, Columns[columnIndex].Width );
		Rows[rowIndex].Items[columnIndex].Color = color;
	}
}


void CNrpTechMap::setCellColor(u32 rowIndex, u32 columnIndex, video::SColor color)
{
	if ( rowIndex < Rows.size() && columnIndex < Columns.size() )
	{
		Rows[rowIndex].Items[columnIndex].Color = color;
	}
}


void CNrpTechMap::setCellData(u32 rowIndex, u32 columnIndex, void *data)
{
	if ( rowIndex < Rows.size() && columnIndex < Columns.size() )
	{
		Rows[rowIndex].Items[columnIndex].assignTech = (AssignTech*)data;
	}
}


const wchar_t* CNrpTechMap::getCellText(u32 rowIndex, u32 columnIndex ) const
{
	if ( rowIndex < Rows.size() && columnIndex < Columns.size() )
	{
		return Rows[rowIndex].Items[columnIndex].Text.c_str();
	}

	return 0;
}


void* CNrpTechMap::getCellData(u32 rowIndex, u32 columnIndex ) const
{
	if ( rowIndex < Rows.size() && columnIndex < Columns.size() )
	{
		return Rows[rowIndex].Items[columnIndex].assignTech;
	}

	return 0;
}


//! clears the list
void CNrpTechMap::clear()
{
	_selected = core::position2di( -1, -1 );
	Rows.clear();
	Columns.clear();

	if (VerticalScrollBar)
		VerticalScrollBar->setPos(0);
	if ( HorizontalScrollBar )
		HorizontalScrollBar->setPos(0);

	recalculateHeights();
	recalculateWidths();
}


void CNrpTechMap::clearRows()
{
	_selected = core::position2di( -1, -1 );
	Rows.clear();

	if (VerticalScrollBar)
		VerticalScrollBar->setPos(0);

	recalculateHeights();
}


/*!
*/
s32 CNrpTechMap::getSelected() const
{
	return -1;
}

/*!
*/
CNrpTechnology* CNrpTechMap::getSelectedTech() const
{
	return Rows[ _selected.Y ].Items[ _selected.X ].assignTech->GetTechnology();
}

//! set wich row is currently selected
void CNrpTechMap::setSelected( s32 index )
{
}


void CNrpTechMap::recalculateWidths()
{
	TotalItemWidth=0;
	for ( u32 i=0; i < Columns.size(); ++i )
	{
		TotalItemWidth += Columns[i].Width;
	}
	checkScrollbars();
}


void CNrpTechMap::recalculateHeights()
{
	TotalItemHeight = 0;
	IGUISkin* skin = Environment->getSkin();
	if (Font != skin->getFont())
	{
		if (Font)
			Font->drop();

		Font = skin->getFont();

		ItemHeight = 0;

		if(Font)
		{
			ItemHeight = overItemHeight_ == 0 ? Font->getDimension(L"A").Height + (CellHeightPadding * 2) : overItemHeight_;
			Font->grab();
		}
	}
	TotalItemHeight = ItemHeight * Rows.size();		//  header is not counted, because we only want items
	checkScrollbars();
}


// automatic enabled/disabling and resizing of scrollbars
void CNrpTechMap::checkScrollbars()
{
	IGUISkin* skin = Environment->getSkin();
	if ( !HorizontalScrollBar || !VerticalScrollBar || !skin)
		return;

	s32 scrollBarSize = skin->getSize(EGDS_SCROLLBAR_SIZE);
	bool wasHorizontalScrollBarVisible = HorizontalScrollBar->isVisible();
	bool wasVerticalScrollBarVisible = VerticalScrollBar->isVisible();
	HorizontalScrollBar->setVisible(false);
	VerticalScrollBar->setVisible(false);

	// CAREFUL: near identical calculations for tableRect and clientClip are also done in draw
	// area of table used for drawing without scrollbars
	core::rect<s32> tableRect(AbsoluteRect);
	tableRect.UpperLeftCorner.X += 1;
	tableRect.UpperLeftCorner.Y += 1;
	s32 headerBottom = tableRect.UpperLeftCorner.Y + ItemHeight;

	// area of for the items (without header and without scrollbars)
	core::rect<s32> clientClip(tableRect);
	clientClip.UpperLeftCorner.Y = headerBottom + 1;

	// needs horizontal scroll be visible?
	if( TotalItemWidth > clientClip.getWidth() )
	{
		clientClip.LowerRightCorner.Y -= scrollBarSize;
		HorizontalScrollBar->setVisible(true);
		HorizontalScrollBar->setMax(TotalItemWidth - clientClip.getWidth());
	}

	// needs vertical scroll be visible?
	if( TotalItemHeight > clientClip.getHeight() )
	{
		clientClip.LowerRightCorner.X -= scrollBarSize;
		VerticalScrollBar->setVisible(true);
		VerticalScrollBar->setMax(TotalItemHeight - clientClip.getHeight());

		// check horizontal again because we have now smaller clientClip
		if ( !HorizontalScrollBar->isVisible() )
		{
			if( TotalItemWidth > clientClip.getWidth() )
			{
				clientClip.LowerRightCorner.Y -= scrollBarSize;
				HorizontalScrollBar->setVisible(true);
				HorizontalScrollBar->setMax(TotalItemWidth - clientClip.getWidth());
			}
		}
	}

	// find the correct size for the vertical scrollbar
	if ( VerticalScrollBar->isVisible() )
	{
		if  (!wasVerticalScrollBarVisible )
			VerticalScrollBar->setPos(0);

		if ( HorizontalScrollBar->isVisible() )
		{
			VerticalScrollBar->setRelativePosition(
				core::rect<s32>(RelativeRect.getWidth() - scrollBarSize, 1,
				RelativeRect.getWidth()-1, RelativeRect.getHeight()-(1+scrollBarSize) ) );
		}
		else
		{
			VerticalScrollBar->setRelativePosition(
				core::rect<s32>(RelativeRect.getWidth() - scrollBarSize, 1,
				RelativeRect.getWidth()-1, RelativeRect.getHeight()-1) );
		}
	}

	// find the correct size for the horizontal scrollbar
	if ( HorizontalScrollBar->isVisible() )
	{
		if ( !wasHorizontalScrollBarVisible )
			HorizontalScrollBar->setPos(0);

		if ( VerticalScrollBar->isVisible() )
		{
			HorizontalScrollBar->setRelativePosition( core::rect<s32>(1, RelativeRect.getHeight() - scrollBarSize, RelativeRect.getWidth()-(1+scrollBarSize), RelativeRect.getHeight()-1) );
		}
		else
		{
			HorizontalScrollBar->setRelativePosition( core::rect<s32>(1, RelativeRect.getHeight() - scrollBarSize, RelativeRect.getWidth()-1, RelativeRect.getHeight()-1) );
		}
	}
}


void CNrpTechMap::refreshControls()
{
	updateAbsolutePosition();

	if ( VerticalScrollBar )
		VerticalScrollBar->setVisible(false);

	if ( HorizontalScrollBar )
		HorizontalScrollBar->setVisible(false);

	recalculateHeights();
	recalculateWidths();
}


//! called if an event happened.
bool CNrpTechMap::OnEvent(const SEvent &event)
{
	if (IsEnabled)
	{

		switch(event.EventType)
		{
		case EET_GUI_EVENT:
			switch(event.GUIEvent.EventType)
			{
			case gui::EGET_SCROLL_BAR_CHANGED:
				if (event.GUIEvent.Caller == VerticalScrollBar)
				{
					// current position will get read out in draw
					return true;
				}
				if (event.GUIEvent.Caller == HorizontalScrollBar)
				{
					// current position will get read out in draw
					return true;
				}
				break;
			case gui::EGET_ELEMENT_FOCUS_LOST:
				{
					CurrentResizedColumn = -1;
					Selecting = false;
				}
				break;
			default:
				break;
			}
			break;
		case EET_MOUSE_INPUT_EVENT:
			{
				if ( !IsEnabled )
					return false;

				core::position2d<s32> p(event.MouseInput.X, event.MouseInput.Y);
				
				switch(event.MouseInput.Event)
				{
				case EMIE_MOUSE_WHEEL:
					VerticalScrollBar->setPos(VerticalScrollBar->getPos() + (s32)event.MouseInput.Wheel*-10);
					return true;

				case EMIE_LMOUSE_PRESSED_DOWN:
				case EMIE_RMOUSE_PRESSED_DOWN:
					_rMouseDown = event.MouseInput.Event == EMIE_RMOUSE_PRESSED_DOWN;
					_startTimeMouseDown = GetTickCount();

					if (Environment->hasFocus(this) &&
						VerticalScrollBar->isVisible() &&
						VerticalScrollBar->getAbsolutePosition().isPointInside(p) &&
						VerticalScrollBar->OnEvent(event))
						return true;

					if (Environment->hasFocus(this) &&
						HorizontalScrollBar->isVisible() &&
						HorizontalScrollBar->getAbsolutePosition().isPointInside(p) &&
						HorizontalScrollBar->OnEvent(event))
						return true;

					if ( dragColumnStart( event.MouseInput.X, event.MouseInput.Y ) )
					{
						Environment->setFocus(this);
						return true;
					}

					if ( selectColumnHeader( event.MouseInput.X, event.MouseInput.Y ) )
						return true;

					Selecting = true;
					Environment->setFocus(this);
					return true;

				case EMIE_LMOUSE_LEFT_UP:

					CurrentResizedColumn = -1;
					Selecting = false;
			
					if (!getAbsolutePosition().isPointInside(p))
					{
						Environment->removeFocus(this);
					}

					if (Environment->hasFocus(this) &&
						VerticalScrollBar->isVisible() &&
						VerticalScrollBar->getAbsolutePosition().isPointInside(p) &&
						VerticalScrollBar->OnEvent(event))
					{
						return true;
					}

					if (Environment->hasFocus(this) &&
						HorizontalScrollBar->isVisible() &&
						HorizontalScrollBar->getAbsolutePosition().isPointInside(p) &&
						HorizontalScrollBar->OnEvent(event))
					{
						return true;
					}

					selectNew( core::position2di( event.MouseInput.X, event.MouseInput.Y ) );
					return true;
				case EMIE_RMOUSE_LEFT_UP:
					 _rMouseDown = false;
				return true;

				case EMIE_MOUSE_MOVED:
					if (Selecting || MoveOverSelect)
					{
						if (getAbsolutePosition().isPointInside(p))
						{
							selectNew( core::position2di( event.MouseInput.X, event.MouseInput.Y) );
							return true;
						}
					}
					break;
				default:
					break;
				}
			}
			break;
		default:
			break;
		}
	}

	return IGUIElement::OnEvent(event);
}


void CNrpTechMap::setColumnOrdering(u32 columnIndex, EGUI_COLUMN_ORDERING mode)
{
	if ( columnIndex < Columns.size() )
		Columns[columnIndex].OrderingMode = mode;
}


void CNrpTechMap::swapRows(u32 rowIndexA, u32 rowIndexB)
{
	if ( rowIndexA >= Rows.size() )
		return;

	if ( rowIndexB >= Rows.size() )
		return;

	Row swap = Rows[rowIndexA];
	Rows[rowIndexA] = Rows[rowIndexB];
	Rows[rowIndexB] = swap;

	if ( _selected.Y == s32(rowIndexA) )
		_selected.Y = rowIndexB;
	else if( _selected.Y == s32(rowIndexB) )
		_selected.Y = rowIndexA;

}


bool CNrpTechMap::dragColumnStart(s32 xpos, s32 ypos)
{
	if ( !ResizableColumns )
		return false;

	if ( ypos > ( AbsoluteRect.UpperLeftCorner.Y + ItemHeight ) )
		return false;

	const s32 CLICK_AREA = 12;	// to left and right of line which can be dragged
	s32 pos = AbsoluteRect.UpperLeftCorner.X+1;

	if ( HorizontalScrollBar && HorizontalScrollBar->isVisible() )
		pos -= HorizontalScrollBar->getPos();

	pos += TotalItemWidth;

	// have to search from the right as otherwise lines could no longer be resized when a column width is 0
	for ( s32 i = (s32)Columns.size()-1; i >= 0 ; --i )
	{
		u32 colWidth = Columns[i].Width;

		if ( xpos >= (pos - CLICK_AREA) && xpos < ( pos + CLICK_AREA ) )
		{
			CurrentResizedColumn = i;
			ResizeStart = xpos;
			return true;
		}

		pos -= colWidth;
	}

	return false;
}

bool CNrpTechMap::selectColumnHeader(s32 xpos, s32 ypos)
{
	if ( ypos > ( AbsoluteRect.UpperLeftCorner.Y + ItemHeight ) )
		return false;

	s32 pos = AbsoluteRect.UpperLeftCorner.X+1;

	if ( HorizontalScrollBar && HorizontalScrollBar->isVisible() )
		pos -= HorizontalScrollBar->getPos();

	for ( u32 i = 0 ; i < Columns.size() ; ++i )
	{
		u32 colWidth = Columns[i].Width;

		if ( xpos >= pos && xpos < ( pos + s32(colWidth) ) )
		{
			setActiveColumn( i, true );

			return true;
		}

		pos += colWidth;
	}

	return false;
}


void CNrpTechMap::orderRows(s32 columnIndex, EGUI_ORDERING_MODE mode)
{
	Row swap;

	if ( columnIndex == -1 )
		columnIndex = getActiveColumn();
	if ( columnIndex < 0 )
		return;

	if ( mode == EGOM_ASCENDING )
	{
		for ( s32 i = 0 ; i < s32(Rows.size()) - 1 ; ++i )
		{
			for ( s32 j = 0 ; j < s32(Rows.size()) - i - 1 ; ++j )
			{
				if ( Rows[j+1].Items[columnIndex].Text < Rows[j].Items[columnIndex].Text )
				{
					swap = Rows[j];
					Rows[j] = Rows[j+1];
					Rows[j+1] = swap;

					if ( _selected.Y == j )
						_selected.Y = j+1;
					else if( _selected.Y == j+1 )
						_selected.Y = j;
				}
			}
		}
	}
	else if ( mode == EGOM_DESCENDING )
	{
		for ( s32 i = 0 ; i < s32(Rows.size()) - 1 ; ++i )
		{
			for ( s32 j = 0 ; j < s32(Rows.size()) - i - 1 ; ++j )
			{
				if ( Rows[j].Items[columnIndex].Text < Rows[j+1].Items[columnIndex].Text)
				{
					swap = Rows[j];
					Rows[j] = Rows[j+1];
					Rows[j+1] = swap;

					if ( _selected.Y == j )
						_selected.Y = j+1;
					else if( _selected.Y == j+1 )
						_selected.Y = j;
				}
			}
		}
	}
}


void CNrpTechMap::selectNew( core::position2di cell, bool onlyHover)
{
	IGUISkin* skin = Environment->getSkin();
	if (!skin)
		return;

	if ( !AbsoluteRect.isPointInside( cell ) )
		return;

	core::position2di newSelected( -1, -1 );
	// find new selected item.
	if( ItemHeight!=0 && _ColumnWidth != 0)
	{
		newSelected.Y = ((cell.Y - AbsoluteRect.UpperLeftCorner.Y - 1) + VerticalScrollBar->getPos()) / ItemHeight;
		newSelected.X = ((cell.X - AbsoluteRect.UpperLeftCorner.X ) + HorizontalScrollBar->getPos()) / _ColumnWidth;
	}

	if (newSelected.Y >= (s32)Rows.size())
		newSelected.Y = Rows.size() - 1;
	else if (newSelected.Y<0)
		newSelected.Y = 0;

	// post the news
	if (Parent && !onlyHover)
	{
		SEvent event;
		event.EventType = EET_GUI_EVENT;
		event.GUIEvent.Caller = this;
		event.GUIEvent.EventType = (_selected != newSelected) ? EGET_TABLE_CHANGED : EGET_TABLE_SELECTED_AGAIN;
		Parent->OnEvent(event);

		if( _selected == newSelected && 
			_selected.Y < static_cast< s32 >( Rows.size() ) &&
			_selected.X < static_cast< s32 >( Rows[ _selected.Y ].Items.size() ) &&
			Rows[ _selected.Y ].Items[ _selected.X ].assignTech )
			DoLuaFunctionsByType( GUIELEMENT_SELECTED_AGAIN, this, Rows[ _selected.Y ].Items[ _selected.X ].assignTech->GetTechnology() );			

		_selected = newSelected;
	}
}


//! draws the element and its children
void CNrpTechMap::draw()
{
	if (!IsVisible)
		return;

	irr::video::IVideoDriver* driver = Environment->getVideoDriver();

	IGUISkin* skin = Environment->getSkin();
	if (!skin)
		return;

	IGUIFont* font = skin->getFont();
	if (!font)
		return;

	// CAREFUL: near identical calculations for tableRect and clientClip are also done in checkScrollbars and selectColumnHeader
	// Area of table used for drawing without scrollbars
	core::rect<s32> tableRect(AbsoluteRect);
	tableRect.UpperLeftCorner.X += 1;
	tableRect.UpperLeftCorner.Y += 1;
	if ( VerticalScrollBar && VerticalScrollBar->isVisible() )
		tableRect.LowerRightCorner.X -= skin->getSize(EGDS_SCROLLBAR_SIZE);
	if ( HorizontalScrollBar && HorizontalScrollBar->isVisible() )
		tableRect.LowerRightCorner.Y -= skin->getSize(EGDS_SCROLLBAR_SIZE);

	s32 headerBottom = tableRect.UpperLeftCorner.Y + 0;

	// area of for the items (without header and without scrollbars)
	core::rect<s32> clientClip(tableRect);
	clientClip.UpperLeftCorner.Y = headerBottom + 1;

	core::rect<s32>* clipRect = 0;
	if (Clip)
		clipRect = &AbsoluteClippingRect;

	// draw background for whole element
	skin->draw3DSunkenPane(this, skin->getColor(EGDC_3D_HIGH_LIGHT), true, _drawBack, AbsoluteRect, clipRect);

	// scrolledTableClient is the area where the table items would be if it could be drawn completely
	core::rect<s32> scrolledTableClient(tableRect);
	scrolledTableClient.UpperLeftCorner.Y = headerBottom + 1;
	scrolledTableClient.LowerRightCorner.Y = scrolledTableClient.UpperLeftCorner.Y + TotalItemHeight;
	scrolledTableClient.LowerRightCorner.X = scrolledTableClient.UpperLeftCorner.X + TotalItemWidth;
	if ( VerticalScrollBar && VerticalScrollBar->isVisible() )
	{
		scrolledTableClient.UpperLeftCorner.Y -= VerticalScrollBar->getPos();
		scrolledTableClient.LowerRightCorner.Y -= VerticalScrollBar->getPos();
	}
	if ( HorizontalScrollBar && HorizontalScrollBar->isVisible() )
	{
		scrolledTableClient.UpperLeftCorner.X -= HorizontalScrollBar->getPos();
		scrolledTableClient.LowerRightCorner.X -= HorizontalScrollBar->getPos();
	}

	// rowRect is around the scrolled row
	core::rect<s32> rowRect(scrolledTableClient);
	rowRect.LowerRightCorner.Y = rowRect.UpperLeftCorner.Y + ItemHeight;

	u32 pos;
	for ( u32 i = 0 ; i < Rows.size() ; ++i )
	{
		if (rowRect.LowerRightCorner.Y >= AbsoluteRect.UpperLeftCorner.Y &&
			rowRect.UpperLeftCorner.Y <= AbsoluteRect.LowerRightCorner.Y)
		{
			core::rect<s32> textRect(rowRect);
			pos = rowRect.UpperLeftCorner.X;

			for ( u32 j = 0 ; j < Columns.size() ; ++j )
			{
				textRect.UpperLeftCorner.X = pos + CellWidthPadding;
				textRect.LowerRightCorner.X = pos + Columns[j].Width - CellWidthPadding;

				// draw selected cell background highlighted
				if ((s32)i == _selected.Y && j == _selected.X && DrawFlags & EGTDF_ACTIVE_ROW )
					driver->draw2DRectangle(skin->getColor(EGDC_HIGH_LIGHT), textRect, &clientClip);

				Cell& cell = Rows[ i ].Items[ j ];
				NrpText text = cell.BrokenText;
				video::ITexture* txs = NULL;
				int minSize = min( textRect.getWidth(), textRect.getHeight() );
				core::dimension2du txsSize( minSize, minSize );
				video::SColor alphaColor = 0xFFFFFFFF;
				if( cell.assignTech != NULL && cell.assignTech->GetTechnology() )
				{
					const CNrpTechnology& refTech = *(cell.assignTech->GetTechnology());
					txs = driver->getTexture( refTech[ TEXTURENORMAL ].As<NrpText>() );
					txsSize = txs ? txs->getOriginalSize() : core::dimension2du( minSize, minSize );
					// draw item text
					if( TS_INDEVELOP == refTech[ STATUS ].As<int>() )
					{
						text.append( "\n(" );
						text.append( static_cast< int >( refTech[ READYWORKPERCENT ].As<float>() ) );
						text.append( ")" );
						alphaColor = 0x80808080;
					}
				}

				{
					float scaleWidth, scaleHeight;
					scaleWidth = textRect.getWidth() / static_cast<float>( txsSize.Width );
					scaleHeight = textRect.getHeight() / static_cast<float>( txsSize.Height );

					//это мы нашли наименьший коэффициент для отображения
					scaleWidth = (( scaleWidth < scaleHeight ) ? scaleWidth : scaleHeight) * 0.43f;
					core::dimension2du hsize( static_cast< u32 >( txsSize.Width * scaleWidth ),
											  static_cast< u32 >( txsSize.Height * scaleWidth ) );

					video::SColor colors[ ] = { alphaColor, alphaColor, alphaColor, alphaColor };
					core::recti scaleTextRect( textRect.getCenter().X - hsize.Width, textRect.getCenter().Y - hsize.Height,
											   textRect.getCenter().X + hsize.Width, textRect.getCenter().Y + hsize.Height	);
					if( txs )
					{
						core::recti rr = scaleTextRect;
						rr.UpperLeftCorner -= core::position2di( 2, 2 );
						rr.LowerRightCorner += core::position2di( 2, 2 );
						driver->draw2DRectangle( rr, colors[ 0 ], colors[ 1 ], colors[ 2 ], colors[ 3 ], &AbsoluteClippingRect );

						driver->draw2DImage( txs, scaleTextRect, core::recti( core::position2di( 0, 0 ), txsSize ), 
											&AbsoluteClippingRect, colors, true );
					}
					else
						if( cell.assignTech )
							driver->draw2DRectangle( 0xffffff00, scaleTextRect, &AbsoluteClippingRect );

					cell.imgTechRect = scaleTextRect;

					if( cell.assignTech && cell.assignTech->GetParent() )
					{
						core::position2di parentPos = cell.assignTech->GetParent()->GetCell();
						core::recti pRect( Rows[ parentPos.Y ].Items[ parentPos.X ].imgTechRect );

						core::position2di lPoint( pRect.LowerRightCorner.X, (pRect.LowerRightCorner.Y + pRect.UpperLeftCorner.Y) / 2 );
						core::position2di rPoint( scaleTextRect.UpperLeftCorner.X, (scaleTextRect.LowerRightCorner.Y + scaleTextRect.UpperLeftCorner.Y) / 2 );

						driver->draw2DLine( lPoint, rPoint, 0xff00ff00 );
					}
				}

				if ((s32)i == _selected.Y && j == _selected.X )
				{
					font->draw( text.ToWide(), textRect, 
						skin->getColor(IsEnabled ? EGDC_HIGH_LIGHT_TEXT : EGDC_GRAY_TEXT), 
						true, true, &clientClip);
				}
				else
				{
					font->draw( text.ToWide(), textRect, 
						IsEnabled ? Rows[i].Items[j].Color : skin->getColor(EGDC_GRAY_TEXT), 
						true, true, &clientClip);
				}

				pos += Columns[j].Width;
			}
		}

		rowRect.UpperLeftCorner.Y += ItemHeight;
		rowRect.LowerRightCorner.Y += ItemHeight;
	}

	if( _rMouseDown && (GetTickCount() - _startTimeMouseDown > 1000) )
	{
		DoLuaFunctionsByType( GUIELEMENT_RMOUSE_HOLD, this );
	}

	IGUIElement::draw();
}


void CNrpTechMap::breakText(const core::stringw& text, core::stringw& brokenText, u32 cellWidth)
{
	IGUISkin* skin = Environment->getSkin();

	if (!skin)
		return;

	if (!Font)
		return;

	IGUIFont* font = skin->getFont();
	if (!font)
		return;

	core::stringw line, lineDots;
	wchar_t c[2];
	c[1] = L'\0';

	const u32 maxLength = cellWidth - (CellWidthPadding * 2);
	const u32 maxLengthDots = cellWidth - (CellWidthPadding * 2) - font->getDimension(L"...").Width;
	const u32 size = text.size();
	u32 pos = 0;

	u32 i;

	for (i=0; i<size; ++i)
	{
		c[0] = text[i];

		if (c[0] == L'\n')
			break;

		pos += font->getDimension(c).Width;
		if ( pos > maxLength )
			break;

		if ( font->getDimension( (line + c).c_str() ).Width > maxLengthDots )
			lineDots = line;

		line += c[0];
	}

	if ( i < size )
		brokenText = lineDots + L"...";
	else
		brokenText = line;
}


//! Set some flags influencing the layout of the table
void CNrpTechMap::setDrawFlags(s32 flags)
{
	DrawFlags = flags;
}


//! Get the flags which influence the layout of the table
s32 CNrpTechMap::getDrawFlags() const
{
	return DrawFlags;
}


//! Writes attributes of the element.
void CNrpTechMap::serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options) const
{
	IGUITable::serializeAttributes(out, options);

	out->addInt("ColumnCount", Columns.size());
	u32 i;
	for (i=0;i<Columns.size(); ++i)
	{
		core::stringc label;

		label = "Column"; label += i; label += "name";
		out->addString(label.c_str(), Columns[i].Name.c_str() );
		label = "Column"; label += i; label += "color";
		out->addColor(label.c_str(), Columns[i].TextColor );
		label = "Column"; label += i; label += "width";
		out->addInt(label.c_str(), Columns[i].Width );
		label = "Column"; label += i; label += "OrderingMode";
		out->addEnum(label.c_str(), Columns[i].OrderingMode, GUIColumnOrderingNames);
	}

	out->addInt("RowCount", Rows.size());
	for (i=0;i<Rows.size(); ++i)
	{
		core::stringc label;

		// Height currently not used and could be recalculated anyway
		//label = "Row"; label += i; label += "height";
		//out->addInt(label.c_str(), Rows[i].Height );

		//label = "Row"; label += i; label += "ItemCount";
		//out->addInt(label.c_str(), Rows[i].Items.size());
		u32 c;
		for ( c=0; c < Rows[i].Items.size(); ++c )
		{
			label = "Row"; label += i; label += "cell"; label += c; label += "text";
			out->addString(label.c_str(), Rows[i].Items[c].Text.c_str() );
			// core::stringw BrokenText;	// can be recalculated
			label = "Row"; label += i; label += "cell"; label += c; label += "color";
			out->addColor(label.c_str(), Rows[i].Items[c].Color );
			// void *data;	// can't be serialized
		}
	}

	// s32 ItemHeight;	// can be calculated
	// TotalItemHeight	// calculated
	// TotalItemWidth	// calculated
	// gui::IGUIFont* Font; // font is just the current font from environment
	// gui::IGUIScrollBar* VerticalScrollBar;		// not serialized
	// gui::IGUIScrollBar* HorizontalScrollBar;		// not serialized

	out->addBool ("Clip", Clip);
	out->addBool ("DrawBack", _drawBack);
	out->addBool ("MoveOverSelect", MoveOverSelect);

	// s32  CurrentResizedColumn;	// runtime info - depends on user action
	out->addBool ("ResizableColumns", ResizableColumns);

	// s32 Selected;	// runtime info - depends on user action
	out->addInt("CellWidthPadding", CellWidthPadding );
	out->addInt("CellHeightPadding", CellHeightPadding );
	// s32 ActiveTab;	// runtime info - depends on user action
	// bool Selecting;	// runtime info - depends on user action
	out->addEnum("CurrentOrdering", CurrentOrdering, GUIOrderingModeNames);
	out->addInt("DrawFlags", DrawFlags);
}


//! Reads attributes of the element
void CNrpTechMap::deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options)
{
	IGUITable::deserializeAttributes(in, options);

	Columns.clear();
	u32 columnCount = in->getAttributeAsInt("ColumnCount");
	u32 i;
	for (i=0;i<columnCount; ++i)
	{
		core::stringc label;
		Column column;

		label = "Column"; label += i; label += "name";
		column.Name = core::stringw(in->getAttributeAsString(label.c_str()).c_str());
		label = "Column"; label += i; label += "color";
		column.TextColor = in->getAttributeAsColor(label.c_str());
		label = "Column"; label += i; label += "width";
		column.Width = in->getAttributeAsInt(label.c_str());
		label = "Column"; label += i; label += "OrderingMode";

		column.OrderingMode = EGCO_NONE;
		s32 co = in->getAttributeAsEnumeration(label.c_str(), GUIColumnOrderingNames);
		if (co > 0)
			column.OrderingMode = EGUI_COLUMN_ORDERING(co);

		Columns.push_back(column);
	}

	Rows.clear();
	u32 rowCount = in->getAttributeAsInt("RowCount");
	for (i=0; i<rowCount; ++i)
	{
		core::stringc label;

		Row row;

		// Height currently not used and could be recalculated anyway
		//label = "Row"; label += i; label += "height";
		//row.Height = in->getAttributeAsInt(label.c_str() );

		Rows.push_back(row);

		//label = "Row"; label += i; label += "ItemCount";
		//u32 itemCount = in->getAttributeAsInt(label.c_str());
		u32 c;
		for ( c=0; c < columnCount; ++c )
		{
			Cell cell;

			label = "Row"; label += i; label += "cell"; label += c; label += "text";
			cell.Text = core::stringw(in->getAttributeAsString(label.c_str()).c_str());
			breakText( cell.Text, cell.BrokenText, Columns[c].Width );
			label = "Row"; label += i; label += "cell"; label += c; label += "color";
			cell.Color = in->getAttributeAsColor(label.c_str());
			cell.assignTech = NULL;

			Rows[Rows.size()-1].Items.push_back(cell);
		}
	}

	ItemHeight = 0;		// calculated
	TotalItemHeight = 0;	// calculated
	TotalItemWidth = 0;	// calculated

	// force font recalculation
	if ( Font )
	{
		Font->drop();
		Font = 0;
	}

	Clip = in->getAttributeAsBool("Clip");
	_drawBack = in->getAttributeAsBool("DrawBack");
	MoveOverSelect = in->getAttributeAsBool("MoveOverSelect");

	CurrentResizedColumn = -1;
	ResizeStart = 0;
	ResizableColumns = in->getAttributeAsBool("ResizableColumns");

	_selected = core::position2di( -1, -1 );
	CellWidthPadding = in->getAttributeAsInt("CellWidthPadding");
	CellHeightPadding = in->getAttributeAsInt("CellHeightPadding");
	ActiveTab = -1;
	Selecting = false;

	CurrentOrdering = (EGUI_ORDERING_MODE) in->getAttributeAsEnumeration("CurrentOrdering", GUIOrderingModeNames);
	DrawFlags = in->getAttributeAsInt("DrawFlags");

	refreshControls();
}

void CNrpTechMap::SetItemHeight( s32 height )
{
	overItemHeight_ = height;
	ItemHeight = overItemHeight_ == 0 ? Font->getDimension(L"A").Height + (CellHeightPadding * 2) : overItemHeight_;
}

AssignTech* CNrpTechMap::FindTechInMap_( const ATECH_ARRAY& parray, CNrpTechnology* obj ) 
{
	for( size_t pos=0; pos < parray.size(); pos++ )
	{
		if( parray[ pos ]->GetTechnology() == obj )
			return parray[ pos ];

		AssignTech* tmpTech = FindTechInMap_( parray[ pos ]->GetChilds(), obj );
		if( tmpTech )
			return tmpTech;
	}

	return NULL;
}

bool CNrpTechMap::_IsAlsoHaveTech( CNrpTechnology* tech )
{
	for( size_t k=0; k < techMap_.size(); k++ )
		if( techMap_[ k ]->FindTech( tech ) )
			return true;

	return false;
}

void CNrpTechMap::AddTechnology( CNrpTechnology* parent, CNrpTechnology* child )
{
	if( _IsAlsoHaveTech( child) )
		return;

	AssignTech* rAssign = NULL;

	if( !parent )
	{
		techMap_.push_back( new AssignTech( NULL, child, "" ) );
	}
	else
	{
		if( techMap_.size() > 0 )
			rAssign = FindTechInMap_( techMap_, parent );

		assert( rAssign != NULL );
		if( rAssign )
			rAssign->AddChild( child, "" );
	}

	RelocateTable_();
}

void CNrpTechMap::AddTechnology( nrp::CNrpTechnology* parent, const NrpText& internalName )
{
	AssignTech* rAssign = NULL;

	if( !parent )
	{
		techMap_.push_back( new AssignTech( NULL, NULL, internalName ) );
	}
	else
	{
		if( techMap_.size() > 0 )
			rAssign = FindTechInMap_( techMap_, parent );

		assert( rAssign != NULL );
		if( rAssign )
			rAssign->AddChild( NULL, internalName );
	}

	RelocateTable_();
}

void CNrpTechMap::AssignTechMapToTable_( const ATECH_ARRAY& pArray )
{
	for( size_t pos=0; pos < pArray.size(); pos++ )
	{
		AssignTech* pTech = pArray[ pos ];
		if( pTech )
		{
			if( pTech->GetCell().X >= getColumnCount() )
				addColumn( L"" );

			Cell& cell = Rows[ pTech->GetCell().Y ].Items[ pTech->GetCell().X ];
			cell.assignTech = pTech;
			cell.Text = core::stringw( "???" ) + cell.assignTech->GetName();

			if( CNrpTechnology* rechnology = pTech->GetTechnology() )
			{
				cell.Text = rechnology->Param( NAME ).As<NrpText>();

				AssignTechMapToTable_( pTech->GetChilds() );
			}
		}
	}
}

void CNrpTechMap::RelocateTable_()
{
	int ypos = 0, xpos = 0;
	for( size_t pos=0; pos < techMap_.size(); pos++ )
	{
		 ypos = techMap_[ pos ]->RootCell( 0, ypos );
		 const CNrpTechnology& refTech = *techMap_[ pos ]->GetTechnology(); 
		// OutputDebugString( (NrpText)refTech[ NAME ] );
		 ypos += !(int)refTech[ NEXTTECHNUMBER ];
	}

	clear();
	for( int i=0; i < ypos+1; i++ )
		 addRow( -1 );

	AssignTechMapToTable_( techMap_ );

	int itemSize = AbsoluteRect.getHeight() / (Rows.size() + 1);
	if( itemSize < 100 )
		itemSize = 100;

	SetItemHeight( itemSize );
	setColumnWidth( itemSize );
}

core::stringw CNrpTechMap::GetSelectedObjectName()
{
	return Rows[ _selected.Y ].Items[ _selected.X ].assignTech->GetName();
}

CNrpTechnology* CNrpTechMap::GetSelectedObject()
{
	return Rows[ _selected.Y ].Items[ _selected.X ].assignTech->GetTechnology();
}

int CNrpTechMap::GetSelectedObjectType()
{
	CNrpTechnology* tech = Rows[ _selected.Y ].Items[ _selected.X ].assignTech->GetTechnology();
	
	return ( tech == NULL ? TS_PROJECT : tech->Param( STATUS ).As<int>() );
}

bool CNrpTechMap::IsTechHaveUnknownChild( nrp::CNrpTechnology* parent )
{
	if( techMap_.size() > 0 )
	{
		AssignTech* rAssign = rAssign = FindTechInMap_( techMap_, parent );

		if( rAssign )
			return rAssign->HaveFutureTech();
	}

	return false;
}

} // end namespace gui

} // end namespace irr
