#include "StdAfx.h"
#include "ImageGUISkin.h"
#include "clipRects.h"
#include "nrpGUIEnvironment.h"

namespace irr
{

namespace gui
{

CImageGUISkin::CImageGUISkin( IGUIEnvironment* env )				//конструктор расширенного скина
{
	native_gui_ = env;			
	native_skin_ = env->getSkin();
	native_skin_->grab();											//чтобы была возможность использовать нативные методы рисования 
																	//
	UseGradient = false;

	native_skin_->setIcon( EGDI_CHECK_BOX_CHECKED, 0 );				//нет иконок у нажатого чекбокса
	// чтобы было видно белые треугольнички на скроллбарах
	native_skin_->setColor(EGDC_WINDOW_SYMBOL, video::SColor(255, 255, 255, 255));
}
//////////////////////////////////////////////////////////////////////

//! destructor
CImageGUISkin::~CImageGUISkin()
{
}
//////////////////////////////////////////////////////////////////////

//! returns default color
video::SColor CImageGUISkin::getColor(EGUI_DEFAULT_COLOR color) const
{
    return native_skin_->getColor(color);
}

//! sets a default color
void CImageGUISkin::setColor(EGUI_DEFAULT_COLOR which, video::SColor newColor)
{
    native_skin_->setColor(which, newColor);
}

//! returns size for the given size type
s32 CImageGUISkin::getSize(EGUI_DEFAULT_SIZE size) const
{
	return native_skin_->getSize(size);
}

//! sets a default size
void CImageGUISkin::setSize(EGUI_DEFAULT_SIZE which, s32 size)
{
    native_skin_->setSize(which, size);
}

//! returns the default font
IGUIFont* CImageGUISkin::getFont(EGUI_DEFAULT_FONT which) const
{
return native_skin_->getFont(which);
}

//! sets a default font
void CImageGUISkin::setFont(IGUIFont* font, EGUI_DEFAULT_FONT which)
{
    native_skin_->setFont(font, which);
}

//! gets the sprite bank stored
IGUISpriteBank* CImageGUISkin::getSpriteBank() const
{
	return native_skin_->getSpriteBank();
}

//! set a new sprite bank or remove one by passing 0
void CImageGUISkin::setSpriteBank(IGUISpriteBank* bank)
{
	native_skin_->setSpriteBank(bank);
}

//! Returns a default icon
u32 CImageGUISkin::getIcon(EGUI_DEFAULT_ICON icon) const
{
return native_skin_->getIcon(icon);
}

//! Sets a default icon
void CImageGUISkin::setIcon(EGUI_DEFAULT_ICON icon, u32 index)
{
native_skin_->setIcon(icon, index);
}

//! Returns a default text. For example for Message box button captions:
//! "OK", "Cancel", "Yes", "No" and so on.
const wchar_t* CImageGUISkin::getDefaultText(EGUI_DEFAULT_TEXT text) const
{
   return native_skin_->getDefaultText( text );
}


//! Sets a default text. For example for Message box button captions:
//! "OK", "Cancel", "Yes", "No" and so on.
void CImageGUISkin::setDefaultText(EGUI_DEFAULT_TEXT which, const wchar_t* newText)
{
	native_skin_->setDefaultText( which, newText );
}

//! draws a standard 3d button pane
/**	Used for drawing for example buttons in normal state.
It uses the colors EGDC_3D_DARK_SHADOW, EGDC_3D_HIGH_LIGHT, EGDC_3D_SHADOW and
EGDC_3D_FACE for this. See EGUI_DEFAULT_COLOR for details.
\param rect: Defining area where to draw.
\param clip: Clip area.
\param element: Pointer to the element which wishes to draw this. This parameter
is usually not used by ISkin, but can be used for example by more complex
implementations to find out how to draw the part exactly. */
void CImageGUISkin::draw3DButtonPaneStandard(	IGUIElement* element,
												const core::rect<s32>& r,
												const core::rect<s32>* clip)
{
	video::IVideoDriver* driver = native_gui_->getVideoDriver();		
	if (!driver)									//нельзя работать с пустым драйвером
		return;

	SImageGUIElementStyle style = Config.Button;	//этот элемент отрисуется если не будет замен	
	bool need_posteffect = false;					//пост эффекты для кнопок, чтобы было плавное угасание ховеред текстуры

	if(  element->getType() == EGUIET_BUTTON )		//если элемент, который пытается отрисоваться кнопка
	{
		if( !element->isEnabled() )					//и он не запрещен
			style = Config.ButtonDisabled;				
		else										//проверяем состояние нажатости		
		{
			if( static_cast< IGUIButton* >( element )->isPressed() )		
				style = Config.ButtonPressed;		//если нажат ставим соответсвующий конфиг	
		}

		need_posteffect = true;						//нужен постэффект для кнопки
	}

	if ( !style.Texture )											//рисованная текстура отсутсвует
	{
		native_skin_->draw3DButtonPaneStandard( element, r, clip );			//поэтому рисуем дефолтовый элемент
		return;
	}
	else
	{
		video::SColor color( 0xff, 0xff, 0xff, 0xff );				//видна вся текстура
		drawElementStyle( element, style, r, clip, &color  );		//рисуем текстуру из конфига

		if( need_posteffect && Config.ButtonHovered.Texture )       //добавляем свистелок
			draw3DButtonPostEffect( element, r, clip );				//	
	}
}

void CImageGUISkin::draw3DButtonPostEffect(	 IGUIElement* element,		//свистелка для кнопки... плавное угасание ховеред текстуры
											 const core::rect<s32>& r,
											 const core::rect<s32>* clip )
{
	if( !element->isEnabled() )
		return; 

	core::recti draw_rect( r );											//
	core::recti clip_rect( *clip );
	bool up = native_gui_->isHovered( element );						//если курсор находится над нами

	s32 ab = element->getAlphaBlend();									//получаем прозрачность кнопки
	if( up )
	{
		if( ab < 0xff )	ab+=core::s32_min( 3, 0xff - ab );											//изменяем прозрачность
	}
	else
	{
		if( ab > 5 ) ab-=core::s32_min( 3, ab - 5 );
	}

	element->setAlphaBlend( ab );										//запоминаем прозрачность, чтобы потом её модно было использовать
	if( ab <= 0xff && ab >= 5 )
	{
		core::dimension2di half_size;									//рассчитываем на сколько надо сдвинуть границы элемента
																		//чтобы центр остался на месте, если разные размеры
																		//текстур
		half_size.Width = Config.ButtonHovered.Texture->getOriginalSize().Width - Config.Button.Texture->getOriginalSize().Width;
		half_size.Height = Config.ButtonHovered.Texture->getOriginalSize().Height - Config.Button.Texture->getOriginalSize().Height;
		half_size /= 2;
		draw_rect.UpperLeftCorner -= half_size;
		draw_rect.LowerRightCorner += half_size;
		clip_rect.UpperLeftCorner -= half_size;
		clip_rect.LowerRightCorner += half_size;
		video::SColor color( ab, 0xff, 0xff, 0xff );
		drawElementStyle( element, Config.ButtonHovered, draw_rect, &clip_rect, &color  );
	}
}

//! draws a pressed 3d button pane
/**	Used for drawing for example buttons in pressed state.
It uses the colors EGDC_3D_DARK_SHADOW, EGDC_3D_HIGH_LIGHT, EGDC_3D_SHADOW and
EGDC_3D_FACE for this. See EGUI_DEFAULT_COLOR for details.
\param rect: Defining area where to draw.
\param clip: Clip area.
\param element: Pointer to the element which wishes to draw this. This parameter
is usually not used by ISkin, but can be used for example by more complex
implementations to find out how to draw the part exactly. */
void CImageGUISkin::draw3DButtonPanePressed(	IGUIElement* element,  
										      const core::rect<s32>& r,
										      const core::rect<s32>* clip)
{
	SImageGUIElementStyle style =  Config.ButtonPressed;								//рисуем нажатую кнопку
	if ( !style.Texture )														//если нет текстуры
	{
		native_skin_->draw3DButtonPanePressed( element, r, clip );							//рисуем дефолтовую кнопку
		return;
	}

	drawElementStyle( element, style, r, clip );							//иначе нажатую кнопку 
}


//! draws a sunken 3d pane
/** Used for drawing the background of edit, combo or check boxes.
\param element: Pointer to the element which wishes to draw this. This parameter
is usually not used by ISkin, but can be used for example by more complex
implementations to find out how to draw the part exactly.
\param bgcolor: Background color.
\param flat: Specifies if the sunken pane should be flat or displayed as sunken
deep into the ground.
\param rect: Defining area where to draw.
\param clip: Clip area.	*/
void CImageGUISkin::draw3DSunkenPane(IGUIElement* element, video::SColor bgcolor,
								bool flat, bool fillBackGround,
								const core::rect<s32>& r,
								const core::rect<s32>* clip)
{
	SImageGUIElementStyle elstyle;															//рисование разных элементов
	switch( element->getType() )
	{
	case EGUIET_CHECK_BOX:																	//чекбокса
		{
			IGUICheckBox* cbx = static_cast< IGUICheckBox* >( element );

			if( cbx->isEnabled() )
			{
				elstyle = cbx->isChecked() 
							? Config.CheckBoxChecked 
							: Config.CheckBox;
			}
			else
				elstyle = Config.CheckBoxDisabled;
		}
	break;
	
	case EGUIET_EDIT_BOX:																	//поле ввода
		{
			IGUICheckBox* ebox = static_cast< IGUICheckBox* >( element );
			if (ebox->isEnabled())
			{
				elstyle = native_gui_->isHovered( element )
										? Config.EditBoxHovered
										: Config.EditBox;				
			}
			else 
				elstyle = Config.EditBoxDisabled;
		}
	break;
	
	case EGUIET_COMBO_BOX:
		{
			IGUIComboBox* combo = static_cast< IGUIComboBox* >( element );
			if (combo->isEnabled())
			{
				elstyle = native_gui_->isHovered( element )
											? Config.ComboBoxHovered
											: Config.ComboBox;
			}
			else
				elstyle = Config.ComboBoxDisabled;
		}
	break;

	default:																				//другие
		 elstyle = Config.SunkenPane;
	break;
	}

	if ( !elstyle.Texture )
		native_skin_->draw3DSunkenPane(element, bgcolor, flat, fillBackGround, r, clip);
	else 
		drawElementStyle( element, elstyle, r, clip );
}

//! draws a window background
/** Used for drawing the background of dialogs and windows.
\param element: Pointer to the element which wishes to draw this. This parameter
is usually not used by ISkin, but can be used for example by more complex
implementations to find out how to draw the part exactly.
\param titleBarColor: Title color.
\param drawTitleBar: True to enable title drawing.
\param rect: Defining area where to draw.
\param clip: Clip area.
\return Returns rect where to draw title bar text. */
core::rect<s32> CImageGUISkin::draw3DWindowBackground(IGUIElement* element,
													bool drawTitleBar, video::SColor titleBarColor,
													const core::rect<s32>& r,
													const core::rect<s32>* cl, 
													core::rect<s32>* checkClientArea )
{
	SImageGUIElementStyle elstyle = Config.Window;
	if ( !elstyle.Texture )																	//рисование окошка если нет скина
	{
		return native_skin_->draw3DWindowBackground(element, drawTitleBar, titleBarColor, r, cl, checkClientArea );
	}

	drawElementStyle( element, elstyle, r, cl );

	return core::rect<s32>( r.UpperLeftCorner.X+Config.Window.DstBorder.Left, 		//со скином
							r.UpperLeftCorner.Y, 
							r.LowerRightCorner.X-Config.Window.DstBorder.Right, 
							r.UpperLeftCorner.Y+Config.Window.DstBorder.Top 
						   );
}

//! draws a standard 3d menu pane
/**	Used for drawing for menus and context menus.
It uses the colors EGDC_3D_DARK_SHADOW, EGDC_3D_HIGH_LIGHT, EGDC_3D_SHADOW and
EGDC_3D_FACE for this. See EGUI_DEFAULT_COLOR for details.
\param element: Pointer to the element which wishes to draw this. This parameter
is usually not used by ISkin, but can be used for example by more complex
implementations to find out how to draw the part exactly.
\param rect: Defining area where to draw.
\param clip: Clip area.	*/
void CImageGUISkin::draw3DMenuPane(IGUIElement* element,
							  const core::rect<s32>& r, const core::rect<s32>* clip)
{
	video::IVideoDriver* driver = native_gui_->getVideoDriver();
	if (!driver)
		return;

	SImageGUIElementStyle elstyle = Config.Window;	
	if( element->getType() == EGUIET_CONTEXT_MENU || element->getType() == EGUIET_MENU )
		elstyle = Config.ContextMenu;

	if( elstyle.Texture != NULL )
		drawElementStyle( element, elstyle, r, clip );
	else
		driver->draw2DRectangle( getColor( EGDC_3D_FACE), r, clip );
}


//! draws a standard 3d tool bar
/**	Used for drawing for toolbars and menus.
\param element: Pointer to the element which wishes to draw this. This parameter
is usually not used by ISkin, but can be used for example by more complex
implementations to find out how to draw the part exactly.
\param rect: Defining area where to draw.
\param clip: Clip area.	*/
void CImageGUISkin::draw3DToolBar(IGUIElement* element,
							    const core::rect<s32>& r,
							    const core::rect<s32>* clip)
{
	video::IVideoDriver* driver = native_gui_->getVideoDriver();
	if (!driver)
		return;

	core::rect<s32> rect = r;

	/*s32 alpha = element->get_AlphaBlend();*/
	video::SColor clr_3dds = getColor(EGDC_3D_DARK_SHADOW);
	video::SColor clr_3dhl = getColor(EGDC_3D_HIGH_LIGHT);
	video::SColor clr_3dsh = getColor(EGDC_3D_SHADOW);
	video::SColor clr_3dfc = getColor(EGDC_3D_FACE);
	video::SColor clr_wnd = getColor( EGDC_WINDOW );
	video::SColor clr_3dlh = getColor( EGDC_3D_LIGHT );
// 	clr_3dds.setAlpha( alpha );
// 	clr_3dhl.setAlpha( alpha );
// 	clr_3dsh.setAlpha( alpha );
// 	clr_3dfc.setAlpha( alpha );
// 	clr_wnd.setAlpha( alpha );
// 	clr_3dlh.setAlpha( alpha );

	rect.UpperLeftCorner.X = r.UpperLeftCorner.X;
	rect.UpperLeftCorner.Y = r.LowerRightCorner.Y - 1;
	rect.LowerRightCorner.Y = r.LowerRightCorner.Y;
	rect.LowerRightCorner.X = r.LowerRightCorner.X;
	driver->draw2DRectangle( clr_3dsh, rect, clip);

	rect = r;
	rect.LowerRightCorner.Y -= 1;

	if (!UseGradient)
	{
		driver->draw2DRectangle( clr_3dfc, rect, clip);
	}
	else
	{
		const video::SColor c1 = clr_3dfc;
		const video::SColor c2 = clr_3dsh;
		driver->draw2DRectangle(rect, c1, c1, c2, c2, clip);
	}
}


//! draws a tab button
/**	Used for drawing for tab buttons on top of tabs.
\param element: Pointer to the element which wishes to draw this. This parameter
is usually not used by ISkin, but can be used for example by more complex
implementations to find out how to draw the part exactly.
\param active: Specifies if the tab is currently active.
\param rect: Defining area where to draw.
\param clip: Clip area.	*/
void CImageGUISkin::draw3DTabButton(IGUIElement* element, 
									bool active,
									const core::rect<s32>& rect, 
									const core::rect<s32>* clip, 
									gui::EGUI_ALIGNMENT alignment )
{
	video::IVideoDriver* driver = native_gui_->getVideoDriver();
	if (!driver)
		return;

	core::rect<s32> tr = rect;

	/*s32 alpha = element->get_AlphaBlend();*/
	video::SColor clr_3dds = getColor(EGDC_3D_DARK_SHADOW);
	video::SColor clr_3dhl = getColor(EGDC_3D_HIGH_LIGHT);
	video::SColor clr_3dsh = getColor(EGDC_3D_SHADOW);
	video::SColor clr_3dfc = getColor(EGDC_3D_FACE);
	video::SColor clr_wnd = getColor( EGDC_WINDOW );
	video::SColor clr_3dlh = getColor( EGDC_3D_LIGHT );
// 	clr_3dds.setAlpha( alpha );
// 	clr_3dhl.setAlpha( alpha );
// 	clr_3dsh.setAlpha( alpha );
// 	clr_3dfc.setAlpha( alpha );
// 	clr_wnd.setAlpha( alpha );
// 	clr_3dlh.setAlpha( alpha );

	tr.LowerRightCorner.X -= 2;
	tr.LowerRightCorner.Y = tr.UpperLeftCorner.Y + 1;
	tr.UpperLeftCorner.X += 1;
	driver->draw2DRectangle( clr_3dhl, tr, clip);

	// draw left highlight
	tr = rect;
	tr.LowerRightCorner.X = tr.UpperLeftCorner.X + 1;
	tr.UpperLeftCorner.Y += 1;
	driver->draw2DRectangle( clr_3dhl, tr, clip);

	// draw grey background
	tr = rect;
	tr.UpperLeftCorner.X += 1;
	tr.UpperLeftCorner.Y += 1;
	tr.LowerRightCorner.X -= 2;
	driver->draw2DRectangle( clr_3dfc, tr, clip);

	// draw right middle gray shadow
	tr.LowerRightCorner.X += 1;
	tr.UpperLeftCorner.X = tr.LowerRightCorner.X - 1;
	driver->draw2DRectangle( clr_3dsh, tr, clip);

	tr.LowerRightCorner.X += 1;
	tr.UpperLeftCorner.X += 1;
	tr.UpperLeftCorner.Y += 1;
	driver->draw2DRectangle( clr_3dds, tr, clip);
}


//! draws a tab control body
/**	\param element: Pointer to the element which wishes to draw this. This parameter
is usually not used by ISkin, but can be used for example by more complex
implementations to find out how to draw the part exactly.
\param border: Specifies if the border should be drawn.
\param background: Specifies if the background should be drawn.
\param rect: Defining area where to draw.
\param clip: Clip area.	*/
void CImageGUISkin::draw3DTabBody(	IGUIElement* element, 
									bool border, 
									bool background,
									const core::rect<s32>& rect, 
									const core::rect<s32>* clip, 
									s32 tabHeight, 
									gui::EGUI_ALIGNMENT alignment)
{
	video::IVideoDriver* driver = native_gui_->getVideoDriver();
	if (!driver)
		return;

	core::rect<s32> tr = rect;

	/*s32 alpha = element->get_AlphaBlend();*/
	video::SColor clr_3dds = getColor(EGDC_3D_DARK_SHADOW);
	video::SColor clr_3dhl = getColor(EGDC_3D_HIGH_LIGHT);
	video::SColor clr_3dsh = getColor(EGDC_3D_SHADOW);
	video::SColor clr_3dfc = getColor(EGDC_3D_FACE);
	video::SColor clr_wnd = getColor( EGDC_WINDOW );
	video::SColor clr_3dlh = getColor( EGDC_3D_LIGHT );
// 	clr_3dds.setAlpha( alpha );
// 	clr_3dhl.setAlpha( alpha );
// 	clr_3dsh.setAlpha( alpha );
// 	clr_3dfc.setAlpha( alpha );
// 	clr_wnd.setAlpha( alpha );
// 	clr_3dlh.setAlpha( alpha );

	// draw border.
	if (border)
	{
		// draw left hightlight
		tr.UpperLeftCorner.Y += getSize(gui::EGDS_BUTTON_HEIGHT) + 2;
		tr.LowerRightCorner.X = tr.UpperLeftCorner.X + 1;
		driver->draw2DRectangle(getColor(EGDC_3D_HIGH_LIGHT), tr, clip);

		// draw right shadow
		tr.UpperLeftCorner.X = rect.LowerRightCorner.X - 1;
		tr.LowerRightCorner.X = tr.UpperLeftCorner.X + 1;
		driver->draw2DRectangle( clr_3dhl, tr, clip);

		// draw lower shadow
		tr = rect;
		tr.UpperLeftCorner.Y = tr.LowerRightCorner.Y - 1;
		driver->draw2DRectangle( clr_3dsh, tr, clip);
	}

	if (background)
	{
		tr = rect;
		tr.UpperLeftCorner.Y += getSize(gui::EGDS_BUTTON_HEIGHT) + 2;
		tr.LowerRightCorner.X -= 1;
		tr.UpperLeftCorner.X += 1;
		tr.LowerRightCorner.Y -= 1;

		if (!UseGradient)
			driver->draw2DRectangle( clr_3dfc, tr, clip);
		else
		{
			const video::SColor c1 = clr_3dfc;
			const video::SColor c2 = clr_3dsh;
			driver->draw2DRectangle(tr, c1, c1, c2, c2, clip);
		}
	}
}

//! draws an icon, usually from the skin's sprite bank
/**	\param parent: Pointer to the element which wishes to draw this icon. 
This parameter is usually not used by IGUISkin, but can be used for example 
by more complex implementations to find out how to draw the part exactly. 
\param icon: Specifies the icon to be drawn.
\param position: The position to draw the icon
\param starttime: The time at the start of the animation
\param currenttime: The present time, used to calculate the frame number
\param loop: Whether the animation should loop or not
\param clip: Clip area.	*/
void CImageGUISkin::drawIcon(IGUIElement* element, EGUI_DEFAULT_ICON icon,
						const core::position2di position,
						u32 starttime, u32 currenttime, 
						bool loop, const core::rect<s32>* clip)
{
   native_skin_->drawIcon( element, icon, position, starttime, currenttime, loop, clip );
}


EGUI_SKIN_TYPE CImageGUISkin::getType() const
{
	return native_skin_->getType();
}


//! draws a 2d rectangle.
void CImageGUISkin::draw2DRectangle(IGUIElement* element,
							   const video::SColor &color, const core::rect<s32>& pos,
							   const core::rect<s32>* clip)
{
	native_gui_->getVideoDriver()->draw2DRectangle(color, pos, clip);
}


//! Writes attributes of the object.
//! Implement this to expose the attributes of your scene node animator for 
//! scripting languages, editors, debuggers or xml serialization purposes.
void CImageGUISkin::serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options) const
{
	native_skin_->serializeAttributes( out, options );
}


//! Reads attributes of the object.
//! Implement this to set the attributes of your scene node animator for 
//! scripting languages, editors, debuggers or xml deserialization purposes.
void CImageGUISkin::deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options)
{
	native_skin_->deserializeAttributes( in, options );
}
//////////////////////////////////////////////////////////////////////////

void CImageGUISkin::drawHorizontalProgressBar( IGUIElement* element, const core::rect<s32>& rectangle, const core::rect<s32>* clip,
											  f32 filledRatio, video::SColor fillColor )
{
	if ( !Config.ProgressBar.Texture || !Config.ProgressBarFilled.Texture )
	{
		return;
	}

	// Draw empty progress bar
	drawElementStyle( element, Config.ProgressBar, rectangle, clip );

	// Draw filled progress bar on top
	if ( filledRatio < 0.0f )
		filledRatio = 0.0f;
	else
		if ( filledRatio > 1.0f )
			filledRatio = 1.0f;

	if ( filledRatio > 0.0f )
	{
		s32 filledPixels = (s32)( filledRatio * rectangle.getSize().Width );
		s32 height = rectangle.getSize().Height;

		core::rect<s32> clipRect = clip? *clip:rectangle;
		if ( filledPixels < height )
		{
			if ( clipRect.LowerRightCorner.X > rectangle.UpperLeftCorner.X + filledPixels )
				clipRect.LowerRightCorner.X = rectangle.UpperLeftCorner.X + filledPixels;

			filledPixels = height;
		}

		core::rect<s32> filledRect = core::rect<s32>( 
			rectangle.UpperLeftCorner.X, 
			rectangle.UpperLeftCorner.Y, 
			rectangle.UpperLeftCorner.X + filledPixels, 
			rectangle.LowerRightCorner.Y );

		drawElementStyle( element, Config.ProgressBarFilled, filledRect, &clipRect, &fillColor );
	}
}
//////////////////////////////////////////////////////////////////////////

void CImageGUISkin::drawElementStyle( IGUIElement* element, const SImageGUIElementStyle& elem, const core::rect<s32>& rect, const core::rect<s32>* clip, video::SColor* pcolor  )
{
	core::rect<s32> srcRect;
	core::rect<s32> dstRect;
	video::IVideoDriver* driver = native_gui_->getVideoDriver();
	core::dimension2du tsize = elem.Texture->getOriginalSize();
	video::ITexture* texture = elem.Texture;

	video::SColor color = elem.Color;
	if ( pcolor )
 		 color = *pcolor;
	else
		 color.setAlpha( element != NULL ? element->getAlphaBlend() : 0xff );

	video::SColor colors[4] = { color, color, color, color };

	core::dimension2di dstSize = rect.getSize();

	// Scale the border if there is insufficient room
	SImageGUIElementStyle::SBorder dst = elem.DstBorder;
	f32 scale = 1.0f;
	if ( dstSize.Width < dst.Left + dst.Right )
	{
		scale = dstSize.Width / (f32)( dst.Left + dst.Right );
	}
	if ( dstSize.Height < dst.Top + dst.Bottom )
	{
		f32 x = dstSize.Height / (f32)( dst.Top + dst.Bottom );
		if ( x < scale )
		{
			scale = x;
		}
	}

	if ( scale < 1.0f )
	{
		dst.Left = (s32)( dst.Left * scale );
		dst.Right = (s32)( dst.Right * scale );
		dst.Top = (s32)( dst.Top * scale );
		dst.Bottom = (s32)( dst.Bottom * scale );
	}

	const SImageGUIElementStyle::SBorder& src = elem.SrcBorder;

	// Draw the top left corner
	srcRect = core::rect<s32>( 0, 0, src.Left, src.Top );
	dstRect = core::rect<s32>( rect.UpperLeftCorner.X, rect.UpperLeftCorner.Y, rect.UpperLeftCorner.X+dst.Left, rect.UpperLeftCorner.Y+dst.Top );
	if ( !clip || clipRects( dstRect, srcRect, *clip ) )
		 driver->draw2DImage( texture, dstRect, srcRect, clip, colors, true );

	// Draw the top right corner
	srcRect = core::rect<s32>( tsize.Width-src.Right, 0, tsize.Width, src.Top );
	dstRect = core::rect<s32>( rect.LowerRightCorner.X-dst.Right, rect.UpperLeftCorner.Y, rect.LowerRightCorner.X, rect.UpperLeftCorner.Y+dst.Top );
	if ( !clip || clipRects( dstRect, srcRect, *clip ) )
		 driver->draw2DImage( texture, dstRect, srcRect, clip, colors, true );

	// Draw the top border
	srcRect = core::rect<s32>( src.Left, 0, tsize.Width-src.Right, src.Top );
	dstRect = core::rect<s32>( rect.UpperLeftCorner.X+dst.Left, rect.UpperLeftCorner.Y, rect.LowerRightCorner.X-dst.Right, rect.UpperLeftCorner.Y+dst.Top );
	if ( !clip || clipRects( dstRect, srcRect, *clip ) )
		driver->draw2DImage( texture, dstRect, srcRect, clip, colors, true );

	// Draw the left border
	srcRect = core::rect<s32>( 0, src.Top, src.Left, tsize.Height-src.Bottom );
	dstRect = core::rect<s32>( rect.UpperLeftCorner.X, rect.UpperLeftCorner.Y+dst.Top, rect.UpperLeftCorner.X+dst.Left, rect.LowerRightCorner.Y-dst.Bottom );
	if ( !clip || clipRects( dstRect, srcRect, *clip ) )
		driver->draw2DImage( texture, dstRect, srcRect, clip, colors, true );

	// Draw the right border
	srcRect = core::rect<s32>( tsize.Width-src.Right, src.Top, tsize.Width, tsize.Height-src.Bottom );
	dstRect = core::rect<s32>( rect.LowerRightCorner.X-dst.Right, rect.UpperLeftCorner.Y+dst.Top, rect.LowerRightCorner.X, rect.LowerRightCorner.Y-dst.Bottom );
	if ( !clip || clipRects( dstRect, srcRect, *clip ) )
		driver->draw2DImage( texture, dstRect, srcRect, clip, colors, true );

	// Draw the middle section
	srcRect = core::rect<s32>( src.Left, src.Top, tsize.Width-src.Right, tsize.Height-src.Bottom );
	dstRect = core::rect<s32>( rect.UpperLeftCorner.X+dst.Left, rect.UpperLeftCorner.Y+dst.Top, rect.LowerRightCorner.X-dst.Right, rect.LowerRightCorner.Y-dst.Bottom );
	if ( !clip || clipRects( dstRect, srcRect, *clip ) )
		driver->draw2DImage( texture, dstRect, srcRect, clip, colors, true );

	// Draw the bottom left corner
	srcRect = core::rect<s32>( 0, tsize.Height-src.Bottom, src.Left, tsize.Height );
	dstRect = core::rect<s32>( rect.UpperLeftCorner.X, rect.LowerRightCorner.Y-dst.Bottom, rect.UpperLeftCorner.X+dst.Left, rect.LowerRightCorner.Y );
	if ( !clip || clipRects( dstRect, srcRect, *clip ) )
		driver->draw2DImage( texture, dstRect, srcRect, clip, colors, true );

	// Draw the bottom right corner
	srcRect = core::rect<s32>( tsize.Width-src.Right, tsize.Height-src.Bottom, tsize.Width, tsize.Height );
	dstRect = core::rect<s32>( rect.LowerRightCorner.X-dst.Right, rect.LowerRightCorner.Y-dst.Bottom, rect.LowerRightCorner.X, rect.LowerRightCorner.Y );
	if ( !clip || clipRects( dstRect, srcRect, *clip ) )
		driver->draw2DImage( texture, dstRect, srcRect, clip, colors, true );

	// Draw the bottom border
	srcRect = core::rect<s32>( src.Left, tsize.Height-src.Bottom, tsize.Width-src.Right, tsize.Height );
	dstRect = core::rect<s32>( rect.UpperLeftCorner.X+dst.Left, rect.LowerRightCorner.Y-dst.Bottom, rect.LowerRightCorner.X-dst.Right, rect.LowerRightCorner.Y );
	if ( !clip || clipRects( dstRect, srcRect, *clip ) )
		driver->draw2DImage( texture, dstRect, srcRect, clip, colors, true );
}
//////////////////////////////////////////////////////////////////////////

void CImageGUISkin::LoadConfig( const SImageGUISkinConfig& config )
{
	Config = config;
}
//////////////////////////////////////////////////////////////////////////

}//namespace gui

}//namespace irr