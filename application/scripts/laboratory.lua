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
		Log({src=base.SCRIPT, dev=base.ODS|base.CON}, "�������� �������w���="..internalName.." ������������ ����������="..tech:GetName() )

		local futureTech = applic:GetTech( internalName );
		--����� ���������� ��� �� �����
		if futureTech:Empty() == 1 then
			--����� � �������� ������� ���������� ���� ����������,
			--���� ��������� ����������� ����������
			local techInvention = applic:GetInvention( internalName, company:GetName() )
			
			--������������ �� �������
			if techInvention:Empty() == 1 then
				--��������� ��� ����������� ����������
				techMap:AddTechnology( tech, internalName )
			else 
				techMap:AddTechnology( tech, techInvention )			
			end
		else
			--����� ���������� ���� �� ����� � ���� ���������
			--������ ��������
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
	company = applic:GetPlayerCompany()
	windowMap = guienv:AddWindow( "media/maps/laboratory_select.png", 0, 0, scrWidth, scrHeight, -1, guienv:GetRootGUIElement() )
	windowMap:SetDraggable( false )
	windowMap:GetCloseButton():SetVisible( false )
	
	base.LogScript( "sworkCreateGenreTechMapWindow="..company:GetName() )
	
	techMap = guienv:AddTechMap( 10, 40, scrWidth - 10, scrHeight - 10, -1, windowMap:Self() )
	techMap:AddLuaFunction( base.GUIELEMENT_SELECTED_AGAIN, "./laboratory.TechSelected()" )
	techMap:SetDrawBack( false )
	
	local tech = nil
	for i=1, applic:GetTechNumber() do
	    tech = applic:GetTech( i-1 )
		
		if tech:GetTechGroup() == typef and not tech:HaveRequireTech() then
			techMap:AddTechnology( nil, tech )
			CreateTechSequence( tech )
		end
	end
	
	--adding closeButton
	button.Stretch( scrWidth - 80, scrHeight - 80, scrWidth, scrHeight, 
	 			    "button_down", windowMap:Self(), -1, "",
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
		--���������� ��� � ����
	    selectedTech = base.CLuaTech( techMap:GetSelectedObject() )
		base.LogScript( base.string.format( "������� ����������=%s ��������=%s ������=%s", 
											selectedTech:GetName(), selectedTech:GetDescriptionLink(), selectedTech:GetStatus() ) )
	    
	    browser:Show()
	    browser:Navigate( selectedTech:GetDescriptionLink() )
	elseif type == base.TS_PROJECT then
		--��� ���������� ������ ��������� ���������
		--�� ��������� ����� ������������ ������� ������������ ������ ������������...
		StartInvention()
	elseif type == base.TS_INDEVELOP then
		selectedTech = base.CLuaInvention( techMap:GetSelectedObject() )
		base.inventionManager.Show( selectedTech:GetInternalName(), company:GetName() )
	end
end

function StartInvention()
	browser:Show()
	browser:Navigate( "media/html/unknownTechnology.htm" )
	
	btnOk = guienv:AddButton( 10, 30, 190, 30 + 20, browser:GetWindow(), -1, "������ ������������" )
	btnOk:SetAction( "./laboratory.AssignInvention()" )
	
	btnCancel = guienv:AddButton( 200, 30, 390, 30 + 20, browser:GetWindow(), -1, "�������" )
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