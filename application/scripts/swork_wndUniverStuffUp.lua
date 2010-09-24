local mode = { }
mode[ "Программисты" ] = "coder"
mode[ "Дизайнеры" ] = "designer"
mode[ "Композиторы" ] = "composer"
mode[ "Тестировщики" ] = "tester"
--"coder" "designer" "composer" "tester"

local modeUserView = "coder"
local windowUpEmployer = nil

local userToUp = nil

function sworkCreateUserInfoWindow( parentWnd, x, y, width, height, userPtr )
	local user = CLuaUser( userPtr )
	local windowg = guienv:AddWindow( "", x, y, x + width, y + height, -1, parentWnd )
	local button = windowg:GetCloseButton()
	local image = nil
	
	button:SetVisible( false )
	windowg:SetText( user:GetName() )
	windowg:SetDraggable( false )

	local xOffset = width / 3	
	image = guienv:AddImage( 30, 30, 30 + 107, 30 + 141, windowg:Self(), -1, "" )
	image:SetImage( user:GetTexture() )
	image:SetScaleImage( true )
	image:SetUseAlphaChannel( true )	

	local prg = guienv:AddProgressBar( windowg:Self(), xOffset, 30, width - 5, 30 + 15, -1 )
	prg:SetPosition( user:GetParam( "knowledgeLevel" ) )						   
	prg:SetImage( "media/stars01.tga" )
	prg:SetFillImage( "media/stars06.tga" )
	guienv:AddLabel( "Опыт", xOffset, 30, width, 30 + 15, -1, windowg:Self() ):SetTextAlignment( EGUIA_CENTER, EGUIA_CENTER )

	prg = guienv:AddProgressBar( windowg:Self(), xOffset, 55, width - 5, 55 + 15, -1 )
	prg:SetPosition( user:GetParam("codeQuality") ) 	
	prg:SetImage( "media/stars01.tga" )
	prg:SetFillImage( "media/stars06.tga" )
	guienv:AddLabel( "Качество", xOffset, 55, width, 55 + 15, -1, windowg:Self() ):SetTextAlignment( EGUIA_CENTER, EGUIA_CENTER )

	prg = guienv:AddProgressBar( windowg:Self(), xOffset, 80, width - 5, 80 + 15, -1 )
	prg:SetPosition( user:GetParam("codeSpeed") ) 
	prg:SetImage( "media/stars01.tga" )
	prg:SetFillImage( "media/stars06.tga" )
	guienv:AddLabel( "Скорость", xOffset, 80, width, 80 + 15, -1, windowg:Self() ):SetTextAlignment( EGUIA_CENTER, EGUIA_CENTER )

	prg = guienv:AddProgressBar( windowg:Self(), xOffset, 105, width - 5, 105 + 15, -1 )
	prg:SetPosition( user:GetParam("stability") ) 
	prg:SetImage( "media/stars01.tga" )
	prg:SetFillImage( "media/stars06.tga" )
	guienv:AddLabel( "Устойчивость", xOffset, 105, width, 105 + 15, -1, windowg:Self() ):SetTextAlignment( EGUIA_CENTER, EGUIA_CENTER )
					   
    guienv:AddLabel( "Зарплата: "..user:GetParam( "wantMoney" ).."$", xOffset, 110, width, 110 + 20, -1, windowg:Self() )
	
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
	local hTemp = ( scrHeight - 150 ) / 3
	
	local xoffset = 10
	local yoffset = 100
	
	local cnt = 0
	Log({src=SCRIPT, dev=ODS|CON}, "ShowAvaibleEmployers:appusers" .. maxuser )
	
	for i=1, maxuser do
		local user = applic:GetUser( i-1 )
	
		Log({src=SCRIPT, dev=ODS|CON}, "ShowAvaibleEmployers:user=" .. user:GetName() .. " type=" .. user:GetTypeName() )
		if modeUserView == user:GetTypeName() then
			if cnt < 3 then
				sworkCreateUserInfoWindow( ptr, xoffset, yoffset + cnt * hTemp, scrWidth / 2 - 30,  hTemp, user:Self() ) 
			else
				sworkCreateUserInfoWindow( ptr, xoffset + scrWidth / 2, yoffset + (cnt - 3) * hTemp, scrWidth / 2 - 30,  hTemp, user:Self() ) 
			end
		    cnt = cnt + 1
		end
		
		if cnt > 6 then 
			return 0 
		end
	end
end

function sworkCreateEmployersWindow( ptr )
	if windowUpEmployer == nil then
		windowUpEmployer = guienv:AddWindow( "media/stuffUpWindowBg.tga", 0, 0, scrWidth, scrHeight, WINDOW_EMPLOYER_SELECT_ID, guienv:GetRootGUIElement() )
		windowUpEmployer:SetDraggable( false )
	else
		windowUpEmployer:SetVisible( true )
		
	end
	
	local btn = windowUpEmployer:GetCloseButton()
	btn:SetVisible( false )
	
	local wTmp = scrWidth / 6
	local xOffset = 20
	local yOffset = 30
	local button = guienv:AddButton( xOffset, yOffset, xOffset + wTmp, 90, windowUpEmployer:Self(), -1, "Программисты" )
	button:SetAction( "sworkWindowUpEmployerChangerUserType" )
	
	xOffset = xOffset + wTmp + 20
	button = guienv:AddButton( xOffset, yOffset, xOffset + wTmp, 90, windowUpEmployer:Self(), -1, "Дизайнеры" )
	button:SetAction( "sworkWindowUpEmployerChangerUserType" )

	xOffset = xOffset + wTmp + 20
	button = guienv:AddButton( xOffset, yOffset, xOffset + wTmp, 90, windowUpEmployer:Self(), -1, "Композиторы" )
	button:SetAction( "sworkWindowUpEmployerChangerUserType" )
	
	xOffset = xOffset + wTmp + 20
	button = guienv:AddButton( xOffset, yOffset, xOffset + wTmp, 90, windowUpEmployer:Self(), -1, "Тестировщики" )
	button:SetAction( "sworkWindowUpEmployerChangerUserType" )
	
	xOffset = xOffset + wTmp + 20
	button = guienv:AddButton( xOffset, yOffset, xOffset + wTmp, 90, windowUpEmployer:Self(), -1, "Выход" )
	button:SetAction( "sworkWindowUpEmployerClose" )
	
	ShowAvaibleEmployers( windowUpEmployer:Self() )
end

function sworkWindowUpEmployerClose( ptr )
	windowUpEmployer:Remove()
	windowUpEmployer = nil
end

function sworkWindowUpEmployerChangerUserType( ptr )
	local button = CLuaButton( ptr )
	modeUserView = mode[ button:GetText() ] 
	sworkCreateEmployersWindow()
end
