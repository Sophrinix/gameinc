#include "StdAfx.h"
#include "NrpRotatableImage.h"
#include <IVideoDriver.h>
#include <IGUIEnvironment.h>
#include <ISceneNode.h>
#include <ITexture.h>

namespace irr
{

namespace gui
{

CNrpRotatableImage::CNrpRotatableImage(const core::recti& rectange, 
									   IGUIEnvironment* env, 
									   s32 id,
									   IGUIElement* parent ) 
					: IGUIElement( gui::EGUIET_ELEMENT, env, parent, id, rectange )
{
	SetupQuadMesh(bodyMesh_, 1.0f);
}

//! render the compass
void CNrpRotatableImage::draw()
{
	if (!IsVisible) return;

	video::IVideoDriver* driver = Environment->getVideoDriver();

	const core::rect<s32> oldViewPort = driver->getViewPort();				//��������� ��������� ������
	driver->setViewPort(AbsoluteRect);										//������������� ��������� ����������� ��� ���� ������� ��������

	const core::matrix4 oldProjMat = driver->getTransform(video::ETS_PROJECTION); //��������� ������� �������� ������
	driver->setTransform(video::ETS_PROJECTION, core::matrix4());			//������ ������ �������� 	

	const core::matrix4 oldViewMat = driver->getTransform(video::ETS_VIEW);	//��������� ������� �����������
	driver->setTransform(video::ETS_VIEW, core::matrix4());					//������ ������ ������� �����������
	driver->setTransform(video::ETS_WORLD, matrix_);						//����� ������� ����������� � ��������� �� ������ ����

	driver->setMaterial(bodyMesh_.Material);								//������������� �������� ���������
	driver->drawMeshBuffer(&bodyMesh_);										//������ �����	

	driver->setTransform(video::ETS_VIEW, oldViewMat);						//���������� ������� ����
	driver->setTransform(video::ETS_PROJECTION, oldProjMat);				//��������������� ��������
	driver->setViewPort(oldViewPort);										//�������������� ��������� ����������� ������

	IGUIElement::draw();
}

void CNrpRotatableImage::SetupQuadMesh(scene::SMeshBuffer& mesh, const f32 width)
{
	const f32 halfWidth = width / 1.f;
	mesh.Vertices.set_used(4);
	mesh.Indices.set_used(6);

	const video::SColor white( 0xFF, 0xFF, 0xFF, 0xFF);

	mesh.Vertices[0] = video::S3DVertex(-halfWidth, -halfWidth, 0.f, 0.f, 0.f, 1.f, white, 0.f, 1.f);
	mesh.Vertices[1] = video::S3DVertex(-halfWidth,  halfWidth, 0.f, 0.f, 0.f, 1.f, white, 0.f, 0.f);
	mesh.Vertices[2] = video::S3DVertex( halfWidth,  halfWidth, 0.f, 0.f, 0.f, 1.f, white, 1.f, 0.f);
	mesh.Vertices[3] = video::S3DVertex( halfWidth, -halfWidth, 0.f, 0.f, 0.f, 1.f, white, 1.f, 1.f);

	mesh.Indices[0] = 0;
	mesh.Indices[1] = 1;
	mesh.Indices[2] = 2;
	mesh.Indices[3] = 2;
	mesh.Indices[4] = 3;
	mesh.Indices[5] = 0;

	mesh.getMaterial().Lighting = false;
	mesh.getMaterial().MaterialType = video::EMT_TRANSPARENT_ALPHA_CHANNEL;
}

//! set body of compass texture
void CNrpRotatableImage::SetTexture( video::ITexture* texture )
{
	bodyMesh_.getMaterial().setTexture(0, texture);
}

}//namespace gui

}//namespace irr