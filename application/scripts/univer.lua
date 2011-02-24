local base = _G

module( "univer" )

btnDesk = nil

local guienv = base.guienv

local scrWidth = base.scrWidth
local scrHeight = base.scrHeight
local button = base.button
local tutorial = base.tutorial
local applic = base.applic
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

function FadeEnterAction()
	univerWindow.visible = true
	guienv:FadeAction( base.FADE_TIME, true, true )
end

function FadeExitAction()
	univerWindow:Remove()
	univerWindow = nil
	guienv:FadeAction( base.FADE_TIME, true, true )
end

function Hide()
	guienv:FadeAction( base.FADE_TIME, false, false )			
	guienv:AddTimer( base.AFADE_TIME, FadeExitAction )	
end


function Show()
	company = applic.playerCompany
	if univerWindow then
		univerWindow.visible = true
	else
		univerWindow = window.fsWindow( "media/maps/univer_dvor.png", Hide )
	end	
	
	tutorial.Update( tutorial.STEP_OVERVIEW_UNIVER )

	--stuff plate	
	btnDesk = button.EqualeTexture( 122, 320, "stuffPlate", univerWindow, -1, "", "./univer.ShowEmployersWindow()" )
	
	--outsourcing
	button.EqualeTexture( 612, 300, "outsorcing", univerWindow, -1, "", "./outsourcing.Show()" )
	
	guienv:FadeAction( base.FADE_TIME, false, false )			
	guienv:AddTimer( base.AFADE_TIME, "univer.FadeEnterAction()" )
end

local function AddProgressBar( windowe, x1, y1, x2, y2, id, pos, textr )
	local prg = guienv:AddProgressBar( windowe, x1, y1, x2, y2, id )
	prg:SetPosition( pos )						   
	prg:SetImage( "media/textures/stars01.tga" )
	prg:SetFillImage( "media/textures/stars06.tga" )
	local label = guienv:AddLabel( textr, x1, y1, x2, y2, -1, windowe )
	label:SetTextAlignment( base.EGUIA_CENTER, base.EGUIA_CENTER )
end

local function _ShowUserInfoWindow( x, y, width, height, user )
	local windowg = guienv:AddWindow( "", x, y, x + width, y + height, -1, windowUpEmployer )
	windowg.closeButton.visible = false
	local image = nil
	
	windowg.text = user.name
	windowg.draggable = false
	
	local xOffset = width / 3	
	image = guienv:AddImage( 30, 30, 30 + 107, 30 + 141, windowg, -1, "" )
	image.texture = user.texture
	image.scaleImage = true
	image.useAlphaChannel = true

	AddProgressBar( windowg, xOffset, 30, width - 5, 30 + 15, -1, 
					user:GetParam( "knowledgeLevel" ), base.STR_EXPERIENCE )
	AddProgressBar( windowg, xOffset, 55, width - 5, 55 + 15, -1, 
					user:GetParam("codeQuality"), base.STR_QUALITY )
	AddProgressBar( windowg, xOffset, 80, width - 5, 80 + 15, -1,
					user:GetParam("codeSpeed"), base.STR_SPEED )
	AddProgressBar( windowg, xOffset, 105, width - 5, 105 + 15, -1,
	                user:GetParam("stability"), base.STR_STAMINA )
					   
	local money = user:GetParam( "wantMoney" )
    guienv:AddLabel( "Зарплата: "..money.."$", xOffset, 110, width, 110 + 20, -1, windowg )
	
	local btn = guienv:AddButton( width / 2 - 50, height - 30, width / 2 + 50, height - 10, windowg, -1, "Нанять" )
	btn.action = UpEmployer
	
	return windowg			   
end

local function _ShowAvaibleEmployers()
	local maxuser = applic.userNumber
	local hTemp = ( scrHeight - 150 ) / 3
	
	local xoffset = 10
	local yoffset = 100
	
	local cnt = 0
	base.LogScript( "ShowAvaibleEmployers:appusers" .. maxuser )
	
	for i=1, #usersWindow do
		guienv:AddToDeletionQueue( usersWindow[ i ] )
		usersWindow[ i ] = nil
	end
	
	local position=1
	for i=1, maxuser do
		local user = applic:GetUser( i-1 )

		if modeUserView == user.getTypeName and user.freeUser then
			if cnt < 3 then
				usersWindow[ position ] = ShowUserInfoWindow( xoffset, yoffset + cnt * hTemp, scrWidth / 2 - 30,  hTemp, user ) 
			else
				usersWindow[ position ] = ShowUserInfoWindow( xoffset + scrWidth / 2, yoffset + (cnt - 3) * hTemp, scrWidth / 2 - 30,  hTemp, user ) 
			end
		    cnt = cnt + 1
		    position = position + 1
		end
		
		if cnt > 6 then 
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
	if windowUpEmployer == nil then
		windowUpEmployer = window.fsWindow( "media/textures/stuffUpWindowBg.png", _CloseUpWindow )
	else
		windowUpEmployer.visible = true
	end
	
	local layout = guienv:AddLayout( 20, 20, "20e", "70+", -1, windowUpEmployer ) 	
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
