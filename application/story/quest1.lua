local base = _G

IncludeScript( "windowTalk" )

module( "quest1" )

local questenv = base.questenv
local window = base.window
local applic = base.applic
local button = base.button
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
	
	_this:SetResult( "test", "test" )
end

local function _AddQuestFirstOutsource()
	questenv:AddActiveQuest( "quest_first_outsource" )
	_Exit()
end

local function _AddAnotherQuest()
	windowTalk.Dialog( "Есть еще задача, которая требует твоего участия"
					   .." Мой знакомый попросил сделать небольшой проект для него, как будет время загляни к нему"
					   .." чтобы обсудить детали, оплата по факту как говорится. И последнее, если будут финансовые трудности"
					   .." звони, не стесняйся, обсудим что можно сделать. Желаю удачи.",
					   { "И так дел хватает, до встречи.", _Exit,
					     "Посмотрим что я смогу сделать для него", _AddQuestFirstOutsource } )
					   					   
	questenv:AddActiveQuest( "quest_get_money" )
end

local function _AddTalentArtistQuest()
	questenv:AddActiveQuest( "quest_talent_artist" )
	_AddAnotherQuest()
end

local function _Scenario()
	windowTalk.Dialog( "Надеюсь ты выбрал правильное решение - я уже перевел деньги на первое время. "..
					   "Хотелось бы увидеть результат уже месяца через три."..
					   "Для начала набери людей, я слышал в городе есть художник, "..
					   "он станет отличным дополнением команды. Где-то у меня его адрес был..."..
					   "Вот, это в старом районе. Надеюсь он еще не получил предложения от конкурентов. Что ты об этом думаешь?",
					   { "Я встречусь с ним", _AddTalentArtistQuest,	
					     "На рынке труда достаточно профессионалов", _AddAnotherQuest } )
end

local function _FreeGame()
	windowTalk.Dialog( "Желаю твоей фирме хороших прибылей.",
					   { "Вернуться в город", _Exit }  )	
end

function Start()
	base.LogScript( "Start quest1" )
	--остнановим игру на время квеста
	_gameSpeed = applic.speed
	applic.speed = 1000
	
	_mainWindow = windowTalk.Show( "media/story/uncleSven.png", "Дядя Альберт", "10%", "10%" )
	windowTalk.Dialog( "Как твоя учеба в университете??? Надеюсь, что достойно... Я обещал брату"
					   .." помочь тебе с работой, моей студии видеоигр"
					   .." нужны хорошие идеи и новые люди. Рискну поставить тебя на должность тех.директора."
					   .." Если покажешь достойные результаты - оформим тебя на постоянной основе,  "
					   .." облажаешься - пеняй на себя, поедешь и дальше грызть гранит науки. Ну что, по рукам???", 
					   { "Согласен.(Включить квесты)", _Scenario, 
						 "Мы пойдем своим путем.(Свободная игра)", _FreeGame } )
end

local function _Register()
	_this = questenv:GetQuest( "quest1" )
	_this.x = 50
	_this.y = 50
	_this.name = "Возвращение домой"
	_this.start = Start
end

_Register()