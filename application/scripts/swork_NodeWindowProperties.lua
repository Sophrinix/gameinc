require("logdefs")
require("elmid")

local sceneManager = CLuaSceneManager( NrpGetSceneManager() )
local guienv = CLuaGuiEnvironment( NrpGetGuiEnvironment() )
 
function sworkCreateWindowPropertySonar( ptr )

	local nodeg = CLuaSceneNode( sceneManager:GetSelectedNode() )
	local windowg = CLuaWindow( guienv:GetElementByName( WINDOW_NODEPROPERTIES_NAME ) )

	--окно свойств еще не создано
	if windowg:Empty() == 1 then   
		Log({src=SCRIPT, dev=ODS}, "ќкна настроек нет, создаем заново" )
		--создаем новое окно и устанавливаем его параметры
		windowg:SetObject( guienv:AddWindow( "", -0, 0, 0, 0, -1, guienv:GetRootGUIElement() ) )
		windowg:SetDraggable( false )
		windowg:SetRect( -100, 0, 0, 200 )
		windowg:SetVisible( true )
		windowg:SetName( WINDOW_NODEPROPERTIES_NAME )	
		
		--создаем кнопку закрыти€ и прив€зываем к ней луа-обработчик
		local closeButton = CLuaButton( guienv:AddButton( 15, 15, 85, 35, windowg:Self(), -1, "Close") )
		closeButton:SetAction( "sworkNodeWindowPropertiesClose" )

		--делаем нивидимой дефолтную кнопку закрыти€
		closeButton:SetObject( windowg:GetCloseButton() )
		closeButton:SetVisible( false )	
		
		--создадим поле ввода, чтобы отобразить текстовую метку нода
		local nameEdit = CLuaEdit( guienv:AddEdit( "", 15, 45, 85, 65, -1, windowg:Self() ) )
		nameEdit:SetName( WINDOW_NODEPROPERTIES_MARKEDIT )
		
		--проверим есть ли св€занный нод дл€ получени€ от него текстовой метки
		if nodeg:Empty() == 0 then
			nameEdit:SetText( sceneManager:GetMarkText( nodeg:Self() ) ) 
		end
		
		--добавл€ем аниматор перемещени€, чтобы окно выехало на экран
		guienv:AddMoveAnimator( windowg:Self(), 0, 0, 1, true, true, false )
	else
		guienv:AddMoveAnimator( windowg:Self(), -100, 0, 1, false, true, true )
	end
		
end

function sworkNodeWindowPropertiesClose( ptr )

	local windowg = CLuaWindow( guienv:GetElementByName( WINDOW_NODEPROPERTIES_NAME ) )
	
	if windowg:Empty() == 1 then
		Log({src=SCRIPT, dev=ODS}, "Ќет окна настроек чтобы его закрывать" )
		return 0
	end
	
	local nodeg = CLuaSceneNode( sceneManager:GetSelectedNode() )
	if nodeg:Empty() == 0 then
		local nameEdit = CLuaEdit( guienv:GetElementByName( WINDOW_NODEPROPERTIES_MARKEDIT ) )
		sceneManager:SetMarkText( nodeg:Self(), nameEdit:GetText() )
	end
	
	guienv:AddMoveAnimator( windowg:Self(), -100, 0, 1, false, true, true )
	
end