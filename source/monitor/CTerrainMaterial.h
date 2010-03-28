#pragma once

#include <irrlicht.h>

namespace irr
{

namespace video
{

class CNrpTerrainMaterial : public IShaderConstantSetCallBack
{
public:
	CNrpTerrainMaterial( IrrlichtDevice* ptr_device, float water_pos )
	{
		ptr_device_ = ptr_device;
		water_pos_ = water_pos;
	}
	
private:
	float water_pos_;

	void OnSetConstants( IMaterialRendererServices* services, s32 userData)
	{
		IVideoDriver* driver = services->getVideoDriver();

		services->setVertexShaderConstant("mWorld", &driver->getTransform(video::ETS_WORLD).pointer()[0], 16);

		if(driver->getDriverType() == video::EDT_DIRECT3D9)
		{
			core::matrix4 worldViewProj;
			worldViewProj = driver->getTransform(video::ETS_PROJECTION);
			worldViewProj *= driver->getTransform(video::ETS_VIEW);
			worldViewProj *= driver->getTransform(video::ETS_WORLD);
			services->setVertexShaderConstant("mWorldViewProj", &worldViewProj.pointer()[0], 16);
		}
		else
		{
			int var0=0;
			services->setPixelShaderConstant("ColoredTextureSampler", (float*)(&var0), 1);
			var0=1;
			services->setPixelShaderConstant("DetailMap", (float*)(&var0), 1);
			var0=2;
			services->setPixelShaderConstant("SeaFloorMap", (float*)(&var0), 1);
		}

		float var2 = water_pos_;
		services->setPixelShaderConstant("WaterPos", (float*)(&var2), 1); 

		float var3 = ptr_device_->getSceneManager()->getActiveCamera()->getPosition().Y < water_pos_ ? 1.f : 0.f;
		services->setPixelShaderConstant("RenderCoustic", (float*)(&var3), 1); 

		float time = (float)ptr_device_->getTimer()->getRealTime();
		services->setVertexShaderConstant("Time", &time, 1);
	};

	irr::IrrlichtDevice* ptr_device_;
	float draw_bottom_;
};

}//namespace video

}//namespace irr