require("logdefs")
require("elmid")

local sceneManager = CLuaSceneManager( NrpGetSceneManager() )
local guienv = CLuaGuiEnvironment( NrpGetGuiEnvironment() )
 
function sworkCreateWindowPropertySonar( ptr )

	local nodeg = CLuaSceneNode( sceneManager:GetSelectedNode() )
	local windowg = CLuaWindow( guienv:GetElementByName( WINDOW_NODEPROPERTIES_NAME ) )

	--���� ������� ��� �� �������
	if windowg:Empty() == 1 then   
		Log({src=SCRIPT, dev=ODS}, "���� �������� ���, ������� ������" )
		--������� ����� ���� � ������������� ��� ���������
		windowg:SetObject( guienv:AddWindow( "", -0, 0, 0, 0, -1, guienv:GetRootGUIElement() ) )
		windowg:SetDraggable( false )
		windowg:SetRect( -100, 0, 0, 200 )
		windowg:SetVisible( true )
		windowg:SetName( WINDOW_NODEPROPERTIES_NAME )	
		
		--������� ������ �������� � ����������� � ��� ���-����������
		local closeButton = CLuaButton( guienv:AddButton( 15, 15, 85, 35, windowg:Self(), -1, "Close") )
		closeButton:SetAction( "sworkNodeWindowPropertiesClose" )

		--������ ��������� ��������� ������ ��������
		closeButton:SetObject( windowg:GetCloseButton() )
		closeButton:SetVisible( false )	
		
		--�������� ���� �����, ����� ���������� ��������� ����� ����
		local nameEdit = CLuaEdit( guienv:AddEdit( "", 15, 45, 85, 65, -1, windowg:Self() ) )
		nameEdit:SetName( WINDOW_NODEPROPERTIES_MARKEDIT )
		
		--�������� ���� �� ��������� ��� ��� ��������� �� ���� ��������� �����
		if nodeg:Empty() == 0 then
			nameEdit:SetText( sceneManager:GetMarkText( nodeg:Self() ) ) 
		end
		
		--��������� �������� �����������, ����� ���� ������� �� �����
		guienv:AddMoveAnimator( windowg:Self(), 0, 0, 1, true, true, false )
	else
		guienv:AddMoveAnimator( windowg:Self(), -100, 0, 1, false, true, true )
	end
		
end

function sworkNodeWindowPropertiesClose( ptr )

	local windowg = CLuaWindow( guienv:GetElementByName( WINDOW_NODEPROPERTIES_NAME ) )
	
	if windowg:Empty() == 1 then
		Log({src=SCRIPT, dev=ODS}, "��� ���� �������� ����� ��� ���������" )
		return 0
	end
	
	local nodeg = CLuaSceneNode( sceneManager:GetSelectedNode() )
	if nodeg:Empty() == 0 then
		local nameEdit = CLuaEdit( guienv:GetElementByName( WINDOW_NODEPROPERTIES_MARKEDIT ) )
		sceneManager:SetMarkText( nodeg:Self(), nameEdit:GetText() )
	end
	
	guienv:AddMoveAnimator( windowg:Self(), -100, 0, 1, false, true, true )
	
end