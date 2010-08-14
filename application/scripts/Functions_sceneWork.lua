--безопасная загрузка  скриптов

IncludeScript("swork_wndCharts")
IncludeScript("swork_wndChartsManage") 
IncludeScript("swork_TargetCameraFunctions") 
IncludeScript("swork_wndReport")
IncludeScript("swork_wndLoanAction")
IncludeScript("swork_wndUniverStuffUp")
IncludeScript("swork_wndProjectManager")
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

function sworkPlayerCompanyReadyProject( ptr )
	local game = CLuaGame( ptr )
	guienv:MessageBox( "Закончена работа на проектом "..game:GetName(), false, false, "", "" )
end

function sworkSelectObjectOnCityScene( ptr )

	local node = CLuaSceneNode( ptr )
	local nodeName = node:GetName()
	
	if nodeName == "bankNode" then
		sceneManager:RemoveSceneFunction( SCENE_LMOUSE_DOUBLE_CLICK, "sworkSelectObjectOnCityScene" )
		ApplicationLoadBankScene()
		return 0
	end
	
	if nodeName == "laborNode" then
		sceneManager:RemoveSceneFunction( SCENE_LMOUSE_DOUBLE_CLICK, "sworkSelectObjectOnCityScene" )
		ApplicationLoadLaborScene()
		return 0		
	end
	
	if nodeName == "officeNode" then
		sceneManager:RemoveSceneFunction( SCENE_LMOUSE_DOUBLE_CLICK, "sworkSelectObjectOnCityScene" )
		ApplicationLoadOfficeScene()
		return 0		
	end
	
	if nodeName == "prNode" then
		sworkCreateWindowReklama()
		return 0
	end
	
	if nodeName == "univerNode" then
		sceneManager:RemoveSceneFunction( SCENE_LMOUSE_DOUBLE_CLICK, "sworkSelectObjectOnCityScene" )
		ApplicationLoadUniverScene()
		return 0
	end
	
	if nodeName == "plantNode" then
		sceneManager:RemoveSceneFunction( SCENE_LMOUSE_DOUBLE_CLICK, "sworkSelectObjectOnCityScene" )
		ApplicationLoadPlantScene()
		return 0		
	end
	
	if nodeName == "gameShopNode" then
		sceneManager:RemoveSceneFunction( SCENE_LMOUSE_DOUBLE_CLICK, "sworkSelectObjectOnCityScene" )
		ApplicationLoadShopScene()
		return 0		
	end
	
end 

function sworkMainLoop( ptr )
	applic:UpdateGameTime( ID_DATETIME_LABEL )	
end

function ToggleConsoleVisible( ptr )
	local console = CLuaConsole( guienv:GetElementByName( "SystemConsole" ) )
	console:ToggleVisible()
end

function sworkSelectObjectOnLaborScene( ptr )
	local node = CLuaSceneNode( ptr )
	local nodeName = node:GetName()

	if nodeName == "graphicTechsNode" then
		sworkCreateVideoTechMapWindow()
		return 0
	end
	
	if nodeName == "soundTechsNode" then
		sworkCreateSoundTechMapWindow()
		return 0
	end
	
	if nodeName == "genreTechsNode" then
		sworkCreateGenreTechMapWindow()
		return 0
	end

	if nodeName == "otherTechsNode" then
		sworkCreateOtherTechMapWindow()
		return 0
	end

	if nodeName == "exitLaborNode" then
		sceneManager:RemoveSceneFunction( SCENE_LMOUSE_DOUBLE_CLICK, "sworkSelectObjectOnLaborScene" )
		ApplicationLoadCityScene()
		return 0
	end
	
	Log({src=SCRIPT, dev=ODS|CON}, "SCRIPT-LABOR:Не могу найти узел для работы "..nodeName )
end

function sworkSelectObjectOnShopScene( ptr )
	local node = CLuaSceneNode( ptr )
	local nodeName = node:GetName()

	if nodeName == "gameInSaleNode" then
		sworkCreateGameInSaleWindow()
		return 0
	end
	
	if nodeName == "gameJournalsNode" then
		sworkCreateGameJournals()
		return 0
	end
	
	if nodeName == "topListMonthNode" then
		sworkCreateMonthTopListWindow() 
		return 0
	end
	
	if nodeName == "allTimeTopListNode" then
		sworkCreateAllTimeTopListWindow() 
		return 0
	end

	if nodeName == "exitShopNode" then
		sceneManager:RemoveSceneFunction( SCENE_LMOUSE_DOUBLE_CLICK, "sworkSelectObjectOnShopScene" )
		ApplicationLoadCityScene()
		return 0
	end
	
	Log({src=SCRIPT, dev=ODS|CON}, "SCRIPT-PLANT:Не могу найти узел для работы "..nodeName )

end

function sworkSelectObjectOnPlantScene( ptr )
	local node = CLuaSceneNode( ptr )
	local nodeName = node:GetName()

	if nodeName == "gameBoxManagerNode" then
		sworkCreateGameBoxManagerWindow()
		return 0
	end
	
	if nodeName == "plantCeNode" then
		sworkCreateDiskProducePlantWindow()
		return 0
	end
	
	if nodeName == "exitPlantNode" then
		sceneManager:RemoveSceneFunction( SCENE_LMOUSE_DOUBLE_CLICK, "sworkSelectObjectOnOfficeScene" )
		ApplicationLoadCityScene()
		return 0
	end
	
	Log({src=SCRIPT, dev=ODS|CON}, "SCRIPT-PLANT:Не могу найти узел для работы "..nodeName )
end

function sworkSelectObjectOnOfficeScene( ptr )
	local node = CLuaSceneNode( ptr )
	local nodeName = node:GetName()

	if nodeName == "createNewProjectNode" then
		sworkCreateWindowWizardProject()
		return 0
	end
	
	if nodeName == "employerManageNode" then
		sworkCreateWindowEmployersManage()
		return 0	
	end
	
	if nodeName == "projectManagerNode" then
		sworkCreateWindowProjectManager()
		return 0
	end

	if nodeName == "exitOfficeNode" then
		sceneManager:RemoveSceneFunction( SCENE_LMOUSE_DOUBLE_CLICK, "sworkSelectObjectOnOfficeScene" )
		ApplicationLoadCityScene()
		return 0
	end
	
	Log({src=SCRIPT, dev=ODS|CON}, "SCRIPT-OFFICE:Не могу найти узел для работы "..nodeName )
end

function sworkSelectObjectOnUniverScene( ptr )
	
	local node = CLuaSceneNode( ptr )
	local nodeName = node:GetName()

	if nodeName == "stuffNode" then
		sworkCreateEmployersWindow()
		return 0
	end

	if nodeName == "exitUniverNode" then
		sceneManager:RemoveSceneFunction( SCENE_LMOUSE_DOUBLE_CLICK, "sworkSelectObjectOnUniverScene" )
		ApplicationLoadCityScene()
		return 0
	end
	
	Log({src=SCRIPT, dev=ODS|CON}, "SCRIPT-BANK:Не могу найти узел для работы "..nodeName )
end

function sworkSelectObjectOnBankScene( ptr )

	local node = CLuaSceneNode( ptr )
	local nodeName = node:GetName()

	if nodeName == "loanNode" then
		sworkCreateWindowLoanAction()
		return 0
	end

	if nodeName == "exitBank" then
		sceneManager:RemoveSceneFunction( SCENE_LMOUSE_DOUBLE_CLICK, "sworkSelectObjectOnBankScene" )
		ApplicationLoadCityScene()
		return 0
	end
	
	Log({src=SCRIPT, dev=ODS|CON}, "SCRIPT-BANK:Не могу найти узел для работы "..nodeName )
end

function ZoomScrollBarChanged( ptr )

	if not ptr then
		return 0
	end
	
	local scrollBar = CLuaScrollBar( ptr )
	local camera = CLuaCamera( sceneManager:GetActiveCamera() )
		
	camera:SetZoom( scrollBar:GetPos() * 10 )
	
end

function RotateCameraFromMiniMap( ptr )

	if not ptr then
		return 0
	end
		
	local miniMap = CLuaMiniMap( ptr )
	local camera = CLuaCamera( sceneManager:GetActiveCamera() )
	x, y, z = camera:GetRotate()
	
	camera:SetRotate( x + miniMap:GetAngleOffset(), y, z )
end

function CameraScaleTrackBarVisibleToggle( ptr )
	
	local b = guienv:GetElementByID( ID_SCALETRACKBAR )
	
	if b then
		local scrb = CLuaScrollBar( b )
	
		scrb:SetVisible( not scrb:GetVisible() )
	end

end

function sworkApplicationClose( ptr )
	NrpApplicationSave()
	applic:SaveBoxAddonsPrice()
	plant:Save( applic:GetCurrentProfile() )
	NrpApplicationClose()
end

sceneManager:AddSceneFunction( SCENE_AFTER_END, "sworkMainLoop" )
applic:AddLuaFunction( APP_DAY_CHANGE, "sworkAppDayChange" )
applic:AddLuaFunction( APP_MONTH_CHANGE, "sworkAppMonthChange" )
applic:AddLuaFunction( APP_YEAR_CHANGE, "sworkAppYearChange" )