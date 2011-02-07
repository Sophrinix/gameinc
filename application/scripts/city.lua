local base = _G

module( "city" )

local guienv = base.guienv
local scrWidth = base.scrWidth
local scrHeight = base.scrHeight
local button = base.button

local mainWindow = nil

btnOffice = nil
btnShop = nil
btnLab = nil
btnBank = nil
btnPlant = nil
btnPizza = nil
btnUniver = nil
btnReklame = nil
showHelp = true

function SetVisible( vis )
	mainWindow:SetVisible( vis )
end

function ShowHelp()
	if showHelp then
		base.tutorial.Update( base.tutorial.STEP_OVERVIEW_CITY )
	end
end	
	
function Show()
	if mainWindow then
		mainWindow:SetVisible( true )
	else
		mainWindow = guienv:AddWindow( "media/maps/city_map.png", 0, 0, scrWidth, scrHeight, -1, guienv:GetRootGUIElement() )
		mainWindow:SetDraggable( false )
		mainWindow:GetCloseButton():SetVisible( false ) 
	end	
	
	ShowHelp()
	
	--Office
	btnOffice = button.EqualeTexture( 730, 43, "office", mainWindow, -1, "", "./office.Show()" )
	--shop	
	btnShop = button.EqualeTexture( 417, 82, "media", mainWindow, -1, "", "./shop.Show()" )
	--labor
	btnLab = button.EqualeTexture( 811, 198, "labor", mainWindow, -1, "", "./laboratory.Show()" )
	--bank
	btnBank = button.EqualeTexture( 323, 389, "bank", mainWindow, -1, "", "./bank.Show()" )
	--plant	
	btnPlant = button.EqualeTexture( 517, 326, "plant", mainWindow, -1, "", "./plant.Show()" )
	--pizza	
	btnPizza = button.EqualeTexture( 105, 42, "pizza", mainWindow, -1, "", "./pizza.Show()" )
	--univer	
	btnUniver = button.EqualeTexture( 0, 287, "univer", mainWindow, -1, "", "./univer.Show()" )
	--reklame
	btnReklame = button.EqualeTexture( 0, 94, "reklame", mainWindow, -1, "", "./reklameManager.Show()" )
end
