local base = _G

IncludeScript("director")

module( "office" )

local guienv = base.guienv
local receptionWindow = nil

local scrWidth = base.scrWidth
local scrHeight = base.scrHeight
local button = base.button
local tutorial = base.tutorial

function FadeEnterAction()
	receptionWindow:SetVisible( true )
	guienv:FadeAction( base.FADE_TIME, true, true )
end

function FadeExitAction()
	receptionWindow:Remove()
	receptionWindow = nil
	guienv:FadeAction( base.FADE_TIME, true, true )
end

function Hide()
	guienv:FadeAction( base.FADE_TIME, false, false )			
	guienv:AddTimer( base.AFADE_TIME, "office.FadeExitAction()" )	
end

function Show()
	if receptionWindow then
		receptionWindow:SetVisible( true )
	else
		receptionWindow = guienv:AddWindow( "media/maps/reception.png", 0, 0, scrWidth, scrHeight, -1, guienv:GetRootGUIElement() )
		receptionWindow:GetCloseButton():SetVisible( false )
		receptionWindow:SetDraggable( false )
		receptionWindow:SetVisible( false )
		
		--adding closeButton
		button.Stretch( scrWidth - 80, scrHeight - 80, scrWidth, scrHeight, 
		 			    "button_down", receptionWindow:Self(), -1, "",
						"./office.Hide()" )
		
	end	
	
	tutorial.Update( tutorial.STEP_OVERVIEW_RECEPTION )
	--directors room
	button.EqualeTexture( 448, 242, "director", receptionWindow:Self(), -1, "", "./director.Show()" )
	--[[
	local showEmployers = sceneManager:GetSceneNodeByName( "employerManageNode" )
	selector = sceneManager:CreateTriangleSelectorFromBoundingBox( showEmployers:Self() )
	showEmployers:SetTriangleSelector( selector )
	sceneManager:SetMarkText( showEmployers:Self(), "employerManageNode" )
	
	local showProjectManager = sceneManager:GetSceneNodeByName( "projectManagerNode" )
	selector = sceneManager:CreateTriangleSelectorFromBoundingBox( showProjectManager:Self() )
	showProjectManager:SetTriangleSelector( selector )
	sceneManager:SetMarkText( showProjectManager:Self(), "projectManagerNode" )
	
	local showInventionManager = sceneManager:GetSceneNodeByName( "inventionManagerNode" )
	selector = sceneManager:CreateTriangleSelectorFromBoundingBox( showInventionManager:Self() )
	showInventionManager:SetTriangleSelector( selector )
	sceneManager:SetMarkText( showInventionManager:Self(), "inventionManagerNode" )
		
	local exitN = sceneManager:GetSceneNodeByName( "exitOfficeNode" )
	selector = sceneManager:CreateTriangleSelectorFromBoundingBox( exitN:Self() )
	exitN:SetTriangleSelector( selector )
	sceneManager:SetMarkText( exitN:Self(), "exit" )
	
	sceneManager:AddSceneFunction( SCENE_LMOUSE_DOUBLE_CLICK, "sworkSelectObjectOnOfficeScene" )
	--]]
	
	guienv:FadeAction( base.FADE_TIME, false, false )			
	guienv:AddTimer( base.AFADE_TIME, "office.FadeEnterAction()" )
end