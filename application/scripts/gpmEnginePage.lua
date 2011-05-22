local base = _G

module( "gpmEnginePage" )

local button = base.button
local applic = base.applic
local guienv = base.guienv
local Log = base.LogScript
local projectWindow = nil
local project = nil
local company = nil 
local gpm = nil

local function _Set( mP, sender )
	sender = base.CLuaLinkBox( sender )
	project.gameEngine = sender.data
	base.gpmEnginePage.Show()
	guienv:SetDragObject( nil, "" )
end

local function _Unset( mP )
	project.gameEngine = nil
	base.gpmEnginePage.Show()
end

local function _ShowAvaibleEngines()
	local maxEngine = company.enginesNumber
	
	local flick = guienv:AddFlick( "55%", "5%", "95%", "95%", 4, -1, projectWindow )
	
	for i=1, maxEngine do
		local gameeng = company:GetEngine( i-1 )
		base.gpmFunctions.LinkBox( flick, 
								   base.string.format( "Движок %d/%d \r( %s )", i, maxEngine, gameeng.name ),
								   gameeng.techGroup, gameeng, base.DRG, nil, nil )			   
	end
end

function Show()
	company = base.applic.playerCompany
	gpm = base.gameprojectManager
	project = gpm.project	
	projectWindow = gpm.projectWindow
	
	gpm.UpdateProjectWindow( "engine" )
	_ShowAvaibleEngines()

	local ge = project.gameEngine
	
	--create linkbox for gameEngine
	local lnk = base.gpmFunctions.LinkBox( projectWindow, "",  
										   base.PT_GAMEENGINE, ge, base.NDRG, 
										   _Set, _Unset )
	lnk:SetPosition( "25%", "33%" )
	lnk.defaultTexture = "media/buttons/GameNoEngine.png"
	
	base.table.insert( gpm.links, lnk )
	
	--lnk = base.gpmFunctions.LinkBox( projectWindow, "Продолжение", base.PT_GAME, nil, base.NDRG, base.ENBL, 
	--								 nil, nil )
	--lnk:SetPosition( "25%", "50%" )
	
	gpm.ShowParams()
end
