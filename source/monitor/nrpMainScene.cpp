#include "StdAfx.h"
#include "math.h"

#include "nrpMainScene.h"
#include "nrpCameraAnimator.h"
#include "nrpLoadingScreen.h"
#include "nrpTerrainMaterial.h"
#include "nrpVideoConfig.h"
#include "NrpSkyConfig.h"
#include "NrpCameraConfig.h"
#include "nrpEngine.h"
#include "nrpGUIEnvironment.h"
#include "nrpButton.h"
#include "NrpScrollBar.h"
#include "nrpGlobalMap.h"
#include "nrpChartCtrl.h"
#include "nrpChartAxis.h"
#include "nrpChartSerie.h"
#include "NrpCustomSceneNode.h"
#include "nrpScript.h"

//////////////////////////////////////////////////////////////////////////

namespace irr
{

namespace scene
{

CNrpMainScene::CNrpMainScene() : INrpScene()
{
	fader_ = NULL;
	waterView_ = ONLY_WATER;
	selectedNode_ = NULL;
	lastTimeNodeSelect_ = 0;
	mouseSceneBLeftEvent_ = false;
}

scene::ISceneNode* waterNode_=NULL;


bool CNrpMainScene::InitWater_()												//����� ����
{
	scene::ISceneManager* smgr = _nrpEngine.GetSceneManager();

	f32 koeff = 2;						
	
	/*scene::IAnimatedMesh* mesh = smgr->addHillPlaneMesh("myHill",
		core::dimension2d<f32>(20,20),
		core::dimension2d<u32>(40,40), 0, 0,
		core::dimension2d<f32>(0,0),
		core::dimension2d<f32>(10,10));

	waterNode_ = smgr->addWaterSurfaceSceneNode(mesh->getMesh(0), 2.0f, 400.0f, 5.0f);
	waterNode_->setPosition(core::vector3df(0.0f, 300.0f, 0.0f));
	waterNode_->setRotation(core::vector3df(0.0f, 0.0f, 0.0f));
	waterNode_->setScale( core::vector3df( 10, 1, 10) );	
	waterNode_->setMaterialTexture(0, smgr->getVideoDriver()->getTexture( "media/water.jpg" ) ); 
	waterNode_->setMaterialTexture(1, smgr->getVideoDriver()->getTexture( "media/water_texture/waterbump.png" ) );
	waterNode_->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR );
	waterNode_->setMaterialFlag(video::EMF_LIGHTING, false );
*/
	return true;
}

//////////////////////////////////////////////////////////////////////////

void CNrpMainScene::RenderScene_()										//��������� �����			
{
	if( _options[ "render3d" ] )
		_nrpEngine.GetSceneManager()->drawAll();								    //��������� ����� �������

	if( _options[ "renderGui" ] )
		_nrpEngine.GetGuiEnvironment()->drawAll();								//������ ��������� ������������
}

CNrpMainScene::~CNrpMainScene(void)
{
}

//��������� ������� �����, ������� ����� �������������
void CNrpMainScene::GetNodeAndIntersectionFromCursor_( scene::ISceneNode*& node, core::vector3df& point, bool &doubleClick )
{
	ISceneManager* smgr = _nrpEngine.GetSceneManager();

	//���������� ������� �� ������
	core::position2di pos = _nrpEngine.GetDevice()->getCursorControl()->getPosition();
	//�������� ��� ��� ��������
	core::line3df line = smgr->getSceneCollisionManager()->getRayFromScreenCoordinates( pos, smgr->getActiveCamera() );		
	//����� ����� ��������� �����������
	core::triangle3df tri;
	//�������� ��������� ������
	node = smgr->getSceneCollisionManager()->getSceneNodeAndCollisionPointFromRay( line, point, tri );
	//������������� ���� �������� ������� ������
	doubleClick = ( selectedNode_ == node ) && ( GetTickCount() - lastTimeNodeSelect_ < 200 );
	lastTimeNodeSelect_ = GetTickCount();
}

bool savePoint=true;
bool CNrpMainScene::OnEvent( const irr::SEvent& event )						//��������� �������
{
	switch( event.EventType ) 
	{	//������� �� ���
		case EET_GUI_EVENT:
		{
			s32 id = event.GUIEvent.Caller->getID();		
            //���� ���� ������� �� ������������� ������� ������ 
			switch( event.GUIEvent.EventType )
			{
				//� ����� ��� ������� ��������
				case gui::EGET_SCROLL_BAR_CHANGED:
				{
					gui::CNrpScrollBar* scr = (gui::CNrpScrollBar*)event.GUIEvent.Caller;
					//� � ����� ���������� �������� ������� ���
					if( scr->getAction().size() )
					{	//���������� � ���������
						nrp::CNrpScript::Instance().CallFunction( scr->getAction(), scr );
						return true;
					}
				}					
				break;
			}
		}
		break;
		//���� ��������� ��������������� ������� �� �����
		case EET_KEY_INPUT_EVENT:
		{	//������� ��� �� ���������
			PCall( SCENE_KEY_INPUT_EVENT, this, (void*)&event );
		}
		break;
		//���������� ������������ ������� �����
		case EET_MOUSE_INPUT_EVENT:
		{
			switch( event.MouseInput.Event )
			{		 	
				//������� ��� ��������� ��� ���
				case EMIE_RMOUSE_LEFT_UP:	
					 PCall( SCENE_RMOUSE_LEFT_UP, this );
				break;
				//��� ��� ��������� ������� ���
				case EMIE_LMOUSE_LEFT_UP:
				{		
					//��������� ����� ������ ����� � �������� ���������� ����������
					gui::IGUIEnvironment* guienv = _nrpEngine.GetGuiEnvironment();
					if( guienv->isHovered( guienv->getRootGUIElement() ) ) //��������� �� ����������� ���� ������ �� ��������� ��� ���
					{
						core::vector3df pnt;
						GetNodeAndIntersectionFromCursor_( selectedNode_, pnt, twinLeftMouseClick_);					
					}
					
					//�������� ������� ���
					mouseSceneBLeftEvent_ = true;
				}
				break;

				case EMIE_MOUSE_MOVED:
				{	//������������ ������� ����������� �����
					PCall( SCENE_MOUSE_MOVED, this );
				}
				break;
				
			}
			return false;
				
		}
		break;

		case EET_USER_EVENT:
		{
			//DoLuaFunctionsByType( SCENE_USER_EVENT, (void*)&event );
		}
		break;
	}
	return false;
}


void CNrpMainScene::OnEnter()
{
	 _options[ "render3d" ] = true;
	 _options[ "renderGui" ] = true;
}

//���� ���� �����
void CNrpMainScene::OnUpdate()
{
	try
	{
		video::IVideoDriver* driver = _nrpEngine.GetVideoDriver();
		
		//�������� ������� ��� �� ������ �����
		PCall( SCENE_BEFORE_BEGIN, this );
		driver->beginScene( true, true, video::SColor(150,50,50,50) );
		
		try
		{
			//�������� ������� ��� �� ������� �����
			PCall( SCENE_BEFORE_RENDER, this );
			//�������� �����
			RenderScene_();
			//�������� ������� ��� ����� ������� �����
			PCall( SCENE_AFTER_RENDER, this );			

			//���������� ���� ��� ��������� ���������� ��������
			try
			{
				if( selectedNode_ != NULL && IsObjectChildOfScene_( selectedNode_ ) )
				{				
					video::SMaterial mat;
					mat.Thickness = 2;
					driver->setMaterial( mat );
					driver->setTransform(video::ETS_WORLD, core::IdentityMatrix );
					core::aabbox3df bbox = selectedNode_->getBoundingBox();
					selectedNode_->setDebugDataVisible( true );
					selectedNode_->getAbsoluteTransformation().transformBoxEx( bbox );

					driver->draw3DBox( bbox, video::SColor( 0xff, 0xff, 0, 0 ) );

					mat.Thickness = 1;
					driver->setMaterial( mat );
				}
				else
					selectedNode_ = NULL;
			}
			catch(...)
			{
				selectedNode_ = NULL;
			}
		}
		catch(...)
		{
			//ErrLog( gfx ) << all << "������ RenderScene" << term;
		}
			
		driver->endScene();
		//�������� ������� ��� ����� ���������� ������� �����
		PCall( SCENE_AFTER_END, this );
	
		if( mouseSceneBLeftEvent_ && ( GetTickCount() - lastTimeNodeSelect_ > 200) )
		{
			PCall( twinLeftMouseClick_ ? SCENE_LMOUSE_DOUBLE_CLICK : SCENE_LMOUSE_LEFT_UP, selectedNode_ );
			mouseSceneBLeftEvent_ = false;
			twinLeftMouseClick_ = false;
		}
	}
	catch(...)
	{
		//ErrLog(gfx) << all << "������ �������� �����" << term;
	}
}

bool CNrpMainScene::IsObjectChildOfScene_( scene::ISceneNode* node )
{
	const core::list< scene::ISceneNode* >& childs = _nrpEngine.GetSceneManager()->getRootSceneNode()->getChildren();

	core::list< scene::ISceneNode* >::ConstIterator pIter = childs.begin();
	for(; pIter != childs.end(); pIter++ )
	if( *pIter == node )
		return true;

	return false;
}

void CNrpMainScene::OnLeave()
{
}

void CNrpMainScene::SetOption( const nrp::NrpText& name, bool amount )
{
	_options[ name ] = amount;
}

core::vector3df CNrpMainScene::GetCurrentWorldPosition()
{
	scene::ISceneNode* ptrNode=NULL;
	core::vector3df point;
	bool t=false;

	GetNodeAndIntersectionFromCursor_( ptrNode, point, t );
	return point;
}

} //namespace scene

} //namesapce irr