local base = _G

IncludeScript( "testDef" )

module( "testerComp" )

local guienv = base.guienv
local button = base.button
local window = base.window
local applic = base.applic
local tutorial = base.tutorial
local sizeLinkBox = 80
local testDef = base.testDef
local Log = base.LogScript

local mainWindow = nil
local picflowGames = nil
local lbDesc = nil

local function _Hide()
	base.package.loaded[ "testDef" ] = false
end

local function _SelectGame()
	local game = base.CLuaDevelopProject( picflowGames.selectedObject )
	
	local ret = {}

	for i=1, game.moduleNumber do
	    local module = game:GetModule( i-1 )
	    local tg = module.techGroup
	    
	    if module then
			Log( "module:"..module.name.." qua:"..module.quality.."  prc:"..module.percentDone )
			
			if ret[ tg ] == nil then
				ret[ tg ] = module.quality
			else
				ret[ tg ] = ( ret[ tg ] + module.quality ) / 2
			end
		end
	end	
	
	local text = "Движок:"..testDef.GetText( base.PT_GAMEENGINE, ret[ base.PT_GAMEENGINE ] )
	text = text .. "\nВидео:"..testDef.GetText( base.PT_VIDEOTECH, ret[ base.PT_VIDEOTECH ] )
	text = text .. "\nЗвук:"..testDef.GetText( base.PT_SOUNDTECH, ret[ base.PT_SOUNDTECH ] )
	text = text .. "\nУправление:" .. testDef.GetText( base.PT_ADVTECH, ret[ base.PT_ADVTECH ] )

	lbDesc.text = text
end

function Show( ptr )
	local txs = base.driver:CreateBlur( "media/textures/testComp.png", 2, 4 )	
	mainWindow = window.fsWindow( txs.path, _Hide )
	
	lbDesc = guienv:AddLabel( "", 278, 417, 770, 655, -1, mainWindow )
	
	picflowGames = guienv:AddPictureFlow( 350, 70, 680, 310, -1, mainWindow )
	picflowGames.drawBody = false
	
	local project = nil
	local company = applic.playerCompany
	
	for i=1, company.devProjectNumber do
		project = company:GetDevProject( i-1 )
		
		if project.techGroup == base.PT_GAME then
			picflowGames:AddItem( project.texture, project.name, project )
		end
	end	
	
	--button.EqualeTexture( 0, 227, "newProject", mainWindow, -1, "", "./projectSelect.Show()")
	--employers manager
	
	--картинка с отображением игры
	picflowGames.onSelect = _SelectGame 
end
