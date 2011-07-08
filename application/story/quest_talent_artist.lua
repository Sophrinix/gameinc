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

local _oldMenPassedCount = 0
local _babkiPassedCount = 0
local _percentApply = 0
local _alhocol = 0

local function _Exit()
	_this.obsolete = true
	windowTalk.Hide()
	
	--восстановим время игры
	applic.speed = _gameSpeed
	questenv:SetResult( "general", "quest_talent_artist", "kistochkin_is_enemy" )
end

local function _Apply()
	questenv:AddActiveQuest( "quest_talent_hack" )
	questenv:SetResult( "general", "quest_talent_artist", "kistochkin_is_friend" )
	windowTalk.Message( "Так бывает что думаешь будто знаешь человека, но стоит посмотреть на него "
						.." в другой ситуации, и оказывается что рядом все это время был ктото другой",
						{ "Вернуться в контору", _Exit } )
end

local function _RandomMonetka()
	local prc = base.math.random(50 + _percentApply - _alhocol )
	if prc > 90 then
		windowTalk.Message( "Монетка ударилась об стену и повисла в паутине, распугав нескольких"
							.."пауков. Отвисшая челюсть собеседника не слишком его красила", 
							{ "Не забудь трудовую принести. ", _Apply } )
	elseif prc > 70 and prc <= 90 then
		windowTalk.Message( "Извини проводить не могу, сам туда дорогу не знаю. Дверь закрой когда "
							.."будешь выходить. Судьба", 
							{ "Ну чтож, бывай", _Exit } )
	elseif prc > 30 and prc <= 70 then
		windowTalk.Message( "Слушай я тут подумал и решил, еще 500$ к зарплате накинуть, устроит?", 
							{ "Нафиг, нафиг", _Exit,
							  "По рукам", _Apply  } )
	elseif prc <= 30 then
		windowTalk.Message( "Должность зама, кожанное кресло и выходной в среду и пятнику, устроит?", 
							{ "Нафиг, нафиг", _Exit,
							  "По рукам", _Apply  } )
	end
end

local function _RandomExit()
	local text = "Вот так сразу - с корабля на бал, да фиг его знает... Нас и здесь неплохо кормят"
				 .."Слушай, а давай монетку кинем"
				 .."\nРешка - иди на х... со своей работой" 
				 .."\nОрел - твоя взяла, да тока я мастер невиданных высот и прошу 2000$ для начала"
				 .."\nНа ребро встанет - пойду твоим замом"
				 .."\nВ воздухе повиснет - буду работать за еду"
	
	windowTalk.Dialog( text,
	                   { "Кидай", _RandomMonetka,
					     "Не люблю авантюристов, может в другой раз", _Exit } )
end

local function _BlackHorse()
	local text = "Да что ты, что ты... эту гадость даже мой кот Васька не пьет"
				 .."Лучше я её на попозжа оставлю, для когонить другого. Здается мне"
				 .."Не с тобой я учился, вроде рожа та, а вот вспомнить не могу"
				 .."Ты чего пришел то? может денег в долг попросить, али трубы горят?"
	
	windowTalk.Dialog( text, { "На работу ко мне пойдешь?", _RandomExit,
							   "Не врали бабки, ты же хроник. Бывай", _Exit } )
end

local function _WhiteHorse()
	_percentApply = _percentApply + 10
	_alhocol = _alhocol + 10
	local text = "Вот и я говорю, что белая лошадь завсегда лучше. Ну давай по стопочке еще..."
	             .."Так зачем ты говоришь пришел???"
	
	windowTalk.Dialog( text, { "Суперские картины будешь рисовать?", _RandomExit,
							   "Да так, повидаться зашел. Спасибо за угощение.", _Exit } )
end

local function _BadQ_2()
	local text = "Нифига то ты не помнишь, не особо мне нравится эта немецкая группа"
	             .."А у меня тут для особых гостей есть угощение заморское, целых два"
	             .."Выбирай Черная или Белая"
	
	windowTalk.Dialog( text, { "Черную", _BlackHorse,
							   "Белую", _WhiteHorse } )
end

local function _GoodQ_1()
	_alhocol = _alhocol + 10
	local text = "Я смотрю ты сноровки не потерял с институтских годов. Опоздавший пьет до дна..."
				 .." Выдох, опачки, а вот и закусончик - на вилочке огурчик"
				 .." Ну вот и разговор завязался, я вот думаю нафига ты сюда приперся?"
				 .." Ладно, между первой и второй, перерывчик небольшой"
	
	windowTalk.Dialog( text, { "Ты где такую водку паленую берешь?", _RandomExit,
							   "Да я повидаться зашел. Бывай", _Exit} )
end

local function _GoodQ_2_M()
	_percentApply = _percentApply + 10
	local text = "Да, хорошее время было - свободное, жаль не повторяется такое никогда"
	             .." А я вот картины рисую, что-то отлично получается, что-то похуже"
	             .." Недавно вон с городской мерии приезжали, заказали плакат на сорок квадратов"
	             .." давай и твой портрет нарисуем, во весь рост и скидку сделаю"
	             
	windowTalk.Dialog( text, { "Может будет постоянно сотрудничать?", _RandomExit,
	                           "Завтра зайду за портретом", _Exit } )
end

local function _BadQ_1()
	local text = "Художник то я, а ты кто? Ба... да это же "..base.applic.profileName
	             .." смотрю приоделся, костюмчик прикупил. А я вот тельник вторую неделю не снимаю."
	             .." на работу еще не устроился. А ты чего не пьешь, или брезгуешь из грязных стаканов"
	             .." употреблять. А помнишь мы на всю катушку музончик включали?"
	             
	windowTalk.Dialog( text, { "Угу, Metallica рулит", _GoodQ_2_M,
	                           "Scorpions forever", _BadQ_2,
	                           "А наливай, вспомним первый курс", _GoodQ_1 } )
end

local function _EnterHouse()
	local text = "Появившись на кухне, особо фурора вы не произвели, пирующие уже достигли кондиции"
	             .." только хозяин дома, одетый в странный наряд из тельняшки, пиджака от костюма тройки"
	             .." и шорты, единственная деталь одежды напомтнающая о худодественных пристрастиях - это"
	             .." шляпа с пером, толи павлина толи еще какого-то страуса. Да еще похож на одногруппника "
	             .." Антона Кисточкина, однако мир тесен. Ну здраствуй, Антон."
	             
    windowTalk.Show( "media/story/talentArtist/AntonKistochkinHouse.png", "Бабки", "10%", "10%" )
	windowTalk.Dialog( text, 
					   { "Здорово, Антоха, это ты тут художник чтоли?", _BadQ_1, 
					     "Здесь старым одногруппникам наливают?", _GoodQ_1,
					     "Ищу талантливого художника для интересной работы", _BadQ_2 } )	
end

local function _GoArtistHome()
	local text = "Подъезд, каких миллионы в стране. Обшарпанные стены,"
	             .." темнеющие от влажности углы, резкий запах привычно бьёт в нос,"
	             .." кошка шмыгнула в подвал. Поднявшись на второй этаж вы оказываетесь"
	             .." перед дверью, пьяный гомон слышится даже на площадке, видно праздник"
	             .." в еще разгаре. "
	             
	windowTalk.Message( text, { "Зайти в квартиру", _EnterHouse, 
								"Вернуться во двор", _GoBabki } )
end

local function _BabkiTalkAboutArtist()
	local text
	if _oldMenPassedCount == 0 then
		text = "Иди отсюда алкаш проклятый, а то щас милицию позовем"
	else
		text = "А ты милок не из милиции случаем? А то совсем надоел супостат проклятый"
		       .." еще художником себя называет, мимо проходит и не поздоровается ни разу, да еще и нагрубить может"
		       .." мы вот его на собрание вызвать хотим, он же подлец третий месяц за квартиру не платит."  
		       .." Алкаш он - это точно... От него только бутылки по подъезду, да все какие-то не наши..."
		       .." На коробках белая лошадь нарисована, наши люди такое не пьют...\n"
		       .." А может ты тоже из этих (хлопает пальцами по горлу)? Точно, и одет неопрятно..." 
		       .." Дружков своих ищешь? Иди отсюда алкаш проклятый"
	end
	
	_babkiPassedCount = _babkiPassedCount + 1
	
	local tableAnser = { "Направиться к деду", _GoOldMan,
	                     "Зайти в подъезд", _GoArtistHome,
					     "Спросить о художнике еще раз"..aboutArtistYet, _BabkiTalkAboutArtist,
					     "Направиться к выходу", _OnEnter }
	
	windowTalk.Show( "media/story/talentArtist/babki.png", "Бабки", "10%", "10%" )
	windowTalk.Dialog( text, tableAnswer )	
end


local function _GoBabki()
    local text, aboutArtistYet = ""
	if _oldMenPassedCount > 0 then
		text = "Старушки не обращают внимания на вас"
		aboutArtistYet = " еще раз"
	else
		text = "Когда вы подходите старушки старушки замолкают, смотрят на вас с интересом, но молчат"
	end
	
	--уже подходили к деду и спрашивали насчет художника
	local tableAnser = { "Направиться к деду", _GoOldMan,
	                     "Зайти в подъезд", _GoArtistHome,
					     "Спросить о художнике"..aboutArtistYet, _BabkiTalkAboutArtist,
					     "Направиться к выходу", _OnEnter }

	windowTalk.Show( "media/story/talentArtist/babki.png", "Бабки", "10%", "10%" )
	windowTalk.Dialog( prefText..text, tableAnswer )	
end

local function _OldMenTalkAboutArtist()
	local prefText = ""
	if _oldMenPassedCount > 0 then
		prefText = "Для особо глухих повторяю."
	end
	
	_oldMenPassedCount = _oldMenPassedCount + 1
	
	local text = "У нас только старики одни остались, не младше тридцатых годов выпуска."
	             .." Из молодых алкаш вот есть - живет на втором этаже. Все какую-то иностранщину слушает"
	             .." мы уже и милицию вызывали, и сами к нему ходили, да все бестолку. Никак его не угомонить,"
	             .." на мои вопросы в чем смысл песен, он отвечал...\n - Дедуля, этож металлика, а не магомаев..."
	             .."\n - Это надо во всю мощь слушать, а вы мне всю радость обламываете"
	
	--уже подходили к деду и спрашивали насчет художника
	local tableAnser = { "Направиться к старушкам", _GoBabki,
					     "Спросить о художнике еще раз", _OldMenTalkAboutArtist,
					     "Направиться к выходу", _OnEnter }

	windowTalk.Show( "media/story/talentArtist/oldMen.png", "Старик", "10%", "10%" )
	windowTalk.Dialog( prefText..text, tableAnswer )	
end

local function _GoOldMan()
	local text = "Дедуля явно недоволен, что вы прервали его медитацию над полуразобранным двигателем"
	
	--уже подходили к деду и спрашивали насчет художника
	local tableAnser = nil
	if _oldMenPassedCount > 0 then		
		tableAnswer { "Направиться к старушкам", _GoBabki,
					  "Спросить о художнике", _OldMenTalkAboutArtist,
					  "Направиться к выходу", _OnEnter }
	else
		tableAnswer { "Направиться к старушкам", _GoBabki,
					  "Спросить о художнике еще раз", _OldMenTalkAboutArtist,
					  "Направиться к выходу", _OnEnter }
	end
	
	windowTalk.Show( "media/story/talentArtist/oldMen.png", "Старик", "10%", "10%" )
	windowTalk.Dialog( text, tableAnswer )	
end

local function _OnEnter()
	windowTalk.Message( "По адресу, данному дядей Альбертом находилось старое здание, уже не один год требовавшее ремонта."
						.."Перед подъездом несут круглосуточную службу вездесущие старушки, посреди двора стоял старенький жигуль, из-за открытого капота "
						.."дедок в трениках и майке-алкашке",
						{ "Идти к деду", _GoOldMan,
						  "Направиться к старушкам", _GoBabki,
						  "Зайти в подъезд", _GoArtistHome,
						  "Вряд ли здесь водятсят приличные художники.(Вернуться в контору)", _Exit } )
end

function Start()
	base.LogScript( "Start quest_talent_artist" )
	--остнановим игру на время квеста
	_gameSpeed = applic.speed
	applic.speed = 1000
	
	_mainWindow = windowTalk.ShowFs( "media/story/talentArtist/AntonKistochkinHouse.png", "Унылый двор", "25%", "25%" )
	_OnEnter()
end

local function _Register()
	_this = questenv:GetQuest( "quest_talent_artist" )
	_this.x = 50
	_this.y = 50
	_this.name = "Талантливый художник"
	_this.start = Start
end

_Register()