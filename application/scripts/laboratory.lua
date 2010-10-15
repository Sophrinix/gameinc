local base = _G

module( "laboratory" )

local company = nil

local lab = nil
local guienv = base.guienv
local scrWidth = base.scrWidth
local scrHeight = base.scrHeight
local browser = base.browser
local button = base.button
local tutorial = base.tutorial
local applic = base.applic
local windowMap = nil
local Log = base.Log

local techMap = nil
local selectedTech = nil
local btnOk = nil
local btnCancel = nil

function Hide()
	guienv:FadeAction( base.FADE_TIME, false, false )			
	guienv:AddTimer( base.AFADE_TIME, "laboratory.FadeExitAction()" )	
end

function Show()	
	if lab then
		lab:SetVisible( true )
	else
		lab = guienv:AddWindow( "media/maps/laboratory_normal.png", 0, 0, scrWidth, scrHeight, -1, guienv:GetRootGUIElement() )
		lab:SetDraggable( false )
		lab:SetVisible( false )
		lab:GetCloseButton():SetVisible( false )
		
		--adding closeButton
		button.Stretch( scrWidth - 80, scrHeight - 80, scrWidth, scrHeight, 
		 			    "button_down", lab:Self(), -1, "",
						"./laboratory.Hide()" )
	end	
	
	tutorial.Update( tutorial.STEP_OVERVIEW_LABORATORY )
	
	button.EqualeTexture( 545, 330, "techMapVideo", lab:Self(), -1, "", "./laboratory.ShowVideoTechMap()" )
	button.EqualeTexture( 372, 213, "techMapSound", lab:Self(), -1, "", "./laboratory.ShowSoundTechMap()" )
	button.EqualeTexture( 749, 222, "techMapAdvTech", lab:Self(), -1, "", "./laboratory.ShowAdvancedTechMap()" )
	button.EqualeTexture( 73, 202,	"techMapGenre", lab:Self(), -1, "", "./laboratory.ShowGenreTechMap()" )

	guienv:FadeAction( base.FADE_TIME, false, false )			
	guienv:AddTimer( base.AFADE_TIME, "laboratory.FadeEnterAction()" )
end

function FadeEnterAction()
	lab:SetVisible( true )
	guienv:FadeAction( base.FADE_TIME, true, true )
end

function FadeExitAction()
	lab:Remove()
	lab = nil
	guienv:FadeAction( base.FADE_TIME, true, true )
end

local function CreateTechSequence( tech )
	if tech:Empty() == 1 then
		return 
	end
	
	for i=1, tech:GetFutureTechNumber() do
		local internalName = tech:GetFutureTechInternalName( i-1 )
		Log({src=base.SCRIPT, dev=base.ODS|base.CON}, "ƒочерн€€ технолоwги€="..internalName.." –одительска€ технологи€="..tech:GetName() )

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

local function CreateWindow( typef )
	company = applic:GetPlayerCompany()
	windowMap = guienv:AddWindow( "media/maps/laboratory_select.png", 0, 0, scrWidth, scrHeight, -1, guienv:GetRootGUIElement() )
	Log( {src=base.SCRIPT, dev=base.ODS|base.CON}, "sworkCreateGenreTechMapWindow="..company:GetName() )
	
	techMap = guienv:AddTechMap( 10, 40, scrWidth - 10, scrHeight - 10, -1, windowMap:Self() )
	techMap:AddLuaFunction( base.GUIELEMENT_SELECTED_AGAIN, "./laboratory.TechSelected()" )
	techMap:SetDrawBack( false )
	
	local tech = nil
	for i=1, applic:GetTechNumber() do
	    tech = applic:GetTech( i-1 )
		
		if tech:GetTechGroup() == typef and not tech:HaveRequireTech() then
			techMap:AddTechnology( nil, tech:Self() )
			CreateTechSequence( tech )
		end
	end
end

function ShowAdvancedTechMap()
	CreateWindow( base.PT_ADVTECH )	
end

function ShowSoundTechMap()
	CreateWindow( base.PT_SOUNDTECH )	
end

function ShowVideoTechMap()
	CreateWindow( base.PT_VIDEOTECH )	
end

function ShowGenreTechMap()
	CreateWindow( base.PT_GENRE )	
end

function TechSelected() 
    local type = techMap:GetSelectedObjectType()
    
	--технологи€ уже в ходу
	if type == base.TS_READY then
	    selectedTech = CLuaTech( techMap:GetSelectedObject() )
		Log({src=base.SCRIPT, dev=base.ODS|base.CON}, "¬ыбрана технологи€="..selectedTech:GetName()..
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
	if type == base.TS_PROJECT then
		StartInvention()
		return 0
	end
	
	--
	if selectedTech:GetStatus() == base.TS_INDEVELOP then
		inventionManager.Show( techMap:GetSelectedObject(), company:GetName() )
		return 0
	end
end

function StartInvention()
	browser:Show()
	browser:Navigate( "media/html/unknownTechnology.htm" )
	
	btnOk = guienv:AddButton( 10, 30, 190, 30 + 20, browser:GetWindow(), -1, "Ќачать исследовани€" )
	btnOk:SetAction( "./laboratory.AssignInvention()" )
	
	btnCancel = guienv:AddButton( 200, 30, 390, 30 + 20, browser:GetWindow(), -1, "«акрыть" )
	btnCancel:SetAction( "./laboratory.CloseConfirmationWindow()" )
end

function AssignInvention()
	local inventionName = techMap:GetSelectedObjectName()
	company:StartInvention( inventionName )
	
	base.inventionManager.Show( inventionName, company:GetName() )
	
	btnOk:Remove()
	btnCancel:Remove()
	browser:Hide()
end

function CloseConfirmationWindow( ptr )
	btnOk:Remove()
	btnCancel:Remove()
	browser:Hide()
end