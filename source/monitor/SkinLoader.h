#ifndef _SKIN_LOADER_H_
#define _SKIN_LOADER_H_

#include "ImageGUISkin.h"

#include <irrString.h>

class CConfigMap;

namespace irr
{

namespace gui
{

class CNrpImageSkinLoader
{
public:
	//! Loads a skin configuration from a file. Texture filenames will be treated
	//! relative to the filename's path.
	static void Load(	io::IFileSystem* fileSystem, 
			  		    video::IVideoDriver* driver,
						const core::stringc& filename,
						OUT SImageGUISkinConfig& skin_config );
private:
	static core::stringc GetContextPath_( const core::stringc& filename );

	//! Loads a .cfg file and stores all the entries in a config map that is
	//! returned. Returns NULL if the file could not be opened. You must delete the
	//! config map when you are done using it.
	static CConfigMap* LoadConfig_( io::IFileSystem* fileSystem, const core::stringc& filename );
	static void ParseGUIElementStyle_( video::IVideoDriver* driver, 
									   CConfigMap* cfg, 
									   SImageGUISkinConfig& elem, 
									   const core::stringc& name, 
									   const core::stringc& context );
};

}//namespace gui

}//namespace irr
#endif