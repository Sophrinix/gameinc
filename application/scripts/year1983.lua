local base = _G

IncludeScript( "magazine" )

module( "year1983" )

local events = {}
local paper = base.magazine
local rbank = base.applic.bank
local applic = base.applic

events[ 102 ] = function()
	local header = "Новые игрушки военных"
	local text = "ARPANET меняет основной протокол с NCP на TCP/IP."
	
	paper.Show( header, text, "font_10" )
end

events[ 107 ] = function()
	local header = "Без бензина"
	local text = "Закончился пробег солнечного автомобиля по Австралии"
	
	paper.Show( header, text, "font_10" )
end

events[ 113 ] = function()
	local header = "Дипломатия победила"
	local text = "Саудовская Аравия восстанавливает дипломатические отношения с Ливией"
	
	paper.Show( header, text, "font_10" )
end

events[ 119 ] = function()
	local header = "Микрокомпьютер"
	local text = "Компания Apple выпустила персональный компьютер Apple Lisa. "
	text = text .. "В новинке используется новейший манипулятор ввода и графический интерфейс пользователя"
	
	paper.Show( header, text, "font_10" )
	rbank:ChangePieCost( "apple_company", "2%+" )
end

events[ 120 ] = function()
	local header = "Получил по заслугам"
	local text = "B Боливии арестован высокопоставленный нацистский военный преступник Клаус Барби"
	
	paper.Show( header, text, "font_10" )
end

events[ 122 ] = function()
	local text = "Бьорн Борг ушёл из большого тенниса после 5 выигранных подряд Уимблдонских турниров."
	--повышение интереса к спортивным играм
end

events[ 126 ] = function()
	local header = "1-2-3-Старт"
	local text =  "Lotus выпускает электронную таблицу Lotus 1-2-3"
	
	paper.Show( header, text, "font_10" )
	rbank:ChangePieCost( "lotus_company", "3%+" )
end

events[ 127 ] = function()
	local header = "Best of the IT 1982"
	local text = "Названы лучшие проекты прошлого года в сфере IT. \"InfoWorld\" выбирает Multiplan производства Microsoft программным продуктом года, а IBM PC - компьютером года"
	
	paper.Show( header, text, "font_10" )
	rbank:ChangePieCost( "microsoft_company", "4%+" )
	rbank:ChangePieCost( "ibm_company", "3%+" )
end

events[ 126 ] = function()
	local text = "для IBM PC совместимых компьютеров выпущена программа Lotus 1-2-3."
	--увеличение продаж ПК
end

events[ 127 ] = function ()
	local header = "Европа без атома"
	local text = "В Женеве возобновляются американо-советские переговоры. Обсуждается предложение СССР о создании безъядерной зоны в Центральной Европе."
	
	paper.Show( header, text )
	applic:GrowInterest( "genre_war", 0.1 )
end 

events[ 212 ] = function()
	local header = "Пожар в Турине"
	local text = "Во время пожара в кинотеатре в Турине погибло 64 человека"
	
	paper.Show( header, text )
end

events[ 213 ] = function()
	local header = "1983 - год Библии"
	local text = "Президент США Рональд Рейган объявил 1983 «Годом Библии»"
	
	paper.Show( header, text )
end

events[ 217 ] = function()
	local header = "Взгляд в будущее"
	local text = "Утверждён стандарт языка ADA ANSI/MIL-STD-1815-A-1983. МО США сделает наименование «Ada» зарегистрированной торговой маркой,"
	text = text .. "и запретит выпускать трансляторы языка, не прошедшие официальную процедуру тестирования на соответствие стандартам"
	
	paper.Show( header, text, "font_10" )
end

events[ 218 ] = function()
	local header = "Резня Ва-Ми" 
	local text = "Во время попытки ограбления в Сиэтле( США ) было убито 13 человек."
	
	paper.Show( header, text )
end

events[ 224 ] = function ()
--1983.02.24	
	local text = "Западные банки дают гарантии Мексике на предоставление займа в размере 5 миллиардов долларов."
	text = text .. "Кроме того, этой стране предоставляется краткосрочный заем в размере 433 миллионов долларов."
	--рост акций американских компаний на 2%
end

events[ 301 ] = function()
	local header = "Испания"
	local text = "Балеарские острова и Мадрид стали автономными коммунами Испании"
	
	paper.Show( header, text )
end

events[ 307 ] = function()
	local header = "Империя зла"
	local text = "Президент США Рональд Рейган назвал СССР «империей зла»"
	
	paper.Show( header, text )
end

events[ 308 ] = function ()
	local header = "Новый формат - Новые возможности"
	local text = "Представлено второе поколение IBM PC на базе шестнадцатибитного процессора Intel 8088. "
	text = text .. "MFM-жёсткий диск с интерфейсом ST-412, объёмом 10 Мбайт, ОЗУ ёмкостью 128. К IBM PC/XT можно "
	text = text .. "добавить второй дисковод 5\" жёсткий диск. Клавиатура имеет 84 клавиши."
	
	paper.Show( header, text, "font_8" )
	rbank:ChangePieCost( "ibm_company", "2%+" )
end

events[ 309 ] = function()
	local header = "Возвращение джедая"
	local text = "Фильм повествует о событиях спустя приблизительно один год после эпизода V. "
	text = text .. "Люк Скайуокер и члены Союза повстанцев направляются на Татуин, чтобы спасти их друга Хана Соло из плена властителя контрабандистов "
	text = text .. "Джаббы Хатта. Империя готовится сокрушить Восстание более мощной Звездой Смерти, "
	text = text .. "в то время как флот мятежников предпринимает массивную атаку на космическую станцию. "
	text = text .. "Люк Скайуокер противостоит своему отцу, Дарту Вейдеру, в поединке перед злым Императором Палпатином."
	
	paper.Show( header, text, "font_8" )
	applic:GrowInterest( "genre_war", 0.1 )
	applic:GrowInterest( "genre_space", 0.1 )
end

events[ 314 ] = function ()
--1983.03.14	
	local text = "Организация стран-экспортеров нефти (ОПЕК) впервые соглашается уменьшить цены на сырую нефть. Цены на высококачественную "
	text = text .. "нефть из Саудовской Аравии резко падают - с 34 до 19 долларов за баррель."
	--
end

events[ 322 ] = function()
	local header = "Астрон"
	local text = "Запущен «Астрон», советский космический ультрафиолетовый телескоп"
	
	paper.Show( header, text )
end

events[ 323 ] = function ()
--1983.03.23	
	local text = "Президент США Рейган провозглашает начало работ по созданию американской системы противоракетной обороны в рамках программы \"Звездные войны\"," 
	text = text .. "в которой предполагается использование искусственных спутников для обнаружения и последующего уничтожения ракет противника."
	--рост интереса в жанре стратегий и шутеров
end

events[ 407 ] = function()
	local header = "Американцы в открытом космосе"
	local text = "Полёт космического челнока Челленджер STS-6. Астронавты Стори Мюсгрейв и Дональд Петерсон, "
	text = text .. "впервые за программу «Спейс Шаттл» совершили выход в открытый космос (продолжительностью 4 часа 10 минут)"
	
	paper.Show( header, text )
end

events[ 415 ] = function()
	local header = "Диснейленд в Токио"
	local text = "Открыт первый Диснейленд за пределами США в Токио"
	
	paper.Show( header, text )
end

events[ 501 ] = function()
	local header = "Новый наставник Apple"
	local text = "Джон Скалли становится у кормила Apple Computer, заменив Марка Марккулу"
	
	paper.Show( header, text )
end

events[ 525 ] = function()
	local header = "Новости технологий"
	local text = "США объявляют о согласии на продажу Китаю высокотехнологичного оборудования"
	
	paper.Show( header, text )
end

events[ 526 ] = function() 
	local header = "Цунами в Японии"
	local text = "Мощное землетрясение и цунами на севере Хонсю(Япония), погибло 104 человека, ранено 163"
end

events[ 601 ] = function()
	local header = "Милионник"
	local text = "Cо сборочной линии Apple сходит миллионный микрокомпьютер"
	
	paper.Show( header, text )
end

events[ 605 ] = function()
	local header = "Пароход протаранио мост"
	local text = "В городе Ульяновск пароход «Александр Суворов» врезается в пролёт моста, по которому шёл товарный поезд."
	text = text .. "Этот пролёт не был предназначен для судоходства (не имел достаточной высоты для прохода под ним судов) и третью палубу теплохода полностью снесло."
	text = text .. "Погибло, по разным данным, от 176 до 600 человек"
	
	paper.Show( header, text )
end

events[ 613 ] = function()
	local text = "Звездная работа"
	local header = "Пионер-10 стал первым искусственным объектом покинувшим пределы солнечной системы"
	
	paper.Show( header, text )
end

events[ 618 ] = function()
	local header = "Первая американка в космосе"
	local text = "Челленджер STS-7: Салли Райд стала первой американкой совершившей космический полёт"	
	
	paper.Show( header, text )
end

events[ 617 ] = function()
--1983.06.17	
	local text = "В Великобритании годовой уровень инфляции падает до 3,7 процента - это самый низкий уровень за последние 15 лет."
	--рост котировок акций английских компаний
end

events[ 701 ] = function()
	local header = "Старт космического эксперимента"
	local text = "Запущен советский спутник «Прогноз-9»: начало эксперимента РЕЛИКТ-1 по изучению реликтового излучения"
	
	paper.Show( header, text )
end

events[ 706 ] = function ()
--1983.07.06	
	local text = "Британское правительство публикует Белую книгу по вопросам обороны \"Доклад об оценке обороноспособности\", "
	text = text .. " в которой подтверждает свое намерение разместить крылатые ракеты на военных базах в Гринэм-Коммоне и Молсуорте"
end

events[ 715 ] = function()
	local header = "Новая консоль"
	local text = "В Японии начались продажи игровой приставки Nintendo Entertainment System"
	
	paper.Show( header, text )
end

events[ 716 ] = function()
	local header = "Авиакатастрофа на Силли"
	local text = "На островах Силли потерпел катастрофу вертолёт Sikorsky S-61, погибло 20 человек"
	
	paper.Show( header, text )
end

events[ 718 ] = function()
	local header = "Женщины в космосе"
	local text = "Полёт второй в мире женщины-космонавта Светланы Савицкой на КК «Салют-7» в составе экипажа из 5 человек"
	
	paper.Show( header, text )
end

events[ 721 ] = function()
	local header = "-90 по цельсию"
	local text = "Зарегистрирована самая низкая температура на Земле за всю историю метеорологических наблюдений. "
	text = " На советской антарктической станции «Восток» термометр на метеоплощадке показал ?89,2 C"
	
	paper.Show( header, show )
end

events[ 722 ] = function()
	local header = "Вокруг света на вертолете"
	local text = "Австралиец Дик Смит завершил своё одиночное кругосветное путешествие на вертолёте"
	
	paper.Show( header, text )
end

events[ 723 ] = function()
	local header = "Планёр Гимли"
	local text = "Из-за отказа двигателей пассажирский самолёт приземлился на военной базе Гимли, штат Манитоба, при помощи планирования"
	
	paper.Show( header, text )
end

events[ 724 ] = function()
	local header = "Оползень в Японии"
	local text = "Сильные дожди и оползень привели к 117 человеческим жертвам в префектуре Симанэ(Япония)"
	
	paper.Show( header, text )
end

events[ 820 ] = function()
	local header = "Эмбарго для комми"
	local text = "Президент США Рейган вводит запрет на поставки в СССР оборудования для строительства трубопроводов"
	
	paper.Show( header, text )
end

events[ 822 ] = function()
	local header = "Новый альбом"
	local text = "Выход третьего студийного альбома группы Depeche Mode под названием Construction Time Again"
	
	paper.Show( header, text )
end 

events[ 830 ] = function()
	local header = "Афроамериканец в космосе"
	local text = "Челленджер STS-8: впервые в космос полетел астронавт афроамериканского происхождения Гайон Блюфорд"
	
	paper.Show( header, text )
end

events[ 901 ] = function()
--1983.09.01	
	local text = "В результате катастрофы южнокорейского пассажирского авиалайнера 'Боинг-747', сбитого советским "
	text = text .. " истребителем в момент нарушения советского воздушного пространства в районе острова Сахалин, погибают 269 человек"
end

function FindEvent( month, day )
    base.LogScript( "find for event "..month * 100 + day )
	if events[ month * 100 + day ] ~= nil then
		base.DebugFunctionCall( events[ month * 100 + day ] )
	end
end
