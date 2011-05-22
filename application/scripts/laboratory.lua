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

local function _CloseConfirmationWindow( ptr )
	btnOk:Remove()
	btnCancel:Remove()
	browser:Hide()
end


local function CreateTechSequence( tech )
	if tech.empty then
		return 
	end
	
	for i=1, tech.ftNumber do
		local internalName = tech:GetFutureTechInternalName( i-1 )
		Log( "ƒочерн€€ технологи€="..internalName.." –одительска€ технологи€="..tech.name )

		local futureTech = applic:GetTech( internalName );
		--такой технологии нет на рынке
		if futureTech == nil or futureTech.empty then
			--может у компании ведутс€ разработки этой технологии,
			--надо подменить добавл€емую технологию
			local techInvention = applic:GetInvention( internalName, company.name )
			
			--исследовани€ не ведутс€
			if techInvention.empty then			
				--будем отрисовывать только первую неисследованную технологию
				if not techMap:IsTechHaveUndiscoverChild( tech ) then
					--добавл€ем как неизвестную технологию
					techMap:AddTechnology( tech, internalName )
				end
			else 
				--добавим как исследование
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
	techMap.onActivate = TechSelected
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
	button.Stretch( "50e", "50e", "0e", "0e", "button_down", windowMap, -1, "", HideTechMap )
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
											selectedTech.name, selectedTech.description, selectedTech.status ) )
	    
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

local function _AssignInvention()
	local inventionName = techMap.selectedName
	
	base.LogScript( "Want start invention "..inventionName )
	local loadFile = base.updates.FindInventionLoadFile( inventionName )
	
	if inventionName == "" or loadFile == "" then
		base.LogScript( "Unknown invention" )
		return
	end
	
	company:StartInvention( loadFile )
	
	base.inventionManager.Show( inventionName, company.name )
	windowMap:Remove()
	windowMap = nil
	
	btnOk:Remove()
	btnCancel:Remove()
	browser:Hide()
end

function StartInvention()
	browser:Show()
	browser:Navigate( "media/html/unknownTechnology.htm" )
	
	btnOk = guienv:AddButton( "5%", 30, "45%+", "20+", browser.window, -1, "Ќачать исследовани€" )
	btnOk.action = _AssignInvention
	
	btnCancel = guienv:AddButton( "55%", 30, "45%+", "20+", browser.window, -1, "«акрыть" )
	btnCancel.action = _CloseConfirmationWindow
end

function ShowHelp()
	tutorial.Update( "lab/main" )
end

function Show()	
	lab = window.fsWindow( "laboratory.png", _Hide )

	base.rightPanel.AddYesNo( "’отите узнать больше о лаборатории?", ShowHelp, button.CloseParent )
	
	btnVideo = button.EqualeTexture( 545, 330, "techMapVideo", lab, -1, "", ShowVideoTechMap )
	btnSound = button.EqualeTexture( 372, 213, "techMapSound", lab, -1, "", ShowSoundTechMap )
	btnTech  = button.EqualeTexture( 749, 222,  "techMapAdvTech", lab, -1, "", ShowAdvancedTechMap )
	btnGenre = button.EqualeTexture( 73, 202,  "techMapGenre", lab, -1, "", ShowGenreTechMap )
end