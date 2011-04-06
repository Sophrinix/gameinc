local base = _G

module( "laboratory" )

local company = nil

local lab = nil
local guienv = base.guienv
local window= base.window
local browser = base.browser
local button = base.button
local tutorial = base.tutorial
local applic = base.applic
local windowMap = nil
local Log = base.LogScript

local techMap = nil
local selectedTech = nil
local btnOk = nil
local btnCancel = nil

btnVideo = nil
btnSound = nil
btnTech = nil
btnGenre = nil

local function _Hide()
end

local function CreateTechSequence( tech )
	if tech.empty then
		return 
	end
	
	for i=1, tech.ftNumber do
		local internalName = tech:GetFutureTechInternalName( i-1 )
		Log( "ƒочерн€€ технолоwги€="..internalName.." –одительска€ технологи€="..tech.name )

		local futureTech = applic:GetTech( internalName );
		--такой технологии нет на рынке
		if futureTech == nil or futureTech.empty then
			--может у компании ведутс€ разработки этой технологии,
			--надо подменить добавл€емую технологию
			local techInvention = applic:GetInvention( internalName, company.name )
			
			--исследовани€ не ведутс€
			if techInvention.empty then
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
	local txsBlur = base.driver:CreateBlur( "laboratory.png", 2, 4 )
	windowMap = guienv:AddWindow( txsBlur.path, 0, 0, "0e", "0e", -1, guienv.root )
	windowMap.draggable = false
	windowMap.closeButton.visible = false
	
	base.LogScript( "sworkCreateGenreTechMapWindow="..company.name )
	
	techMap = guienv:AddTechMap( 10, 40, "10e", "10e", -1, windowMap )
	techMap:AddLuaFunction( base.GUIELEMENT_SELECTED_AGAIN, TechSelected )
	techMap.drawBack = false
	
	local tech = nil
	for i=1, applic.techNumber do
	    tech = applic:GetTech( i-1 )
		base.assert( tech ~= nil )
		
		if tech.techGroup == typef and not tech.haveRequireTech then
			techMap:AddTechnology( nil, tech )
			CreateTechSequence( tech )
		end
	end
	
	--adding closeButton
	button.Stretch( "80e", "80e", "0e", "0e", 
	 			    "button_down", windowMap, -1, "",
					HideTechMap )
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
    local type = techMap.selectedType
    
	if type == base.TS_READY then
		--технологи€ уже в ходу
	    selectedTech = base.CLuaTech( techMap.selectedObject )
		base.LogScript( base.string.format( "¬ыбрана технологи€=%s ќписание=%s —татус=%s", 
											selectedTech.name, selectedTech.description, selectedTech.datus ) )
	    
	    browser:Show()
	    browser:Navigate( selectedTech.description )
	elseif type == base.TS_PROJECT then
		--или технологию только предстоит изобрести
		--то повторный выбор предполагает желание пользовател€ начать исследование...
		StartInvention()
	elseif type == base.TS_INDEVELOP then
		selectedTech = base.CLuaInvention( techMap.selectedObject )
		base.inventionManager.Show( selectedTech.uniq, company.name )
	end
end

function StartInvention()
	browser:Show()
	browser:Navigate( "media/html/unknownTechnology.htm" )
	
	btnOk = guienv:AddButton( "5%", 30, "45%+", "20+", browser.window, -1, "Ќачать исследовани€" )
	btnOk.action = AssignInvention
	
	btnCancel = guienv:AddButton( "55%", 30, "45%+", "20+", browser.window, -1, "«акрыть" )
	btnCancel.action = CloseConfirmationWindow
end

function AssignInvention()
	local inventionName = techMap.selectedObjectName
	local loadFile = base.updates.FindInventionLoadFile( inventionName )
	company:StartInvention( loadFile )
	
	base.inventionManager.Show( inventionName, company.name )
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

function Show()	
	lab = window.fsWindow( "laboratory.png", _Hide )
	
	tutorial.Update( tutorial.STEP_OVERVIEW_LABORATORY )
	
	btnVideo = button.EqualeTexture( 545, 330, "techMapVideo", lab, -1, "", ShowVideoTechMap )
	btnSound = button.EqualeTexture( 372, 213, "techMapSound", lab, -1, "", ShowSoundTechMap )
	btnTech  = button.EqualeTexture( 749, 222,  "techMapAdvTech", lab, -1, "", ShowAdvancedTechMap )
	btnGenre = button.EqualeTexture( 73, 202,  "techMapGenre", lab, -1, "", ShowGenreTechMap )
end