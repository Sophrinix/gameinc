local company = CLuaCompany( applic:GetPlayerCompany() )

local windowLabor = CLuaWindow( nil )
local techMap = CLuaTechMap( nil )
local selectedTech = CLuaTech( nil )
local width = 800
local height = 600

local function CreateTechSequence( tech )
	if tech:Empty() == 1 then
		return 
	end
	
	for i=1, tech:GetFutureTechNumber() do
		local ftech = CLuaTech( tech:GetFutureTech( i-1 ) )
		if ftech:Empty() == 0 then
			Log({src=SCRIPT, dev=ODS|CON}, "ƒочерн€€ технологи€="..ftech:GetName().." –одительска€ технологи€="..tech:GetName() )
			techMap:AddTechnology( tech:Self(), ftech:Self() )
			CreateTechSequence( ftech )
		end
	end
end

function sworkCreateGenreTechMapWindow( ptr )
	windowLabor:SetObject( guienv:AddWindow( "", 0, 0, width, height, WINDOW_SHOP_ID, guienv:GetRootGUIElement() ) )
	
	techMap:SetObject( guienv:AddTechMap( 10, 20, width - 10, height - 10, -1, windowLabor:Self() ) )
	techMap:AddLuaFunction( GUIELEMENT_TABLE_SELECTED_AGAIN, "sworkTechMapWindowTechSelected" )
	
	local tech = CLuaTech( nil )
	for i=1, applic:GetTechNumber() do
	    tech:SetObject( applic:GetTech( i-1 ) )
		
		if tech:GetTechGroup() == PT_GENRE and not tech:HaveRequireTech() then
			techMap:AddTechnology( nil, tech:Self() )
			CreateTechSequence( tech )
		end
	end
end

function sworkTechMapWindowTechSelected( ptr )
	selectedTech:SetObject( ptr )
	--технологи€ уже в ходу
	browser:Show()
	Log({src=SCRIPT, dev=ODS|CON}, "¬ыбрана технологи€="..selectedTech:GetName().." ќписание="..tech:GetDescriptionLink() )

	browser:Navigate( tech:GetDescriptionLink() )

	if tech:GetStatus() == TS_READY then
	    local btn = CLuaButton( guienv:AddButton( 10, 10, 10 + 140, 10 + 20, browser:GetWindow(), -1, "«акрыть" ) )
	    btn:SetAction( "sworkTechMapWindowClose" )
	else--или технологию только предстоит изобрести
	    --работа над изобретением еще не велась
	    if tech:GetStatus() == TS_UNKNOWN then 
			local btn = CLuaButton( guienv:AddButton( 10, 10, 10 + 140, 10 + 20, browser:GetWindow(), -1, "Ќачать исследовани€" ) )
			btn:SetAction( "sworkTechMapWindowStartInvent" )
		else--работа ведетс€ в текущий момент
			local btn = CLuaButton( guienv:AddButton( 10, 10, 10 + 140, 10 + 20, browser:GetWindow(), -1, "«акрыть исследовани€" ) )
			btn:SetAction( "sworkTechMapWindowStopInvent" )
		end
	end
end