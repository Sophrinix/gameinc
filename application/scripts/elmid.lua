--[[
�������� �������� ����
--]]
ID_MAINMENU = 10000
ID_USERNAME_LABEL = ID_MAINMENU + 9
ID_DATETIME_LABEL = ID_MAINMENU + 10   

--[[
������� ����������� ������� � ��������� ������
--]]

WHITE_COLOR = 4294967295
BLACK_COLOR = 4278190080

function IncludeScript( pathName )
	local ok, message = pcall( require, pathName )
	if not ok then 
		Log({src=SCRIPT, dev=ODS|CON}, "ERROR: �� ���� ��������� " .. pathName )
		Log({src=SCRIPT, dev=ODS|CON}, message )
	end
end

function DebugFunctionCall( func, ... )
	local ok, message = pcall( func, ... )
	if not ok then 
		Log({src=SCRIPT, dev=ODS|CON}, "ERROR: �� ���� ��������� �������" )
		Log({src=SCRIPT, dev=ODS|CON}, message )
	end
end