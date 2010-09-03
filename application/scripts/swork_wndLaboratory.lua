local company = nil

local windowLabor = nil
local techMap = CLuaTechMap( nil )
local selectedTech = CLuaTech( nil )
local btnOk = CLuaButton( nil )
local btnCancel = CLuaButton( nil )

local width = scrWidth
local height = scrHeight

local function CreateTechSequence( tech )
	if tech:Empty() == 1 then
		return 
	end
	
	for i=1, tech:GetFutureTechNumber() do
		local internalName = tech:GetFutureTechInternalName( i-1 )
		Log({src=SCRIPT, dev=ODS|CON}, "ƒочерн€€ технолоwги€="..internalName.." –одительска€ технологи€="..tech:GetName() )

		local futureTech = applic:GetTech( internalName );
		--такой технологии нет на рынке
		if futureTech:Empty() == 1 then
			--может у компании ведутс€ разработки этой технологии,
			--надо подменить добавл€емую технологию
			local techInvention = applic:GetInvention( internalName, company:GetName() )
			
			--исследовани€ не ведутс€
			if techInvention:Empty() == 1 then
				--добавл€ем как неизвестную технологию
				techMap:AddTechnology( tech:Self(), internalName )
			else 
				techMap:AddTechnology( tech:Self(), techInvention:Self() )			
			end
		else
			--така€ технологи€ есть на рынке и надо построить
			--дерево потомков
			techMap:AddTechnology( tech:Self(), futureTech:Self() )		
			CreateTechSequence( futureTech )
		end
	end
end

function sworkCreateGenreTechMapWindow( ptr )
	company = applic:GetPlayerCompany()
	windowLabor = guienv:AddWindow( "", 0, 0, width, height, WINDOW_SHOP_ID, guienv:GetRootGUIElement() )
	Log( {src=SCRIPT, dev=ODS|CON}, "sworkCreateGenreTechMapWindow="..company:GetName() )
	
	techMap:SetObject( guienv:AddTechMap( 10, 20, width - 10, height - 10, -1, windowLabor:Self() ) )
	techMap:AddLuaFunction( GUIELEMENT_SELECTED_AGAIN, "sworkTechMapWindowTechSelected" )
	
	local tech = nil
	for i=1, applic:GetTechNumber() do
	    tech = applic:GetTech( i-1 )
		
		if tech:GetTechGroup() == PT_GENRE and not tech:HaveRequireTech() then
			techMap:AddTechnology( nil, tech:Self() )
			CreateTechSequence( tech )
		end
	end
end

function sworkTechMapWindowTechSelected( ptr ) 
    local type = techMap:GetSelectedObjectType()
    
	--технологи€ уже в ходу
	if type == TS_READY then
	    selectedTech = CLuaTech( techMap:GetSelectedObject() )
		Log({src=SCRIPT, dev=ODS|CON}, "¬ыбрана технологи€="..selectedTech:GetName()..
									   " ќписание="..selectedTech:GetDescriptionLink()..
									   " —татус="..selectedTech:GetStatus() )
	    
	    browser:Show()
	    browser:Navigate( selectedTech:GetDescriptionLink() )
	    --local btn = CLuaButton( guienv:AddButton( 10, 10, 10 + 140, 10 + 20, browser:GetWindow(), -1, "«акрыть" ) )
	    --btn:SetAction( "sworkTechMapWindowClose" )
	    return 0
	end
	
	--или технологию только предстоит изобрести
	--то повторный выбор предполагает желание пользовател€ начать исследование...
	if type == TS_PROJECT then
		sworkTechMapWindowStartInvention()
		return 0
	end
	
	--
	if selectedTech:GetStatus() == TS_INDEVELOP then
		sworkShowInventionManager( techMap:GetSelectedObject(), company:GetName() )
		return 0
	end
end

function sworkTechMapWindowStartInvention()
	browser:Show()
	browser:Navigate( "media/html/unknownTechnology.htm" )
	
	btnOk = guienv:AddButton( 10, 30, 190, 30 + 20, browser:GetWindow(), -1, "Ќачать исследовани€" )
	btnOk:SetAction( "sworkTechMapWindowAssignInventionToCompany" )
	
	btnCancel = guienv:AddButton( 200, 30, 390, 30 + 20, browser:GetWindow(), -1, "«акрыть" )
	btnCancel:SetAction( "sworkTechMapWindowCloseConfirmationWindow" )
end

function sworkTechMapWindowAssignInventionToCompany( ptr )
	local inventionName = techMap:GetSelectedObjectName()
	company:StartInvention( inventionName )
	
	sworkShowInventionManager( inventionName, company:GetName() )
	
	btnOk:Remove()
	btnCancel:Remove()
	browser:Hide()
end

function sworkTechMapWindowCloseConfirmationWindow( ptr )
	btnOk:Remove()
	btnCancel:Remove()
	browser:Hide()
end