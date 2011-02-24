local base = _G

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
	comboxProjects = guienv:AddComboBox( "", 140, 60, "300+", "30+", -1, windowProjectManager )
	--пробежимся по всем проектам компании
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
		windowUserInfo = guienv:AddWindow( "", 490, 140, "150e", "60e", -1, windowProjectManager )
		windowUserInfo:AddLuaFunction( base.GUIELEMENT_LBXITEM_SELECTED, "./projectManager.ListBoxItemSelected()" )
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
	
	local function dd( named, valuel, posy )
		local lbd = guienv:AddLabel( named, 5, posy, "5e", "20+", -1, windowUserInfo )
		lbd.color = base.toColor( 0xff, 0xff, 0xff, 0xff )
		local prg = guienv:AddProgressBar( windowUserInfo, 80, posy, "5r", "20+", -1 )
		prg.progress = valuel
		prg.image = "media/textures/stars01.png"
		prg.fillImage = "media/textures/stars06.png"
	end
	
	dd( "Опыт", currentEmployer:GetParam( "knowledgeLevel" ), 30 )
	dd( "Качество", currentEmployer:GetParam("codeQuality"), 50 )
	dd( "Скорость", currentEmployer:GetParam("codeSpeed"), 70 )
	dd( "Устойчивость", currentEmployer:GetParam("stability"), 90 ) 
				   
	local lbx = guienv:AddComponentListBox( 10, 135, "10e", "10e", -1, windowUserInfo )
	for i=1, currentEmployer:GetWorkNumber() do
		lbx:AddItem( "", currentEmployer:GetWork( i-1 ) )
		lbx:SetItemTextColor( i-1, 0xff, 0xff, 0xff, 0xff )
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
	windowProjectManager:Remove()
	windowProjectManager = nil
	windowUserInfo = nil
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

	--project componentlistbox 
	local offsetX = 140 + 300
	lbxComponents = guienv:AddComponentListBox( 140, 100, offsetX, "60e", -1, windowProjectManager )
	lbxComponents.itemHeigth = 40
	
	localBtnToggleTask = button.Stretch( offsetX + 10, "50%", "50+", "60+", "tuda",
								     	 windowProjectManager, -1, "Добавить задание",
								    	 ToggleComponentLider )
	--users combobox
	comboxUsers = guienv:AddComboBox( "", offsetX + 10, 110, "150e", "20+", -1, windowProjectManager )
	
	--coders
	local layout = guienv:AddLayout( offsetX, 50, "80%", "60+", 10, -1, windowProjectManager )
	button.LayoutButton( "", layout, -1, base.STR_CODERS, function () UpdateUsersListBox( base.STR_CODERS ) end )
	button.LayoutButton( "", layout, -1, base.STR_DESIGNERS, function () UpdateUsersListBox( base.STR_DESIGNERS ) end )
	button.LayoutButton( "", layout, -1, base.STR_COMPOSERS, function () UpdateUsersListBox( base.STR_COMPOSERS ) end )
	button.LayoutButton( "", layout, -1, base.STR_TESTERS, function () UpdateUsersListBox( base.STR_TESTERS ) end )
	--поставим для просмотра первого кодера
	UpdateUsersListBox( base.STR_CODERS )	
	
	windowProjectManager:AddLuaFunction( base.GUIELEMENT_CMBXITEM_SELECTED, "./projectManager.ComboBoxItemSelected()" )
	windowProjectManager:AddLuaFunction( base.GUIELEMENT_LBXITEM_SELECTED, "./projectManager.ListBoxItemSelected()" )
end

function ListBoxItemSelected( mp )
	selectedListBox = base.CLuaComponentListBox( mp )
	currentComponent = base.CLuaDevelopModule( selectedListBox:GetSelectedObject() )
	LogScript( "!!!!!!!!!!!! CurrentComponent "..currentComponent.name )

	if selectedListBox.object == lbxComponents.object then
		localBtnToggleTask:SetText( "Добавить задание" )
		button.SetEqualeImage( localBtnToggleTask, "tuda" )	
	else
		localBtnToggleTask:SetText( "Убрать задание" )	
		button.SetEqualeImage( localBtnToggleTask, "suda" )	
	end
end

local function ShowUnworkedGameProjectComponent()
	lbxComponents:Clear()
	
	for i=1, currentProject:GetModuleNumber() do
	    local module = currentProject:GetModule( i-1 )
		if not module.empty then 
		  lbxComponents:AddItem( module.name, module )	
		  lbxComponents:SetItemTextColor( i-1, 0xff, 0xff, 0xff, 0xff )
		end
	end
end

function ToggleComponentLider()
	if selectedListBox.object == lbxComponents.object then
		LogScript( "PROJECT-MANAGER:Add component to "..currentEmployer.name )
		if currentComponent ~= nil and not currentComponent.empty then
			currentEmployer:AddWork( currentComponent )	
			if currentComponent:GetEmployerPosibility( currentEmployer ) < 0.4 then
				guienv:MessageBox( currentEmployer.name .. " имеет недостаточные навыки.", true, false, "", "" )
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
		ShowWindowUserInfo( base.CLuaUser( cmbx:GetSelectedObject() ) ) 	
	elseif cmbx.object == comboxProjects.object then
		currentProject = base.CLuaDevelopProject( cmbx:GetSelectedObject() )
		
		if currentProject:GetTechGroup() == base.PT_GAME then
			ShowUnworkedGameProjectComponent()
		elseif currentProject:GetTechGroup() == base.PT_GAMEENGINE then
			ShowUnworkedEngineProjectComponent()
		end
	end
end