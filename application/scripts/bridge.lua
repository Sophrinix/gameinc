local base = _G

module( "bridge" )

local scrWidth = base.scrWidth
local scrHeight = base.scrHeight
local button = base.button
local tutorial = base.tutorial
local guienv = base.guienv
local applic = base.applic
local company = nil
local mainWindow = nil
local tblCompanies = nil


function FadeEnterAction()
	mainWindow.visible = true
	guienv:FadeAction( base.FADE_TIME, true, true )
	guienv:AddTimer( base.AFADE_TIME, "guienv:FadeAction( 0, true, true )" )	
end

function FadeExitAction()
	mainWindow:Remove()
	mainWindow = nil
	guienv:FadeAction( base.FADE_TIME, true, true )
end

function Hide()
	guienv:FadeAction( base.FADE_TIME, false, false )			
	guienv:AddTimer( base.AFADE_TIME, "bridge.FadeExitAction()" )	
end

function FillTableCompanies()
	tblCompanies:ClearRows()

	for i=1, applic.companyNumber do
		local cmp = applic.GetCompany( i-1 )
		
		local idx = tblCompanies:AddRow( tblCompanies:GetRowCount() )
		tblCompanies:SetCellText( idx, 0, cmp.name, 0xff, 0xff, 0, 0 )
		tblCompanies:SetCellText( idx, 1, cmp.profitLastYear, 0xff, 0xff, 0, 0 )
		tblCompanies:SetCellText( idx, 2, cmp.pieCost, 0xff, 0xff, 0, 0 )
		tblCompanies:SetCellText( idx, 3, cmp.dividend, 0xff, 0xff, 0, 0 )
		tblCompanies:SetCellText( idx, 4, 0, 0xff, 0xff, 0, 0 )
	end
end

function Show()
	company = base.applic.playerCompany
	
	if mainWindow == nil then	
		mainWindow = window.fsWindow( "media/maps/bank_select.png", Hide )
		
		tblCompanies = guienv:AddTable( 10, 135, "10e", "10e", -1, mainWindow )
		tblCompanies:SetRowHeight( 24 )
		tblCompanies:AddColumn( "Название", -1 )
		tblCompanies:SetColumnWidth( 0, 100 )
		tblCompanies:AddColumn( "Прибыль", -1 )
		tblCompanies:SetColumnWidth( 1, 100 )
		tblCompanies:AddColumn( "Стоимость акции", -1 )
		tblCompanies:SetColumnWidth( 2, 100 )
		tblCompanies:AddColumn( "Дивиденды", -1 )
		tblCompanies:SetColumnWidth( 3, 100 )
		tblCompanies:AddColumn( "Изменение", -1 )
		tblCompanies:SetColumnWidth( 4, 100 )
	end
end
