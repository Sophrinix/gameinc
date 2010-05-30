#pragma once
#include <IGUIElement.h>
#include <SMeshBuffer.h>

namespace irr
{

namespace video
{
	class ITexture;
}

namespace gui
{

class CNrpRotatableImage : public IGUIElement
{ 
public:
	CNrpRotatableImage(const core::rect<s32>& rectange, IGUIEnvironment* env, s32 id, IGUIElement* parent );

	//! render the compass
	virtual void draw();

	//! set body of compass texture
	virtual void SetTexture( video::ITexture* texture);

	virtual core::vector3df getRotate()
	{
		return matrix_.getRotationDegrees();
	}

	//! set compass heading
	virtual void SetRotate(const core::vector3df angles )
	{
		matrix_.makeIdentity();
		matrix_.setRotationDegrees( angles );		
	}

protected:

	scene::SMeshBuffer bodyMesh_;

	core::matrix4 matrix_;

	//! sets up a quad mesh of given dimension
	virtual void SetupQuadMesh(scene::SMeshBuffer& mesh, const f32 width);

};

}//namespace gui

}//namespace irr
