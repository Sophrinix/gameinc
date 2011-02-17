local base = _G

module( "projectManager" ) 

local button = base.button
local applic = base.applic
local company = nil
local guienv = base.guienv
local LogScript = base.LogScript
local scrWidth = base.scrWidth
local scrHeight = base.scrHeight

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
	comboxProjects = guienv:AddComboBox( "", 140, 60, 140 + 300, 80, -1, windowProjectManager )
	--пробежимся по всем проектам компании
	LogScript( "Company DevProject number="..company:GetDevProjectNumber() )
	for i=1, company:GetDevProjectNumber() do
		local ptrProject = company:GetDevProject( i-1 )
		
		local function dd( pt )
			if pt == base.PT_GAME then return base.STR_GAME else return base.STR_ENGINE end
		end
		
		comboxProjects:AddItem( dd( ptrProject:GetTechGroup() ) .. ":" .. ptrProject:GetName(), ptrProject )	
	end
end

local function ShowWindowUserInfo( userPtr )
	currentEmployer = userPtr
	
	if windowUserInfo == nil then
		windowUserInfo = guienv:AddWindow( "", 490, 140, scrWidth - 150, scrHeight - 60, -1, windowProjectManager )
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
		lbd:SetOverrideColor( 0xff, 0xff, 0xff, 0xff )
		local prg = guienv:AddProgressBar( windowUserInfo, 80, posy, "5r", "20+", -1 )
		prg:SetPosition( valuel )						   
		prg:SetImage( "media/textures/stars01.png" )
		prg:SetFillImage( "media/textures/stars06.png" )
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
	for i=1, company:GetUserNumber() do
		local user = company:GetUser( i-1 )
		if user and modeUserView == user:GetTypeName() then
			comboxUsers:AddItem( user:GetName(), user.object )
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

function Show( ptr )	
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
	lbxComponents = guienv:AddComponentListBox( 140, 80, offsetX, "60e", -1, windowProjectManager )
	lbxComponents:SetItemHeigth( 40 )
	
	localBtnToggleTask = button.Stretch( offsetX + 10, "50%", "50+", "60+", "tuda",
								     	 windowProjectManager, -1, "Добавить задание",
								    	 ToggleComponentLider )
	--users combobox
	comboxUsers = guienv:AddComboBox( "", offsetX + 10, 110, "150e", "20+", -1, windowProjectManager )
	
	--coders
	button.Stretch( offsetX + 10, 50, "90+", "50+", "", windowProjectManager, -1, 
					base.STR_CODERS, function () UpdateUsersListBox( base.STR_CODERS ) end )
	UpdateUsersListBox( base.STR_CODERS )	
	
	--designers
	button.Stretch( offsetX + 110, 50, "100+", "50+", "", windowProjectManager, -1, base.STR_DESIGNERS,
				    function () UpdateUsersListBox( base.STR_DESIGNERS ) end )

	--composers
	button.Stretch( offsetX + 220, 50, "100+", "50+", "", windowProjectManager, -1, base.STR_COMPOSERS,
				    function () UpdateUsersListBox( base.STR_COMPOSERS ) end )
	
	--testers
	button.Stretch( offsetX + 330, 50, "100+", "50+", "", windowProjectManager, -1, base.STR_TESTERS,
				    function () UpdateUsersListBox( base.STR_TESTERS ) end )
	
	windowProjectManager:AddLuaFunction( base.GUIELEMENT_CMBXITEM_SELECTED, "./projectManager.ComboBoxItemSelected()" )
	windowProjectManager:AddLuaFunction( base.GUIELEMENT_LBXITEM_SELECTED, "./projectManager.ListBoxItemSelected()" )
end

function ListBoxItemSelected( mp )
	selectedListBox = base.CLuaComponentListBox( mp )
	currentComponent = base.CLuaDevelopModule( selectedListBox:GetSelectedObject() )
	LogScript( "!!!!!!!!!!!! CurrentComponent "..currentComponent:GetName() )

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
		  lbxComponents:AddItem( module:GetName(), module )	
		  lbxComponents:SetItemTextColor( i-1, 0xff, 0xff, 0xff, 0xff )
		end
	end
end

function ToggleComponentLider()
	if selectedListBox.object == lbxComponents.object then
		LogScript( "PROJECT-MANAGER:Add component to "..currentEmployer:GetName() )
		if currentComponent ~= nil and not currentComponent.empty then
			currentEmployer:AddWork( currentComponent )	
			if currentComponent:GetEmployerPosibility( currentEmployer ) < 0.4 then
				guienv:MessageBox( currentEmployer:GetName() .. " имеет недостаточные навыки.", true, false, "", "" )
			end
		end
	else
		LogScript( "PROJECT-MANAGER:Remove component from "..currentEmployer:GetName() )
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