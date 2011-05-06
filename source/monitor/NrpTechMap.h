#pragma once

#include <irrlicht.h>
#include "LuaFunctionality.h"

namespace nrp
{
	class CNrpTechnology;
}

namespace irr
{
namespace gui
{

class IGUIFont;
class IGUIScrollBar;
class AssignTech;
typedef core::array< AssignTech* > ATECH_ARRAY;

class AssignTech
{
public:

	AssignTech( AssignTech* parent, nrp::CNrpTechnology* self, const core::stringw& name )
	{
		parent_ = parent;
		data_ = self;
		name_ = name;
	}

	AssignTech* GetParent() const
	{
		return parent_;
	}

	void AddChild( nrp::CNrpTechnology* tech, const core::stringw& name )
	{
		techs_.push_back( new AssignTech( this, tech, name ) );
	}

	nrp::CNrpTechnology* GetTechnology() const { return data_; }
	
	const ATECH_ARRAY& GetChilds() { return techs_; }

	bool HaveFutureTech()
	{
		for( size_t k=0; k < techs_.size(); k++ )
			if( techs_[ k ]->data_ == NULL )
				return true;

		return false;
	}

	const core::stringw& GetName() { return name_; }

	bool FindTech( nrp::CNrpTechnology* pTech )
	{
		if( data_ == pTech )
			return true;

		for( size_t k=0; k < techs_.size(); k++ )
			if( techs_[ k ]->FindTech( pTech ) )
				return true;

		return false;
	}

	int RootCell( int xpos, int ypos ) 
	{
		cell_ = core::position2di( xpos, ypos );
		int offset = ypos;
		for( size_t pos=0; pos < techs_.size(); pos++ )
		{
			 offset = techs_[ pos ]->RootCell( xpos+1, offset );
			 offset += (techs_.size() > 0 ? 1 : 0);
		}
		
		return offset;
	}

	core::position2di GetCell() const { return cell_; }
private:
	AssignTech() {};
	ATECH_ARRAY techs_;
	AssignTech* parent_;
	nrp::CNrpTechnology* data_;
	core::position2di cell_;
	core::stringw name_;
};

class CNrpTechMap : public IGUITable, public ILuaFunctionality
{
public:
	//! constructor
	CNrpTechMap(IGUIEnvironment* environment, IGUIElement* parent,
		s32 id, const core::rect<s32>& rectangle, bool clip=true,
		bool drawBack=false, bool moveOverSelect=true);

	//! destructor
	~CNrpTechMap();

	//! Added technology to map
	virtual void AddTechnology( nrp::CNrpTechnology* parent, nrp::CNrpTechnology* child );
	virtual void AddTechnology( nrp::CNrpTechnology* parent, const NrpText& internalName );

	virtual bool IsTechHaveUnknownChild( nrp::CNrpTechnology* parent );

	//! set a column width
	virtual void setColumnWidth(u32 width);

	//! columns can be resized by drag 'n drop
	virtual void setResizableColumns(bool resizable);

	//! can columns be resized by dran 'n drop?
	virtual bool hasResizableColumns() const;

	//! This tells the table control which ordering mode should be used when
	//! a column header is clicked.
	/** \param columnIndex: The index of the column header.
	\param state: If true, a EGET_TABLE_HEADER_CHANGED message will be sent and you can order the table data as you whish.*/
	//! \param mode: One of the modes defined in EGUI_COLUMN_ORDERING
	virtual void setColumnOrdering(u32 columnIndex, EGUI_COLUMN_ORDERING mode);
	virtual int GetSelectedObjectType();

	//! clears the table, deletes all items in the table
	virtual void clear();

	//! called if an event happened.
	virtual bool OnEvent(const SEvent &event);

	//! draws the element and its children
	virtual void draw();
	//! Set flags, as defined in EGUI_TABLE_DRAW_FLAGS, which influence the layout
	virtual void setDrawFlags(s32 flags);

	//! Get the flags, as defined in EGUI_TABLE_DRAW_FLAGS, which influence the layout
	virtual s32 getDrawFlags() const;
	core::stringw GetSelectedObjectName();
	nrp::CNrpTechnology* GetSelectedObject();

	//! Writes attributes of the object.
	//! Implement this to expose the attributes of your scene node animator for
	//! scripting languages, editors, debuggers or xml serialization purposes.
	virtual void serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options=0) const;

	//! Reads attributes of the object.
	//! Implement this to set the attributes of your scene node animator for
	//! scripting languages, editors, debuggers or xml deserialization purposes.
	virtual void deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options=0);

	virtual void SetDrawBack( bool drawBack ) { _drawBack = drawBack; }

protected:
	virtual void refreshControls();
	virtual void checkScrollbars();

private:

	struct Cell
	{
		Cell() : assignTech(0) {}
		core::stringw Text;
		core::stringw BrokenText;
		core::recti imgTechRect;
		video::SColor Color;
		AssignTech* assignTech;
	};

	struct Row
	{
		Row() {}
		core::array<Cell> Items;
	};

	struct Column
	{
		Column() : Width(0), OrderingMode(EGCO_NONE) {}
		core::stringw Name;
		video::SColor TextColor;
		u32 Width;
		EGUI_COLUMN_ORDERING OrderingMode;
	};

	//! Adds a column
	//! If columnIndex is outside the current range, do push new colum at the end
	virtual void addColumn(const wchar_t* caption, s32 columnIndex=-1);
	virtual void setColumnWidth(u32 columnIndex, u32 width) {}

	//! Returns which row is currently selected
	virtual s32 getSelected() const;

	//! set wich row is currently selected
	virtual void setSelected( s32 index );

	//! remove a column from the table
	virtual void removeColumn(u32 columnIndex);

	//! Returns the number of columns in the table control
	virtual s32 getColumnCount() const;

	//! Makes a column active. This will trigger an ordering process.
	/** \param idx: The id of the column to make active.
	\return True if successful. */
	virtual bool setActiveColumn(s32 columnIndex, bool doOrder=false);

	//! Returns which header is currently active
	virtual s32 getActiveColumn() const;

	//! Returns the ordering used by the currently active column
	virtual EGUI_ORDERING_MODE getActiveColumnOrdering() const;

	//! Returns amount of rows in the tabcontrol
	virtual s32 getRowCount() const;

	//! Returns amount of selected technology
	nrp::CNrpTechnology* getSelectedTech() const;

	//! adds a row to the table
	/** \param rowIndex: zero based index of rows. The row will be
	inserted at this position. If a row already exists
	there, it will be placed after it. If the row is larger
	than the actual number of rows by more than one, it
	won't be created. Note that if you create a row that is
	not at the end, there might be performance issues*/
	virtual u32 addRow(u32 rowIndex);

	//! Remove a row from the table
	virtual void removeRow(u32 rowIndex);

	//! clear the table rows, but keep the columns intact
	virtual void clearRows();

	//! Swap two row positions. This is useful for a custom ordering algo.
	virtual void swapRows(u32 rowIndexA, u32 rowIndexB);

	//! This tells the table to start ordering all the rows. You
	//! need to explicitly tell the table to reorder the rows when
	//! a new row is added or the cells data is changed. This makes
	//! the system more flexible and doesn't make you pay the cost
	//! of ordering when adding a lot of rows.
	//! \param columnIndex: When set to -1 the active column is used.
	virtual void orderRows(s32 columnIndex=-1, EGUI_ORDERING_MODE mode=EGOM_NONE);


	//! Set the text of a cell
	virtual void setCellText(u32 rowIndex, u32 columnIndex, const core::stringw& text);

	//! Set the text of a cell, and set a color of this cell.
	virtual void setCellText(u32 rowIndex, u32 columnIndex, const core::stringw& text, video::SColor color);

	//! Set the data of a cell
	//! data will not be serialized.
	virtual void setCellData(u32 rowIndex, u32 columnIndex, void *data);

	//! Set the color of a cell text
	virtual void setCellColor(u32 rowIndex, u32 columnIndex, video::SColor color);

	//! Get the text of a cell
	virtual const wchar_t* getCellText(u32 rowIndex, u32 columnIndex ) const;

	//! Get the data of a cell
	virtual void* getCellData(u32 rowIndex, u32 columnIndex ) const;

	virtual void SetItemHeight( s32 height );

	void breakText(const core::stringw &text, core::stringw & brokenText, u32 cellWidth);
	void selectNew( core::position2di cell, bool onlyHover=false);
	bool selectColumnHeader(s32 xpos, s32 ypos);
	bool dragColumnStart(s32 xpos, s32 ypos);
	void recalculateHeights();
	void recalculateWidths();
	AssignTech* FindTechInMap_( const ATECH_ARRAY& parray, nrp::CNrpTechnology* obj );
	void AssignTechMapToTable_( const ATECH_ARRAY& pArray );
	void RelocateTable_();
	bool _IsAlsoHaveTech( nrp::CNrpTechnology* tech );
	core::array< Column > Columns;
	core::array< Row > Rows;

	ATECH_ARRAY techMap_;
	bool _rMouseDown;
	gui::IGUIFont* Font;
	gui::IGUIScrollBar* VerticalScrollBar;
	gui::IGUIScrollBar* HorizontalScrollBar;
	bool Clip;
	bool _drawBack;
	bool MoveOverSelect;
	bool Selecting;
	s32  CurrentResizedColumn;
	s32  ResizeStart;
	bool ResizableColumns;

	s32 ItemHeight, _ColumnWidth;
	s32 overItemHeight_;
	s32 TotalItemHeight;
	s32 TotalItemWidth;
	core::position2di _selected;
	s32 CellHeightPadding;
	s32 CellWidthPadding;
	s32 ActiveTab;
	EGUI_ORDERING_MODE CurrentOrdering;
	s32 DrawFlags;
	s32 _startTimeMouseDown;
};

} // end namespace gui
} // end namespace irr
