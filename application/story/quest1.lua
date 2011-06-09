local base = _G

IncludeScript( "windowTalk" )

module( "quest1" )

local questenv = base.questenv
local window = base.window
local applic = base.applic
local windowTalk = base.windowTalk
local guienv = base.guienv
local _mainWindow = nil
local _this = nil

local function _Exit()
	_this.obsolete = true
	windowTalk.Hide()
	
	--восстановим время игры
	applic.speed = 500
end

local function _AddAnotherQuest()
	windowTalk.Dialog( "Совсем забыл. Мой знакомый попросил сделать небольшой проект для него, как будет время загляни к нему"
					   .." чтобы обсудить детали, оплата по факту как говорится. И последнее, если будут финансовые трудности"
					   .." звони, не стесняйся, обсудим что можно сделать. Желаю удачи.",
					   { "Вернуться в город", _Exit } )
					   
	questenv:SetActiveQuest( "quest_get_money" )
	questenv:SetActiveQuest( "quest_first_outsource" )
end

local function _AddOldFriendQuest()
	questenv:SetActiveQuest( "quest_old_friend" )
	_AddAnotherQuest()
end


local function _TalkOldFriend()	
	windowTalk.Dialog( "У нас в городе есть художник, он станет отличным дополнением команды. Где-то у меня его адрес был..."
					   .." Вот, это в старом районе. Надеюсь он еще не получил предложения от конкурентов. Что ты об этом думаешь?",
					   { "Я встречусь с ним", _AddOldFriendQuest,	
					     "На рынке труда достаточно профессионалов", _AddAnotherQuest } )			   					   
end

local function _Scenario()
	windowTalk.Dialog( "Решение достойное. Для начала набери людей, потом приступай к созданию моей первой "..
					   "игры. Хотелось бы увидеть результат уже месяца через три.\n\tЯ уже перевел деньги на счет "..
					   "фирмы, на первое время. Желаю удачи...",
					   { "Дальше", _TalkOldFriend } )						
end

local function _FreeGame()
	windowTalk.Dialog( "Решение достойное. Для начала набери людей, потом приступай к созданию моей первой "..
					   "игры. Хотелось бы увидеть результат уже месяца через три.\n\tЯ уже перевел деньги на счет "..
					   "фирмы, на первое время. Еще один момент...",
					   { "Вернуться в город", _Exit }  )	
end

function Start()
	base.LogScript( "Start quest1" )
	--остнановим игру на время квеста
	applic.speed = 1000
	
	_mainWindow = windowTalk.Show( "media/story/uncleSven.png", "Дядя Альберт", "10%", "10%" )
	windowTalk.Dialog( "Как твоя учеба в университете??? Надеюсь, что достойно... Я обещал брату"
					   .." помочь тебе с работой, моей студии по производству видеоигр"
					   .." нужны новые идеи и люди. Рискну поставить тебя на должность тех.директора."
					   .." Если покажешь достойные результаты, оформим тебя на постоянной основе. "
					   .." Если облажаешься - пеняй на себя, вернешься к учебе. Ну что, по рукам???", 
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