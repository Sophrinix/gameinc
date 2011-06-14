#include "StdAfx.h"
#include "LuaDriver.h"

#include <IVideoDriver.h>
#include <assert.h>
#include <memory>
#include "LuaTexture.h"


using namespace irr;

namespace nrp
{
CLASS_NAME CLASS_LUADRIVER( "CLuaDriver" );

BEGIN_LUNA_METHODS(CLuaDriver)
	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	LUNA_AUTONAME_FUNCTION( CLuaDriver, GetScreenSize )
	LUNA_AUTONAME_FUNCTION( CLuaDriver, GetTexture )
	LUNA_AUTONAME_FUNCTION( CLuaDriver, CreateBlur )
    LUNA_AUTONAME_FUNCTION( CLuaDriver, CreateGrayscale )
	LUNA_AUTONAME_FUNCTION( CLuaDriver, AddTexture )
	LUNA_AUTONAME_FUNCTION( CLuaDriver, AddRenderTargetTexture )
	LUNA_AUTONAME_FUNCTION( CLuaDriver, RemoveTexture )
	LUNA_AUTONAME_FUNCTION( CLuaDriver, SetRenderTarget )
END_LUNA_METHODS

BEGIN_LUNA_PROPERTIES(CLuaDriver)
	LUNA_ILUAOBJECT_PROPERTIES( CLuaDriver )
END_LUNA_PROPERTIES

CLuaDriver::CLuaDriver(lua_State *L, bool ex) : ILuaObject(L, CLASS_LUADRIVER, ex)
{}

int CLuaDriver::GetScreenSize( lua_State *L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 1, 1, "Function CLuaDriver:GetScreenSize not need any parameter");

	core::dimension2di size(0, 0);

	IF_OBJECT_NOT_NULL_THEN size = _object->getScreenSize();
	lua_pushinteger( L, size.Width );
	lua_pushinteger( L, size.Height );

	return 2;
}

void Blur (int* source, int* dest, core::dimension2du size, size_t radius) 
{
	size_t windowSize = radius * 2 + 1;
	size_t radiusPlusOne = radius + 1;

	int sumAlpha;
	int sumRed;
	int sumGreen;
	int sumBlue;

	int srcIndex = 0;
	int dstIndex;
	int pixel;

	int latLength = 256 * windowSize;
	std::auto_ptr< int > sumLookupTable( new int[latLength] );
	
	int* tmpR = sumLookupTable.get();
	for (int i = 0; i < latLength; i++, tmpR++ ) 
		*tmpR = i / windowSize;

	std::auto_ptr< int > indexLookupTable( new int[radiusPlusOne] );
	if (radius < size.Width ) 
	{
		for (size_t i = 0; i < radiusPlusOne; i++) 
		{
			indexLookupTable.get()[i] = i;
		}
	} 
	else 
	{
		for (size_t i = 0; i < size.Width; i++) 
		{
			indexLookupTable.get()[i] = i;
		}

		for (size_t i = size.Width; i < radiusPlusOne; i++) 
		{
			indexLookupTable.get()[i] = size.Width - 1;
		}
	}

	for (size_t y = 0; y < size.Height; y++) 
	{
		sumAlpha = sumRed = sumGreen = sumBlue = 0;
		dstIndex = y;

		pixel = source[srcIndex];
		sumAlpha += radiusPlusOne * ((pixel >> 24) & 0xFF);
		sumRed   += radiusPlusOne * ((pixel >> 16) & 0xFF);
		sumGreen += radiusPlusOne * ((pixel >>  8) & 0xFF);
		sumBlue  += radiusPlusOne * ( pixel        & 0xFF);

		for (size_t i = 1; i <= radius; i++) 
		{
			pixel = source[srcIndex + indexLookupTable.get()[i]];
			sumAlpha += (pixel >> 24) & 0xFF;
			sumRed   += (pixel >> 16) & 0xFF;
			sumGreen += (pixel >>  8) & 0xFF;
			sumBlue  +=  pixel        & 0xFF;
		}

		for  (size_t x = 0; x < size.Width; x++) 
		{
			dest[dstIndex] = sumLookupTable.get()[sumAlpha] << 24 |
				sumLookupTable.get()[sumRed]   << 16 |
				sumLookupTable.get()[sumGreen] <<  8 |
				sumLookupTable.get()[sumBlue];
			dstIndex += size.Height;

			size_t nextPixelIndex = x + radiusPlusOne;
			if (nextPixelIndex >= size.Width) 
			{
				nextPixelIndex = size.Width - 1;
			}

			int previousPixelIndex = x - radius;
			if (previousPixelIndex < 0) 
			{
				previousPixelIndex = 0;
			}

			int nextPixel = source[srcIndex + nextPixelIndex];
			int previousPixel = source[srcIndex + previousPixelIndex];

			sumAlpha += (nextPixel     >> 24) & 0xFF;
			sumAlpha -= (previousPixel >> 24) & 0xFF;

			sumRed += (nextPixel     >> 16) & 0xFF;
			sumRed -= (previousPixel >> 16) & 0xFF;

			sumGreen += (nextPixel     >> 8) & 0xFF;
			sumGreen -= (previousPixel >> 8) & 0xFF;

			sumBlue += nextPixel & 0xFF;
			sumBlue -= previousPixel & 0xFF;
		}

		srcIndex += size.Width;
	}
}

int CLuaDriver::CreateBlur( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 4, 4, "Function CLuaDriver:CreateBlur need name, iterations, radius parameter");

	NrpText fileName = lua_tostring( L, 2 );
	int iterations = lua_tointeger( L, 3 );
	int radius = lua_tointeger( L, 4 );

	IF_OBJECT_NOT_NULL_THEN
	{
		NrpText blurTxsName = NrpText( "blur_" ) + fileName;
		video::ITexture* blurTxs = _object->getTexture( blurTxsName );
		video::ITexture* txs = _object->getTexture( fileName );

		if( txs != NULL && blurTxs == NULL )
		{
			int* pixelsTxs = (int*)txs->lock();

			blurTxs = _object->addTexture( txs->getOriginalSize(), blurTxsName );
			int* pixelsBlur = (int*)blurTxs->lock();

			std::auto_ptr< int > src( new int[ txs->getOriginalSize().getArea() ] );
			std::auto_ptr< int > dst( new int[ txs->getOriginalSize().getArea() ] );

			memcpy( src.get(), pixelsTxs, txs->getOriginalSize().getArea() * sizeof( int ) );
			for (int i = 0; i < iterations; i++) 
			{
				// horizontal pass
				Blur(src.get(), dst.get(), txs->getOriginalSize(), radius);
				// vertical pass
				core::dimension2du s2( txs->getOriginalSize().Height, txs->getOriginalSize().Width );
				Blur(dst.get(), src.get(), s2, radius);
			}
			memcpy( pixelsBlur, src.get(), txs->getOriginalSize().getArea() * sizeof( int ) );

			txs->unlock();
			blurTxs->unlock();
		}

		//lua_pop( L, argc );
		lua_pushlightuserdata( L, blurTxs );
		Luna< CLuaTexture >::constructor( L );
		return 1;
	}
	
	lua_pushnil( L );
	return 1;
}

int CLuaDriver::GetTexture( lua_State *L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaDriver:GetTexture need string parameter");

	NrpText fileName = lua_tostring( L, 2 );

	video::ITexture* txs = NULL;
	IF_OBJECT_NOT_NULL_THEN
	{
		txs = _object->getTexture( fileName );

		if( !txs )
			Log( HW ) << "Can't create CLuaTexture for file " << fileName << term;
	
		lua_pushlightuserdata( L, txs );
		Luna< CLuaTexture >::constructor( L );
		return 1;		
	}
	//lua_pop( L, argc );
	
	lua_pushnil( L );
	return 1;
}

int CLuaDriver::AddTexture( lua_State *L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 4, 4, "Function CLuaDriver:AddTexture need 3 parameter");

	u32 width = (u32)lua_tointeger( L, 2 );
	u32 height = (u32)lua_tointeger( L, 3 );
	NrpText fileName = lua_tostring( L, 4 );

	video::ITexture* txs = NULL;
	IF_OBJECT_NOT_NULL_THEN txs = _object->addTexture( core::dimension2du( width, height ), fileName );
	lua_pushlightuserdata( L, txs );

	return 1;
}

int CLuaDriver::RemoveTexture( lua_State *L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 2, 2, "Function CLuaDriver:RemoveTexture need string parameter");

	NrpText fileName = lua_tostring( L, 2 );

	IF_OBJECT_NOT_NULL_THEN 
	{
		video::ITexture* txs = _object->getTexture( fileName );
		_object->removeTexture( txs );
	}

	return 1;
}

int CLuaDriver::AddRenderTargetTexture( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 4, 4, "Function CLuaDriver:AddRenderTargetTexture need 3 parameter");

	u32 width = (u32)lua_tointeger( L, 2 );
	u32 height = (u32)lua_tointeger( L, 3 );
	NrpText fileName = lua_tostring( L, 4 );

	video::ITexture* txs = NULL;
	IF_OBJECT_NOT_NULL_THEN txs = _object->addRenderTargetTexture( core::dimension2du( width, height ), fileName, video::ECF_A8R8G8B8 );

	//lua_pop( L, argc );
	lua_pushlightuserdata( L, txs );
	Luna< CLuaTexture >::constructor( L );

	return 1;
}

int CLuaDriver::SetRenderTarget( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck(L, argc == 8, 8, "Function CLuaDriver:SetRenderTarget need 3 parameter");

	video::ITexture* txs = (video::ITexture*)lua_touserdata( L, 2 );
	bool clearBackBuffer = lua_toboolean( L, 3 ) > 0;
	bool clearZBuffer = lua_toboolean( L, 4 ) > 0;
	video::SColor color( lua_tointeger( L, 5 ), lua_tointeger( L, 6 ), lua_tointeger( L, 7 ), lua_tointeger( L, 8) );

	IF_OBJECT_NOT_NULL_THEN _object->setRenderTarget( txs, clearBackBuffer, clearZBuffer, color );

	return 1;
}

const char* CLuaDriver::ClassName()
{
	return ( CLASS_LUADRIVER );
}

int CLuaDriver::CreateGrayscale( lua_State* L )
{
    int argc = lua_gettop(L);
    luaL_argcheck(L, argc == 2, 2, "Function CLuaDriver:CreateGrayscale need name parameter");

    NrpText fileName = lua_tostring( L, 2 );

    IF_OBJECT_NOT_NULL_THEN
    {
        NrpText grayTxsName = NrpText( "grayscale_" ) + fileName;
        video::ITexture* grayTxs = _object->getTexture( grayTxsName );
        video::ITexture* txs = _object->getTexture( fileName );

        if( txs != NULL && grayTxs == NULL )
        {
            int* pixelsTxs = (int*)txs->lock();

            grayTxs = _object->addTexture( txs->getOriginalSize(), grayTxsName );
            int* pixelsGray = (int*)grayTxs->lock();

            //set the number of bytes per pixel
            int pixelSize = 3;

            core::dimension2du size = txs->getOriginalSize();
            for (u32 y = 0; y < size.Height; y++)
            {
                //get the data from the original image
                int offset = y * size.Width;

                for (u32 x = 0; x < size.Width; x++)
                {
                    //create the grayscale version                    
                    int pixel = pixelsTxs[ x + offset ];
                    u8 grayScale = (u8)((( pixel >> 16) & 0xff ) * 0.3) + (u8)(((pixel >> 8) & 0xff ) * 0.59) + (u8)((pixel & 0xff) * 0.11);

                    //set the new image's pixel to the grayscale version
                    pixelsGray[ x + offset ] = video::SColor( (pixel>>24) & 0xff, grayScale, grayScale, grayScale ).color; //B
                }
            }

            txs->unlock();
            grayTxs->unlock();
        }

        //lua_pop( L, argc );
        lua_pushlightuserdata( L, grayTxs );
        Luna< CLuaTexture >::constructor( L );
        return 1;
    }

    lua_pushnil( L );
    return 1;
}

}