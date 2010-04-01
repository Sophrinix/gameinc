#pragma once
#include <S3DVertex.h>
#include <iscenenode.h>

namespace irr
{

namespace scene
{

class CNrpCustomSceneNode : public ISceneNode
{

public:
	CNrpCustomSceneNode( ISceneNode* parent, ISceneManager* mgr, s32 id=-1,
					  const core::vector3df& position = core::vector3df(0,0,0),
					  const core::vector3df& rotation = core::vector3df(0,0,0),
					  const core::vector3df& scale = core::vector3df(1.0f, 1.0f, 1.0f) );
	~CNrpCustomSceneNode();

	void OnRegisterSceneNode();
	void render();
	const core::aabbox3d<f32>& getBoundingBox() const;
	u32 getMaterialCount() const;
	video::SMaterial& getMaterial(u32 i);

private:
	core::aabbox3d<f32> Box;
	video::S3DVertex Vertices[4];
	video::SMaterial Material;
};

}//namespace scene

}//namespace irr