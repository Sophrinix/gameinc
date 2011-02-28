--[[
Элементы главного меню
--]]
ID_MAINMENU = 10000
ID_USERNAME_LABEL = ID_MAINMENU + 9
ID_DATETIME_LABEL = ID_MAINMENU + 10   

--[[
Функция подключения скрипта с проверкой ошибок
--]]

function toColor( a, r, g, b )
	local ret = a * 16777216 + r * 65536 + g * 256 + b;
	return ret
end

WHITE_COLOR = 4294967295
BLACK_COLOR = 4278190080

function IncludeScript( pathName )
	local ok, message = pcall( require, pathName )
	if not ok then 
		Log({src=SCRIPT, dev=ODS|CON}, "ERROR: Не могу загрузить " .. pathName )
		Log({src=SCRIPT, dev=ODS|CON}, message )
	end
end

function DebugFunctionCall( func, ... )
	local ok, message = pcall( func, ... )
	if not ok then 
		Log({src=SCRIPT, dev=ODS|CON}, "ERROR: Не могу выполнить функцию" )
		Log({src=SCRIPT, dev=ODS|CON}, message )
	end
end