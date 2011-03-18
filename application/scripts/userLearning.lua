local base = _G

module( "userLearning" )

local emp = nil
local mainWindow = nil
local guienv = base.guienv
local button = base.button

local cmbxPlace = nil
local cmbxType = nil
local cmbxDuration = nil

local place = 0
local type = 0
local duration = 0
local btnsPlaces = {}
local btnsSkill = {}
local btnsDuration = {}
local params = { "skill_coding", "skill_drawing", "skill_sound", "skill_testing" }

local function _GetMoney()
	local money = base.math.pow( 2, place ) * 2000
	money = money * duration
	return money
end

local function _Apply()
	if emp then
		emp:AddLearning( btnsPlaces[ place ].name .."[".. btnsSkill[ type ].name .."]", 
						 params[ type ], duration * 7, 
						 base.math.random( 0, place * duration )
					   )
					   
	    base.pda.Show( emp.name.." отправлен на обучение" )
	    
	    base.applic.playerCompany:AddBalance( "Обучение "..emp.name, -_GetMoney() )
	end
	
	mainWindow:Remove()
end

local function _CheckLabel()
	lbMoney.text = "$".._GetMoney()
end

local function _SetPlace( sender )
	for i=1, #btnsPlaces do
		btnsPlaces[ i ].pressed = false
	end
	
	sender = base.CLuaButton( sender )
	sender.pressed = true
	place = sender.id
	_CheckLabel()
end

local function _SetSkill( sender )
	for i=1, #btnsSkill do
		btnsSkill[ i ].pressed = false
	end
	
	sender = base.CLuaButton( sender )
	sender.pressed = true
	type = sender.id
	_CheckLabel()
end

local function _SetDuration( sender )
	for i=1, #btnsDuration do
		btnsDuration[ i ].pressed = false
	end
	
	sender = base.CLuaButton( sender )
	sender.pressed = true
	duration = sender.id
	_CheckLabel()
end

function Show()
	emp = base.userManager.currentEmployer

	if emp ~= nil then
		if emp.yetLearning then
			guienv:MessageBox( "Работник уже проходит обучение", false, false, button.CloseParent, nil )
			return
		end
		
		mainWindow = guienv:AddWindow( "learning.png", "25%", "25%", "50%+", "50%+", -1, guienv.root )
		mainWindow.closeButton.visible = false
		
		layout = guienv:AddLayout( "10%", "5%", "90%", "80%", 4, -1, mainWindow ) 

		lbMoney = guienv:AddLabel( "$".._GetMoney(), "5%", "85%", "95%", "95%", -1, mainWindow )
		lbMoney:SetTextAlignment( base.EGUIA_CENTER, base.EGUIA_CENTER )
			
		btnsPlaces[ 1 ] = button.LayoutPushButton( "local_course", layout, 1, "Местные курсы", _SetPlace  )
		btnsPlaces[ 1 ].pressed = true
		place = 1
		btnsPlaces[ 2 ] = button.LayoutPushButton( "institut", layout, 2, "Областной институт", _SetPlace )
		btnsPlaces[ 3 ] = button.LayoutPushButton( "universitet", layout, 3, "Столичный университет", _SetPlace )
		btnsPlaces[ 4 ] = button.LayoutPushButton( "europe_center", layout, 4, "Европейский центр обучения", _SetPlace )

		btnsSkill[ 1 ] = button.LayoutPushButton( "coding_skill", layout, 1, "Программирование", _SetSkill )
		btnsSkill[ 1 ].pressed = true
		type = 1
		btnsSkill[ 2 ] = button.LayoutPushButton( "design_skill", layout, 2, "Дизайн", _SetSkill )
		btnsSkill[ 3 ] = button.LayoutPushButton( "music_skill", layout, 3, "Музыка", _SetSkill )
		btnsSkill[ 4 ] = button.LayoutPushButton( "tester_skill", layout, 4, "Тестирование", _SetSkill )

		btnsDuration[ 1 ] = button.LayoutPushButton( "one_week", layout, 1, "7", _SetDuration )
		btnsDuration[ 1 ].pressed = true
		btnsDuration[ 1 ].font = "font_20"
		duration = 1
		btnsDuration[ 2 ] = button.LayoutPushButton( "one_week", layout, 2, "14", _SetDuration )
		btnsDuration[ 2 ].font = "font_20"
		btnsDuration[ 3 ] = button.LayoutPushButton( "one_week", layout, 3, "21", _SetDuration )
		btnsDuration[ 3 ].font = "font_20"
		btnsDuration[ 4 ] = button.LayoutPushButton( "one_week", layout, 4, "28", _SetDuration )
		btnsDuration[ 4 ].font = "font_20"
		
		button.Stretch( "30e", "30e", "0e", "0e", "button_ok", mainWindow, -1, "",	_Apply )
		button.Stretch( 0, "30e", "30+", "30+", "button_down", mainWindow, -1, "", button.CloseParent )

		_CheckLabel()
	end
end
