local base = _G

module( "gpmTechPage" )

local button = base.button
local applic = base.applic
local guienv = base.guienv
local Log = base.LogScript
local projectWindow = nil
local project = nil
local company = nil 
local gpm = nil
local CLuaLinkBox = base.CLuaLinkBox
local CheckType = base.gpmFunctions.CheckType

local function _Set( mp, sender )
	sender = CLuaLinkBox( sender )
	mp = CLuaLinkBox( mp )
	local redraw = false
	if CheckType( mp, sender, base.PT_ADVTECH ) then 
		project:AddTech( sender.data )
		redraw = true
	elseif CheckType( mp, sender, base.PT_SCRIPTS ) then
		project.scriptEngine = sender.data 
		redraw = true
	elseif CheckType( mp, sender, base.PT_MINIGAME ) then 
		project.miniGameEngine = sender.data 
		redraw = true
	elseif CheckType( mp, sender, base.PT_PHYSIC ) then 
		project.physicEngine = sender.data 
		redraw = true
	end
	
	if redraw then
		base.gpmTechPage.Show()
		guienv:SetDragObject( nil, "" )
	end
end

local function _Unset( mp, _ )
	mp = CLuaLinkBox( mp )
	local redraw = false
	
	if base.PT_ADVTECH == mp.type then 	
		project:RemoveTech( mp.data )
		redraw = true
	elseif base.PT_SCRIPTS == mp.type then 
		project.scriptEngine = nil
		redraw = true
	elseif base.PT_MINIGAME == mp.type then 
		project.miniGameEngine = nil
		redraw = true
	elseif base.PT_PHYSIC == mp.type then 
		project.physicEngine = nil 
		redraw = true
	end
	
	if redraw then 
		guienv:SetDragObject( nil, "" )
		base.gpmTechPage.Show()
	end
end

local function _ShowAvaibleScriptAndMiniGames()
	local companyName = company.name
	local maxPublicTech = applic.techNumber
	
	local flick = guienv:AddFlick( "55%", "5%", "95%", "95%", 3, -1, projectWindow )
	
	for i=1, maxPublicTech do
		local tech = applic:GetTech( i-1 )
		local techCompany = tech.company
		local tg = tech.techGroup
		
		if (techCompany.empty or techCompany.name == companyName) and
			(tg == base.PT_SCRIPTS or tg == base.PT_MINIGAME or tg == base.PT_PHYSIC or tg == base.PT_ADVTECH) then	
				base.gpmFunctions.LinkBox( flick, tech.name, tg, tech, base.DRG, nil, nil )
		end
	end	
	--Log("SCRIPT-CREATEGP:ShowAvaibleScriptLevel public script = " .. showeddLinks )
end

function Show()
	company = base.applic.playerCompany
	gpm = base.gameprojectManager
	project = gpm.project	
	projectWindow = gpm.projectWindow
	
	gpm.UpdateProjectWindow( "tech" )
	_ShowAvaibleScriptAndMiniGames()
	
	local flick = guienv:AddFlick( "5%", "5%", "45%", "95%", 3, -1, projectWindow )

	local se = project.scriptEngine
	local lnk = base.gpmFunctions.LinkBox( flick, "Скрипты", base.PT_SCRIPTS, se, base.NDRG, _Set, _Unset )
	lnk.defaultTexture = "media/buttons/scriptsNoImage.png"
	if not se.empty then lnk.text = se.name	end
	
	local mg = project.miniGameEngine
	lnk = base.gpmFunctions.LinkBox( flick, "Миниигры", base.PT_MINIGAME, mg, base.NDRG, _Set, _Unset )
	lnk.defaultTexture = "media/buttons/minigameNoImage.png"						   
	if not mg.empty then	lnk.text = mg.name end

	local ph = project.physicEngine
	lnk = base.gpmFunctions.LinkBox( flick, "Физика", base.PT_PHYSIC, ph, base.NDRG, _Set, _Unset )
	lnk.defaultTexture = "media/buttons/physicNoImage.png" 
	if not ph.empty then lnk.text = ph.name end

	local maxProjectAdvTech = project.gameEngine.techNumber
	for i=1, maxProjectAdvTech do
		local tech = project:GetTech( i-1, base.PT_ADVTECH )
		lnk = base.gpmFunctions.LinkBox( flick, tech.name, base.PT_ADVTECH, tech, base.NDRG, _Set, _Unset )
    	lnk.defaultTexture = "media/buttons/advTechNoImage.png"
	end
	
	gpm.ShowParams()
end
