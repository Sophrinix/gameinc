local base = _G

module( "testDef" )

local table = base.table
advtech = {} 
videotech = {}
soundtech = {}
engine = {}

defs = {}

defs[ base.PT_PLUGIN ] = engine
defs[ base.PT_VIDEOTECH ] = videotech
defs[ base.PT_SOUNDTECH ] = soundtech
defs[ base.PT_ADVTECH ] = advtech

table.insert( advtech, "Управление катастрофическое." )
table.insert( advtech, "Так просто невозможно играть." )
table.insert( advtech, "Невозможное управление." )
table.insert( advtech, "Слишком запутано." )
table.insert( advtech, "Управление неважное." )
table.insert( advtech, "Его нужно улучшить." )
table.insert( advtech, "Управление все еще немного сложное." )
table.insert( advtech, "Разве нельзя его немного улучшить?" )
table.insert( advtech, "Управление ниже среднего." )
table.insert( advtech, "Не повредит еще несколько изменений." )
table.insert( advtech, "Довольно разумное управление. Еще несколько улучшений, и оно будет идеальным." )
table.insert( advtech, "Довольно хорошее управление. Еще несколькоизменений, и придраться будет не к чему." )
table.insert( advtech, "Управление очень хорошее. Здесь трудно к чему-либо придраться." )
table.insert( advtech, "Отличное управление. Изменений не требуется." )

table.insert( engine, "О каком движке вы говорите?." )
table.insert( engine, "Хм... Надо почитать документацию." )
table.insert( engine, "Вчера изучали примеры использования." )
table.insert( engine, "Завтра попытаемся запустить тесты." )
table.insert( engine, "Пишем тестовый уровень." )
table.insert( engine, "Тестируем тестовый уровень." )
table.insert( engine, "Разбираемся с функционалом движка." )
table.insert( engine, "Пробуем дополнительные возможности." )
table.insert( engine, "Иногда нам удается запустить первый уровень." )
table.insert( engine, "Вчера всем отделом тестировали первый уровень." )
table.insert( engine, "Консультации с разработчиками движка приносят свои плоды." )
table.insert( engine, "Мы работаем с движком без чтения документации." )
table.insert( engine, "Запускаются даже продвинутые примеры." )
table.insert( engine, "Мы знаем этот движок вдоль и поперек." )

table.insert( videotech, "Вы называете это графикой? Не смешите мои тапочки!" )
table.insert( videotech, "Графика в совершенно непотребном состоянии." )
table.insert( videotech, "Она определенно нуждается в улучшении." )
table.insert( videotech, "Очень плохая графика." )
table.insert( videotech, "С графикой нужно что-то делать." )
table.insert( videotech, "Над графикой стоит еще немного поработать. Она может быть лучше." )
table.insert( videotech, "Графика ниже среднего. Она может быть немного лучше." )
table.insert( videotech, "Нормальная графика. Еще несколько улучшений, и она станет хорошей." )
table.insert( videotech, "Довольно хорошая графика. Требуется всего несколько завершающих мазков." )
table.insert( videotech, "Очень хорошая графика. Осталось исправить всего несколько неудачных решений." )
table.insert( videotech, "Великолепная графика. Оставьте ее такой, какая она есть." )

table.insert( soundtech, "Вы называете эту какофонию звуком?" )
table.insert( soundtech, "От нее может лопнуть голова." )
table.insert( soundtech, "Невыносимый звук." )
table.insert( soundtech, "Его нужно улучшить." )
table.insert( soundtech, "Не очень хороший звук." )
table.insert( soundtech, "Его еще улучшать и улучшать." )
table.insert( soundtech, "Звук еще недостаточно хорош." )
table.insert( soundtech, "Подправьте его немного." )
table.insert( soundtech, "Звук ниже среднего." )
table.insert( soundtech, "Вы способны на большее." )
table.insert( soundtech, "Нормальный звук. Еще несколько штрихов, и это будет чарующая музыка." )
table.insert( soundtech, "Неплохой звук. Если не принимать во внимание нескольких режущих слух ноток." )
table.insert( soundtech, "Хороший звук. Больше сказать нечего." )
table.insert( soundtech, "Невероятный звук. Изменений больше не требуется." )
