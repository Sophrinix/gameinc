local base = _G

IncludeScript( "windowTalk" )

module( "quest_talent_artist" )

local questenv = base.questenv
local window = base.window
local applic = base.applic
local windowTalk = base.windowTalk
local guienv = base.guienv
local _mainWindow = nil
local _this = nil
local _gameSpeed = nil

local function _Exit()
	_this.obsolete = true
	windowTalk.Hide()
	
	--восстановим время игры
	applic.speed = _gameSpeed
end

function Start()
	base.LogScript( "Start quest_talent_artist" )
	--остнановим игру на время квеста
	_gameSpeed = applic.speed
	applic.speed = 1000
	
	_mainWindow = windowTalk.Show( "media/story/AntonKistochkinHouse.png", "Антон Кисточкин", "10%", "10%" )
	windowTalk.Dialog( "",
							{ } )
end

local function _Register()
	_this = questenv:GetQuest( "quest_talent_artist" )
	_this.x = 50
	_this.y = 50
	_this.name = "Талантливый художник"
	_this.start = Start
end

_Register()