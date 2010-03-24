require("elmid")
require("NerpaActionType")

local guienv = CLuaGuiEnvironment( NrpGetGuiEnvironment() )
local sceneManager = CLuaSceneManager( NrpGetSceneManager() )

local chartWidth = 400
local chartHeight = 300
local STEP = 5

function sworkToggleGraphicWindowVisible( ptr )

	local windowg = CLuaWindow( guienv:GetElementByName( WINDOW_CHART_NAME ) )
	
	if windowg:Empty() == 1 then	
		sworkChartWindowCreate( nil )
	else
		sworkChartWindowClose( nil )
	end

end

function sworkChartWindowCreate( ptr )

	local windowg = CLuaWindow( guienv:AddWindow( "", 
	                                              -chartWidth, 100, 
												  0 + 10, 100 + chartHeight + 10, 
												  -1, 
												  guienv:GetRootGUIElement() ) )
	
	Log({src=SCRIPT, dev=ODS}, "Окна графиков нет, создаем заново" )
	
	windowg:SetDraggable( true )
	windowg:SetName( WINDOW_CHART_NAME )
			
	local closeButton = CLuaButton( windowg:GetCloseButton() )
	closeButton:SetVisible( false )
	
	closeButton:SetObject( guienv:AddButton( chartWidth + 10 - 15, 0, chartWidth + 10, 15, windowg:Self(), -1, "X") )
	closeButton:SetAction( "sworkChartWindowClose" )
	
	closeButton:SetObject( guienv:AddButton( chartWidth + 10 - 45, 0, chartWidth + 10 - 15, 15, windowg:Self(), -1, "Упр.") )
	closeButton:SetAction( "sworkToggleChartWindowManageVisible" )

	
	local chart = CLuaChart( guienv:AddChart( 5, 5, chartWidth, chartHeight, -1, windowg:Self() ) )
	chart:SetAxisAutomatic( AN_BOTTOM, 1 )
	chart:SetAxisAutomatic( AN_LEFT, 1 )
	chart:SetName( CHART_ELEMENT_NAME )
	
	local numSerie = chart:AddSerie( ST_LINE_SERIE )
	
	local cnt=0
	for cnt=0,6000 do
		chart:AddPoint( numSerie, cnt, cnt )
	end
	
	chart:Update()
	windowg:SetVisible( true )
	
	--создадим аниматор, который переместит окно в новые координаты, оставит его видимым, а потом удалится сам
	guienv:AddMoveAnimator( windowg:Self(), 0, 150, STEP, true, true, false )
	sceneManager:AddSceneFunction( SCENE_AFTER_END, "sworkUpdateGraphicsData" )
	
end


function sworkChartWindowClose( ptr )

	local windowg = CLuaWindow( guienv:GetElementByName( WINDOW_CHART_NAME ) )
	
	if windowg:Empty() == 1 then
		Log({src=SCRIPT, dev=ODS}, "Нет окна графиков чтобы его закрывать" )
	else
		guienv:RemoveAnimators( windowg:Self() ) 
		guienv:AddMoveAnimator( windowg:Self(), -chartWidth, 150, STEP, false, true, true )
	end
	
	sceneManager:RemoveSceneFunction( SCENE_AFTER_END, "sworkUpdateGraphicsData" )
	
end