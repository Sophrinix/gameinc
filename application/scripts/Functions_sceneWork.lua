--безопасная загрузка  скриптов

IncludeScript("swork_wndCharts")
IncludeScript("swork_wndChartsManage") 
IncludeScript("swork_TargetCameraFunctions") 
IncludeScript("swork_wndLoanAction")
IncludeScript("swork_wndUniverStuffUp")
IncludeScript("projectManager")	--as module
IncludeScript("swork_wndGameBoxCreate")
IncludeScript("swork_wndDiskPlant")
IncludeScript("swork_wndReklama")
IncludeScript("swork_wndLaboratory" )

local company = applic:GetPlayerCompany()
local plant = NrpGetPlant()

function sworkAppDayChange( ptr )
	local userLabel = CLuaLabel( guienv:GetElementByID( ID_USERNAME_LABEL ) )
	local name = company:GetName()
	local summ = company:GetBalance()
	userLabel:SetText( name..":   $"..summ)
end

function sworkAppMonthChange( ptr )
	applic:CreateNewFreeUsers()
	applic:CheckNewTechs()
end

function sworkAppYearChange( ptr )

end

function sworkInventionFinished( ptr )
	local inv = CLuaTech( ptr )
	guienv:MessageBox( "Закончена работа на изобретением "..inv:GetName(), false, false, "", "" )
end

function sworkPlayerCompanyReadyProject( ptr )
	local game = CLuaGame( ptr )
	guienv:MessageBox( "Закончена работа на проектом "..game:GetName(), false, false, "", "" )
end

function sworkMainLoop( ptr )
	applic:UpdateGameTime( ID_DATETIME_LABEL )	
end

function ToggleConsoleVisible( ptr )
	local console = CLuaConsole( guienv:GetElementByName( "SystemConsole" ) )
	console:ToggleVisible()
end

function sworkSelectObjectOnOfficeScene( ptr )
	local node = CLuaSceneNode( ptr )
	local nodeName = node:GetName()
	
	
	if nodeName == "inventionManagerNode" then
		sworkCreateWindowCompanyInventionManager()
		return 0
	end

	if nodeName == "exitOfficeNode" then
		sceneManager:RemoveSceneFunction( SCENE_LMOUSE_DOUBLE_CLICK, "sworkSelectObjectOnOfficeScene" )
		ApplicationLoadCityScene()
		return 0
	end
	
	Log({src=SCRIPT, dev=ODS|CON}, "SCRIPT-OFFICE:Не могу найти узел для работы "..nodeName )
end

function sworkApplicationClose( ptr )
	NrpApplicationSave()
	applic:SaveBoxAddonsPrice()
	plant:Save( applic:GetCurrentProfile() )
	NrpApplicationClose()
end

function sworkDrawOnTopWindows( ptr )
	for i=1, #alwaysOnTopWindow do
		alwaysOnTopWindow[ i ]:Draw()
		
		guienv:BringToFront( alwaysOnTopWindow[ i ]:Self() )
	end
end

sceneManager:AddSceneFunction( SCENE_AFTER_END, "sworkMainLoop" )
sceneManager:AddSceneFunction( SCENE_AFTER_RENDER, "sworkDrawOnTopWindows" )
applic:AddLuaFunction( APP_DAY_CHANGE, "sworkAppDayChange" )
applic:AddLuaFunction( APP_MONTH_CHANGE, "sworkAppMonthChange" )
applic:AddLuaFunction( APP_YEAR_CHANGE, "sworkAppYearChange" )
applic:AddLuaFunction( APP_INVENTION_FINISHED, "sworkInventionFinished" )