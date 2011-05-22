local base = _G

IncludeScript("office")
IncludeScript("laboratory" )
IncludeScript("bank")
IncludeScript("shop")
IncludeScript("plantrd")
IncludeScript("univer")
IncludeScript("reklameManager")

module( "city" )

local guienv = base.guienv
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

function ShowHelp()
	if showHelp then
		base.tutorial.Update( "city/main" )
	end
end	

local function _CreateButtons()
	--Office
	btnOffice = button.EqualeTexture( 730, 43, "office", mainWindow, -1, "", base.office.Show )
	--shop	
	btnShop = button.EqualeTexture( 417, 82, "media", mainWindow, -1, "", base.shop.Show )
	--labor
	btnLab = button.EqualeTexture( 811, 198, "labor", mainWindow, -1, "", base.laboratory.Show )
	--bank
	btnBank = button.EqualeTexture( 323, 389, "bank", mainWindow, -1, "", base.bank.Show )
	--plant	
	btnPlant = button.EqualeTexture( 517, 326, "plant", mainWindow, -1, "", base.plantrd.Show )
	--pizza	
	--btnPizza = button.EqualeTexture( 105, 42, "pizza", mainWindow, -1, "", base.pizza.Show )
	--univer	
	btnUniver = button.EqualeTexture( 0, 287, "univer", mainWindow, -1, "", base.univer.Show )
	--reklame
	btnReklame = button.EqualeTexture( 0, 94, "reklame", mainWindow, -1, "", base.reklameManager.Show )
end
	
function Show()	
	if mainWindow then
		mainWindow.visible = true
	else
		mainWindow = guienv:AddWindow( "media/maps/city_map.png", 0, 0, "0e", "0e", -1, guienv.root )
		mainWindow.draggable = false
		mainWindow.closeButton.visible = false
		mainWindow.visible = false
		
		_CreateButtons()
		base.browser:Hide()
	end	
	
	base.rightPanel.AddYesNo( "Хотите больше узнать о городе?", ShowHelp, button.CloseParent )
	
	guienv:FadeAction( 20, base.FADE_OUT, true )
	guienv:AddTimer( 520, function () 
								guienv:FadeAction( 1000, base.FADE_IN, base.REMOVE_ON_END )
								mainWindow.visible = true
						   end,
				     nil )
end
