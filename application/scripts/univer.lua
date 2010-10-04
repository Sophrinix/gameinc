local base = _G

module( "univer" )

local guienv = base.guienv

local scrWidth = base.scrWidth
local scrHeight = base.scrHeight
local button = base.button
local tutorial = base.tutorial
local applic = base.applic

local mode = { }
mode[ "Программисты" ] = "coder"
mode[ "Дизайнеры" ] = "designer"
mode[ "Композиторы" ] = "composer"
mode[ "Тестировщики" ] = "tester"
--"coder" "designer" "composer" "tester"

local modeUserView = "coder"
local windowUpEmployer = nil

local userToUp = nil

function Show()
	if univerWindow then
		univerWindow:SetVisible( true )
	else
		univerWindow = guienv:AddWindow( "media/univer_dvor.tga", 0, 0, scrWidth, scrHeight, -1, guienv:GetRootGUIElement() )
		univerWindow:GetCloseButton():SetVisible( false )
		univerWindow:SetDraggable( false )
		
		--adding closeButton
		button.Stretch( scrWidth - 80, scrHeight - 80, scrWidth, scrHeight, 
		 			    "button_down", univerWindow:Self(), -1, "",
						"./univerWindow:Hide()" )
	end	
	
	tutorial.Update( tutorial.STEP_OVERVIEW_UNIVER )

	--stuff plate	
	button.EqualeTexture( 122, 320, "stuffPlate", univerWindow:Self(), -1, "", "./univer.ShowEmployersWindow()" )
	
	--outsourcing
	button.EqualeTexture( 612, 300, "outsorcing", univerWindow:Self(), -1, "", "./outsourcing.Show()" )
end

local function AddProgressBar( windowe, x1, y1, x2, y2, id, pos, textr )
	local prg = guienv:AddProgressBar( windowe:Self(), x1, y1, x2, y2, id )
	prg:SetPosition( pos )						   
	prg:SetImage( "media/stars01.tga" )
	prg:SetFillImage( "media/stars06.tga" )
	local label = guienv:AddLabel( textr, x1, y1, x2, y2, -1, windowe:Self() )
	label:SetTextAlignment( base.EGUIA_CENTER, base.EGUIA_CENTER )
end

function ShowUserInfoWindow( x, y, width, height, user )
	local windowg = guienv:AddWindow( "", x, y, x + width, y + height, -1, windowUpEmployer:Self() )
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

	AddProgressBar( windowg, xOffset, 30, width - 5, 30 + 15, -1, 
					user:GetParam( "knowledgeLevel" ), base.STR_EXPERIENCE )
	AddProgressBar( windowg, xOffset, 55, width - 5, 55 + 15, -1, 
					user:GetParam("codeQuality"), base.STR_QUALITY )
	AddProgressBar( windowg, xOffset, 80, width - 5, 80 + 15, -1,
					user:GetParam("codeSpeed"), base.STR_SPEED )
	AddProgressBar( windowg, xOffset, 105, width - 5, 105 + 15, -1,
	                user:GetParam("stability"), base.STR_STAMINA )
					   
    guienv:AddLabel( "Зарплата: "..user:GetParam( "wantMoney" ).."$", xOffset, 110, width, 110 + 20, -1, windowg:Self() )
	
	local btn = guienv:AddButton( width / 2 - 50, height - 30, width / 2 + 50, height - 10, windowg:Self(), -1, "Нанять" )
	btn:SetAction( "./univer.UpEmployer()" )				   
end

function UpEmployer()
	local button = base.CLuaButton( base.NrpGetSender() )
	local windowg = base.CLuaWindow( button:GetParent() )
	local name = windowg:GetText() 
	
	userToUp = applic:GetUserByName( name )
	if userToUp:GetParam( "contractMoney" ) > 0 then 
	    local money = userToUp:GetParam( "contractMoney" ) * userToUp:GetParam( "wantMoney" )
		guienv:MessageBox( "Деньги за контракт $" .. money, true, true, "./univer.UpContractUser()", "" )
		return 0
	end
	
	sworkEmployContractUser()
	guienv:AddToDeletionQueue( windowg:Self() )
end

function UpContractUser()
	local company = applic:GetPlayerCompany()
	company:AddUser( userToUp:Self() )
	sworkCreateEmployersWindow()
end

local function ShowAvaibleEmployers()
	local maxuser = applic:GetUserNumber()
	local hTemp = ( scrHeight - 150 ) / 3
	
	local xoffset = 10
	local yoffset = 100
	
	local cnt = 0
	base.Log({src=base.SCRIPT, dev=base.ODS|base.CON}, "ShowAvaibleEmployers:appusers" .. maxuser )
	
	for i=1, maxuser do
		local user = applic:GetUser( i-1 )
	
		base.Log({src=base.SCRIPT, dev=base.ODS|base.CON}, "ShowAvaibleEmployers:user=" .. user:GetName() .. " type=" .. user:GetTypeName() )
		if modeUserView == user:GetTypeName() and user:IsFreeUser() then
			if cnt < 3 then
				ShowUserInfoWindow( xoffset, yoffset + cnt * hTemp, scrWidth / 2 - 30,  hTemp, user ) 
			else
				ShowUserInfoWindow( xoffset + scrWidth / 2, yoffset + (cnt - 3) * hTemp, scrWidth / 2 - 30,  hTemp, user ) 
			end
		    cnt = cnt + 1
		end
		
		if cnt > 6 then 
			return 0 
		end
	end
end

function ShowEmployersWindow()
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
	
	ShowAvaibleEmployers()
end

function Hide()
	windowUpEmployer:Remove()
	windowUpEmployer = nil
end

function ChangeUserType( name )
	modeUserView = mode[ name ] 
	ShowEmployersWindow()
end
