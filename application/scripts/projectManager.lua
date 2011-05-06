local base = _G

IncludeScript( "userInfo" )

module( "projectManager" ) 

local button = base.button
local window = base.window
local applic = base.applic
local company = nil
local guienv = base.guienv
local LogScript = base.LogScript

local mode = { }
mode[ base.STR_CODERS ] = "coder"
mode[ base.STR_DESIGNERS ] = "designer"
mode[ base.STR_COMPOSERS ] = "composer"
mode[ base.STR_TESTERS ] = "tester"
--"coder" "designer" "composer" "tester"
local modeUserView = "coder"
local OUTPUT_ALL = base.ODS|base.CON

local currentEmployer = nil
local currentProject = nil
local currentComponent = nil
local localBtnToggleTask = nil
local selectedListBox = nil
local windowProjectManager = nil

windowUserInfo = nil
layoutUserTypes = nil
comboxProjects = nil
comboxUsers = nil
lbxComponents = nil
lbxWorks = nil

local function _ShowModuleInformation()
	local component = base.CLuaDevelopModule( lbxComponents.selectedObject )
	LogScript( "Show information for " .. component.name )
	LogScript( "errorNumber = " .. component.errorNumber )
end

local function _CreateProjectsComboBox()
	comboxProjects = guienv:AddComboBox( "", 140, 50, "50%", "30+", -1, windowProjectManager )
	--пробежимся по всем проектам компании
	LogScript( "Company DevProject number="..company.devProjectNumber )
	
	local function dd( pt )
			if pt == base.PT_GAME then return base.STR_GAME else return base.STR_ENGINE end
	end
		
	for i=1, company.devProjectNumber do
		local ptrProject = company:GetDevProject( i-1 )
		
		comboxProjects:AddItem( dd( ptrProject.techGroup ) .. ":" .. ptrProject.name, ptrProject )	
	end
end

local function _ShowWindowUserInfo( userPtr )
	currentEmployer = userPtr
	
	if windowUserInfo then
		windowUserInfo:Remove()
	end
	
	windowUserInfo = guienv:AddWindow( "", "50%", "20%", "60e", "40%", -1, windowProjectManager )
	windowUserInfo.drawBody = false
	windowUserInfo.closeButton.visible = false
	windowUserInfo.draggable = false
	lbxWorks:Clear()
 
	if currentEmployer ~= nil and currentEmployer.empty == false then
		base.userInfo.Update( currentEmployer, windowUserInfo )
		
		for i=1, currentEmployer.workNumber do
			lbxWorks:AddItem( "", currentEmployer:GetWork( i-1 ) )
			--lbx:SetItemTextColor( i-1, 0xff, 0xff, 0xff, 0xff )
		end
	end
end

local function ShowAvaibleCompanyUsers()
	company = applic.playerCompany
	
	comboxUsers:Clear()
	for i=1, company.userNumber do
		local user = company:GetUser( i-1 )
		if user and modeUserView == user.typeName then
			comboxUsers:AddItem( user.name, user.object )
		end
	end
	
	_ShowWindowUserInfo( base.CLuaUser( comboxUsers:GetItem( 0 ) ) ) 	
end

function UpdateUsersListBox( newMode )
	modeUserView = mode[ newMode ] 
	currentEmployer = nil
	currentComponent = nil
	ShowAvaibleCompanyUsers()
end

function Hide()
	windowProjectManager = nil
	windowUserInfo = nil
	base.package.loaded[ "userInfo" ] = false
end

local function _ShowUnworkedGameProjectComponent()
	lbxComponents:Clear()
	
	for i=1, currentProject.moduleNumber do
	    local module = currentProject:GetModule( i-1 )
		if not module.empty then 
		  lbxComponents:AddItem( module.name, module )	
		  --lbxComponents:SetItemTextColor( i-1, 0xff, 0xff, 0xff, 0xff )
		end
	end
end

local function _ToggleComponentLider()
	if selectedListBox.object == lbxComponents.object then
		LogScript( "PROJECT-MANAGER:Add component to "..currentEmployer.name )
		
		if currentComponent ~= nil and not currentComponent.empty then
			if currentEmployer.typeName == "tester" then
			    if currentComponent.percentDone < 0.4 then
					guienv:MessageBox( "Низкая готовность компонента не позволяет\n чтолибо потестировать.", false, false, button.CloseParent, button.NoFunction )
					return
			    end
			    
				if currentComponent.errorNumber == 0 then
					guienv:MessageBox( "Тестировщики уже проверяли этот компонент\n Похоже, ошибок больше не осталось.", false, false, button.CloseParent, button.NoFunction )
					return					
				end
			end
			
			currentEmployer:AddWork( currentComponent )	
				
			--if currentComponent:GetEmployerPosibility( currentEmployer ) < 0.4 then
			--	guienv:MessageBox( currentEmployer.name .. " имеет недостаточные навыки.", false, false, button.CloseParent, button.NoFunction )
			--end
		end
	else
		LogScript( "PROJECT-MANAGER:Remove component from "..currentEmployer.name )
		if not currentComponent.empty then
			currentEmployer:RemoveWork( currentComponent )
		end
	end
	
	_ShowUnworkedGameProjectComponent( currentProject )
	_ShowWindowUserInfo( currentEmployer )
end

local function _ComboBoxItemSelected( mp )
	local cmbx = base.CLuaComboBox( mp )
	
	if cmbx.object == comboxUsers.object then
		_ShowWindowUserInfo( base.CLuaUser( cmbx.selectedObject ) ) 	
	elseif cmbx.object == comboxProjects.object then
		currentProject = base.CLuaDevelopProject( cmbx.selectedObject )
		
		if currentProject.techGroup == base.PT_GAME then
			_ShowUnworkedGameProjectComponent()
		elseif currentProject.techGroup == base.PT_GAMEENGINE then
			ShowUnworkedEngineProjectComponent()
		end
	end
end

local function _WorkedSelect()
	currentComponent = base.CLuaDevelopModule( lbxWorks.selectedObject )
	LogScript( "!!!!!!!!!!!! CurrentComponent "..currentComponent.name )

	localBtnToggleTask.tooltip = "Убрать задание"
	button.SetEqualeImage( localBtnToggleTask, "suda" )	
	selectedListBox = lbxWorks
end

local function _UnworkedSelect()
	currentComponent = base.CLuaDevelopModule( lbxComponents.selectedObject )
	LogScript( "!!!!!!!!!!!! CurrentComponent "..currentComponent.name )

	localBtnToggleTask.tooltip = "Добавить задание"
	button.SetEqualeImage( localBtnToggleTask, "tuda" )	
	selectedListBox = lbxComponents
end

function Show()	
	company = applic.playerCompany
	if windowProjectManager == nil then
		windowProjectManager = window.fsWindow( "media/textures/monitor.png", Hide )
	else
		windowProjectManager.visible = true
		return
	end
	
	_CreateProjectsComboBox()

	lbxComponents = guienv:AddComponentListBox( 140, 100, "50%", "60e", -1, windowProjectManager )
	lbxComponents.itemHeigth = 40
	lbxComponents.onChangeSelect = _UnworkedSelect
	lbxComponents.onLmbDblClick = _ToggleComponentLider
	lbxComponents.onRmbClick = _ShowModuleInformation
	
	lbxWorks = guienv:AddComponentListBox( "55%", "40%", "150e", "60e", -1, windowProjectManager )
	lbxWorks.itemHeigth = 40
	lbxWorks.onChangeSelect = _WorkedSelect
	lbxWorks.onLmbDblClick = _ToggleComponentLider

	localBtnToggleTask = button.Stretch( "50%", "50%", "50+", "60+", "tuda",
								     	 windowProjectManager, -1, "",
								    	 _ToggleComponentLider )
	--users combobox
	comboxUsers = guienv:AddComboBox( "", "50%", "15%", "140e", "4%+", -1, windowProjectManager )
	
	--coders
	layoutUserTypes = guienv:AddLayout( "50%", 50, "140e", "15%", 10, -1, windowProjectManager )
	button.LayoutButton( "", layoutUserTypes, -1, base.STR_CODERS, function () UpdateUsersListBox( base.STR_CODERS ) end )
	button.LayoutButton( "", layoutUserTypes, -1, base.STR_DESIGNERS, function () UpdateUsersListBox( base.STR_DESIGNERS ) end )
	button.LayoutButton( "", layoutUserTypes, -1, base.STR_COMPOSERS, function () UpdateUsersListBox( base.STR_COMPOSERS ) end )
	button.LayoutButton( "", layoutUserTypes, -1, base.STR_TESTERS, function () UpdateUsersListBox( base.STR_TESTERS ) end )
	--поставим для просмотра первого кодера
	UpdateUsersListBox( base.STR_CODERS )	
	
	if comboxProjects.itemCount > 0 then
		comboxProjects.itemIndex = 0
	end 
	
	windowProjectManager:AddLuaFunction( base.GUIELEMENT_CMBXITEM_SELECTED, _ComboBoxItemSelected )
	
	base.tutorial.Update( base.tutorial.STEP_OVERVIEW_PROJECTMANAGER )
end