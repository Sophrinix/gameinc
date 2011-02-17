local base = _G

module( "gpmGenrePage" )

local button = base.button
local applic = base.applic
local guienv = base.guienv
local Log = base.LogScript
local projectWindow = nil
local project = nil
local company = nil 
local gpm = nil

local function _ShowAvaibleGenreModules()
	local flick = guienv:AddFlick( "55%", "5%", "95%", "95%", 3, -1, projectWindow )

	for i=1, applic.techNumber do
		local tech = applic:GetTech( i-1) 
		local tg = tech.techGroup
		local techCompany = tech.company
		
		if( techCompany.empty or techCompany.name == companyName) and tech.techGroup == base.PT_GENRE 
		then
			local lnk = base.gpmFunctions.LinkBox( flick, tech.name, tech.techGroup, tech, 
												   base.DRG, nil, nil )												  
		end
	end	
end

local function _Unset( mp, _ )
	mp = base.CLuaLinkBox( mp )
	project:RemoveTech( mp.data )
	guienv:SetDragObject( nil, "" )
	base.gpmGenrePage.Show()
end

local function _Set( _, sender )
	sender = base.CLuaLinkBox( sender )
	project:AddTech( sender.data )
	guienv:SetDragObject( nil, "" )
	base.gpmGenrePage.Show()
end

function Show()
	gpm = base.gameprojectManager
	project = gpm.project	
	projectWindow = gpm.projectWindow	
	gpm.UpdateProjectWindow( "genre" )
	
	_ShowAvaibleGenreModules()
	
	local maxModuleNumber = project.gameEngine.genresNumber
	local flick = guienv:AddFlick( "5%", "5%", "45%", "95%", 3, -1, projectWindow )

	for i=1, maxModuleNumber do
		local genre = project:GetTech( i-1, base.PT_GENRE )
		local lnk = base.gpmFunctions.LinkBox( flick, 
											   base.string.format( "Модуль %d/%d \n%s", i, maxModuleNumber, genre.name ), 
											   base.PT_GENRE, genre, base.NDRG, _Set, _Unset )
											   
		lnk.defaultTexture = "media/buttons/genreNoImage2.png"
	end
	
	gpm.ShowParams()
end
