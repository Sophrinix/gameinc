--[[
�������� �������� ����
--]]
ID_MAINMENU = 10000
ID_USERNAME_LABEL = ID_MAINMENU + 9
ID_DATETIME_LABEL = ID_MAINMENU + 10   
--[[
�������� � ������ �������
--]]
ID_PORTFELLE_WINDOW = 10300
NAME_PORTFELLE_WINDOW = "_SystemWindowPortfelle_"

--[[
���� ������
--]]
WINDOW_REPORT_NAME = "_WindowReport_"
WINDOW_REPORT_ID = 10800

--[[
���� �����
--]]
WINDOW_LOAN_ACTION_NAME = "_WindowLoanAction_"
WNDLOANACTION_GETLOAN_EDIT = "_WindowLoanActionGetLoanEdit_"
WNDLOANACTION_MAXSUM_LABEL = "_WindowLoanActionMaxSumLabel_"
WNDLOANACTION_TABLE = "_WindowLoanActionTableLoans_"

--[[
���� ������ ������������ �������
--]]
WINDOW_PROJECTWIZ_NAME = "_WindowSelectProjectCreating_"

--[[
���� �������� ������� ����
--]]
WINDOW_GAME_WIZZARD = "_WindowCreateGameProject_"
WNDGMWIZ_NAME_EDIT = "_WindowGameWizzardNameEdit_"

--[[
���� ������ ���������
--]]
WINDOW_STUFFUP_NAME = "_SystemWindowStuff_"
WINDOW_EMPLOYER_SELECT_ID = 11000

--[[
���� ���������� ���������
--]]
WINDOW_PRJMANAGE_NAME = "_SystemWindowManageProject_"
WINDOW_PRJMANAGE_ID	= 11200

--[[
���� ���������� �������� �����
--]]
WINDOW_GAMEBOXWIZ_NAME	= "_SystemWindowGameBoxManager_"
WINDOW_GAMEBOXWIZ_ID = 11300

--[[
���� ���������� ������������� ������
--]]
WINDOW_DISKPRODUCEPLANT_NAME = "_SystemWindowDiskProducePlant_"
WINDOW_DISKPRODUCEPLANT_ID = 11400
WNDDPP_CMBX_PRODUCE_TYPE = 11401

--[[
���� ���������� ��������
--]]
WINDOW_REKLAME_NAME = "_SystemWindowReklame_"
WINDOW_REKLAME_ID = 11500

--[[
���� �������� ��������
--]]
WINDOW_SHOP_NAME = "_SystemWindowShop_"
WINDOW_SHOP_ID = 11600

--[[
������� ����������� ������� � ��������� ������
--]]

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