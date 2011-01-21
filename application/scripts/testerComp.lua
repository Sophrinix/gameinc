local base = _G

module( "testerComp" )

local guienv = base.guienv
local scrWidth = base.scrWidth
local scrHeight = base.scrHeight
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
	guienv:AddTimer( base.AFADE_TIME, "testerComp.FadeExitAction()" )	
end

function Show( ptr )
	if mainWindow then
		mainWindow:SetVisible( true )
	else
		mainWindow = guienv:AddWindow( "media/textures/testComp.png", 0, 0, 
									   scrWidth, scrHeight, -1, 
									   guienv:GetRootGUIElement() )
		mainWindow:SetDraggable( false )
		mainWindow:SetVisible( false )
		mainWindow:GetCloseButton():SetVisible( false )
		mainWindow:SetDrawBody( false )
		
		--adding closeButton
		button.Stretch( "95%", "95%", "100%", "100%", 
		 			    "button_down", 
		 			    mainWindow, -1, "",
						"./testerComp.Hide()" )
	end	
	
	lbDesc = guienv:AddLabel( "", 278, 417, 770, 655, -1, mainWindow )
	
	picflowGames = guienv:AddPictureFlow( 350, 70, 680, 310, -1, mainWindow )
	picflowGames:SetDrawBorder( false )
	
	local project = nil
	local company = applic.playerCompany
	
	for i=1, company:GetDevProjectNumber() do
		project = company:GetDevProject( i-1 )
		
		if project:GetTechGroup() == base.PT_GAME then
			picflowGames:AddItem( project:GetName(), "", project )
		end
	end	
	
	--button.EqualeTexture( 0, 227, "newProject", mainWindow, -1, "", "./projectSelect.Show()")
	--employers manager
	
	--картинка с отображением игры
	mainWindow:AddLuaFunction( base.GUIELEMENT_LBXITEM_SELECTED, "./testerComp.SelectGame()" )
	
	guienv:FadeAction( base.FADE_TIME, false, false )			
	guienv:AddTimer( base.AFADE_TIME, "testerComp.FadeEnterAction()" )
end

function SelectGame()
	local game = base.CLuaDevelopProject( picflowGames:GetSelectedObject() )
	
	local ret = {}
	-- base.PT_GAMEENGINE=0, base.PT_VIDEOTECH=0, base.PT_SOUNDTECH=0, base.PT_ADVTECH=0 }
	for i=1, game:GetModuleNumber() do
	    local module = game:GetModule( i-1 )
	    local tg = module:GetTechGroup()
	    local dt = testDef.defs[ tg ]
	    
	    Log( "module:"..module:GetName() )
	    if dt and module:Empty() == 0 then
			Log( "module:"..module:GetName().." qua:"..module:GetQuality().."  prc:"..module:GetPercentDone() )
			local quality = base.math.floor( module:GetQuality() / 100 * (#dt) )
			
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
	
	lbDesc:SetText( text )
end

function FadeEnterAction()
	mainWindow:SetVisible( true )
	guienv:FadeAction( base.FADE_TIME, true, true )
end

function FadeExitAction()
	mainWindow:Remove()
	mainWindow = nil
	guienv:FadeAction( base.FADE_TIME, true, true )
end

