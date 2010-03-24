--[[
Элементы главного меню
--]]
ID_MAINMENU = 10000
ID_SCALETRACKBAR = ID_MAINMENU + 1
ID_TRAS_IND = ID_MAINMENU + 2
ID_START_STOP = ID_MAINMENU + 3
ID_ADMINING = ID_MAINMENU + 4
ID_FUNC = ID_MAINMENU + 5
ID_LOGOUT = ID_MAINMENU + 6
ID_QUIT = ID_MAINMENU + 7
ID_VIEWMODE_TOGGLE = ID_MAINMENU + 8
ID_USERNAME_LABEL = ID_MAINMENU + 9
ID_DATETIME_LABEL = ID_MAINMENU + 10   
--[[
Кнопки функционального меню
--]]
ID_FUNC_WND = 10100
ID_BACK_FROM_ADVFUNC_FRAME = ID_FUNC_WND + 1
ID_CONSOLE_TOGGLE_VISIBLE = ID_FUNC_WND + 2
ID_SONAR_TOGGLE_ENABLED = ID_FUNC_WND + 3 
ID_GRAPHIC_TOGGLE_VISIBLE = ID_FUNC_WND + 4
ID_SELECT_OBJECT_IN_SCENE = ID_FUNC_WND + 5
ID_DRAWREGION_IN_SCENE	= ID_FUNC_WND + 6
--[[
кнопки админки
--]]
ID_ADMINKA = 10200
ID_BACK_FROM_ADMINING_FRAME = ID_ADMINKA + 1
ID_USER_MANAG_TOGGLE_VISIBLE = ID_ADMINKA + 2
ID_REPORT_TOGGLE_VISIBLE = ID_ADMINKA + 3
ID_ARCHIVE_TOGGLE_VISIBLE = ID_ADMINKA + 4
ID_HARDWARE_TOGGLE_VISIBLE = ID_ADMINKA + 5
ID_PLAY_REC = ID_ADMINKA + 6
--[[
формуляр и список событий
--]]
ID_STORE_WINDOW = 10300
NAME_STORE_WINDOW = "_SystemWindowStore_"
--[[
Окно свойств объекта
--]]
ID_NODEPROPERTIES_WINDOW = 10400
WINDOW_NODEPROPERTIES_NAME = "_NodeWindowProperties_"
WINDOW_NODEPROPERTIES_MARKEDIT = "_MarkEditWindowProperties_"
--[[
Окно графиков
--]]
ID_WINDOW_CHART = 10500
WINDOW_CHART_NAME = "_ChartWindow_"
ID_WINDOW_CHART_MANAGE = ID_WINDOW_CHART + 1
WINDOW_CHART_MANAGE_NAME = "_ChartWindowManage_"
CHART_ELEMENT_NAME = "_ChartElementName_"

--[[
Окно трассового индикатор
--]]
WINDOW_TRACE_INDICATOR_NAME="_TraceIndicatorWindow_"
ID_WINDOW_TRACE_INDICATOR=10600

--[[
Сцена регистрации пользователя
--]]
EDIT_LOGIN_NAME="_EditLoginSceneLogin_"
EDIT_PASSWORD_NAME="_EditPassSceneLogin_"

--[[
Окно отображения допкамеры
--]]
WINDOW_ADVANCED_CAMERA_NAME = "_WindowAdvancedCamera_"
WINDOW_ADVANCED_CAMERA_ID = 10700

--[[
Окно отчета
--]]
WINDOW_REPORT_NAME = "_WindowReport_"
WINDOW_REPORT_ID = 10800

--[[
Окно банка
--]]
WINDOW_LOAN_ACTION_NAME = "_WindowLoanAction_"
WNDLOANACTION_GETLOAN_EDIT = "_WindowLoanActionGetLoanEdit_"
WNDLOANACTION_MAXSUM_LABEL = "_WindowLoanActionMaxSumLabel_"
WNDLOANACTION_TABLE = "_WindowLoanActionTableLoans_"

--[[
Окно выбора создаваемого проекта
--]]
WINDOW_PROJECTWIZ_NAME = "_WindowSelectProjectCreating_"

--[[
Окно создания проекта игры
--]]
WINDOW_GAME_WIZZARD = "_WindowCreateGameProject_"
WNDGMWIZ_NAME_EDIT = "_WindowGameWizzardNameEdit_"

--[[
Функция подключения скрипта с проверкой ошибок
--]]

function IncludeScript( pathName )
	local ok, message = pcall( require, pathName )
	if not ok then 
		Log({src=SCRIPT, dev=ODS|CON}, "ERROR: Не могу загрузить " .. pathName )
		Log({src=SCRIPT, dev=ODS|CON}, message )
	end
end