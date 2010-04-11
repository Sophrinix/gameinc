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

local userToUp = nil

function sworkCreateUserInfoWindow( parentWnd, x, y, width, height, userPtr )
	local user = CLuaUser( userPtr )
	local windowg = CLuaWindow( guienv:AddWindow( "", x, y, x + width, y + height, -1, parentWnd ) )
	local button = CLuaButton( windowg:GetCloseButton() )
	button:SetVisible( false )
	windowg:SetText( user:GetName() )
	windowg:SetDraggable( false )

	guienv:AddLabel( "Опыт", 5, 30, width, 30 + 20, -1, windowg:Self() )
	local prg = CLuaProgressBar( guienv:AddProgressBar( windowg:Self(), 50, 30, width - 5, 30 + 20, -1 ) )
	prg:SetPosition( user:GetParam( "knowledgeLevel" ) )						   
	prg:SetImage( "media/starprogressbarB.png" )
	prg:SetFillImage( "media/starprogressbar.png" )
	
	guienv:AddLabel( "Качество", 5, 50, width, 50 + 20, -1, windowg:Self() )
	prg:SetObject( guienv:AddProgressBar( windowg:Self(), 50, 55, width - 5, 55 + 20, -1 ) )
	prg:SetPosition( user:GetParam("codeQuality") ) 	
	prg:SetImage( "media/starprogressbarB.png" )
	prg:SetFillImage( "media/starprogressbar.png" )

    guienv:AddLabel( "Скорость", 5, 70, width, 70 + 20, -1, windowg:Self() )
	prg:SetObject( guienv:AddProgressBar( windowg:Self(), 50, 80, width - 5, 80 + 20, -1 ) )
	prg:SetPosition( user:GetParam("codeSpeed") ) 
	prg:SetImage( "media/starprogressbarB.png" )
	prg:SetFillImage( "media/starprogressbar.png" )

    guienv:AddLabel( "Устойчивость", 5, 90, width, 90 + 20, -1, windowg:Self() )
	prg:SetObject( guienv:AddProgressBar( windowg:Self(), 50, 105, width - 5, 105 + 20, -1 ) )
	prg:SetPosition( user:GetParam("stability") ) 
	prg:SetImage( "media/starprogressbarB.png" )
	prg:SetFillImage( "media/starprogressbar.png" )
					   
    guienv:AddLabel( "Зарплата: "..user:GetParam( "wantMoney" ).."$", 5, 110, width, 110 + 20, -1, windowg:Self() )
	
	windowg:AddLuaFunction( GUIELEMENT_LMOUSE_LEFTUP, "sworkUpEmployer" )				   
end

function sworkUpEmployer( ptr )
	local event = CLuaEvent( ptr )
	local windowg = CLuaWindow( event:GetGuiCaller() )
	local name = windowg:GetText() 
	
	local userToUp = applic:GetUserByName( name )
	if userToUp:GetParam( "contractMoney" ) > 0 then 
	    local money = userToUp:GetParam( "contractMoney" ) * userToUp:GetParam( "wantMoney" )
		guienv:MessageBox( "Деньги за контракт $" .. money, true, true, "sworkEmployContractUser", "" )
		return 0
	end
	
	sworkEmployContractUser()
end

function sworkEmployContractUser()
	local company = CLuaCompany( applic:GetPlayerCompany() )
	company:AddUser( user:Self() )
	applic:RemoveUser( user:Self() )
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
	local windowg = CLuaWindow( guienv:AddWindow( "", 0, 0, 800, 600, WINDOW_EMPLOYER_SELECT, guienv:GetRootGUIElement() ) )
	
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

function sworkChangerUserType( ptr )
	local button = CLuaButton( ptr )
	modeUserView = mode[ button:GetText() ] 
	local windowg = CLuaWindow( guienv:GetElementByID( WINDOW_EMPLOYER_SELECT ) )
	windowg:Remove()
	sworkCreateEmployersWindow()
end