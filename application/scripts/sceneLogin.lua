
NrpInitializeLoginScene()

IncludeScript( "LoginFunctions" )

--создание названий полей ввода

local label = guienv:AddLabel( "Профиль:" .. applic.profile, scrWidth / 2 - 150, "170e", "150+", "150e", -1, guienv.root )
label.color = toColor( 0xff, 0xc0, 0xc0, 0xc0 )
								
--создание полей ввода				
local btnSelectProfile = guienv:AddButton( "55%", "170e", "80+",  "150e", guienv.root, -1, "Выбрать")
--btnSelectProfile.action = slogin_SelectProfile

--создание кнопки входа 
local layout = guienv:AddLayout( "12%", "120e", "88%", "60+", 4, -1, guienv.root )
guienv:AddButton( "Новая игра", layout, LoginFunctions.NewGame )
guienv:AddButton( "Продолжить", layout, LoginFunctions.Continue )
guienv:AddButton( "Настройки", layout, LoginFunctions.ShowOptions )
guienv:AddButton( "Выход", layout, LoginFunctions.Quit )

browser:Show()
browser:Navigate( "media/html/intro.htm" )

--guienv:AddLigthing( btnNewGame:Self(), btnExitGame:Self(), "media/textures/larrow.png", 10 )
--