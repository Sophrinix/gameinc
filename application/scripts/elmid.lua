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
	local ret = a * 256 + r;
	ret = ret * 256 + g
	ret = ret * 256 + b
	
	return ret
end

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