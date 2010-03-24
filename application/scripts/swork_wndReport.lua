local sceneManager = CLuaSceneManager( NrpGetSceneManager() )
local guienv = CLuaGuiEnvironment( NrpGetGuiEnvironment() )

local WND_WIDTH = 800
local WND_HEIGHT = 600
local STEP = 5 
local reportCreater
local BUTTON_CONNECT = "_ReportButtonConnect_"
local TABLE_REPORT = "_ReportTable_"
local EDIT_QUERY = "_EditQuery_"

function sworkWindowReportToggleVisible( ptr )

	local windowg = CLuaWindow( guienv:GetElementByName( WINDOW_REPORT_NAME ) )
	
	if windowg:Empty() == 1 then	
		sworkWindowReportCreate( nil )
	else
		sworkWindowReportClose( nil )
	end

end

function sworkWindowReportCreate( ptr )

	local windowg = CLuaWindow( guienv:AddWindow( "", 
	                                              -WND_WIDTH, 100, 
												  0, 100 + WND_HEIGHT, 
												  -1, 
												  guienv:GetRootGUIElement() ) )
	
	Log({src=SCRIPT, dev=ODS}, "ќкна отчетов нет, создаем заново" )
	
	windowg:SetDraggable( true )
	windowg:SetName( WINDOW_REPORT_NAME )

	local button = CLuaButton( windowg:GetCloseButton() )
	button:SetVisible( false )
	
	button:SetObject( guienv:AddButton( WND_WIDTH - 15, 0, WND_WIDTH, 15, windowg:Self(), -1, "X") )
	button:SetAction( "sworkWindowReportClose" )
	
	button:SetObject( guienv:AddButton( 5, 20, 5 + 150, 20 + 20, windowg:Self(), -1, "Connect to DB" ) )
	button:SetName( BUTTON_CONNECT )
	button:SetAction( "sworkReportConnectToLogDb" )
	
	reportCreater = CLuaReport( "localdb", "root", "mamboo", "server" )
	
	if reportCreater:Open() then
		button:SetText( "Connected" )
	else
		button:SetText( "Not connected" )
	end
	
	button:SetObject( guienv:AddButton( 5, 45, 70, 45 + 65, windowg:Self(), -1, "Sel.Column" ) )
	button:SetAction( "sworkReportSelectColumn" )
	
	local repTable = CLuaTable( guienv:AddTable( 160, 20, WND_WIDTH-5, WND_HEIGHT-30, ID_REPORT_TABLE, windowg:Self() ) )
	repTable:SetName( TABLE_REPORT )
	
	local editBox = CLuaEdit( guienv:AddEdit( "", 5, WND_HEIGHT - 25, WND_WIDTH - 5, WND_HEIGHT - 5, -1, windowg:Self() ) )
	editBox:SetName( EDIT_QUERY )
	

	--создадим аниматор, который переместит окно в новые координаты, оставит его видимым, а потом удалитс€ сам
	guienv:AddMoveAnimator( windowg:Self(), 0, 150, STEP, true, true, false )

end

function sworkReportSelectColumn( ptr )

end

function sworkReportConnectToLogDb( ptr )

	local btn = CLuaButton( guienv:GetElementByName( BUTTON_CONNECT ) )
	local tbl = CLuaTable( guienv:GetElementByName( TABLE_REPORT ) )
	
	if reportCreater:IsOpen() and reportCreater:Query() then
		
		if reportCreater:GetRowCount() > 1000 then
		    
			return 0
		end
		
		while tbl:GetColumnCount() > 0 do
			tbl:RemoveColumn( 0 )
		end
		
		local fieldCount = reportCreater:GetFieldCount()-1
		for i=0, fieldCount do
			tbl:AddColumn( reportCreater:GetFieldName( i ), -1 )
		end

		local rowIndex = 0
		local fieldPos = 0
		local rowPos = 0
		for rowPos=0, reportCreater:GetRowCount() - 1 do
			reportCreater:NextRow()
			rowIndex= tbl:AddRow( tbl:GetRowCount() )
		
			for fieldPos=0, fieldCount do
				tbl:SetCellText( rowPos, fieldPos, reportCreater:GetCellValue( fieldPos ), 0xff, 0xff, 0, 0 )
			end
		end
	end
		
end

function sworkWindowReportClose( ptr )

	local windowg = CLuaWindow( guienv:GetElementByName( WINDOW_REPORT_NAME ) )
	
	if windowg:Empty() == 1 then
		Log({src=SCRIPT, dev=ODS}, "Ќет окна отчетов чтобы его закрывать" )
	else	
	    reportCreater:Close()
		guienv:RemoveAllAnimators( windowg:Self() ) 
		guienv:AddMoveAnimator( windowg:Self(), -WND_WIDTH, 150, STEP, false, true, true )
	end	
	
end
