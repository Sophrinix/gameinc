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
	windowTalk.Dialog( "Итак все формальности улажены, пора приступать - я уже перевел деньги на первое время. "..
					   "Хотелось бы увидеть результат уже месяца через три."..
					   "Для начала набери людей, я слышал в городе есть художник, "..
					   "он станет отличным дополнением команды. Где-то у меня его адрес был..."..
					   "Вот, это в старом районе. Надеюсь он еще не получил предложения от конкурентов. Что ты об этом думаешь?",
					   { "Я встречусь с ним", _AddTalentArtistQuest,	
					     "На рынке труда достаточно профессионалов", _AddAnotherQuest } )
end

local function _Prof( index )
	if index == 0 then
		
	elseif index == 1 then
	
	elseif index == 2 then
	
	elseif index == 3 then
	
	end

	_Scenario()
end

local function _Difq( levelDificult )
    base.applic.difficult = levelDificult

	windowTalk.Dialog( "С этим закончили, определимся теперь с профессией, что изучал?", 
					   { "Программирование (повышены параметры разработки)", function() _Prof( 0 ) end,
					     "Дизайн (повышены параметры рисования)", function() _Prof( 1 ) end,
					     "Управление предприятием (выше параметры управления)", function() _Prof( 2 ) end,
					     "С какой стороны подходить к орудию (удача вам улыбается чаще)", function() _Prof( 3 ) end } )		
end

local function _DifficultQ()
	windowTalk.Dialog( "Для оформления на должность нужно соблюсти некоторые формальности"
					   .."Мне надо знать с каким димломом ты закончил обучение", 
					   { "Всегда отличником был (нет поблажек в игре)", function() _Difq( 1 ) end,
					     "Закончил без троек (более мягкие условия игры, больше денег)", function() _Difq( 2 ) end,
					     "С серединочки на половиночку (игры создаются быстрее, мало плохих событий)", function() _Difq( 3 ) end,
					     "Кто сказал что я закончил, вранье, армия - вот школа жизни", function() _Difq( 4 ) end } )	
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
					   { "Согласен.(Включить квесты)", _DifficultQ, 
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