#include "StdAfx.h"

#include "NrpWindow.h"
#include "NrpActionType.h"
#include "nrpGUIEnvironment.h"
#include <IGUIFont.h>
#include <IGUIEnvironment.h>
#include <IGUIButton.h>
#include <ITexture.h>
#include <IVideoDriver.h>


namespace irr
{
namespace gui
{

//! constructor
CNrpWindow::CNrpWindow(IGUIEnvironment* environment, IGUIElement* parent, video::ITexture* texture, s32 id, core::rect<s32> rectangle)
	: IGUIWindow(environment, parent, id, rectangle), Dragging(false), IsDraggable(true), DrawBackground(true), DrawTitlebar(true)
{
#ifdef _DEBUG
			setDebugName("CNrpWindow");
#endif

	IGUISkin* skin = 0;
	if (environment)
		skin = environment->getSkin();

	background_ = NULL;
	if( texture )
	{
		background_	= texture;
		background_->grab();
	}

	IGUISpriteBank* sprites = 0;
	video::SColor color(255,255,255,255);

	s32 buttonw = 15;
	if (skin)
	{
		buttonw = skin->getSize(EGDS_WINDOW_BUTTON_WIDTH);
		sprites = skin->getSpriteBank();
		color = skin->getColor(EGDC_WINDOW_SYMBOL);
	}
	s32 posx = RelativeRect.getWidth() - buttonw - 4;

	IGUIButton* btn = Environment->addButton( core::rect<s32>(posx, 3, posx + buttonw, 3 + buttonw), 
													this, -1,
													L"", 
													skin ? skin->getDefaultText(EGDT_WINDOW_CLOSE) : L"Close" );
	btn->setSubElement(true);
	btn->setTabStop(false);
	btn->setAlignment(EGUIA_LOWERRIGHT, EGUIA_LOWERRIGHT, EGUIA_UPPERLEFT, EGUIA_UPPERLEFT);
	buttons_.push_back( btn );
	posx -= buttonw + 2;

	btn = Environment->addButton( core::rect<s32>(posx, 3, posx + buttonw, 3 + buttonw), 
														this, 
														-1,
														L"", 
														skin ? skin->getDefaultText(EGDT_WINDOW_RESTORE) : L"Restore" );
	btn->setVisible(false);
	btn->setSubElement(true);
	btn->setTabStop(false);
	btn->setAlignment(EGUIA_LOWERRIGHT, EGUIA_LOWERRIGHT, EGUIA_UPPERLEFT, EGUIA_UPPERLEFT);
	buttons_.push_back( btn );
	posx -= buttonw + 2;

	btn = Environment->addButton(	core::rect<s32>(posx, 3, posx + buttonw, 3 + buttonw), 
														this, 
														-1,
														L"", 
														skin ? skin->getDefaultText(EGDT_WINDOW_MINIMIZE) : L"Minimize" );
	btn->setVisible(false);
	btn->setSubElement(true);
	btn->setTabStop(false);
	btn->setAlignment(EGUIA_LOWERRIGHT, EGUIA_LOWERRIGHT, EGUIA_UPPERLEFT, EGUIA_UPPERLEFT);
	buttons_.push_back( btn );

	buttons_[ BTNE_MINIMAZE ]->grab();
	buttons_[ BTNE_MAXIMAZE ]->grab();
	buttons_[ BTNE_CLOSE ]->grab();

	// this element is a tab group
	setTabGroup(true);
	setTabStop(true);
	setTabOrder(-1);
}


//! destructor
CNrpWindow::~CNrpWindow()
{
	if (buttons_[ BTNE_MINIMAZE ])
		buttons_[ BTNE_MINIMAZE ]->drop();

	if (buttons_[ BTNE_MAXIMAZE ])
		buttons_[ BTNE_MAXIMAZE ]->drop();

	if (buttons_[ BTNE_CLOSE ])
		buttons_[ BTNE_CLOSE ]->drop();
}


//! called if an event happened.
bool CNrpWindow::OnEvent(const SEvent& event)
{
	if (IsEnabled)
	{
		switch(event.EventType)
		{
		case EET_GUI_EVENT:
			if( event.GUIEvent.EventType == EGET_LISTBOX_CHANGED )
			{
				DoLuaFunctionsByType( GUIELEMENT_LBXITEM_SELECTED, (void*)event.GUIEvent.Caller );
				return true;
			}

			if( event.GUIEvent.EventType == EGET_COMBO_BOX_CHANGED )
			{
				DoLuaFunctionsByType( GUIELEMENT_CMBXITEM_SELECTED, (void*)event.GUIEvent.Caller );
				return true;
			}

			if (event.GUIEvent.EventType == EGET_ELEMENT_FOCUS_LOST)
			{
				Dragging = false;
			}
			else
				if (event.GUIEvent.EventType == EGET_ELEMENT_FOCUSED)
				{
					if (Parent && ((event.GUIEvent.Caller == this) || isMyChild(event.GUIEvent.Caller)))
						Parent->bringToFront(this);
				}
				else
					if (event.GUIEvent.EventType == EGET_BUTTON_CLICKED)
					{
						if (event.GUIEvent.Caller == buttons_[ BTNE_CLOSE ] )
						{
							if (Parent)
							{
								// send close event to parent
								SEvent e;
								e.EventType = EET_GUI_EVENT;
								e.GUIEvent.Caller = this;
								e.GUIEvent.Element = 0;
								e.GUIEvent.EventType = EGET_ELEMENT_CLOSED;

								// if the event was not absorbed
								if (!Parent->OnEvent(e))
									remove();

								return true;

							}
							else
							{
								remove();
								return true;
							}
						}
					}
		break;

		case EET_MOUSE_INPUT_EVENT:
			switch(event.MouseInput.Event)
			{
			case EMIE_LMOUSE_PRESSED_DOWN:
				DragStart.X = event.MouseInput.X;
				DragStart.Y = event.MouseInput.Y;
				Dragging = IsDraggable;
				if (Parent)
					Parent->bringToFront(this);
				DoLuaFunctionsByType( GUIELEMENT_LMOUSE_DOWN, (void*)&event );
				return true;
			case EMIE_RMOUSE_LEFT_UP:
			case EMIE_LMOUSE_LEFT_UP:
				Dragging = false;
				DoLuaFunctionsByType( event.MouseInput.Event == EMIE_LMOUSE_LEFT_UP ? 
											GUIELEMENT_LMOUSE_LEFTUP : 
											GUIELEMENT_RMOUSE_LEFTUP , 
									  (void*)&event );
				return true;
			case EMIE_MOUSE_MOVED:
				if ( !event.MouseInput.isLeftPressed() )
					Dragging = false;

				if (Dragging)
				{
					// gui window should not be dragged outside its parent
					if (Parent &&
						(event.MouseInput.X < Parent->getAbsolutePosition().UpperLeftCorner.X +1 ||
						event.MouseInput.Y < Parent->getAbsolutePosition().UpperLeftCorner.Y +1 ||
						event.MouseInput.X > Parent->getAbsolutePosition().LowerRightCorner.X -1 ||
						event.MouseInput.Y > Parent->getAbsolutePosition().LowerRightCorner.Y -1))
						return true;

					move(core::position2d<s32>(event.MouseInput.X - DragStart.X, event.MouseInput.Y - DragStart.Y));
					DragStart.X = event.MouseInput.X;
					DragStart.Y = event.MouseInput.Y;
					return true;
				}
				break;
			default:
				break;
			}
		default:
			break;
		}
	}

	return IGUIElement::OnEvent(event);
}


//! Updates the absolute position.
void CNrpWindow::updateAbsolutePosition()
{
	IGUIElement::updateAbsolutePosition();
}


//! draws the element and its children
void CNrpWindow::draw()
{
	DoLuaFunctionsByType( GUIELEMENT_BEFORE_DRAW, this );

	if ( IsVisible )
	{
		//выполн€ютс€ функции до отрисовки элемента
		
		IGUISkin* skin = Environment->getSkin();

		core::recti rect = AbsoluteRect;

		// draw body fast
		if ( DrawBackground )
		{
			rect = skin->draw3DWindowBackground(this, DrawTitlebar, skin->getColor(EGDC_ACTIVE_BORDER),
								        		AbsoluteRect, &AbsoluteClippingRect);

			if (DrawTitlebar && Text.size())
			{
				rect.UpperLeftCorner.X += skin->getSize(EGDS_TITLEBARTEXT_DISTANCE_X);
				rect.UpperLeftCorner.Y += skin->getSize(EGDS_TITLEBARTEXT_DISTANCE_Y);
				rect.LowerRightCorner.X -= skin->getSize(EGDS_WINDOW_BUTTON_WIDTH) + 5;

				IGUIFont* font = skin->getFont(EGDF_WINDOW);
				if (font)
				{
					font->draw(	Text.c_str(), rect,
								skin->getColor(EGDC_ACTIVE_CAPTION), 
								false, 
								true, 
								&AbsoluteClippingRect);
				}
			}
		}

		if( background_ )
		{   
			core::recti rsize( core::vector2di( 0, 0 ), background_->getSize() );
			video::SColor color( AlphaBlend, 0xff, 0xff, 0xfe );
			video::SColor colors[ 4 ] = { color, color, color, color };
			Environment->getVideoDriver()->draw2DImage( background_, AbsoluteRect, rsize, 0, colors, true );
		}

		//выполн€ютс€ функции после отрисовки элемента
	}

	IGUIElement::draw();

	DoLuaFunctionsByType( GUIELEMENT_AFTER_DRAW, this );
}


//! Returns pointer to the close button
IGUIButton* CNrpWindow::getCloseButton() const
{
	return buttons_[ BTNE_CLOSE ];
}


//! Returns pointer to the minimize button
IGUIButton* CNrpWindow::getMinimizeButton() const
{
	return buttons_[ BTNE_MINIMAZE ];
}


//! Returns pointer to the maximize button
IGUIButton* CNrpWindow::getMaximizeButton() const
{
	return buttons_[ BTNE_MAXIMAZE ];
}

//! Returns true if the window is draggable, false if not
bool CNrpWindow::isDraggable() const
{
	return IsDraggable;
}

//! Sets whether the window is draggable
void CNrpWindow::setDraggable(bool draggable)
{
	IsDraggable = draggable;

	if (Dragging && !IsDraggable)
		Dragging = false;
}

//! Set if the window background will be drawn
void CNrpWindow::setDrawBackground(bool draw)
{
	DrawBackground = draw;
}

//! Get if the window background will be drawn
bool CNrpWindow::getDrawBackground() const
{
	return DrawBackground;
}

//! Set if the window titlebar will be drawn
void CNrpWindow::setDrawTitlebar(bool draw)
{
	DrawTitlebar = draw;
}

//! Get if the window titlebar will be drawn
bool CNrpWindow::getDrawTitlebar() const
{
	return DrawTitlebar;
}

//! Writes attributes of the element.
void CNrpWindow::serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options=0) const
{
	IGUIWindow::serializeAttributes(out,options);

	out->addBool("IsDraggable",	    IsDraggable);
	out->addBool("DrawBackground",	DrawBackground);
	out->addBool("DrawTitlebar",	DrawTitlebar);

	// Currently we can't just serialize attributes of sub-elements.
	// To do this we either
	// a) allow further serialization after attribute serialiation (second function, callback or event)
	// b) add an IGUIElement attribute
	// c) extend the attribute system to allow attributes to have sub-attributes
	// We just serialize the most important info for now until we can do one of the above solutions.
	out->addBool("IsCloseVisible", buttons_[ BTNE_CLOSE ]->isVisible());
	out->addBool("IsMinVisible", buttons_[ BTNE_MINIMAZE ]->isVisible());
	out->addBool("IsRestoreVisible", buttons_[ BTNE_MAXIMAZE ]->isVisible());
}


//! Reads attributes of the element
void CNrpWindow::deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options=0)
{
	IGUIWindow::deserializeAttributes(in,options);

	Dragging = false;
	IsDraggable = in->getAttributeAsBool("IsDraggable");
	DrawBackground = in->getAttributeAsBool("DrawBackground");
	DrawTitlebar = in->getAttributeAsBool("DrawTitlebar");

	buttons_[ BTNE_CLOSE ]->setVisible( in->getAttributeAsBool("IsCloseVisible") );
	buttons_[ BTNE_MINIMAZE ]->setVisible( in->getAttributeAsBool("IsMinVisible") );
	buttons_[ BTNE_MAXIMAZE ]->setVisible( in->getAttributeAsBool("IsRestoreVisible") );
}

void CNrpWindow::setBackgroundTexture( video::ITexture* texture )
{
	if( background_ )
		background_->drop();

	if( texture )
		texture->grab();

	background_ = texture;
}

core::recti CNrpWindow::getClientRect() const
{
	return core::recti(0, 0, 0, 0);
}

void CNrpWindow::setAlphaBlend( u32 new_alpha )
{
	core::list<IGUIElement*>::Iterator it = Children.begin();
	for (; it != Children.end(); it++)
		(*it)->setAlphaBlend( new_alpha );

	IGUIWindow::setAlphaBlend( new_alpha );
}

} // end namespace gui
} // end namespace irr