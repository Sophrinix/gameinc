#include "StdAfx.h"
#include "NrpCustomSceneNode.h"
#include <ISceneManager.h>
#include <IVideoDriver.h>
#include <ICameraSceneNode.h>

namespace irr
{

namespace scene
{

CNrpCustomSceneNode::CNrpCustomSceneNode( ISceneNode* parent, ISceneManager* smgr, 
									s32 id, const core::vector3df& position, 
									const core::vector3df& rotation, 
									const core::vector3df& scale ) : ISceneNode( parent, smgr, id, position, rotation, scale )
{
	Material.Wireframe = false;
	Material.Lighting = false;

	Vertices[0] = video::S3DVertex(0,0,10, 1,1,0,video::SColor(255,0,255,255), 0, 1);
	Vertices[1] = video::S3DVertex(10,0,-10, 1,0,0,video::SColor(255,255,0,255), 1, 1);
	Vertices[2] = video::S3DVertex(0,20,0, 0,1,1,video::SColor(255,255,255,0), 1, 0);
	Vertices[3] = video::S3DVertex(-10,0,-10, 0,0,1,video::SColor(255,0,255,0), 0, 0);

	Box.reset(Vertices[0].Pos);
	for (s32 i=1; i<4; ++i)
		Box.addInternalPoint(Vertices[i].Pos);
}

CNrpCustomSceneNode::~CNrpCustomSceneNode()
{
}

void CNrpCustomSceneNode::OnRegisterSceneNode()
{
	if (IsVisible)
		SceneManager->registerNodeForRendering(this);

	ISceneNode::OnRegisterSceneNode();
}

void CNrpCustomSceneNode::render()
{
	ICameraSceneNode* camera = SceneManager->getActiveCamera();

	u16 indices[] = { 0,2,3, 2,1,3, 1,0,3, 2,0,1 };
	video::IVideoDriver* driver = SceneManager->getVideoDriver();

	video::S3DVertex vertices[4];
	memcpy( vertices, Vertices, sizeof(video::S3DVertex ) * 4 );
	vertices[ 0 ].Pos *= RelativeScale;
	vertices[ 1 ].Pos *= RelativeScale;
	vertices[ 2 ].Pos *= RelativeScale;
	vertices[ 3 ].Pos *= RelativeScale;

	core::vector3df posf = ((scene::ICameraSceneNode*) Parent )->getTarget();
	core::vector3df posff = ((scene::ICameraSceneNode*) Parent )->getPosition();
	posff = posf + RelativeTranslation;

	vertices[ 0 ].Pos += posff;
	vertices[ 1 ].Pos += posff;
	vertices[ 2 ].Pos += posff;
	vertices[ 3 ].Pos += posff;

	driver->setMaterial(Material);
	driver->setTransform(video::ETS_WORLD, core::IdentityMatrix );
	driver->drawIndexedTriangleList( vertices, 4, indices, 4);
}

const core::aabbox3d<f32>& CNrpCustomSceneNode::getBoundingBox() const
{
	return Box;
}

u32 CNrpCustomSceneNode::getMaterialCount() const
{
	return 1;
}

video::SMaterial& CNrpCustomSceneNode::getMaterial(u32 i)
{
	return Material;
}	

}//namespace scene

}//namespace irr