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
	comboxProjects = guienv:AddComboBox( "", 140, 60, 140 + 300, 80, -1, windowProjectManager:Self() )
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
		windowUserInfo = guienv:AddWindow( "", 490, 140, scrWidth - 150, scrHeight - 60, -1, windowProjectManager:Self() )
		windowUserInfo:AddLuaFunction( base.GUIELEMENT_LBXITEM_SELECTED, "./projectManager.ListBoxItemSelected()" )
		windowUserInfo:SetDrawBody( false )
		windowUserInfo:GetCloseButton():SetVisible( false )
		windowUserInfo:SetDraggable( false )
	else
		base.CLuaElement( windowUserInfo:Self() ):RemoveChilds()
		windowUserInfo:SetVisible( true )
	end

	local widdddd, hhhhhhh = windowUserInfo:GetSize()
	
	if currentEmployer == nil or currentEmployer:Empty() == 1 then
	    windowUserInfo:SetVisible( false )
		return 0
	end
	
	local function dd( named, valuel, posy )
		local lbd = guienv:AddLabel( named, 5, posy, widdddd, posy + 20, -1, windowUserInfo:Self() )
		lbd:SetOverrideColor( 0xff, 0xff, 0xff, 0xff )
		local prg = guienv:AddProgressBar( windowUserInfo:Self(), 80, posy, widdddd - 5, posy + 20, -1 )
		prg:SetPosition( valuel )						   
		prg:SetImage( "media/textures/stars01.png" )
		prg:SetFillImage( "media/textures/stars06.png" )
	end
	
	dd( "Опыт", currentEmployer:GetParam( "knowledgeLevel" ), 30 )
	dd( "Качество", currentEmployer:GetParam("codeQuality"), 50 )
	dd( "Скорость", currentEmployer:GetParam("codeSpeed"), 70 )
	dd( "Устойчивость", currentEmployer:GetParam("stability"), 90 ) 
				   
	local lbx = guienv:AddComponentListBox( 10, 135, widdddd - 10, hhhhhhh - 10, -1, windowUserInfo:Self() )
	for i=1, currentEmployer:GetWorkNumber() do
		lbx:AddItem( "", currentEmployer:GetWork( i-1 ) )
		lbx:SetItemTextColor( i-1, 0xff, 0xff, 0xff, 0xff )
	end
end

local function ShowAvaibleCompanyUsers()
	local company = applic:GetPlayerCompany()
	
	comboxUsers:Clear()
	for i=1, company:GetUserNumber() do
		local user = company:GetUser( i-1 )
		if user and modeUserView == user:GetTypeName() then
			comboxUsers:AddItem( user:GetName(), user:Self() )
		end
	end
	ShowWindowUserInfo( nil ) 	
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

	company = applic:GetPlayerCompany()
	if windowProjectManager == nil then
		windowProjectManager = guienv:AddWindow( "media/textures/monitor.png", 0, 0, base.scrWidth, base.scrHeight, -1, guienv:GetRootGUIElement() )
		windowProjectManager:GetCloseButton():SetVisible( false )
		windowProjectManager:SetDraggable( false )
		--adding closeButton
		button.Stretch( scrWidth - 80, scrHeight - 60, scrWidth - 20, scrHeight, 
		 			    "poweroff", windowProjectManager:Self(), -1, "",
						"./projectManager.Hide()" )
	else
		windowProjectManager:SetVisible( true )
		return
	end
	
	localCreateProjectsComboBox()

	--project componentlistbox 
	local offsetX = 140 + 300
	lbxComponents = guienv:AddComponentListBox( 140, 80, offsetX, scrHeight - 60, -1, windowProjectManager:Self() )
	lbxComponents:SetItemHeigth( 40 )
	
	localBtnToggleTask = button.Stretch( offsetX + 10, scrHeight / 2, offsetX + 60, scrHeight / 2 + 60, "tuda",
								     	 windowProjectManager:Self(), -1, "Добавить задание",
								    	 "./projectManager.ToggleComponentLider()" )
	--users combobox
	comboxUsers = guienv:AddComboBox( "", offsetX + 10, 110, scrWidth - 150, 130, -1, windowProjectManager:Self() )
	
	--coders
	button.Stretch( offsetX + 10, 50, offsetX + 100, 100, "", windowProjectManager:Self(), -1, 
					base.STR_CODERS, "./projectManager.UpdateUsersListBox( STR_CODERS )" )
	UpdateUsersListBox( base.STR_CODERS )	
	
	--designers
	button.Stretch( offsetX + 110, 50, offsetX + 210, 100, "", windowProjectManager:Self(), -1, base.STR_DESIGNERS,
				    "./projectManager.UpdateUsersListBox( STR_DESIGNERS )" )

	--composers
	button.Stretch( offsetX + 220, 50, offsetX + 320, 100, "", windowProjectManager:Self(), -1, base.STR_COMPOSERS,
				    "./projectManager.UpdateUsersListBox( STR_COMPOSERS )" )
	
	--testers
	button.Stretch( offsetX + 330, 50, offsetX + 430, 100, "", windowProjectManager:Self(), -1, base.STR_TESTERS,
				    "./projectManager.UpdateUsersListBox( STR_TESTERS )" )
	
	windowProjectManager:AddLuaFunction( base.GUIELEMENT_CMBXITEM_SELECTED, "./projectManager.ComboBoxItemSelected()" )
	windowProjectManager:AddLuaFunction( base.GUIELEMENT_LBXITEM_SELECTED, "./projectManager.ListBoxItemSelected()" )
end

function ListBoxItemSelected()
	selectedListBox = base.CLuaComponentListBox( base.NrpGetSender() )
	currentComponent = base.CLuaDevelopModule( selectedListBox:GetSelectedObject() )
	LogScript( "!!!!!!!!!!!! CurrentComponent "..currentComponent:GetName() )

	if selectedListBox:Self() == lbxComponents:Self() then
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
		if module:Empty() == 0 then 
		  lbxComponents:AddItem( module:GetName(), module:Self() )	
		  lbxComponents:SetItemTextColor( i-1, 0xff, 0xff, 0xff, 0xff )
		end
	end
end

function ToggleComponentLider()
	if selectedListBox:Self() == lbxComponents:Self() then
		LogScript( "PROJECT-MANAGER:Add component to "..currentEmployer:GetName() )
		if currentComponent ~= nil and currentComponent:Empty() == 0 then
			currentEmployer:AddWork( currentComponent:Self() )	
			if currentComponent:GetEmployerPosibility( currentEmployer:Self() ) < 0.4 then
				guienv:MessageBox( currentEmployer:GetName() .. " имеет недостаточные навыки.", true, false, "", "" )
			end
		end
	else
		LogScript( "PROJECT-MANAGER:Remove component from "..currentEmployer:GetName() )
		if currentComponent:Empty() == 0 then
			currentEmployer:RemoveWork( currentComponent:Self() )
		end
	end
	
	ShowUnworkedGameProjectComponent( currentProject:Self() )
	ShowWindowUserInfo( currentEmployer )
end

function ComboBoxItemSelected()
	local cmbx = base.CLuaComboBox( base.NrpGetSender() )
	
	if cmbx:Self() == comboxUsers:Self() then
		ShowWindowUserInfo( base.CLuaUser( cmbx:GetSelectedObject() ) ) 	
	elseif cmbx:Self() == comboxProjects:Self() then
		currentProject = base.CLuaDevelopProject( cmbx:GetSelectedObject() )
		
		if currentProject:GetTechGroup() == base.PT_GAME then
			ShowUnworkedGameProjectComponent()
		elseif currentProject:GetTechGroup() == base.PT_GAMEENGINE then
			ShowUnworkedEngineProjectComponent()
		end
	end
end