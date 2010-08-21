local mode = { }
mode[ "Программисты" ] = "coder"
mode[ "Дизайнеры" ] = "designer"
mode[ "Композиторы" ] = "composer"
mode[ "Тестировщики" ] = "tester"
--"coder" "designer" "composer" "tester"

local modeUserView = "coder"
local windowUpEmployer = nil

local width = 800
local height = 600
local userToUp = nil

function sworkCreateUserInfoWindow( parentWnd, x, y, width, height, userPtr )
	local user = CLuaUser( userPtr )
	local windowg = guienv:AddWindow( "", x, y, x + width, y + height, -1, parentWnd )
	local button = windowg:GetCloseButton()
	
	button:SetVisible( false )
	windowg:SetText( user:GetName() )
	windowg:SetDraggable( false )

	guienv:AddLabel( "Опыт", 5, 30, width, 30 + 20, -1, windowg:Self() )
	local prg = guienv:AddProgressBar( windowg:Self(), 50, 30, width - 5, 30 + 20, -1 )
	prg:SetPosition( user:GetParam( "knowledgeLevel" ) )						   
	prg:SetImage( "media/starprogressbarB.png" )
	prg:SetFillImage( "media/starprogressbar.png" )
	
	guienv:AddLabel( "Качество", 5, 50, width, 50 + 20, -1, windowg:Self() )
	prg = guienv:AddProgressBar( windowg:Self(), 50, 55, width - 5, 55 + 20, -1 )
	prg:SetPosition( user:GetParam("codeQuality") ) 	
	prg:SetImage( "media/starprogressbarB.png" )
	prg:SetFillImage( "media/starprogressbar.png" )

    guienv:AddLabel( "Скорость", 5, 70, width, 70 + 20, -1, windowg:Self() )
	prg = guienv:AddProgressBar( windowg:Self(), 50, 80, width - 5, 80 + 20, -1 )
	prg:SetPosition( user:GetParam("codeSpeed") ) 
	prg:SetImage( "media/starprogressbarB.png" )
	prg:SetFillImage( "media/starprogressbar.png" )

    guienv:AddLabel( "Устойчивость", 5, 90, width, 90 + 20, -1, windowg:Self() )
	prg = guienv:AddProgressBar( windowg:Self(), 50, 105, width - 5, 105 + 20, -1 )
	prg:SetPosition( user:GetParam("stability") ) 
	prg:SetImage( "media/starprogressbarB.png" )
	prg:SetFillImage( "media/starprogressbar.png" )
					   
    guienv:AddLabel( "Зарплата: "..user:GetParam( "wantMoney" ).."$", 5, 110, width, 110 + 20, -1, windowg:Self() )
	
	local btn = guienv:AddButton( width / 2 - 50, height - 30, width / 2 + 50, height - 10, windowg:Self(), -1, "Нанять" )
	btn:SetAction( "sworkUpEmployer" )				   
end

function sworkUpEmployer( ptr )
	local button = CLuaButton( ptr )
	local windowg = CLuaWindow( button:GetParent() )
	local name = windowg:GetText() 
	
	userToUp = applic:GetUserByName( name )
	if userToUp:GetParam( "contractMoney" ) > 0 then 
	    local money = userToUp:GetParam( "contractMoney" ) * userToUp:GetParam( "wantMoney" )
		guienv:MessageBox( "Деньги за контракт $" .. money, true, true, "sworkEmployContractUser", "" )
		return 0
	end
	
	sworkEmployContractUser()
	guienv:AddToDeletionQueue( windowg:Self() )
end

function sworkEmployContractUser()
	local company = applic:GetPlayerCompany()
	company:AddUser( userToUp:Self() )
	sworkCreateEmployersWindow()
end

local function ShowAvaibleEmployers( ptr )
	local maxuser = applic:GetUserNumber()
	local hTemp = height / 4
	
	local xoffset = 10
	local yoffset = hTemp
	
	local cnt = 0
	Log({src=SCRIPT, dev=ODS|CON}, "ShowAvaibleEmployers:appusers" .. maxuser )
	
	for i=1, maxuser do
		local user = applic:GetUser( i-1 )
	
		Log({src=SCRIPT, dev=ODS|CON}, "ShowAvaibleEmployers:user=" .. user:GetName() .. " type=" .. user:GetTypeName() )
		if modeUserView == user:GetTypeName() then
			if cnt < 3 then
				sworkCreateUserInfoWindow( ptr, xoffset, yoffset + cnt * hTemp, width / 2,  hTemp, user:Self() ) 
			else
				sworkCreateUserInfoWindow( ptr, xoffset + width / 2, yoffset + (cnt - 3) * hTemp, width / 2,  hTemp, user:Self() ) 
			end
		    cnt = cnt + 1
		end
		
		if cnt > 6 then 
			return 0 
		end
	end
end

function sworkCreateEmployersWindow( ptr )
	windowUpEmployer = CLuaWindow( guienv:GetElementByID( WINDOW_EMPLOYER_SELECT_ID ) )
	
	if windowUpEmployer:Empty() == 1 then
		windowUpEmployer = guienv:AddWindow( "", 0, 0, scrWidth, scrHeight, WINDOW_EMPLOYER_SELECT_ID, guienv:GetRootGUIElement() )
	else
		local elm = CLuaElement( windowUpEmployer:Self() )
		elm:RemoveChilds()
	end
	
	local btn = windowUpEmployer:GetCloseButton()
	btn:SetVisible( false )
	
	local button = guienv:AddButton( 10, 10, 200, 100, windowUpEmployer:Self(), -1, "Программисты" )
	button:SetAction( "sworkWindowUpEmployerChangerUserType" )
	
	button = guienv:AddButton( 210, 10, 400, 100, windowUpEmployer:Self(), -1, "Дизайнеры" )
	button:SetAction( "sworkWindowUpEmployerChangerUserType" )

	button = guienv:AddButton( 410, 10, 600, 100, windowUpEmployer:Self(), -1, "Композиторы" )
	button:SetAction( "sworkWindowUpEmployerChangerUserType" )
	
	button = guienv:AddButton( 610, 10, 800, 100, windowUpEmployer:Self(), -1, "Тестировщики" )
	button:SetAction( "sworkWindowUpEmployerChangerUserType" )
	
	button = guienv:AddButton( scrWidth - 60, 10, scrWidth - 10, 60, windowUpEmployer:Self(), -1, "X" )
	button:SetAction( "sworkWindowUpEmployerClose" )
	
	ShowAvaibleEmployers( windowUpEmployer:Self() )
end

function sworkWindowUpEmployerClose( ptr )
   
end

function sworkWindowUpEmployerChangerUserType( ptr )
	local button = CLuaButton( ptr )
	modeUserView = mode[ button:GetText() ] 
	sworkCreateEmployersWindow()
end