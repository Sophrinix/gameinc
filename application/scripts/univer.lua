local base = _G

IncludeScript( "userInfo" )

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
local layoutWindows  = nil

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
	
	tutorial.Update( "univer/main" )

	--stuff plate	
	btnDesk = button.EqualeTexture( 122, 320, "stuffPlate", univerWindow, -1, "", ShowEmployersWindow )
	
	--outsourcing
	button.EqualeTexture( 612, 300, "outsorcing", univerWindow, -1, "", ShowOutsourcingWindow )
end

local function _UpContractUser()
	company:AddUser( userToUp )
	base.univer.ShowAvaibleEmployers()
end

local function _UpEmployer( userToUp )
	if userToUp == nil then 
		return 
	end
	
	if userToUp:GetParam( "contractMoney" ) > 0 then 
	    local money = userToUp:GetParam( "contractMoney" ) * userToUp:GetParam( "wantMoney" )
		guienv:MessageBox( "Деньги за контракт $" .. money, true, true, _UpContractUser, nil )
		return 0
	end
	
	company:AddUser( userToUp )
	base.univer.ShowAvaibleEmployers()
end

function ShowAvaibleEmployers()
	local maxuser = applic.userNumber

	base.LogScript( "ShowAvaibleEmployers:appusers" .. maxuser )
	
	for i=1, #usersWindow do
		guienv:AddToDeletionQueue( usersWindow[ i ].window )
	end
	
	usersWindow = nil
	usersWindow = {}

	local position=1
	for i=1, maxuser do
		local mUser = applic:GetUser( i-1 )
		
		if modeUserView == mUser.typeName and mUser.freeUser then
			usersWindow[ position ] = { window=guienv:AddWindow( "", 0, 0, 0, 0, -1, layoutWindows ), user=mUser }
		    position = position + 1
		end
		
		if position > 7 then break end
	end
	
	for i=1, #usersWindow do
		base.userInfo.Update( usersWindow[ i ].user, usersWindow[ i ].window )
		button.Stretch( "30e", "30e", "0e", "0e", "button_ok", usersWindow[ i ].window, -1, "",	function () _UpEmployer( usersWindow[ i ].user ) end )
	end
end

function CloseEmployersWindow()
	windowUpEmployer:Remove()
	windowUpEmployer = nil
end

local function _CloseUpWindow()
	windowUpEmployer:Remove()
	windowUpEmployer = nil
end

local function _ChangeUserType( name )
	modeUserView = mode[ name ] 
	base.univer.ShowAvaibleEmployers()
end

function ShowEmployersWindow()
	windowUpEmployer = window.fsWindow( "media/textures/stuffUpWindowBg.png", _CloseUpWindow )

	layoutWindows = guienv:AddLayout( "5%", "15%", "95%", "95%", 2, -1, windowUpEmployer )
	
	local layout = guienv:AddLayout( 20, 20, "20e", "70+", 4, -1, windowUpEmployer ) 	
	button.LayoutButton( "", layout, -1, base.STR_CODERS, function () _ChangeUserType( base.STR_CODERS ) end )
	button.LayoutButton( "", layout, -1, base.STR_DESIGNERS, function () _ChangeUserType( base.STR_DESIGNERS ) end )
	button.LayoutButton( "", layout, -1, base.STR_COMPOSERS, function () _ChangeUserType( base.STR_COMPOSERS ) end )
	button.LayoutButton( "", layout, -1, base.STR_TESTERS, function () _ChangeUserType( base.STR_TESTERS ) end )
	
	base.univer.ShowAvaibleEmployers()
end


