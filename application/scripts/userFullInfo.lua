local base = _G

module( "userFullInfo" )

local guienv = base.guienv

local winInfo = nil

local function _ShowUsersParameters( lbx, user )
	lbx:Clear()
	lbx:AddItem( "����=" .. user.knowledges, nil )
	lbx:AddItem( "��������=" .. user.quality, nil )
	lbx:AddItem( "��������=" .. user:GetParam( "codeSpeed" ), nil )
	lbx:AddItem( "������������=" .. user:GetParam( "stability" ), nil )
	lbx:AddItem( "��������=" .. user:GetParam( "character" ), nil )		
	lbx:AddItem( "����������������=" .. user:GetSkill( base.SKILL_CODING ), nil )	
end

function Show( user )
	if winInfo ~= nil then winInfo:Remove() end
	
	winInfo = guienv:AddWindow( "", "25%", "25%", "50%+", "50%+", -1, guienv.root )
	winInfo.closeButton.visible = false
	winInfo.text = user.name
	winInfo.draggable = false 

	winInfo:AddLuaFunction( base.GUIELEMENT_RMOUSE_LEFTUP, CloseUserInfoWindow )
	
	local listBox = guienv:AddListBox( 10, 20, "10e", "10e", -1, winInfo )
	
	_ShowUsersParameters( listBox, user )
end
