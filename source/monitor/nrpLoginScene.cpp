#include "stdafx.h"
#include "nrpLoginScene.h"
#include "NrpText.h"
#include "nrpEngine.h"
#include "nrpButton.h"
#include "nrpScript.h"
#include "HTMLEngine.h"
#include "NrpBrowserWindow.h"
#include "Nrp2dPictureFlow.h"

//////////////////////////////////////////////////////////////////////////

namespace irr
{
 
namespace scene
{

gui::CNrp2DPictureFlow* pf = NULL;

bool CNrpLoginScene::OnEvent( const irr::SEvent& event )
{
	switch(event.EventType)
	{
	case EET_GUI_EVENT:
		if (event.GUIEvent.EventType == gui::EGET_BUTTON_CLICKED)
		{
			gui::CNrpButton* btn = (gui::CNrpButton*)event.GUIEvent.Caller;

			if( btn->getOnClickAction().size() )
			{
				nrp::CNrpScript::Instance().CallFunction( btn->getOnClickAction(), btn );
				return true;
			}
		}
		break;

	case EET_KEY_INPUT_EVENT:
		if( event.KeyInput.Key == KEY_RETURN && event.KeyInput.PressedDown == false )
		{		
			/*gui::IGUIElement* focused_elm = ptr_app_->GetGuiEnvironment()->getFocus();
			if (focused_elm == edlogin_)
				ptr_app_->GetGuiEnvironment()->setFocus(edpassw_);
			else if (focused_elm == edpassw_)
				RegisterUser_();*/
			return true;
		}

		if( event.KeyInput.Key == KEY_ESCAPE && !event.KeyInput.PressedDown )
		{
			/*if( ptr_app_->GetAuth().GetCurrentUser().IsRigth( RN_SHUTDOWN ) )
				ptr_app_->Close();
				*/
			return true;
		}

		{
			gui::IGUIElement* elm = _nrpEngine.GetGuiEnvironment()->getFocus();
			if( elm && elm == pf )
				elm->OnEvent( event );
		}
		break;
	}

	return false;
}
//////////////////////////////////////////////////////////////////////////

void CNrpLoginScene::OnUpdate()
{
	video::IVideoDriver* driver = _nrpEngine.GetVideoDriver(); 
	scene::ISceneManager* smgr = _nrpEngine.GetSceneManager();
	
	driver->beginScene( true, true, 0 );

	_nrpEngine.GetSceneManager()->drawAll();
	_nrpEngine.GetGuiEnvironment()->drawAll();

	driver->endScene();
}
//////////////////////////////////////////////////////////////////////////

void CNrpLoginScene::OnEnter()
{
	video::IVideoDriver* driver =_nrpEngine.GetVideoDriver();
	scene::ISceneManager* smgr = _nrpEngine.GetSceneManager();
	gui::IGUIEnvironment* env = _nrpEngine.GetGuiEnvironment();

	core::dimension2du scr_size = driver->getScreenSize();
	core::position2di offset( scr_size.Width / 2 - 168, 3 * scr_size.Height / 4 );

	// просто добавь воды
	scene::IAnimatedMesh* mesh = smgr->addHillPlaneMesh("myHill",
		core::dimension2d<f32>(20,20),
		core::dimension2d<u32>(40,40), 0, 0,
		core::dimension2d<f32>(0,0),
		core::dimension2d<f32>(10,10) );
	
/*	waterNode_ = smgr->addWaterSurfaceSceneNode(mesh->getMesh(0), 2.0f, 400.0f, 5.0f);
	waterNode_->setPosition(core::vector3df(0.0f, 7.0f, 0.0f));
	waterNode_->setRotation(core::vector3df(0.0f, 45.0f, 0.0f));
	waterNode_->setMaterialTexture(0, driver->getTexture( "media/stones.jpg" ) );
	waterNode_->setMaterialTexture(1, driver->getTexture( "media/water.jpg" ) ); 
	waterNode_->setMaterialType(video::EMT_TRANSPARENT_REFLECTION_2_LAYER);*/

	// необходимо создать источник света,
	// иначе ничего не будет видно
/*	lightNode_ = smgr->addLightSceneNode(0, core::vector3df(0,0,0),
		video::SColorf(0.0f, 0.6f, 0.7f, 1.0f), 1200.0f);


	// источник света будет вращаться вокруг воды
	scene::ISceneNodeAnimator* anim = 0;
	anim = smgr->createFlyCircleAnimator(core::vector3df(0,150,0),250.0f);
	lightNode_->addAnimator(anim);
	anim->drop();

	// добавить камеру, которая будет смотреть сверху на булькающую воду
	camera_ = smgr->addCameraSceneNode();
	camera_->setPosition(core::vector3df(0,150,0));
	camera_->setRotation(core::vector3df(0,0,0));
	camera_->setTarget(core::vector3df(0,0,0));
	smgr->setActiveCamera( camera_ );*/

/*	pf = new gui::CNrp2DPictureFlow( env, env->getRootGUIElement(), core::recti( 0, 0, 400, 400 ), core::recti( 0, 0, 150, 150 ), -1 );
	for( int k=0; k < 15; k++ )
		pf->addItem( driver->getTexture( "media/devka.jpg" ), L"" );
*/
	env->addImage( driver->getTexture("media/textures/splash.png"), core::position2di( 0, 0 ) );

	// эффект fade-in
	fader_ = env->addInOutFader();
	fader_->fadeIn( 5000 );
}
//////////////////////////////////////////////////////////////////////////

void CNrpLoginScene::OnLeave()
{
	if( fader_ ) 
		fader_->remove();

	if( mainLogo_) 
		mainLogo_->remove();
	
	if (lightNode_)
		lightNode_->remove();

	if( camera_ ) 
		camera_->remove();

	if( waterNode_ ) 
		waterNode_->remove();

	_nrpEngine.GetSceneManager()->setActiveCamera( NULL );
	_nrpEngine.GetSceneManager()->clear();
	_nrpEngine.GetGuiEnvironment()->clear();
}
//////////////////////////////////////////////////////////////////////////

CNrpLoginScene::CNrpLoginScene(): SCENE_ID(10000)
{
	fader_ = NULL;
	mainLogo_ = NULL;
	camera_ = NULL;
	waterNode_ = NULL;
	lightNode_ = NULL;
	someMesh_ = NULL;
}
//////////////////////////////////////////////////////////////////////////

} //namespace scene

} //namespace irr