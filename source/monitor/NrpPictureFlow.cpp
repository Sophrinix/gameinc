#include "stdafx.h"
#include "NrpPictureFlow.h"
#include "NrpRotatableImage.h"

namespace irr
{

namespace gui
{

CNrpPictureFlow::CNrpPictureFlow( scene::ISceneManager* smgr, 
								  IGUIEnvironment* env, 
								  IGUIElement* parent, 
								  core::recti rectangle, 
								  s32 id )
: IGUIListBox( env, parent, id, rectangle )
{
	activeIndex_ = 0;
	rttexture_ = Environment->getVideoDriver()->addRenderTargetTexture( core::dimension2du( 512, 512), "renderTargetTexture", video::ECF_A8R8G8B8 );

	fixedcamera_ = smgr->addCameraSceneNode( 0 );
	fixedcamera_->setPosition(core::vector3df(0,150,0));
	fixedcamera_->setRotation(core::vector3df(0,0,0));
	fixedcamera_->setTarget(core::vector3df(0,0,0));
	smgr_ = smgr;
}

u32 CNrpPictureFlow::addItem( video::ITexture* texture, const wchar_t* text )
{
	video::SMaterial mat;
	mat.AntiAliasing = true;
	mat.MaterialType = video::EMT_TRANSPARENT_VERTEX_ALPHA;
	mat.Lighting = false;
	mat.setTexture( 0, texture );//Environment->getVideoDriver()->getTexture( "media/devka.jpg" ) );
	scene::IMesh* meshk = smgr_->getGeometryCreator()->createPlaneMesh( core::dimension2df( 50, 50 ), core::dimension2du( 1, 1), 
		&mat, core::dimension2df( 1, 1 ) );

	scene::IMeshBuffer* buff = meshk->getMeshBuffer( mat );
	video::E_VERTEX_TYPE typed = buff->getVertexType();
	video::S3DVertex* v = (video::S3DVertex*)buff->getVertices();
	v[ 2 ].Color = 0xc0c0c0c0;
	v[ 3 ].Color = 0xc0c0c0c0;
	v[ 0 ].Color = 0xc0c0c0c0;
	v[ 1 ].Color = 0xc0c0c0c0;
	scene::IMeshSceneNode* node = smgr_->addMeshSceneNode(  meshk, smgr_->getRootSceneNode(), -1 );

	node->setVisible( false );
	CNrpImageDescription* descr = new CNrpImageDescription();
	images_.push_back( descr );
	descr->mesh = node;
	descr->position = core::vector3df( 0, 0, 0 );
	descr->needAngle = 0;
	descr->scale = core::vector3df( 1, 1, 1);

	UpdateImages_();

	return images_.size() - 1;
}

irr::u32 CNrpPictureFlow::addItem( const wchar_t* text )
{
	return addItem( NULL, text );
}

void CNrpPictureFlow::UpdateImages_()
{
	if( activeIndex_ < (int)images_.size() )
	{
		images_[ activeIndex_ ]->position = core::vector3df( 0, -50.f, 0 );
		images_[ activeIndex_ ]->scale = core::vector3df( 2, 1, 2);
		images_[ activeIndex_ ]->needAngle = 0;
	}

	for( int k=0; k < activeIndex_; k++ )
	{
		images_[ k ]->position = core::vector3df( 0, -50, 0 - 40.f - (activeIndex_ - k ) * 20 );
		images_[ k ]->scale = core::vector3df( 1, 1, 1);
		images_[ k ]->needAngle = 45.f;
	}

	for( int k=activeIndex_+1; k < (int)images_.size(); k++ )
	{
		images_[ k ]->position = core::vector3df( 0, -50, 0 + 40.f - (activeIndex_ - k ) * 20 );
		images_[ k ]->scale = core::vector3df( 1, 1, 1);
		images_[ k ]->needAngle = -45.f;
	}
}

void CNrpPictureFlow::draw()
{
	if( !IsVisible )
		return;

	Environment->getVideoDriver()->draw2DImage( rttexture_, AbsoluteRect, 
												core::recti( core::position2di( 0, 0 ), rttexture_->getSize() ), 
												&AbsoluteClippingRect, NULL, true );

	IGUIElement::draw();
}

void CNrpPictureFlow::PreRender()
{
	if( !IsVisible )
		return;

	scene::ICameraSceneNode* camera = smgr_->getActiveCamera();
	Environment->getVideoDriver()->setRenderTarget( rttexture_, true, true, video::SColor( 0x00000000 ) );

	smgr_->setActiveCamera( fixedcamera_ );

	const core::list< scene::ISceneNode* >& childs = smgr_->getRootSceneNode()->getChildren();

	core::array< scene::ISceneNode* > restore;
	for( core::list< scene::ISceneNode* >::ConstIterator k=childs.begin(); 
		k != childs.end();
		k++ )
	if( (*k)->isVisible() )
	{
		(*k)->setVisible( false );
		restore.push_back( *k );
	}

	for( size_t pos=0; pos < images_.size(); pos++ )
		images_[ pos ]->mesh->setVisible( true );

	smgr_->drawAll();

	for( size_t pos=0; pos < images_.size(); pos++ )
		images_[ pos ]->mesh->setVisible( false );

	for( size_t k=0; k < restore.size(); k++ )
		restore[ k ]->setVisible(true);

	Environment->getVideoDriver()->setRenderTarget( NULL, true, true, video::SColor( 0 ) );
	smgr_->setActiveCamera( camera );	

	UpdatePositions_();
}

void CNrpPictureFlow::UpdatePositions_()
{
	for( size_t k=0; k < images_.size(); k++ )
	{
		if( images_[ k ]->mesh->getAbsolutePosition() != images_[ k ]->position )
		{
			core::vector3df pos = images_[ k ]->mesh->getAbsolutePosition();
			core::vector3df offset = (-pos + images_[ k ]->position)/ (f32)Environment->getVideoDriver()->getFPS();
			images_[ k ]->mesh->setPosition( pos + offset );
		}

		if( images_[ k ]->mesh->getRotation().X != images_[ k ]->needAngle )
		{
			f32 angle = images_[ k ]->mesh->getRotation().X;
			f32 offset = (-angle +  images_[ k ]->needAngle ) / (f32)Environment->getVideoDriver()->getFPS();
			images_[ k ]->mesh->setRotation( core::vector3df( angle + offset, 0, 0 ) );
		}

		if( images_[ k ]->mesh->getScale() != images_[ k ]->scale )
		{
			core::vector3df scale = images_[ k ]->mesh->getScale();
			core::vector3df offset = (-scale + images_[ k ]->scale)/ (f32)Environment->getVideoDriver()->getFPS();
			images_[ k ]->mesh->setScale( scale + offset );
		}
	}
}

void CNrpPictureFlow::Prev()
{
	if( (int)activeIndex_ - 1 >= 0 )
		activeIndex_--;

	UpdateImages_();
}

void CNrpPictureFlow::Next()
{
	if( (int)activeIndex_ + 1 < (int)images_.size() )
		activeIndex_++;

	UpdateImages_();
}

bool CNrpPictureFlow::OnEvent( const SEvent& event )
{
	s32 curTime = GetTickCount();
	switch( event.EventType )
	{
	case EET_KEY_INPUT_EVENT:
		//uncheck for double event of keyInput
		if (event.KeyInput.PressedDown && event.KeyInput.Key == KEY_RIGHT && (curTime - lastTimeKey_ > 20))
		{
			Next();
			lastTimeKey_ = GetTickCount();
			return true;
		}

		if (event.KeyInput.PressedDown && event.KeyInput.Key == KEY_LEFT && (curTime - lastTimeKey_ > 20))
		{
			Prev();
			lastTimeKey_ = GetTickCount();
			return true;
		}
	break;

	case EET_MOUSE_INPUT_EVENT:
		if( event.MouseInput.Event == EMIE_RMOUSE_PRESSED_DOWN )
		{
			Prev();
			return true;
		}

		if( event.MouseInput.Event == EMIE_LMOUSE_PRESSED_DOWN )
		{
			Next();
			return true;
		}
	break;
	}

	return IGUIElement::OnEvent(event);
}

CNrpPictureFlow::~CNrpPictureFlow()
{
	for( size_t pos=0; pos < images_.size(); pos++ )
	{
		images_[ pos ]->mesh->remove();
		delete images_[ pos ];
	}
}

const wchar_t* CNrpPictureFlow::getListItem( u32 id ) const
{
	return core::stringw( images_[ id ]->mesh->getName() ).c_str();
}

void CNrpPictureFlow::setSelected( const wchar_t *item )
{

}

void CNrpPictureFlow::setSelected( s32 index )
{
	if( index < (int)images_.size() && index >= 0 )
		activeIndex_ = index;
}

void CNrpPictureFlow::removeItem( u32 index )
{
	if( index < images_.size() )
	{
		images_[ index ]->mesh->remove();
		delete images_[ index ];
		images_.erase( index );
	}
}

void CNrpPictureFlow::clear()
{
	for( u32 k=0; k < images_.size(); k++ )
	{
		images_[ k ]->mesh->remove();
		delete images_[ k ];
	}

	images_.clear();
	activeIndex_ = 0;
}
}//end namespace gui

}//end namespace irr