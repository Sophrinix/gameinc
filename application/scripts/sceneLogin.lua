
NrpInitializeLoginScene()

IncludeScript( "LoginFunctions" )
IncludeScript( "button" )

--создание названий полей ввода

local label = guienv:AddLabel( "Профиль:" .. applic.profile, scrWidth / 2 - 150, "170e", "150+", "150e", -1, guienv.root )
label.color = NrpARGB( 0xff, 0xc0, 0xc0, 0xc0 )
								
--создание полей ввода				
local btnSelectProfile = guienv:AddButton( "80%", "100e", "0e",  "0e", guienv.root, -1, "Выбрать")
--btnSelectProfile.action = slogin_SelectProfile

--создание кнопоку входа 
button.Stretch( "10.9%", "57.4%", "67.5%", "98%", "btnNewGame", guienv.root, -1, "", LoginFunctions.NewGame )
button.Stretch( "4.6%", "0", "41%", "55%", "btnContinueGame", guienv.root, -1, "",	LoginFunctions.Continue )
button.Stretch( "51.3%", "51%", "74.3%", "69%", "btnOptionsGame", guienv.root, -1, "", LoginFunctions.ShowOptions )
button.Stretch( "29.5%", "6%", "96%", "51.6%", "btnExitGame", guienv.root, -1, "", LoginFunctions.Quit )

browser:Show()
browser:Navigate( "media/html/intro.htm" )

--local img = guienv:AddImage( 0, 0, 200, 200, guienv.root, -1, "" );
--img.scale = true
--local txs = driver:CreateBlur( "media/02.bmp" )
--img.texture = txs.path