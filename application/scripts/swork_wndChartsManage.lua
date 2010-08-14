require("elmid")
require("NerpaActionType")

local chartWidth = 380
local chartHeight = 200
local VISIBLE = true
local REM_SELF_WHEN_STOP = true
local REM_PARENT_WHEN_STOP = true
local STEP = 5 
local MAX_GRAPHIC = 4

local chart_snr = { } 
local chart_datatype = { }
local offset_point = { }

for i=0, MAX_GRAPHIC-1 do
	chart_snr[ i ] = 0
	chart_datatype[ i ] = 0
	offset_point[ i ] = 0
end

function sworkToggleChartWindowManageVisible( ptr )

	local wndManage = CLuaWindow( guienv:GetElementByName( WINDOW_CHART_MANAGE_NAME ) )
	
	if wndManage:Empty() == 1 then	
		sworkChartWindowManageCreate( nil )
	else
		sworkChartWindowManageClose( nil )
	end

end

function sworkChartWindowManageCreate( ptr )

	local windowg = guienv:AddWindow( "", 
	                                  -chartWidth, 100, 
									  0 + 10, 100 + chartHeight + 10, 
									  -1, 
									  guienv:GetRootGUIElement() )
	
	Log({src=SCRIPT, dev=ODS}, "Окна управления графиками нет, создаем заново" )
	
	windowg:SetDraggable( true )
	windowg:SetName( WINDOW_CHART_MANAGE_NAME )
			
	local closeButton = CLuaButton( windowg:GetCloseButton() )
	closeButton:SetVisible( false )
	
	closeButton:SetObject( guienv:AddButton( chartWidth + 10 - 15, 0, chartWidth + 10, 15, windowg:Self(), -1, "X") )
	closeButton:SetAction( "sworkChartWindowManageClose" )
	
	local hdrw = CLuaHardware( NrpGetHardwareManager() )
	local maxSonar = hdrw:GetSonarNumber()
	for i=0, MAX_GRAPHIC-1 do
	    guienv:AddLabel( "МА:К", 5, 20, 40, 40, -1, windowg:Self() )
		
		local cmbx = CLuaComboBox( guienv:AddComboBox( 	"",
														45 + i * 80, 20, 95 + i * 80, 40,
														20100 + i,
														windowg:Self() ) )
		cmbx:AddItem( "Нет", nil )
		for nSonar=1, maxSonar do 
			cmbx:AddItem( nSonar..":1", nil )
			cmbx:AddItem( nSonar..":2", nil )
			cmbx:AddItem( nSonar..":3", nil )
			cmbx:AddItem( nSonar..":4", nil )
		end
		
		cmbx:SetSelected( chart_snr[ i ] )
		
		guienv:AddLabel( "Тип", 5, 50, 40, 70, -1, windowg:Self() )
		
		cmbx:SetObject( guienv:AddComboBox( "",
											45 + i * 80, 50, 125 + i * 80, 70,
											20200 + i,
											windowg:Self() ) )
		
		cmbx:AddItem( "Нет", nil )
		cmbx:AddItem( "RF", nil );
		cmbx:AddItem( "Y", nil );
		cmbx:AddItem( "Z", nil );
		
		cmbx:SetSelected( chart_datatype[ i ] )
		
		guienv:AddLabel( "См.", 5, 80, 40, 100, -1, windowg:Self() )
		
		local editBox = CLuaEdit( guienv:AddEdit(  offset_point[ i ],
												   45 + i * 80, 80, 125 + i * 80, 100,
												   20300 + i,
												   windowg:Self() ) )
												 
		editBox:SetText( offset_point[ i ] )
											 
	end
	
	guienv:AddMoveAnimator( windowg:Self(), 0, 0, STEP, VISIBLE, REM_SELF_WHEN_STOP, not REM_PARENT_WHEN_STOP )
						
end

function sworkChartWindowManageClose( ptr )

	local windowg = CLuaWindow( guienv:GetElementByName( WINDOW_CHART_MANAGE_NAME ) )
	
	if windowg:Empty() == 1 then
		Log({src=SCRIPT, dev=ODS}, "Нет окна управления графиками чтобы его закрывать" )
		return 0
	end

	sworkChartWindowManageUpdate( nil )
	--создадим аниматор, который удалит окно при завершении перемешения
	guienv:AddMoveAnimator( windowg:Self(), -chartWidth, 100, STEP, not VISIBLE, REM_SELF_WHEN_STOP, REM_PARENT_WHEN_STOP )

end

function sworkChartWindowManageUpdate( ptr )

	--local windowg = CLuaWindow( guienv:GetElementByName( WINDOW_CHART_MANAGE_NAME ) )
	
	for i=0, MAX_GRAPHIC-1 do
		local cmbxsnr = CLuaComboBox( guienv:GetElementByID( 20100 + i ) )
		local cmbxdt = CLuaComboBox( guienv:GetElementByID( 20200 + i ) )
		
		chart_snr[ i ] = cmbxsnr:GetSelected()
		chart_datatype[ i ] = cmbxdt:GetSelected()
				
		if chart_snr[ i ] > 0 and chart_datatype[ i ] > 0 then
			local editBox = CLuaEdit( guienv:GetElementByID( 20300 + i ) )
			
			local chart = CLuaChart(  guienv:GetElementByName( CHART_ELEMENT_NAME ) )
			chart:SetOffsetPoints( i, 0, editBox:GetText() )
			
			offset_point[ i ] = editBox:GetText()
		end
		
		Log({src=SCRIPT, dev=ODS|CON}, i..": snr="..chart_snr[ i ].." dt="..chart_datatype[ i ].." offset="..offset_point[ i ] )
	end
	
end

function sworkUpdateGraphicsData( ptr )
	
	local chart = CLuaChart(  guienv:GetElementByName( CHART_ELEMENT_NAME ) )
	local hdrw = CLuaHardware( NrpGetHardwareManager() )
	
	for i=0, MAX_GRAPHIC-1 do
		if chart_snr[ i ] > 0 and chart_datatype[ i ] > 0 then
			local snr = CLuaSonar( hdrw:GetSonar( chart_snr[ i ] ) )
			local width, length = snr:GetDataSize( 0, CHNL_RFYZ );
			 
			if chart_datatype[ i ] == 1 and snr:IsDataUpdated( chart:Self(), 0, CHNL_RFYZ )	then
				Log({src=SCRIPT, dev=ODS}, "update snr="..chart_snr[ i ].." datatype="..chart_datatype[ i ] )
				
				chart:UpdateData( i, 
								  snr:GetData( chart:Self(), 0, CHNL_RFYZ ),
								  length, 
								  CHNL_RFYZ_ASRF )
			end
		end
	end

end