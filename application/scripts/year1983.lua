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

events[ 126 ] = function()
	local header = "1-2-3-Старт"
	local text =  "Lotus выпускает электронную таблицу Lotus 1-2-3"
	
	paper.Show( header, text, "font_10" )
	rbank:ChangePieCost( "lotus_company", "3%+" )
end

events[ 127 ] = function()
	local header = "Best of the Best"
	local text = "Названы лучшие проекты прошлого года в сфере IT. \"InfoWorld\" выбирает Multiplan производства Microsoft программным продуктом года, а IBM PC - компьютером года"
	
	paper.Show( header, text, "font_10" )
	rbank:ChangePieCost( "microsoft_company", "4%+" )
	rbank:ChangePieCost( "ibm_company", "3%+" )
end

events[ 122 ] = function()
	local text = "Бьорн Борг ушёл из большого тенниса после 5 выигранных подряд Уимблдонских турниров."
	--повышение интереса к спортивным играм
end

events[ 126 ] = function()
	local text = "для IBM PC совместимых компьютеров выпущена программа Lotus 1-2-3."
	--увеличение продаж ПК
end

events[ 127 ] = function ()
--1983.01.27	
	local text = "В Женеве возобновляются американо-советские переговоры. Обсуждается предложение СССР о создании безъядерной зоны в Центральной Европе."
	applic:GrowInterest( "genre_war", 0.1 )
end 

events[ 217 ] = function()
	local header = "Взгляд в будущее"
	local text = "Утверждён стандарт языка ADA ANSI/MIL-STD-1815-A-1983. МО США сделает наименование «Ada» зарегистрированной торговой маркой,"
	text = text .. "и запретит выпускать трансляторы языка, не прошедшие официальную процедуру тестирования на соответствие стандартам"
	
	paper.Show( header, text, "font_10" )
end

events[ 224 ] = function ()
--1983.02.24	
	local text = "Западные банки дают гарантии Мексике на предоставление займа в размере 5 миллиардов долларов. Кроме того, этой стране предоставляется краткосрочный заем в размере 433 миллионов долларов."
	--рост акций американских компаний на 2%
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
	local text = "Организация стран-экспортеров нефти (ОПЕК) впервые соглашается уменьшить цены на сырую нефть. Цены на высококачественную нефть из Саудовской Аравии резко падают - с 34 до 19 долларов за баррель."
	--
end

events[ 323 ] = function ()
--1983.03.23	
	local text = "Президент США Рейган провозглашает начало работ по созданию американской системы противоракетной обороны в рамках программы \"Звездные войны\", в которой предполагается использование искусственных спутников для обнаружения и последующего уничтожения ракет противника."
	--рост интереса в жанре стратегий и шутеров
end

events[ 501 ] = function()
	local header = "Новый наставник Apple"
	local text = "Джон Скалли становится у кормила Apple Computer, заменив Марка Марккулу"
	
	paper.Show( header, text )
end

events[ 601 ] = function()
	local header = "Милионник"
	local text = "Cо сборочной линии Apple сходит миллионный микрокомпьютер"
	
	paper.Show( header, text )
end

events[ 617 ] = function ()
--1983.06.17	
	local text = "В Великобритании годовой уровень инфляции падает до 3,7 процента - это самый низкий уровень за последние 15 лет."
	--рост котировок акций английских компаний
end

events[ 706 ] = function ()
--1983.07.06	
	local text = "Британское правительство публикует Белую книгу по вопросам обороны \"Доклад об оценке обороноспособности\", в которой подтверждает свое намерение разместить крылатые ракеты на военных базах в Гринэм-Коммоне и Молсуорте"
end

events[ 901 ] = function()
--1983.09.01	
	local text = "В результате катастрофы южнокорейского пассажирского авиалайнера 'Боинг-747' (номер рейса рейса KAL-007), сбитого советским истребителем в момент нарушения советского воздушного пространства в районе острова Сахалин, погибают 269 человек"
end

function FindEvent( month, day )
    base.LogScript( "find for event "..month * 100 + day )
	if events[ month * 100 + day ] ~= nil then
		base.DebugFunctionCall( events[ month * 100 + day ] )
	end
end
