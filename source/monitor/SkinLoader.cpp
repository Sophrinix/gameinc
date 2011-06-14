#include "stdafx.h"
#include "SkinLoader.h"
#include "ConfigMap.h"
#include "ConfigReader.h"
#include "Logger.h"
#include <IReadFile.h>
#include <IVideoDriver.h>
#include <assert.h>

#define SRCBORDER ".SrcBorder"
#define DSTBORDER ".DstBorder"
#define MARGIN ".Margin"
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

        map->setConfig( name, value );
    }

    return map;
}
//////////////////////////////////////////////////////////////////////////

void CNrpImageSkinLoader::ParseGUIElementStyle_( video::IVideoDriver* driver, 
												 CConfigMap* cfg, 
												 SImageGUISkinConfig& config, 
												 const core::stringc& name, 
												 const core::stringc& context )
{
    assert( config.configs.find( name ) == config.configs.end() );
    
    SImageGUIElementStyle elem;

    core::stringc s = cfg->getConfig( name + SRCBORDER );
    int ret = sscanf_s( s.c_str(), "%d , %d , %d , %d", &elem.srcBorder.Top, &elem.srcBorder.Left, &elem.srcBorder.Bottom, &elem.srcBorder.Right );
	elem.srcBorder.valid = (ret == 4);
	if( s.size() > 0 && !elem.srcBorder.valid )
		Log( HW ) << "Can't parse config " << name.c_str() << SRCBORDER << " = " << s.c_str() << term;

    s = cfg->getConfig(name+MARGIN);
    ret = sscanf_s( s.c_str(), "%d , %d , %d , %d", &elem.margin.Top, &elem.margin.Left, &elem.margin.Bottom, &elem.margin.Right );
    elem.margin.valid = (ret == 4);
    if( s.size() > 0 && !elem.margin.valid )
        Log( HW ) << "Can't parse config " << name.c_str() << MARGIN << " = " << s.c_str() << term;

    s = cfg->getConfig(name+DSTBORDER);
    ret = sscanf_s( s.c_str(), "%d , %d , %d , %d", &elem.dstBorder.Top, &elem.dstBorder.Left, &elem.dstBorder.Bottom, &elem.dstBorder.Right );
	elem.dstBorder.valid = (ret == 4);
	if( s.size() > 0 && !elem.dstBorder.valid )
		Log( HW ) << "Can't parse config " << name.c_str() << DSTBORDER << " = " << s.c_str() << term;

    s = cfg->getConfig(name+".Texture");
    elem.texture = driver->getTexture((context + s).c_str());

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

    config.configs[ name ] = elem;
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
    
        const core::array< core::stringc >& items = cfg->GetControls();

        for( int i=0; i < items.size(); i++ )
            ParseGUIElementStyle_( driver, cfg, skin_config, items[ i ], context );
        /*
        ParseGUIElementStyle_( driver, cfg, skin_config.configs[ SImageGUISkinConfig::Button ], "Button", context );
        ParseGUIElementStyle_( driver, cfg, skin_config.configs[ SImageGUISkinConfig::ButtonPressed ], "ButtonPressed", context );
        ParseGUIElementStyle_( driver, cfg, skin_config.configs[ SImageGUISkinConfig::ButtonHovered ], "ButtonHovered", context );
        ParseGUIElementStyle_( driver, cfg, skin_config.configs[ SImageGUISkinConfig::ButtonDisabled ], "ButtonDisabled", context );

		ParseGUIElementStyle_( driver, cfg, skin_config.configs[ SImageGUISkinConfig::SunkenPane ], "SunkenPane", context );
		ParseGUIElementStyle_( driver, cfg, skin_config.configs[ SImageGUISkinConfig::Window ], "Window", context );
		ParseGUIElementStyle_( driver, cfg, skin_config.configs[ SImageGUISkinConfig::WindowCaption ], "WindowCaption", context );
		ParseGUIElementStyle_( driver, cfg, skin_config.configs[ SImageGUISkinConfig::ProgressBar ], "ProgressBar", context );
		ParseGUIElementStyle_( driver, cfg, skin_config.configs[ SImageGUISkinConfig::ProgressBarFilled ], "ProgressBarFilled", context );
		ParseGUIElementStyle_( driver, cfg, skin_config.configs[ SImageGUISkinConfig::CheckBox ], "CheckBox", context );
		ParseGUIElementStyle_( driver, cfg, skin_config.configs[ SImageGUISkinConfig::CheckBoxChecked ], "CheckBoxChecked", context );
		ParseGUIElementStyle_( driver, cfg, skin_config.configs[ SImageGUISkinConfig::CheckBoxDisabled ], "CheckBoxDisabled", context );
		ParseGUIElementStyle_( driver, cfg, skin_config.configs[ SImageGUISkinConfig::CheckBoxDisabled ], "CheckBoxHovered", context );
		ParseGUIElementStyle_( driver, cfg, skin_config.configs[ SImageGUISkinConfig::EditBox ], "EditBox", context );
		ParseGUIElementStyle_( driver, cfg, skin_config.configs[ SImageGUISkinConfig::EditBoxHovered ], "EditBoxFocused", context );
		ParseGUIElementStyle_( driver, cfg, skin_config.configs[ SImageGUISkinConfig::EditBoxDisabled ], "EditBoxDisabled", context );
		ParseGUIElementStyle_( driver, cfg, skin_config.configs[ SImageGUISkinConfig::ComboBox ], "ComboBox", context );
		ParseGUIElementStyle_( driver, cfg, skin_config.configs[ SImageGUISkinConfig::ComboBoxHovered ], "ComboBoxFocused", context );
		ParseGUIElementStyle_( driver, cfg, skin_config.configs[ SImageGUISkinConfig::ComboBoxDisabled ], "ComboBoxDisabled", context );
		//ParseGUIElementStyle( driver, cfg, skin.ListBox, "ListBox", context );
		ParseGUIElementStyle_( driver, cfg, skin_config.configs[ SImageGUISkinConfig::ContextMenu ], "ContextMenu", context );
		ParseGUIElementStyle_( driver, cfg, skin_config.configs[ SImageGUISkinConfig::WindowCloseButton ], "WindowCloseButton", context );
		ParseGUIElementStyle_( driver, cfg, skin_config.configs[ SImageGUISkinConfig::WindowCloseHoveredButton ], "WindowCloseHoveredButton", context );
		ParseGUIElementStyle_( driver, cfg, skin_config.configs[ SImageGUISkinConfig::WindowClosePressedButton ], "WindowClosePressedButton", context );
*/
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