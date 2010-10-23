
NrpInitializeLoginScene()

--создание названий полей ввода

local label = guienv:AddLabel( "Профиль:" .. applic:GetCurrentProfile(), scrWidth / 2 - 150, scrHeight - 170,
										  scrWidth / 2 - 150 + 150, scrHeight - 150, -1, nil )
label:SetOverrideColor( 0xff, 0xc0, 0xc0, 0xc0 )
								
--создание полей ввода				
local btnSelectProfile = guienv:AddButton( scrWidth / 2 + 150, scrHeight - 170, scrWidth / 2 + 150 + 60,  scrHeight - 150, 
								 	       guienv:GetRootGUIElement(), -1, "Другой")
btnSelectProfile:SetAction( "slogin_SelectProfile" )

--создание кнопки входа 

local btnNewGame = guienv:AddButton( scrWidth / 2 - 350, scrHeight - 120, scrWidth / 2 - 200, scrHeight - 80, 
									 guienv:GetRootGUIElement(), -1, "Новая игра")
btnNewGame:SetAction( "slogin_CreateNewGame" )


local btnContinueGame = guienv:AddButton( scrWidth / 2 - 200, scrHeight - 120, scrWidth / 2 - 50, scrHeight - 80, 
											 guienv:GetRootGUIElement(), -1, "Продолжить")
btnContinueGame:SetAction( "slogin_ContinueLastGame" )

local btnOptionsGame = guienv:AddButton( scrWidth / 2 + 50, scrHeight - 120, scrWidth / 2 + 200, scrHeight - 80, 
											 guienv:GetRootGUIElement(), -1, "Настройки")
btnOptionsGame:SetAction( "slogin_OptionsGame" )


local btnExitGame = guienv:AddButton( scrWidth / 2 + 200, scrHeight - 120, scrWidth / 2 + 350, scrHeight - 80, 
											 guienv:GetRootGUIElement(), -1, "Выход")
btnExitGame:SetAction( "slogin_CloseApp" )

browser:Show()
browser:Navigate( "media/html/intro.htm" )

IncludeScript( "functions_sceneLogin" )

--guienv:AddLigthing( btnNewGame:Self(), btnExitGame:Self(), "media/textures/larrow.png", 10 )
--