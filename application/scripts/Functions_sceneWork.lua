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

local sceneManager = CLuaSceneManager( NrpGetSceneManager() )
local guienv = CLuaGuiEnvironment( NrpGetGuiEnvironment() )
local app = CLuaApplication( NrpGetApplication() )
local company = CLuaCompany( app:GetPlayerCompany() )
local plant = CLuaPlant( NrpGetPlant() )

function sworkAppDayChange( ptr )
	local userLabel = CLuaLabel( guienv:GetElementByID( ID_USERNAME_LABEL ) )
	local name = company:GetName()
	local summ = company:GetBalance()
	userLabel:SetText( name..":   $"..summ)
end

function sworkAppMonthChange( ptr )
	app:CreateNewFreeUsers()
end

function sworkAppYearChange( ptr )

end

function sworkPlayerCompanyReadyProject( ptr )
	local game = CLuaGame( ptr )
	guienv:MessageBox( "Закончена работа на проектом "..game:GetName(), false, false, "", "" )
end

function sworkSelectObjectOnCityScene( ptr )

	local node = CLuaSceneNode( ptr )
	
	if node:GetName() == "bankNode" then
		SetVisibleObjects( citySceneObjects, false )
		SetVisibleObjects( bankSceneObjects, true )
		sceneManager:AddSceneFunction( SCENE_LMOUSE_DOUBLE_CLICK, "sworkSelectObjectOnBankScene" )
		return 0
	end
	
	if node:GetName() == "officeNode" then
		SetVisibleObjects( citySceneObjects, false )
		SetVisibleObjects( officeSceneObjects, true )
		sceneManager:AddSceneFunction( SCENE_LMOUSE_DOUBLE_CLICK, "sworkSelectObjectOnOfficeScene" )
		return 0		
	end
	
	if node:GetName() == "prNode" then
		sworkCreateWindowReklama()
	end
	
	if node:GetName() == "univerNode" then
		SetVisibleObjects( citySceneObjects, false )
		SetVisibleObjects( univerSceneObjects, true )
		sceneManager:AddSceneFunction( SCENE_LMOUSE_DOUBLE_CLICK, "sworkSelectObjectOnUniverScene" )
		return 0
	end
	
	if node:GetName() == "plantNode" then
		SetVisibleObjects( citySceneObjects, false )
		SetVisibleObjects( plantSceneObjects, true )
		sceneManager:AddSceneFunction( SCENE_LMOUSE_DOUBLE_CLICK, "sworkSelectObjectOnPlantScene" )
		return 0		
	end
	
	if node:GetName() == "gameShopNode" then
		SetVisibleObjects( citySceneObjects, false )
		SetVisibleObjects( shopSceneObjects, true )
		sceneManager:AddSceneFunction( SCENE_LMOUSE_DOUBLE_CLICK, "sworkSelectObjectOnShopScene" )
		return 0		
	end
	
end 

function sworkMainLoop( ptr )
	app:UpdateGameTime( ID_DATETIME_LABEL )	
end

function ToggleConsoleVisible( ptr )
	local console = CLuaConsole( guienv:GetElementByName( "SystemConsole" ) )
	console:ToggleVisible()
end

function sworkSelectObjectOnShopScene( ptr )
	local node = CLuaSceneNode( ptr )
	local nodeName = node:GetName()

	if nodeName == "gameInSale" then
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
		SetVisibleObjects( shopSceneObjects, false )
		SetVisibleObjects( citySceneObjects, true )
		sceneManager:RemoveSceneFunction( SCENE_LMOUSE_DOUBLE_CLICK, "sworkSelectObjectOnShopScene" )
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
		SetVisibleObjects( plantSceneObjects, false )
		SetVisibleObjects( citySceneObjects, true )
		sceneManager:RemoveSceneFunction( SCENE_LMOUSE_DOUBLE_CLICK, "sworkSelectObjectOnOfficeScene" )
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
		SetVisibleObjects( officeSceneObjects, false )
		SetVisibleObjects( citySceneObjects, true )
		sceneManager:RemoveSceneFunction( SCENE_LMOUSE_DOUBLE_CLICK, "sworkSelectObjectOnOfficeScene" )
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
		SetVisibleObjects( univerSceneObjects, false )
		SetVisibleObjects( citySceneObjects, true )
		sceneManager:RemoveSceneFunction( SCENE_LMOUSE_DOUBLE_CLICK, "sworkSelectObjectOnUniverScene" )
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

	if nodeName == "exitBankNode" then
		SetVisibleObjects( bankSceneObjects, false )
		SetVisibleObjects( citySceneObjects, true )
		sceneManager:RemoveSceneFunction( SCENE_LMOUSE_DOUBLE_CLICK, "sworkSelectObjectOnBankScene" )
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
	app:SaveBoxAddonsPrice()
	plant:Save( app:GetCurrentProfile() )
	NrpApplicationClose()
end

sceneManager:AddSceneFunction( SCENE_AFTER_END, "sworkMainLoop" )
app:AddLuaFunction( APP_DAY_CHANGE, "sworkAppDayChange" )
app:AddLuaFunction( APP_MONTH_CHANGE, "sworkAppMonthChange" )
app:AddLuaFunction( APP_YEAR_CHANGE, "sworkAppYearChange" )