#include "stdafx.h"
#include "SkinLoader.h"
#include "ConfigMap.h"
#include "ConfigReader.h"
#include "Logger.h"
#include <IReadFile.h>
#include <IVideoDriver.h>

#define SRCBORDER ".SrcBorder"
#define DSTBORDER ".DstBorder"
#define LEFTUP "leftup"

using namespace nrp;

namespace irr
{

namespace gui
{

core::stringc CNrpImageSkinLoader::GetContextPath_( const core::stringc& filename )
{
    core::stringc path;
    s32 i=filename.size()-1;
    while ( i >= 0 && filename[i] != '/' )
    {
        i--;
    }
    path = filename.subString( 0, i+1 );
    return path;
}

CConfigMap* CNrpImageSkinLoader::LoadConfig_( io::IFileSystem* fileSystem, const core::stringc& filename )
{
    io::IReadFile* file = fileSystem->createAndOpenFile( filename.c_str() );

    if ( !file )
        return 0;

    CConfigMap* map = new CConfigMap();

    CConfigReader reader;
    reader.Load(file);

    file->drop();

    while ( reader.Next() )
    {
        core::stringc name = reader.GetName();
        core::stringc value = reader.GetValue();

        map->setConfig(name,value);
    }

    return map;
}
//////////////////////////////////////////////////////////////////////////

void CNrpImageSkinLoader::ParseGUIElementStyle_( video::IVideoDriver* driver, 
												 CConfigMap* cfg, 
												 SImageGUIElementStyle& elem, 
												 const core::stringc& name, 
												 const core::stringc& context )
{
    core::stringc s = cfg->getConfig( name + SRCBORDER );
    int ret = sscanf_s( s.c_str(), "%d , %d , %d , %d", &elem.SrcBorder.Top, &elem.SrcBorder.Left, &elem.SrcBorder.Bottom, &elem.SrcBorder.Right );
	elem.SrcBorder.valid = (ret == 4);
	if( s.size() > 0 && !elem.SrcBorder.valid )
		Log( HW ) << "Can't parse config " << name.c_str() << SRCBORDER << " = " << s.c_str() << term;

    s = cfg->getConfig(name+DSTBORDER);
    ret = sscanf_s( s.c_str(), "%d , %d , %d , %d", &elem.DstBorder.Top, &elem.DstBorder.Left, &elem.DstBorder.Bottom, &elem.DstBorder.Right );
	elem.DstBorder.valid = (ret == 4);
	if( s.size() > 0 && !elem.DstBorder.valid )
		Log( HW ) << "Can't parse config " << name.c_str() << DSTBORDER << " = " << s.c_str() << term;

    s = cfg->getConfig(name+".Texture");
    elem.Texture = driver->getTexture((context + s).c_str());

    s = cfg->getConfig(name+".Color");
    if ( s.size() > 0 )
    {
        s32 a,r,g,b;
        sscanf_s( s.c_str(), "%d , %d , %d , %d", &a, &r, &g, &b );
        elem.Color = video::SColor(a,r,g,b);
    }

	s = cfg->getConfig(name + ".MinSize" );
	s32 w = 0, h = 0;
	sscanf_s( s.c_str(), "%d , %d ", &w, &h );
	elem.minSize = core::dimension2du( w, h );

	s = cfg->getConfig(name + ".Align" );
	if ( s.size() > 0 )
	{
		std::map< std::string, int > rMap; 
		rMap[ LEFTUP ] = EGUIA_UPPERLEFT;
		rMap[ "rightdown" ] = EGUIA_LOWERRIGHT;
		rMap[ "center" ] = EGUIA_CENTER;
		rMap[ "scale" ] = EGUIA_SCALE;

		char left[ 32 ], right[ 32 ], top[ 32 ], bottom[ 32 ];
		ret = sscanf_s( s.c_str(), "%s %s %s %s", top, 31, left, 31, bottom, 31, right, 31 );
		
		elem.align.Top = rMap[ top ];
		elem.align.Left = rMap[ left ];
		elem.align.Bottom = rMap[ bottom ];
		elem.align.Right = rMap[ right ];
		elem.align.valid = ( ret == 4 );
	}	
}

void CNrpImageSkinLoader::Load( io::IFileSystem* fileSystem, 
								video::IVideoDriver* driver, 
								const core::stringc& filename,
								SImageGUISkinConfig& skin_config )
{
	try
	{
		CConfigMap* cfg = LoadConfig_(fileSystem, filename);
	
	    if ( !cfg )
		    return;

		core::stringc context = GetContextPath_(filename);
    
		ParseGUIElementStyle_( driver, cfg, skin_config.Button, "Button", context );
		ParseGUIElementStyle_( driver, cfg, skin_config.ButtonPressed, "ButtonPressed", context );
		ParseGUIElementStyle_( driver, cfg, skin_config.ButtonHovered, "ButtonHovered", context );
		ParseGUIElementStyle_( driver, cfg, skin_config.ButtonDisabled, "ButtonDisabled", context );

		ParseGUIElementStyle_( driver, cfg, skin_config.SunkenPane, "SunkenPane", context );
		ParseGUIElementStyle_( driver, cfg, skin_config.Window, "Window", context );
		ParseGUIElementStyle_( driver, cfg, skin_config.WindowCaption, "WindowCaption", context );
		ParseGUIElementStyle_( driver, cfg, skin_config.ProgressBar, "ProgressBar", context );
		ParseGUIElementStyle_( driver, cfg, skin_config.ProgressBarFilled, "ProgressBarFilled", context );
		ParseGUIElementStyle_( driver, cfg, skin_config.CheckBox, "CheckBox", context );
		ParseGUIElementStyle_( driver, cfg, skin_config.CheckBoxChecked, "CheckBoxChecked", context );
		ParseGUIElementStyle_( driver, cfg, skin_config.CheckBoxDisabled, "CheckBoxDisabled", context );
		ParseGUIElementStyle_( driver, cfg, skin_config.CheckBoxDisabled, "CheckBoxHovered", context );
		ParseGUIElementStyle_( driver, cfg, skin_config.EditBox, "EditBox", context );
		ParseGUIElementStyle_( driver, cfg, skin_config.EditBoxHovered, "EditBoxFocused", context );
		ParseGUIElementStyle_( driver, cfg, skin_config.EditBoxDisabled, "EditBoxDisabled", context );
		ParseGUIElementStyle_( driver, cfg, skin_config.ComboBox, "ComboBox", context );
		ParseGUIElementStyle_( driver, cfg, skin_config.ComboBoxHovered, "ComboBoxFocused", context );
		ParseGUIElementStyle_( driver, cfg, skin_config.ComboBoxDisabled, "ComboBoxDisabled", context );
		//ParseGUIElementStyle( driver, cfg, skin.ListBox, "ListBox", context );
		ParseGUIElementStyle_( driver, cfg, skin_config.ContextMenu, "ContextMenu", context );
		ParseGUIElementStyle_( driver, cfg, skin_config.WindowCloseButton, "WindowCloseButton", context );
		ParseGUIElementStyle_( driver, cfg, skin_config.WindowCloseHoveredButton, "WindowCloseHoveredButton", context );
		ParseGUIElementStyle_( driver, cfg, skin_config.WindowClosePressedButton, "WindowClosePressedButton", context );

		delete cfg;
	}
	catch(...)
	{
#ifdef _DEBUG
		OutputDebugString( "Some Error on load skin config" );
#endif
	}
}
//////////////////////////////////////////////////////////////////////////

} //namespace gui

} //namespace irr