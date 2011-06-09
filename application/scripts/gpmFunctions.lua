local base = _G

DRG = true
NDRG = false
ENBL = true
DSBL = false

module( "gpmFunctions" )

local sizeLinkBox = 80
local lighting = nil

local function _Ligthing( sender, _ )
	local dlink = base.CLuaLinkBox( sender )	
	base.LogScript( "gpmFunction.Ligthing to #links="..#(base.gameprojectManager.links) )
	--��������� �� �������
	for i=1, #(base.gameprojectManager.links) do
		--�������� �� �� ������������ � �������� �����
		if base.gameprojectManager.links[ i ].type == dlink.type and not base.gameprojectManager.links[ i ].draggable then
			lighting = base.guienv:AddLigthing( base.gameprojectManager.links[ i ], nil, "sphere.png", 10 )		
	    end	
	end
end

function LinkBox( parentr, textr, group, datar, draggable, funcSet, funcUnset )
	local project = base.gameprojectManager.project
	local linkModule = base.guienv:AddLinkBox( textr, 0, 0, sizeLinkBox, sizeLinkBox, -1, parentr )
	linkModule.color = base.NrpARGB( 0xFF, 0, 0, 0 )
				
	linkModule.type = group
	linkModule.enabled = true
	if datar then 
		linkModule.data = datar.object
		linkModule.texture = datar.texture
		
		local enabled = (not draggable or datar.empty or not project:IsMyTech( datar ))	
		if enabled then linkModule.alpha = 0xff
				   else linkModule.alpha = 0x80 end
	
		linkModule.enabled = enabled
	end
	
	linkModule.draggable = draggable
	linkModule.defaultTexture = "media/buttons/linboxNoImage.png"
	if funcSet then linkModule.setFunction = funcSet end
	if funcUnset then linkModule.unsetFunction = funcUnset end
	
	if draggable then linkModule.dragStartFunction = _Ligthing end
	
	return linkModule
end

function CheckType( m, s, t )
	return ((m.type == s.type) and (m.type == t))
end

