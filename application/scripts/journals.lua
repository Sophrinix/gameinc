local base = _G

module( "journals" )

local button = base.button
local guienv = base.guienv
local browser = base.browser
local currentGame = nil
local scrWidth = base.scrWidth
local scrHeight = base.scrHeight
local applic = base.applic

local MainWindow = nil
local lbxGames = nil
local picflowGames = nil

function ShowHistoryReview()
	currentGame = base.CLuaGame( lbxGames:GetSelectedObject() )
	browser:Show()
	browser:Navigate( currentGame:GetDescriptionLink() )
end

local function ShowAvaibleGames()
	local game = nil
	for i=1, applic:GetGamesNumber() do
		game = applic:GetGame( i-1 )
		lbxGames:AddItem( game:GetName(), game:Self() )
	end	
end

function Show()
	if mainWindow then
		mainWindow:SetVisible( true )
	else
		mainWindow = guienv:AddWindow( "media/textures/magazine.png", 0, 0, scrWidth, scrHeight, -1, guienv:GetRootGUIElement() )
		mainWindow:SetDraggable( false )
		mainWindow:GetCloseButton():SetVisible( false )
		mainWindow:SetDrawBody( false )
		
		--adding closeButton
		button.Stretch( scrWidth - 80, scrHeight - 80, scrWidth, scrHeight, 
		 			    "button_down", mainWindow:Self(), -1, "",
						"./journals.Hide()" )
						
		lbxGames = guienv:AddComponentListBox( 320, 50, 730, 520, -1, mainWindow:Self() )						
		--picflowGames = guienv:AddPictureFlow( 60, 10, scrWidth - 10, scrHeight / 3, -1, campaniesWindow:Self() )
		--picflowGames:SetPictureRect( 0, 0, scrHeight / 3 - 40, scrHeight / 3 - 40 )
		--picflowGames:SetDrawBorder( false )		
		
		mainWindow:AddLuaFunction( base.GUIELEMENT_LBXITEM_SELECTED, "./journals.ShowHistoryReview()" )				
	end
	
	ShowAvaibleGames()
end

function Hide()
	mainWindow:Remove()
	mainWindow = nil
end