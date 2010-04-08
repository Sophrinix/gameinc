local sceneManager = CLuaSceneManager( NrpGetSceneManager() )
local guienv = CLuaGuiEnvironment( NrpGetGuiEnvironment() )
local applic = CLuaApplication( NrpGetApplication() )
local mode = { }
mode[ "Программисты" ] = "coder"
mode[ "Дизайнеы" ] = "designer"
mode[ "Композиторы" ] = "composer"
mode[ "Тестировщики" ] = "tester"
--"coder" "designer" "composer" "tester"

local modeUserView = "coder"

local width = 800
local height = 600

function sworkCreateUserInfoWindow( parentWnd, x, y, width, height, userPtr )
	
	local user = CLuaUser( userPtr )
	local windowg = CLuaWindow( guienv:AddWindow( "", x, y, x + width, y + height, -1, parentWnd ) )
	windowg:SetText( user:GetName() )
	windowg:SetDraggable( false )

	guienv:AddLabel( "Опыт", 5, 30, 5 + 40, 30 + 20, -1, windowg:Self() )
	local prg = CLuaProgressBar( guienv:AddProgressBar( windowg:Self(), 50, 30, width - 5, 30 + 20, -1 ) )
	prg:SetPosition( user:GetExperience( SKILL_MIDDLE ) )						   
	prg:SetImage( "media/starprogressbar.png" )
	prg:SetFillImage( "media/starprogressbarf.png" )
	
--[[	guienv:AddLabel( "Качество", 5, 55, 5 + 40, 55 + 20, -1, windowg:Self() )
	guienv:AddProgressBar( "media/starprogressbar.png", "media/starprogressbarf.png", 
						   50, 55, width - 5, 55 + 20, user:GetParam("codeQuality") ) 	

    guienv:AddLabel( "Скорость", 5, 80, 5 + 40, 80 + 20, -1, windowg:Self() )
	guienv:AddProgressBar( "media/starprogressbar.png", "media/starprogressbarf.png", 
						   50, 80, width - 5, 80 + 20, user:GetParam("codeSpeed") ) 

    guienv:AddLabel( "Устойчивость", 5, 105, 5 + 40, 105 + 20, -1, windowg:Self() )
	guienv:AddProgressBar( "media/starprogressbar.png", "media/starprogressbarf.png", 
						   50, 105, width - 5, 105 + 20, user:GetParam("stability") ) 
						   
    guienv:AddLabel( "Зарплата: "..user:GetParam( "salary" ).."$", 5, 130, 5 + 40, 130 + 20, -1, windowg:Self() )
	
	local button = CLuaButton( guienv:AddButton( width / 2 - 15, height - 35, width / 2 + 15, height - 5, windowg:Self(), -1, "Нанять") )
	button:SetAction( "sworkUpEmployer" )
--]]					   
end

local function ShowAvaibleEmployers( ptr )
	local maxuser = applic:GetUserNumber()
	local hTemp = height / 4
	
	local xoffset = 10
	local yoffset = 0
	
	local cnt = 0
	
	for i=1, maxuser do
		local user = CLuaUser( applic:GetUser( i-1 ) )
		if modeUserView == user:GetTypeName() then
		    cnt = cnt + 1
			if cnt < 4 then
				sworkCreateUserInfoWindow( ptr, xoffset, yoffset + cnt * hTemp, width / 2,  hTemp, user:Self() ) 
			else
				sworkCreateUserInfoWindow( ptr, xoffset, yoffset + (cnt - 4) * hTemp, width / 2,  hTemp, user:Self() ) 
			end
		end
		
		if cnt > 4 then
			xoffset = width / 2 + 10
			yoffset = 10
		end
		
		if cnt > 8 then 
			return 0 
		end
	end
end

function sworkCreateEmployersWindow( ptr )
	local windowg = CLuaWindow( guienv:AddWindow( "", 0, 0, 800, 600, -1, guienv:GetRootGUIElement() ) )
	
	local button = CLuaButton( guienv:AddButton( 10, 10, 200, 100, windowg:Self(), -1, "Программисты" ) )
	button:SetAction( "sworkChangerUserType" )
	
	button:SetObject( guienv:AddButton( 210, 10, 400, 100, windowg:Self(), -1, "Дизайнеры" ) )
	button:SetAction( "sworkChangerUserType" )

	button:SetObject( guienv:AddButton( 410, 10, 600, 100, windowg:Self(), -1, "Композиторы" ) )
	button:SetAction( "sworkChangerUserType" )
	
	button:SetObject( guienv:AddButton( 610, 10, 800, 100, windowg:Self(), -1, "Тестировщики" ) )
	button:SetAction( "sworkChangerUserType" )
	
	ShowAvaibleEmployers( windowg:Self() )
end

local function sworkChangerUserType( ptr )

	local button = CLuaButton( ptr )
	modeUserView = mode[ button:GetText() ] 
	
end