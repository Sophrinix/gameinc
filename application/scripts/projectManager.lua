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

local comboxProjects = nil
local comboxUsers = nil
local lbxComponents = nil
local selectedListBox = nil

local windowProjectManager = nil
local windowUserInfo = nil

local function localCreateProjectsComboBox()
	comboxProjects = guienv:AddComboBox( "", 140, 60, "50%", "30+", -1, windowProjectManager )
	--пробежимс€ по всем проектам компании
	LogScript( "Company DevProject number="..company.devProjectNumber )
	for i=1, company.devProjectNumber do
		local ptrProject = company:GetDevProject( i-1 )
		
		local function dd( pt )
			if pt == base.PT_GAME then return base.STR_GAME else return base.STR_ENGINE end
		end
		
		comboxProjects:AddItem( dd( ptrProject.techGroup ) .. ":" .. ptrProject.name, ptrProject )	
	end
end

local function ShowWindowUserInfo( userPtr )
	currentEmployer = userPtr
	
	if windowUserInfo == nil then
		windowUserInfo = guienv:AddWindow( "", "50%", "20%", "60e", "40%", -1, windowProjectManager )
		windowUserInfo:AddLuaFunction( base.GUIELEMENT_LBXITEM_SELECTED, ListBoxItemSelected )
		windowUserInfo.drawBody = false
		windowUserInfo.closeButton.visible = false
		windowUserInfo.draggable = false
	else
		base.CLuaElement( windowUserInfo ):RemoveChilds()
		windowUserInfo.visible = true 
	end

	if currentEmployer == nil or currentEmployer.empty then
	    windowUserInfo.visible = false
		return 0
	end
	
	base.userInfo.Update( currentEmployer, windowUserInfo )
				   
	local lbx = guienv:AddComponentListBox( "55%", "40%", "150e", "60e", -1, windowProjectManager )
	for i=1, currentEmployer.workNumber do
		lbx:AddItem( "", currentEmployer:GetWork( i-1 ) )
		--lbx:SetItemTextColor( i-1, 0xff, 0xff, 0xff, 0xff )
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
	
	if comboxUsers.itemCount > 0 then
		ShowWindowUserInfo( base.CLuaUser( comboxUsers:GetItem( 0 ) ) ) 	
	end
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

function Show()	
	company = applic.playerCompany
	if windowProjectManager == nil then
		windowProjectManager = window.fsWindow( "media/textures/monitor.png", Hide )
	else
		windowProjectManager.visible = true
		return
	end
	
	localCreateProjectsComboBox()

	lbxComponents = guienv:AddComponentListBox( 140, 100, "50%", "60e", -1, windowProjectManager )
	lbxComponents.itemHeigth = 40
	
	localBtnToggleTask = button.Stretch( "50%", "50%", "50+", "60+", "tuda",
								     	 windowProjectManager, -1, "",
								    	 ToggleComponentLider )
	--users combobox
	comboxUsers = guienv:AddComboBox( "", "50%", "15%", "140e", "4%+", -1, windowProjectManager )
	
	--coders
	local layout = guienv:AddLayout( "50%", "5%", "140e", "15%", 10, -1, windowProjectManager )
	button.LayoutButton( "", layout, -1, base.STR_CODERS, function () UpdateUsersListBox( base.STR_CODERS ) end )
	button.LayoutButton( "", layout, -1, base.STR_DESIGNERS, function () UpdateUsersListBox( base.STR_DESIGNERS ) end )
	button.LayoutButton( "", layout, -1, base.STR_COMPOSERS, function () UpdateUsersListBox( base.STR_COMPOSERS ) end )
	button.LayoutButton( "", layout, -1, base.STR_TESTERS, function () UpdateUsersListBox( base.STR_TESTERS ) end )
	--поставим дл€ просмотра первого кодера
	UpdateUsersListBox( base.STR_CODERS )	
	
	windowProjectManager:AddLuaFunction( base.GUIELEMENT_CMBXITEM_SELECTED, ComboBoxItemSelected )
	windowProjectManager:AddLuaFunction( base.GUIELEMENT_LBXITEM_SELECTED, ListBoxItemSelected )
end

function ListBoxItemSelected( mp )
	selectedListBox = base.CLuaComponentListBox( mp )
	currentComponent = base.CLuaDevelopModule( selectedListBox.selectedObject )
	LogScript( "!!!!!!!!!!!! CurrentComponent "..currentComponent.name )

	if selectedListBox.object == lbxComponents.object then
		localBtnToggleTask.tooltip = "ƒобавить задание"
		button.SetEqualeImage( localBtnToggleTask, "tuda" )	
	else
		localBtnToggleTask.tooltip = "”брать задание"
		button.SetEqualeImage( localBtnToggleTask, "suda" )	
	end
end

local function ShowUnworkedGameProjectComponent()
	lbxComponents:Clear()
	
	for i=1, currentProject.moduleNumber do
	    local module = currentProject:GetModule( i-1 )
		if not module.empty then 
		  lbxComponents:AddItem( module.name, module )	
		  --lbxComponents:SetItemTextColor( i-1, 0xff, 0xff, 0xff, 0xff )
		end
	end
end

function ToggleComponentLider()
	if selectedListBox.object == lbxComponents.object then
		LogScript( "PROJECT-MANAGER:Add component to "..currentEmployer.name )
		if currentComponent ~= nil and not currentComponent.empty then
			currentEmployer:AddWork( currentComponent )	
			if currentComponent:GetEmployerPosibility( currentEmployer ) < 0.4 then
				guienv:MessageBox( currentEmployer.name .. " имеет недостаточные навыки.", true, false, nil, nil )
			end
		end
	else
		LogScript( "PROJECT-MANAGER:Remove component from "..currentEmployer.name )
		if not currentComponent.empty then
			currentEmployer:RemoveWork( currentComponent )
		end
	end
	
	ShowUnworkedGameProjectComponent( currentProject )
	ShowWindowUserInfo( currentEmployer )
end

function ComboBoxItemSelected( mp )
	local cmbx = base.CLuaComboBox( mp )
	
	if cmbx.object == comboxUsers.object then
		ShowWindowUserInfo( base.CLuaUser( cmbx.selectedObject ) ) 	
	elseif cmbx.object == comboxProjects.object then
		currentProject = base.CLuaDevelopProject( cmbx.selectedObject )
		
		if currentProject.techGroup == base.PT_GAME then
			ShowUnworkedGameProjectComponent()
		elseif currentProject.techGroup == base.PT_GAMEENGINE then
			ShowUnworkedEngineProjectComponent()
		end
	end
end