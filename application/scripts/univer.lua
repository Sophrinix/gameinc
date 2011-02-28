local base = _G

module( "univer" )

btnDesk = nil

local guienv = base.guienv

local button = base.button
local tutorial = base.tutorial
local applic = base.applic
local window = base.window
local univerWindow = nil

local mode = { }
mode[ base.STR_CODERS ] = "coder"
mode[ base.STR_DESIGNERS ] = "designer"
mode[ base.STR_COMPOSERS ] = "composer"
mode[ base.STR_TESTERS ] = "tester"
--"coder" "designer" "composer" "tester"

local modeUserView = "coder"
local windowUpEmployer = nil

local userToUp = nil
local company = nil

local usersWindow = {}

local function _Hide()
end

local function ShowOutsourcingWindow()
end

function Show()
	company = applic.playerCompany

	univerWindow = base.window.fsWindow( "univer_dvor.png", _Hide )
	
	tutorial.Update( tutorial.STEP_OVERVIEW_UNIVER )

	--stuff plate	
	btnDesk = button.EqualeTexture( 122, 320, "stuffPlate", univerWindow, -1, "", ShowEmployersWindow )
	
	--outsourcing
	button.EqualeTexture( 612, 300, "outsorcing", univerWindow, -1, "", ShowOutsourcingWindow )
end

local function _AddProgressBar( windowe, pos, texture )
	local prg = guienv:AddProgressBar( windowe, 0, 0, 1, 1, -1 )
	prg.position = pos
	prg.image = texture
	prg.fillImage = "media/pogressbar/prg_star_fill.png"
	--local label = guienv:AddLabel( textr, x1, y1, x2, y2, -1, windowe )
	--label:SetTextAlignment( base.EGUIA_CENTER, base.EGUIA_CENTER )
end

local function _ShowUserInfoWindow( x, y, width, height, user, parent )
	local windowg = guienv:AddWindow( "", x, y, width, height, -1, parent )
	windowg.closeButton.visible = false
	
	local money = user:GetParam( "wantMoney" )
	windowg.text = user.name.." ($"..money..")"
	windowg.draggable = false
	
	local image = guienv:AddImage( 20, 20, "100+", "140+", windowg, -1, "" )
	image.texture = user.texture
	image.scaleImage = true
	image.useAlphaChannel = true
	
	local layout = guienv:AddLayout( 130, 20, "95%", "40e", 2, -1, windowg )
	_AddProgressBar( layout, user:GetParam( "knowledgeLevel" ), "media/progressbar/prg_experience.png" )
	_AddProgressBar( layout, user:GetParam("codeQuality"), "media/progressbar/prg_quality.png" )
	_AddProgressBar( layout, user:GetParam("codeSpeed"), "media/progressbar/prg_speed.png" )
	_AddProgressBar( layout, user:GetParam("stability"), "media/progressbar/prg_stamina.png" )
					   
	local btn = guienv:AddButton( "25%", "30e", "50%+", "5e", windowg, -1, "Нанять" )
	btn.action = UpEmployer
	
	return windowg			   
end

local function _ShowAvaibleEmployers()
	local maxuser = applic.userNumber

	base.LogScript( "ShowAvaibleEmployers:appusers" .. maxuser )
	
	for i=1, #usersWindow do
		guienv:AddToDeletionQueue( usersWindow[ i ] )
		usersWindow[ i ] = nil
	end

	local layout = guienv:AddLayout( "5%", "10%", "95%", "95%", 2, -1, windowUpEmployer )
	local position=1
	for i=1, maxuser do
		local user = applic:GetUser( i-1 )

		if modeUserView == user.typeName and user.freeUser then
			usersWindow[ position ] = _ShowUserInfoWindow( "2%", (5 + (position-1) * 32).."%", "45%+",  "31%+", user, layout ) 
		    position = position + 1
		end
		
		if position > 7 then 
			return 0 
		end
	end
end

function UpEmployer( mp )
	local button = base.CLuaButton( mp )
	local windowg = base.CLuaWindow( button.parent )
	
	userToUp = applic:GetUserByName( windowg.text )

	if userToUp:GetParam( "contractMoney" ) > 0 then 
	    local money = userToUp:GetParam( "contractMoney" ) * userToUp:GetParam( "wantMoney" )
		guienv:MessageBox( "Деньги за контракт $" .. money, true, true, UpContractUser, nil )
		return 0
	end
	
	company:AddUser( userToUp )
	guienv:AddToDeletionQueue( windowg )
	_ShowAvaibleEmployers()
end

function UpContractUser()
	company:AddUser( userToUp )
	_ShowAvaibleEmployers()
end

function CloseEmployersWindow()
	windowUpEmployer:Remove()
	windowUpEmployer = nil
end

local function _CloseUpWindow()
	windowUpEmployer:Remove()
	windowUpEmployer = nil
end

function ShowEmployersWindow()
	windowUpEmployer = window.fsWindow( "media/textures/stuffUpWindowBg.png", _CloseUpWindow )
	
	local layout = guienv:AddLayout( 20, 20, "20e", "70+", 4, -1, windowUpEmployer ) 	
	button.LayoutButton( "", windowUpEmployer, -1, base.STR_CODERS, function () _ChangeUserType( STR_CODERS ) end )
	button.LayoutButton( "", windowUpEmployer, -1, base.STR_DESIGNERS, function () _ChangeUserType( STR_DESIGNERS ) end )
	button.LayoutButton( "", windowUpEmployer, -1, base.STR_COMPOSERS, function () _ChangeUserType( STR_COMPOSERS ) end )
	button.LayoutButton( "", windowUpEmployer, -1, base.STR_TESTERS, function () _ChangeUserType( STR_TESTERS ) end )
	
	_ShowAvaibleEmployers()
end

function ChangeUserType( name )
	modeUserView = mode[ name ] 
	ShowEmployersWindow()
end
