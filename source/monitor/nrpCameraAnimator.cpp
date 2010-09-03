//Камера
#include "StdAfx.h"
#include "nrpCameraAnimator.h" 
//////////////////////////////////////////////////////////////////////////

namespace irr
{

namespace scene
{

typedef enum {LMOUSE_BTN=0, MMOUSE_BTN, RMOUSE_BTN, BMOUSE_COUNT } BMOUSE_NAME;
//////////////////////////////////////////////////////////////////////////

CNrpCameraAnimator::CNrpCameraAnimator(	 gui::ICursorControl* cursor, f32 angle, f32 rotateSpeed, 
					   f32 zoomSpeed, f32 translationSpeed ) 
					   : cursorControl_(cursor), currentAngle_(angle), zooming_(false), rotating_(false), moving_(false),
					   translating_(false), zoomSpeed_(zoomSpeed), rotateSpeed_(rotateSpeed), translateSpeed_(translationSpeed),
					   currentZoom_(70.0f), rotX_(0.0f), rotY_(0.0f), oldCamera_(0), mousePos_(0.5f, 0.5f) 
{ 
	needZoom_ = currentZoom_ = zoomSpeed_;
	needAngle_ = currentAngle_;
	stepAngle_ = stepZoom_ = 0;
	
	if (cursorControl_)
	{
		cursorControl_->grab();
		mousePos_ = cursorControl_->getRelativePosition();
	}

	allKeysUp();
} 
//////////////////////////////////////////////////////////////////////////

bool CNrpCameraAnimator::OnEvent( const SEvent &event)								//обработка кнопок клавы и мышки
{ 
	if (event.EventType != EET_MOUSE_INPUT_EVENT /*|| !InputReceiverEnabled*/)
		return false;

	switch(event.MouseInput.Event)											//события от мышки обрабатываются перед клавой
	{
	case EMIE_LMOUSE_PRESSED_DOWN:											//запоминание состояния кнопок мыши
		mouseKeys_[ LMOUSE_BTN ] = true;
	break;

	case EMIE_RMOUSE_PRESSED_DOWN:
		mouseKeys_[ RMOUSE_BTN ] = true;
	break;

	case EMIE_MMOUSE_PRESSED_DOWN:
		mouseKeys_[ MMOUSE_BTN ] = true;
	break;

	case EMIE_LMOUSE_LEFT_UP:
		mouseKeys_[ LMOUSE_BTN ] = false;
	break;

	case EMIE_RMOUSE_LEFT_UP:
		mouseKeys_[ RMOUSE_BTN ] = false;
	break;

	case EMIE_MMOUSE_LEFT_UP:
		mouseKeys_[ MMOUSE_BTN ] = false;
	break;

	case EMIE_MOUSE_MOVED:													//передвижение
		mousePos_ = cursorControl_->getRelativePosition();
	break;

	case EMIE_MOUSE_WHEEL:													//обработка событий колеса
		{
			f32 tmpzoom = currentZoom_ - event.MouseInput.Wheel * zoomSpeed_;
			setZoom( tmpzoom );
		}
	break;

	case EMIE_COUNT:
	break;

	}
	return true;
} 
//////////////////////////////////////////////////////////////////////////

void CNrpCameraAnimator::allKeysUp() 
{ 
	/*for(int i = 0;i < KEY_KEY_CODES_COUNT;i++) 
		Keys[i] = false; */
	for (s32 i=0; i < MOUSE_BTN_NUMBER; ++i) 
		mouseKeys_[i] = false; 
} 
//////////////////////////////////////////////////////////////////////////

void CNrpCameraAnimator::animateNode(ISceneNode *node, u32 timeMs)		//обработка положения камеры
{ 
	//Alt + LM = Rotate around camera pivot
	//Alt + LM + MM = Dolly forth/back in view direction (speed % distance camera pivot - max distance to pivot)
	//Alt + MM = Move on camera plane (Screen center is about the mouse pointer, depending on move speed)

	if (!node || node->getType() != ESNT_CAMERA)
		return;

	ICameraSceneNode* camera = static_cast<ICameraSceneNode*>(node);

	// If the camera isn't the active camera, and receiving input, then don't process it.
	if(!camera->isInputReceiverEnabled())
		return;

	scene::ISceneManager * smgr = camera->getSceneManager();
	if(smgr && smgr->getActiveCamera() != camera)
		return;

	if (oldCamera_ != camera)
	{
		oldTarget_ = camera->getTarget();
		oldCamera_ = camera;
	}

	core::vector3df target = camera->getTarget();

	const SViewFrustum* va = camera->getViewFrustum();

	f32 nRotX = rotX_ > 360.f ? rotX_ - 360.f : rotX_;

	if( abs( currentAngle_ - needAngle_ ) > abs( stepAngle_ ) )
		currentAngle_ += stepAngle_;
	f32 nRotY = 45.f;

	if( abs(currentZoom_ - needZoom_ ) > abs(stepZoom_) )
		currentZoom_ += stepZoom_;

	f32 nZoom = currentZoom_;

	// Translation ---------------------------------

	core::vector3df translate(oldTarget_);

	core::vector3df tvectX = pos_ - target;
	core::vector3df UpVector(camera->getUpVector());

	tvectX = tvectX.crossProduct( UpVector );
	tvectX.normalize();

	core::vector3df tvectY = (va->getFarLeftDown() - va->getFarRightDown());
	tvectY = tvectY.crossProduct(UpVector.Y > 0 ? pos_ - target : target - pos_);
	tvectY.normalize();

	if (isMouseKeyDown( LMOUSE_BTN ) && !zooming_)							//левая клавиша мышки - это движение по осям икс и зет
	{
		if (!translating_)													//первое нажатие на лкм
		{
			translateStart_.X = mousePos_.X;
			translateStart_.Y = mousePos_.Y;
			translating_ = true;												//регистрируем движение
		}
		else
		{																	//пересчет координат для движения	
			f32 zoomKoeff = max( currentZoom_ / 1000.f, 1.f ) * translateSpeed_;

			translate += tvectX * (translateStart_.X - mousePos_.X) * zoomKoeff;
			translate.X += tvectX.Z * ( translateStart_.Y - mousePos_.Y ) * zoomKoeff;
			translate.Z -= tvectX.X * ( translateStart_.Y - mousePos_.Y ) * zoomKoeff;
		}
	}
	else
	{
		if (translating_)													//снимаем нажатие пкм
		{
			f32 zoomKoeff = max( currentZoom_ / 1000.f, 1.f) * translateSpeed_;
			
			translate += tvectX * (translateStart_.X - mousePos_.X) * zoomKoeff;
			translate.X += tvectX.Z * ( translateStart_.Y - mousePos_.Y ) * zoomKoeff;
			translate.Z -= tvectX.X * ( translateStart_.Y - mousePos_.Y ) * zoomKoeff;

			oldTarget_ = translate;						
		}

		translating_ = false;												//снимаем флаг перемещения
	}

	// Rotation ------------------------------------

	if (isMouseKeyDown( MMOUSE_BTN ) && !zooming_)							//вращение камеры
	{
		if (!rotating_)														//если первое нажатие лкм
		{
			rotateStart_.X = mousePos_.X;						
			rotateStart_.Y = mousePos_.Y;
			rotating_ = true;												//регистрируем вращение камеры
			nRotX = rotX_;
		}
		else
		{
			nRotX += (rotateStart_.X - mousePos_.X) * rotateSpeed_; 
		}
	}
	else
	{
		if (rotating_)														//завершение вращения камеры
		{	 
			rotX_ = rotX_ + (rotateStart_.X - mousePos_.X) * rotateSpeed_;
			nRotX = rotX_;
		}

		rotating_ = false;
	}	
	// Set Pos ------------------------------------

	target = translate;										
	
	pos_.X = nZoom + target.X;												//определяем координаты камеры
	pos_.Z = target.Z;
	pos_.Y = target.Y;
	
	pos_.rotateXYBy( nRotY, target );					 					//поворачиваем камеру на угол к горизонту
	target.rotateXZBy(-nRotX, pos_ );										//поворачиваем фокус относительно глаза
	
	// Rotation Error ----------------------------

	UpVector.set(0, 1, 0);
	UpVector.rotateXYBy(-nRotY, core::vector3df(0,0,0));
	UpVector.rotateXZBy(-nRotX+180.f, core::vector3df(0,0,0));

	core::vector3df resultPos = CorrectCameraPositionWithTerrains_( pos_ );

	camera->setPosition(resultPos);
	camera->setTarget(target);
	camera->setUpVector(UpVector);
	camera->setRotation( core::vector3df( nRotX, nRotY, 0 ) );
} 

core::vector3df CNrpCameraAnimator::CorrectCameraPositionWithTerrains_( const core::vector3df& position)
{
	core::vector3df result = position;
	core::list< scene::ITerrainSceneNode* >::Iterator pIter = terrains_.begin();

	for(; pIter != terrains_.end(); pIter++ )
	{
		core::aabbox3df	box = (*pIter)->getBoundingBox();
		if( box.isPointInside( result ) )
		{
			f32 y = (*pIter)->getHeight( result.X - box.MinEdge.X, result.Z - box.MinEdge.Z );	
			if( result.Y < y + 100 )
				result.Y = y + 100;
			break;
		}
	}

	return result;
}

ISceneNodeAnimator* CNrpCameraAnimator::createClone( ISceneNode* node, ISceneManager* newManager/*=0*/ )
{
	CNrpCameraAnimator * newAnimator =
		new CNrpCameraAnimator(cursorControl_, currentAngle_, rotateSpeed_, zoomSpeed_, translateSpeed_);
	return newAnimator;
}

//! Sets the rotation speed
void CNrpCameraAnimator::setRotateSpeed(f32 speed)
{
	rotateSpeed_ = speed;
}


//! Sets the movement speed
void CNrpCameraAnimator::setMoveSpeed(f32 speed)
{
	translateSpeed_ = speed;
}

//! Sets the zoom speed
void CNrpCameraAnimator::setZoomSpeed(f32 speed)
{
	zoomSpeed_ = speed;
}

//! Gets the rotation speed
f32 CNrpCameraAnimator::getRotateSpeed() const
{
	return rotateSpeed_;
}

// Gets the movement speed
f32 CNrpCameraAnimator::getMoveSpeed() const
{
	return translateSpeed_;
}

//! Gets the zoom speed
f32 CNrpCameraAnimator::getZoomSpeed() const
{
	return zoomSpeed_;
}

bool CNrpCameraAnimator::isMouseKeyDown( s32 key )
{
	return mouseKeys_[key];
}

void CNrpCameraAnimator::setZoom( f32 amount )
{
	f32 future_angle = -360.f + amount / 5000.f * 90.f;

	if( amount > 5000.f ) amount = 5000.f;
	if( amount < 100.f ) amount = 100.f;
	if( future_angle >  -270.f ) future_angle = -270.f; 
	if( future_angle < -320.f ) future_angle = -320.f;

	if( future_angle <= -270.f && future_angle >= -320.f )			 
	{
		needZoom_ = amount;			//изменяем скорость пермещения в зависимости от зума
		needAngle_ = future_angle;
		s32 koeff = oldCamera_->getSceneManager()->getVideoDriver()->getFPS();
		koeff = koeff > 20 ? 20 : koeff;
		stepZoom_ = ( needZoom_ - currentZoom_ ) / (f32)koeff;
		stepAngle_ = ( needAngle_ - currentAngle_ ) / (f32)koeff;
	}		
}
}//namespace scene

}//namespace irr