/********************************************************************
	created:	2009/10/01
	filename: 	nrpGUIEnvironment.h
	file base:	CNrpGUIEnvironment
	author:		Dalerank
	
	purpose:	расширенный класс фабрики элементов
*********************************************************************/
#pragma once

#include <irrlicht.h>
#include "NrpText.h"

namespace irr
{

namespace gui
{

class IGUIAnimator;
class CNrpGuiLinkBox;
class CNrp2DPictureFlow;
class CNrpTechMap;
class CNrpGuiLink;

class CNrpGUIEnvironment : public irr::gui::IGUIEnvironment
{
public:
	CNrpGUIEnvironment( gui::IGUIEnvironment* native_gui, ICursorControl* cursor );
	~CNrpGUIEnvironment();

	ICursorControl* getCursorControl() const;

	IGUIComboBox* addComboBox(const core::recti& rectangle,
									gui::IGUIElement* parent=0, 
									s32 id=-1);

	gui::IGUICheckBox* addCheckBox(bool checked, const core::recti& rectangle,
											gui::IGUIElement* parent = 0, 
											s32 id = -1, 
											const wchar_t* text = 0);

	gui::IGUIButton* addButton( const core::recti& rectangle,
									 gui::IGUIElement* parent = 0, 
									 s32 id=-1, 
									 const wchar_t* text = 0, 
									 const wchar_t* tooltiptext = 0);

	gui::CNrpGuiLink* addLink( const core::recti& rectangle,
							  gui::IGUIElement* parent = 0, 
							  s32 id=-1, 
							  const wchar_t* text = 0, 
							  const wchar_t* tooltiptext = 0);

	gui::IGUIButton* addButton( const core::recti& rectangle, 
									 gui::IGUIElement* parent/*=0*/, 
									 const char* normal_texture, 
									 const char* pressed_texture, 
									 const char* hovered_image, 
									 s32 id/*=-1*/ );

	gui::CNrpGuiLinkBox* addLinkBox( IGUIElement* parent, s32 id, core::recti rectangle );

	virtual bool removeFocus(IGUIElement* element);
	virtual bool hasFocus(IGUIElement* element) const;
	virtual io::IFileSystem* getFileSystem() const;
	virtual IOSOperator* getOSOperator() const;
	
	gui::IGUIScrollBar* addScrollBar(bool horizontal, 
										  const core::recti& rectangle,
										  gui::IGUIElement* parent=0, 
										  s32 id=-1);

	gui::IGUIElement* addProgressBar( IGUIElement* parent, s32 id, core::recti rectangle );

	gui::IGUIImage* addImage(  const core::recti& rectangle, 
									gui::IGUIElement* parent/* =0 */, 
									s32 id/* =-1 */, 
									const wchar_t* text/* =0 */);

	gui::IGUIElement* AddDestructor( gui::IGUIElement* parent, int time );

	gui::IGUIWindow* addMessageBox(const wchar_t* text, s32 flags, core::array< int >& funcRefs );
	gui::IGUIWindow* addMessageBox(const wchar_t* caption, const wchar_t* text=0,
										bool modal = true, 
										s32 flags = gui::EMBF_OK, 
										gui::IGUIElement* parent=0, 
										s32 id=-1,
										video::ITexture* image=0);

	gui::IGUIElement* getRootGUIElement();								//выдает первый элемент
	bool setFocus( gui::IGUIElement* element);
	gui::IGUIElement* getFocus() const;

	gui::IGUIImage* addImage(	video::ITexture* image, 
								core::position2di pos,
								bool useAlphaChannel=true, 
								gui::IGUIElement* parent=0, 
								s32 id=-1, 
								const wchar_t* text=0);

	gui::IGUIListBox* addListBox(const core::recti& rectangle,
									   gui::IGUIElement* parent=0, 
									   s32 id=-1, 
										bool drawBackground=false);
	gui::IGUIListBox* addComponentListBox( const core::recti& rectangle,
										gui::IGUIElement* parent,
										s32 id );

	gui::CNrp2DPictureFlow* addPictureFlow( const core::recti& rectangle,
											const core::recti& pictureRect,
											s32 id, gui::IGUIElement* parent );

	gui::IGUIEditBox* addEditBox(	const wchar_t* text, 
									const core::recti& rectangle,
									bool border = true, 
									gui::IGUIElement* parent = 0, 
									s32 id = -1);

	gui::IGUIStaticText* addStaticText(const wchar_t* text, 
										const core::recti& rectangle,
										bool border=false, 
										bool wordWrap=true, 
										gui::IGUIElement* parent=0, 
										s32 id=-1,
										bool fillBackground = false);
	

	gui::IGUIColorSelectDialog* addColorSelectDialog(const wchar_t* text,
											bool modal = true,
											gui::IGUIElement* parent=0, 
											s32 id = -1);

	gui::IGUISkin* getSkin() const;
	virtual void setSkin(IGUISkin* skin);

	gui::IGUIContextMenu* addContextMenu(const core::recti& rectangle,
												gui::IGUIElement* parent=0,
												s32 id=-1 );

	gui::IGUIFileOpenDialog* addFileOpenDialog(const wchar_t* title = 0,
													bool modal=true, 
													gui::IGUIElement* parent=0, 
													s32 id=-1);

	gui::IGUIFont* getFont( const io::path& filename );
	gui::IGUIFont* getBuiltInFont() const;

	gui::IGUIAnimator* addBlendAnimator( IGUIElement* parent, u32 min, u32 max, f32 step,
										 bool visOnStop, bool remSelf, bool remParent );

	gui::IGUIAnimator* addHoveredAnimator( IGUIElement* parent, u32 min, u32 max, u32 step,
										   bool visOnStop, bool remSelf, bool remParent );

	gui::IGUIAnimator* addMoveAnimator( IGUIElement* parent, core::position2di stopPos, u32 step, 
										bool visibleOnStop, bool removeOnStop, bool removeParentOnStop );
	gui::IGUIAnimator* addLuaAnimator( IGUIElement* parent, int funcRef );

	gui::IGUIAnimator* addTextTimeAnimator( IGUIElement* parent );

	gui::IGUIAnimator* addTextRunnerAnimator( IGUIElement* parent, const wchar_t* text );

	void drawAll();

	video::IVideoDriver* getVideoDriver() const;

	IrrlichtDevice* getDevice();

	virtual void clear();

	virtual bool postEventFromUser(const SEvent& event);
	virtual void setUserEventReceiver(IEventReceiver* evr);
	virtual IGUISkin* createSkin(EGUI_SKIN_TYPE type);

	virtual IGUIImageList* createImageList( video::ITexture* texture,
											core::dimension2di	imageSize, bool useAlphaChannel ) ;

	virtual IGUISpriteBank* getSpriteBank(const io::path& filename);
	virtual IGUISpriteBank* addEmptySpriteBank(const io::path& name);

	virtual IGUIWindow* addWindow(	const core::recti& rectangle, bool modal = false,
									const wchar_t* text=0, IGUIElement* parent=0, s32 id=-1);

	virtual IGUIWindow* addWindow( video::ITexture* texture,
						 		   const core::recti& rectangle, 
								   const wchar_t* text=0, 
								   IGUIElement* parent=0, 
								   s32 id=-1);

	virtual IGUIElement* addModalScreen(IGUIElement* parent);
	virtual IGUITreeView* addTreeView(	const core::recti& rectangle,
										IGUIElement* parent=0, s32 id=-1, bool drawBackground=false,
										bool scrollBarVertical = true, bool scrollBarHorizontal = false);

	virtual IGUIMeshViewer* addMeshViewer(	const core::recti& rectangle,
											IGUIElement* parent=0, s32 id=-1, const wchar_t* text=0);
	virtual IGUISpinBox* addSpinBox(	const wchar_t* text, const core::recti& rectangle,
										bool border=true,IGUIElement* parent=0, s32 id=-1);

	virtual IGUIInOutFader* addInOutFader(const core::recti* rectangle=0, IGUIElement* parent=0, s32 id=-1);

	virtual IGUITabControl* addTabControl(	const core::recti& rectangle,
											IGUIElement* parent=0, bool fillbackground=false,
											bool border=true, s32 id=-1);

	virtual IGUITab* addTab(	const core::recti& rectangle,
								IGUIElement* parent=0, s32 id=-1);

	virtual IGUIContextMenu* addMenu(IGUIElement* parent=0, s32 id=-1);

	virtual IGUIToolBar* addToolBar(IGUIElement* parent=0, s32 id=-1);

	virtual IGUITable* addTable(const core::recti& rectangle,
								IGUIElement* parent=0, 
								s32 id=-1, 
								bool drawBackground=false);

	CNrpTechMap* AddTechMap(const core::recti& rectangle,
			  			    IGUIElement* parent=0, 
						    s32 id=-1, 
						    bool drawBackground=false);

	gui::IGUIScrollBar* addCicrcleScrollBar( IGUIElement* parent, 
											 s32 id, 
											 const core::recti& rectangle );

	gui::IGUIAnimator* addRectAnimator( IGUIElement* parent, 
										const core::recti& minrect, 
										const core::recti& maxrect, 
										s32 step );

	gui::IGUIAnimator* addSpringAnimator( IGUIElement* parent, 
										  const core::recti& startrect, 
										  const core::recti& endrect, 
										  s32 time );

	gui::IGUIAnimator* addCursorPosAnimator( IGUIElement* parent, core::position2di offset );

	gui::IGUIFont* addFont(const io::path& name, IGUIFont* font);

	virtual IGUIElementFactory* getDefaultGUIElementFactory() const;
	virtual void registerGUIElementFactory(IGUIElementFactory* factoryToAdd);
	virtual u32 getRegisteredGUIElementFactoryCount() const;
	virtual IGUIElementFactory* getGUIElementFactory(u32 index) const;
	virtual IGUIElement* addGUIElement(const c8* elementName, IGUIElement* parent=0);
	virtual bool saveGUI(io::IWriteFile* file, IGUIElement* start=0);
	virtual bool saveGUI(const io::path& file, IGUIElement* start=0);
	virtual bool loadGUI(io::IReadFile* file, IGUIElement* parent=0);
	virtual bool loadGUI(const io::path& filename, IGUIElement* parent=0);
	virtual bool isHovered( IGUIElement* element ) const;

	virtual void setDragObject( IGUIElement* elm, video::ITexture* txs );
	virtual IGUIElement* getDragObject() const { return _dragObjectSave; }
 
	virtual void serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options=0) const;
	virtual void deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options=0);
	virtual void writeGUIElement(io::IXMLWriter* writer, IGUIElement* node);
	virtual void readGUIElement(io::IXMLReader* reader, IGUIElement* node);
	virtual void addToDeletionQueue( IGUIElement* ptrElement );
	virtual void RemoveAnimators( IGUIElement* elm );
	virtual void AddTopElement( IGUIElement* elm );
private:
	IGUIEnvironment* _nativeEnv;									//указатель на простую фабрику элементов
	ICursorControl* _cursor;
	core::map< stringw, gui::IGUIFont* > fonts_;												//основной шрифт

	core::list< IGUIElement* > _deletionQueue;

	IGUIElement* _dragObjectSave;
	video::ITexture* _dragTexture;
	core::array< IGUIElement* > _overlay;
	bool CreateSkin_();
	void LoadFonts_();
};

}	//namespace gui

}	//namespace irr