/********************************************************************
	created:	2009/11/19
	created:	19:11:2009   16:22
	filename: 	ImageGUISkin.h
	file path:	nerpa_sc
	file base:	ImageGUISkin
	file ext:	h
	author:		Dalerank
	
	purpose:	Это замена стандартному отрисовщику иррлихта
				реализует все функции стандартного отрисовщика
*********************************************************************/
#pragma once

#include <irrlicht.h>
#include <IGUISkin.h>
#include "nrpButton.h"

namespace irr
{

namespace gui
{
	
struct SImageGUIElementStyle						//описание как надо рисовать элемент
{													//для всех элементов отрисовка происходит 
													//как наложение текстур
	struct SBorder
	{
		s32 Top, Left, Bottom, Right;
		SBorder() : Top(0), Left(0), Bottom(0), Right(0) {}
	};
	
	SBorder SrcBorder;
	SBorder DstBorder;
	video::ITexture* Texture;						//текстура для отрисовки
	video::SColor Color;							//

	SImageGUIElementStyle() : Texture(0), Color(255,255,255,255) {}
};
	
struct SImageGUISkinConfig							//карта элементов которые может отрисовывать скин
{
	SImageGUIElementStyle SunkenPane,				//обычная панель	
						  Window,					//окно
						  WindowCaption,			//шапка окна
						  Button,					//кнопка в обычном состоянии
						  ButtonPressed,			//кнопка в нажатом состоянии
						  ButtonHovered,			//кнопка, когда над ней курсор находится, а также в фокусе
						  ButtonDisabled,			//нажатая кнопка	
						  ProgressBar,				//прогрессбар
						  ProgressBarFilled,		//заполнение прогрессбара	
						  CheckBox,					//флажок пустой
						  CheckBoxChecked,			//флажок установленный	
						  CheckBoxDisabled,			//отключенный
						  EditBox,					//поле ввода
						  EditBoxHovered,			//поле ввода когда над ним курсор находится
						  EditBoxDisabled,			//отключенный элемент
						  ComboBox,					//выпадаюзий список
						  ComboBoxHovered,			//он же, на ним курсор
						  ComboBoxDisabled,			//отключенный
						  ContextMenu,				//контекстное меню
						  ListBox;					//список
};

class CImageGUISkin : public IGUISkin
{
public:
	SImageGUISkinConfig Config;						//конфиг скина	

	CImageGUISkin( IGUIEnvironment* env );
	
	~CImageGUISkin(void);

	void LoadConfig( const SImageGUISkinConfig& config );

	//! returns default color
	virtual video::SColor getColor(gui::EGUI_DEFAULT_COLOR color) const;

	//! sets a default color
	virtual void setColor(gui::EGUI_DEFAULT_COLOR which, video::SColor newColor);

	//! returns size for the given size type
	virtual s32 getSize(gui::EGUI_DEFAULT_SIZE size) const;

	//! sets a default size
	virtual void setSize(gui::EGUI_DEFAULT_SIZE which, s32 size);

	//! returns the default font
	virtual gui::IGUIFont* getFont(gui::EGUI_DEFAULT_FONT which=gui::EGDF_DEFAULT) const;

	//! sets a default font
	virtual void setFont(irr::gui::IGUIFont* font, irr::gui::EGUI_DEFAULT_FONT which=irr::gui::EGDF_DEFAULT);

	//! sets the sprite bank used for drawing icons
	virtual void setSpriteBank(gui::IGUISpriteBank* bank);

	//! gets the sprite bank used for drawing icons
	virtual gui::IGUISpriteBank* getSpriteBank() const;

	//! Returns a default icon
	/** Returns the sprite index within the sprite bank */
	virtual u32 getIcon( gui::EGUI_DEFAULT_ICON icon) const;

	//! Sets a default icon
	/** Sets the sprite index used for drawing icons like arrows, 
	close buttons and ticks in checkboxes 
	\param icon: Enum specifying which icon to change
	\param index: The sprite index used to draw this icon */
	virtual void setIcon(gui::EGUI_DEFAULT_ICON icon, u32 index);

	//! Returns a default text.
	/** For example for Message box button captions:
	"OK", "Cancel", "Yes", "No" and so on. */
	virtual const wchar_t* getDefaultText(gui::EGUI_DEFAULT_TEXT text) const;

	//! Sets a default text. 
	/** For example for Message box button captions:
	"OK", "Cancel", "Yes", "No" and so on. */
	virtual void setDefaultText(gui::EGUI_DEFAULT_TEXT which, const wchar_t* newText);

	//! draws a standard 3d button pane
	/**	Used for drawing for example buttons in normal state. 
	It uses the colors EGDC_3D_DARK_SHADOW, EGDC_3D_HIGH_LIGHT, EGDC_3D_SHADOW and
	EGDC_3D_FACE for this. See EGUI_DEFAULT_COLOR for details. 
	\param rect: Defining area where to draw.
	\param clip: Clip area.
	\param element: Pointer to the element which wishes to draw this. This parameter
	is usually not used by ISkin, but can be used for example by more complex 
	implementations to find out how to draw the part exactly. */
	virtual void draw3DButtonPaneStandard(gui::IGUIElement* element, 
		const core::rect<s32>& rect,
		const core::rect<s32>* clip=0);

	//! draws a pressed 3d button pane
	/**	Used for drawing for example buttons in pressed state. 
	It uses the colors EGDC_3D_DARK_SHADOW, EGDC_3D_HIGH_LIGHT, EGDC_3D_SHADOW and
	EGDC_3D_FACE for this. See EGUI_DEFAULT_COLOR for details. 
	\param rect: Defining area where to draw.
	\param clip: Clip area.
	\param element: Pointer to the element which wishes to draw this. This parameter
	is usually not used by ISkin, but can be used for example by more complex 
	implementations to find out how to draw the part exactly. */
	virtual void draw3DButtonPanePressed(gui::IGUIElement* element, 
		const core::rect<s32>& rect,
		const core::rect<s32>* clip=0);

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
	virtual void draw3DSunkenPane(gui::IGUIElement* element,
		video::SColor bgcolor,
		bool flat,
		bool fillBackGround,
		const core::rect<s32>& rect,
		const core::rect<s32>* clip=0);

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
	virtual core::rect<s32> draw3DWindowBackground(
		gui::IGUIElement* element,
		bool drawTitleBar, 
		video::SColor titleBarColor,
		const core::rect<s32>& rect,
		const core::rect<s32>* clip=0,
		core::rect<s32>* checkClientArea=0 );

	//! draws a standard 3d menu pane
	/**	Used for drawing for menus and context menus. 
	It uses the colors EGDC_3D_DARK_SHADOW, EGDC_3D_HIGH_LIGHT, EGDC_3D_SHADOW and
	EGDC_3D_FACE for this. See EGUI_DEFAULT_COLOR for details. 
	\param element: Pointer to the element which wishes to draw this. This parameter
	is usually not used by ISkin, but can be used for example by more complex 
	implementations to find out how to draw the part exactly. 
	\param rect: Defining area where to draw.
	\param clip: Clip area.	*/
	virtual void draw3DMenuPane(gui::IGUIElement* element,
		const core::rect<s32>& rect,
		const core::rect<s32>* clip=0);

	//! draws a standard 3d tool bar
	/**	Used for drawing for toolbars and menus.
	\param element: Pointer to the element which wishes to draw this. This parameter
	is usually not used by ISkin, but can be used for example by more complex 
	implementations to find out how to draw the part exactly. 
	\param rect: Defining area where to draw.
	\param clip: Clip area.	*/
	virtual void draw3DToolBar(gui::IGUIElement* element,
		const core::rect<s32>& rect,
		const core::rect<s32>* clip=0);

	//! draws a tab button
	/**	Used for drawing for tab buttons on top of tabs.
	\param element: Pointer to the element which wishes to draw this. This parameter
	is usually not used by ISkin, but can be used for example by more complex 
	implementations to find out how to draw the part exactly. 
	\param active: Specifies if the tab is currently active.
	\param rect: Defining area where to draw.
	\param clip: Clip area.	*/
	virtual void draw3DTabButton(IGUIElement* element, 
								 bool active,
								 const core::rect<s32>& rect, 
								 const core::rect<s32>* clip=0, 
								 gui::EGUI_ALIGNMENT alignment=EGUIA_UPPERLEFT);

	//! draws a tab control body
	/**	\param element: Pointer to the element which wishes to draw this. This parameter
	is usually not used by ISkin, but can be used for example by more complex 
	implementations to find out how to draw the part exactly. 
	\param border: Specifies if the border should be drawn.
	\param background: Specifies if the background should be drawn.
	\param rect: Defining area where to draw.
	\param clip: Clip area.	*/
	virtual void draw3DTabBody(	IGUIElement* element, 
								bool border, 
								bool background,
								const core::rect<s32>& rect, 
								const core::rect<s32>* clip=0, 
								s32 tabHeight=-1, 
								gui::EGUI_ALIGNMENT alignment=EGUIA_UPPERLEFT ) ;

	//! draws an icon, usually from the skin's sprite bank
	/**	\param element: Pointer to the element which wishes to draw this icon. 
	This parameter is usually not used by IGUISkin, but can be used for example 
	by more complex implementations to find out how to draw the part exactly. 
	\param icon: Specifies the icon to be drawn.
	\param position: The position to draw the icon
	\param starttime: The time at the start of the animation
	\param currenttime: The present time, used to calculate the frame number
	\param loop: Whether the animation should loop or not
	\param clip: Clip area.	*/
	virtual void drawIcon(gui::IGUIElement* element, 
		gui::EGUI_DEFAULT_ICON icon,
		const core::position2di position,
		u32 starttime=0, irr::u32 currenttime=0, 
		bool loop=false, 
		const core::rect<s32>* clip=0);


	//! draws a 2d rectangle.
	/** \param element: Pointer to the element which wishes to draw this icon. 
	This parameter is usually not used by IGUISkin, but can be used for example 
	by more complex implementations to find out how to draw the part exactly. 
	\param color: Color of the rectangle to draw. The alpha component specifies how 
	transparent the rectangle will be.
	\param pos: Position of the rectangle.
	\param clip: Pointer to rectangle against which the rectangle will be clipped.
	If the pointer is null, no clipping will be performed. */
	virtual void draw2DRectangle(gui::IGUIElement* element, 
		const video::SColor &color, 
		const core::rect<irr::s32>& pos, 
		const core::rect<irr::s32>* clip = 0);

	virtual void drawHorizontalProgressBar( 
		gui::IGUIElement* element, 
		const core::rect<irr::s32>& rectangle, 
		const core::rect<irr::s32>* clip,
		f32 filledRatio, 
		video::SColor fillColor );

	//! get the type of this skin
	virtual gui::EGUI_SKIN_TYPE getType() const;

	//! Writes attributes of the object.
	//! Implement this to expose the attributes of your scene node animator for 
	//! scripting languages, editors, debuggers or xml serialization purposes.
	virtual void serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options=0) const;

	//! Reads attributes of the object.
	//! Implement this to set the attributes of your scene node animator for 
	//! scripting languages, editors, debuggers or xml deserialization purposes.
	virtual void deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options=0);

	void drawElementStyle(  gui::IGUIElement* element, 
							const SImageGUIElementStyle& elem, 
							const core::rect<irr::s32>& rect, 
							const core::rect<irr::s32>* clip, 
							video::SColor* color=0 );

	gui::IGUISkin* GetNativeSkin() { return native_skin_; }
private:
	    
	gui::IGUISkin* native_skin_;									//дефолтовый скин, который выполняет то, что не 
																	//смог обработать мой
	IGUIEnvironment* native_gui_;									//дефолтовый гуи, который реализует остальные функции
																	//		
	bool UseGradient;

	void CImageGUISkin::draw3DButtonPostEffect( IGUIElement* element, const core::rect<s32>& r, const core::rect<s32>* clip );
};

} //namespace gui

} //namespace irr