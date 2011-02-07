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

btnVideo = nil
btnSound = nil
btnTech = nil
btnGenre = nil

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
		button.Stretch( "80e", "80e", "0e", "0e", "button_down", lab, -1, "", "./laboratory.Hide()" )
	end	
	
	tutorial.Update( tutorial.STEP_OVERVIEW_LABORATORY )
	
	btnVideo = button.EqualeTexture( 545, 330, "techMapVideo", lab, -1, "", "./laboratory.ShowVideoTechMap()" )
	btnSound = button.EqualeTexture( 372, 213, "techMapSound", lab, -1, "", "./laboratory.ShowSoundTechMap()" )
	btnTech = button.EqualeTexture( 749, 222,  "techMapAdvTech", lab, -1, "", "./laboratory.ShowAdvancedTechMap()" )
	btnGenre = button.EqualeTexture( 73, 202,  "techMapGenre", lab, -1, "", "./laboratory.ShowGenreTechMap()" )

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
				techMap:AddTechnology( tech, internalName )
			else 
				techMap:AddTechnology( tech, techInvention )			
			end
		else
			--така€ технологи€ есть на рынке и надо построить
			--дерево потомков
			techMap:AddTechnology( tech, futureTech )		
			CreateTechSequence( futureTech )
		end
	end
end

function HideTechMap()
	windowMap:Remove()
	windowMap = nil
end

local function CreateWindow( typef )
	company = applic.playerCompany
	windowMap = guienv:AddWindow( "media/maps/laboratory_select.png", 0, 0, scrWidth, scrHeight, -1, guienv:GetRootGUIElement() )
	windowMap:SetDraggable( false )
	windowMap:GetCloseButton():SetVisible( false )
	
	base.LogScript( "sworkCreateGenreTechMapWindow="..company:GetName() )
	
	techMap = guienv:AddTechMap( 10, 40, "10e", "10e", -1, windowMap )
	techMap:AddLuaFunction( base.GUIELEMENT_SELECTED_AGAIN, "./laboratory.TechSelected()" )
	techMap:SetDrawBack( false )
	
	local tech = nil
	for i=1, applic.techNumber do
	    tech = applic:GetTech( i-1 )
		
		if tech:GetTechGroup() == typef and not tech:HaveRequireTech() then
			techMap:AddTechnology( nil, tech )
			CreateTechSequence( tech )
		end
	end
	
	--adding closeButton
	button.Stretch( "80e", "80e", "0e", "0e", 
	 			    "button_down", windowMap, -1, "",
					"./laboratory.HideTechMap()" )
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
    
	if type == base.TS_READY then
		--технологи€ уже в ходу
	    selectedTech = base.CLuaTech( techMap:GetSelectedObject() )
		base.LogScript( base.string.format( "¬ыбрана технологи€=%s ќписание=%s —татус=%s", 
											selectedTech:GetName(), selectedTech:GetDescriptionLink(), selectedTech:GetStatus() ) )
	    
	    browser:Show()
	    browser:Navigate( selectedTech:GetDescriptionLink() )
	elseif type == base.TS_PROJECT then
		--или технологию только предстоит изобрести
		--то повторный выбор предполагает желание пользовател€ начать исследование...
		StartInvention()
	elseif type == base.TS_INDEVELOP then
		selectedTech = base.CLuaInvention( techMap:GetSelectedObject() )
		base.inventionManager.Show( selectedTech:GetInternalName(), company:GetName() )
	end
end

function StartInvention()
	browser:Show()
	browser:Navigate( "media/html/unknownTechnology.htm" )
	
	btnOk = guienv:AddButton( "5%", 30, "45%+", "20+", browser.window, -1, "Ќачать исследовани€" )
	btnOk:SetAction( "./laboratory.AssignInvention()" )
	
	btnCancel = guienv:AddButton( "55%", 30, "45%+", "20+", browser.window, -1, "«акрыть" )
	btnCancel:SetAction( "./laboratory.CloseConfirmationWindow()" )
end

function AssignInvention()
	local inventionName = techMap:GetSelectedObjectName()
	local loadFile = base.updates.FindInventionLoadFile( inventionName )
	company:StartInvention( loadFile )
	
	base.inventionManager.Show( inventionName, company:GetName() )
	windowMap:Remove()
	windowMap = nil
	
	btnOk:Remove()
	btnCancel:Remove()
	browser:Hide()
end

function CloseConfirmationWindow( ptr )
	btnOk:Remove()
	btnCancel:Remove()
	browser:Hide()
end