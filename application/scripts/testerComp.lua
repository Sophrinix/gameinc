local base = _G

module( "testerComp" )

local guienv = base.guienv
local button = base.button
local applic = base.applic
local tutorial = base.tutorial
local sizeLinkBox = 80
local testDef = base.testDef
local Log = base.LogScript

local mainWindow = nil
local picflowGames = nil
local lbDesc = nil

function Hide()
	guienv:FadeAction( base.FADE_TIME, false, false )			
	guienv:AddTimer( base.AFADE_TIME, FadeExitAction )	
end

function Show( ptr )
	if mainWindow then
		mainWindow.visible = true
		return
	else
		mainWindow = window.fsWindow( "media/textures/testComp.png", Hide )
	end	
	
	lbDesc = guienv:AddLabel( "", 278, 417, 770, 655, -1, mainWindow )
	
	picflowGames = guienv:AddPictureFlow( 350, 70, 680, 310, -1, mainWindow )
	picflowGames.drawBorder = false
	
	local project = nil
	local company = applic.playerCompany
	
	for i=1, company.devProjectNumber do
		project = company:GetDevProject( i-1 )
		
		if project.techGroup == base.PT_GAME then
			picflowGames:AddItem( project.name, "", project )
		end
	end	
	
	--button.EqualeTexture( 0, 227, "newProject", mainWindow, -1, "", "./projectSelect.Show()")
	--employers manager
	
	--картинка с отображением игры
	mainWindow:AddLuaFunction( base.GUIELEMENT_LBXITEM_SELECTED, SelectGame )
	
	guienv:FadeAction( base.FADE_TIME, false, false )			
	guienv:AddTimer( base.AFADE_TIME, FadeEnterAction )
end

function SelectGame()
	local game = base.CLuaDevelopProject( picflowGames.selectedObject )
	
	local ret = {}
	-- base.PT_GAMEENGINE=0, base.PT_VIDEOTECH=0, base.PT_SOUNDTECH=0, base.PT_ADVTECH=0 }
	for i=1, game.moduleNumber do
	    local module = game:GetModule( i-1 )
	    local tg = module.techGroup
	    local dt = testDef.defs[ tg ]
	    
	    Log( "module:"..module.name )
	    if dt and not module.empty then
			Log( "module:"..module.name.." qua:"..module.quality.."  prc:"..module.percentDone )
			local quality = base.math.floor( module.quality / 100 * (#dt) )
			
			if ret[ tg ] == nil then
				ret[ tg ] = 1
			end
			
			ret[ tg ] = ( ret[ tg ] + quality ) / 2
			Log( "ret["..tg.."]="..ret[ tg ] )
		end
	end	
	
	local text = "Движок:"..testDef.engine[ ret[ base.PT_PLUGIN ] ]
	text = text .. "\nВидео:".. testDef.videotech[ ret[ base.PT_VIDEOTECH ] ]
	text = text .. "\nЗвук:".. testDef.soundtech[ ret[ base.PT_SOUNDTECH ] ]
	text = text .. "\nУправление:" .. testDef.advtech[ ret[ base.PT_ADVTECH ] ]
	
	lbDesc.text = text
end

function FadeEnterAction()
	mainWindow.visible = true
	guienv:FadeAction( base.FADE_TIME, true, true )
end

function FadeExitAction()
	mainWindow:Remove()
	mainWindow = nil
	guienv:FadeAction( base.FADE_TIME, true, true )
end

