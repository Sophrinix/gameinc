#include "stdafx.h"
#include "SkinLoader.h"
#include "ConfigMap.h"
#include "ConfigReader.h"
#include <IReadFile.h>
#include <IVideoDriver.h>

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
//////////////////////////////////////////////////////////////////////////

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
    core::stringc s = cfg->getConfig(name+".SrcBorder");
    sscanf_s( s.c_str(), "%d , %d , %d , %d", &elem.SrcBorder.Top, &elem.SrcBorder.Left, &elem.SrcBorder.Bottom, &elem.SrcBorder.Right );

    s = cfg->getConfig(name+".DstBorder");
    sscanf_s( s.c_str(), "%d , %d , %d , %d", &elem.DstBorder.Top, &elem.DstBorder.Left, &elem.DstBorder.Bottom, &elem.DstBorder.Right );

    s = cfg->getConfig(name+".Texture");
    elem.Texture = driver->getTexture((context + s).c_str());

    s = cfg->getConfig(name+".Color");
    if ( s != "" )
    {
        s32 a,r,g,b;
        sscanf_s( s.c_str(), "%d , %d , %d , %d", &a, &r, &g, &b );
        elem.Color = video::SColor(a,r,g,b);
    }
}
//////////////////////////////////////////////////////////////////////////

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
		//ParseGUIElementStyle( driver, cfg, skin.WindowCaption, "WindowCaption", context );
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