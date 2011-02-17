local base = _G

DRG = true
NDRG = false
ENBL = true
DSBL = false

module( "gpmFunctions" )

local sizeLinkBox = 80

function LinkBox( parentr, textr, group, datar, draggable, funcSet, funcUnset )
	local project = base.gameprojectManager.project
	local linkModule = base.guienv:AddLinkBox( textr, 0, 0, sizeLinkBox, sizeLinkBox, -1, parentr )
			
	base.table.insert( base.gameprojectManager.links, linkModule )
	
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
	
	return linkModule
end

function Ligthing( dlink )
	Log( "localCreateLigthingBetweenElements elements"..#links )
	--проидемся по вкладке
	for i=1, #links do
		--проверим их на соответствие с заданным типом
		if links[ i ].moduleType == dlink.moduleType() and not links[ i ].draggable then
				guienv:AddLigthing( links[ i ], dlink, "media/textures/sphere.png", 10 )		
	    end	
	end
end

