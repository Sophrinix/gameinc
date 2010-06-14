#pragma once

#include "include/irrlicht.h"

using namespace irr;
//*! класс для собственного списка
class CListItem
{
private:
	   s32 level;
       CListItem* parent;	   
public:
	   core::rect<s32> textect;
	   core::stringw text;
	   s32 icon;
	   void* obj;
	   bool open;
  	   
/*	   struct ListItemOverrideColor
	   {
		   ListItemOverrideColor() : Use(false) {}
		   bool Use;
		   video::SColor Color;
	   };
	   ListItemOverrideColor OverrideColors[EGUI_LBC_COUNT];
*/
	   CListItem()
	   {
		   level =  0;
		   obj = NULL;	    
		   open = false;
		   parent = NULL;
	   }

	   CListItem( CListItem* Parent, void* Obj )
	   {
		   level = Parent != NULL ? Parent->GetLevel()+1 : 0;
		   obj = Obj;	    
		   open = false;
		   parent = Parent;
	   }

	   CListItem( CListItem* Parent, core::stringw Text, void* Obj )
	   {
		   level = Parent != NULL ? Parent->GetLevel()+1 : 0;
		   text = Text;
		   obj = Obj;	    
		   open = false;
		   parent = Parent;
	   }

	   bool erase( CListItem* rmItem )
	   {
		   for( size_t cnt=0; cnt < Items.size(); cnt++ )
		   {
			    if( &(Items[ cnt ]) == rmItem )
				{
					Items.erase( cnt );
					return true;
				}
				else if( Items[ cnt ].erase( rmItem ) )
					 return true;
		   }

		   return false;
	   }

	   CListItem* insert( core::stringw Text, void* ptrObj, size_t Position )
	   {
			if( Position >= Items.size() )
				return NULL;

			Items.insert( CListItem( this, Text, ptrObj ), Position );
			return &Items[ Position ];
	   }

	   CListItem* add( core::stringw Text, void* ptrObj )
	   {
			Items.push_back( CListItem( this, Text, ptrObj ) );
			return &Items.getLast();
	   }

	   void clear()
	   {
		   for( size_t cnt=0; cnt < Items.size(); cnt++ )
			    Items[ cnt ].clear();
	   }

	   inline s32 GetLevel() { return level; }
	   inline CListItem* get_Parent() { return parent; }
		//core::stringw get_Text() { return text; }

		// A multicolor extension

	   core::array< CListItem > Items;
};
