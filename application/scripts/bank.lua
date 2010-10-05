local base = _G

module( "bank" )

local guienv = base.guienv

local scrWidth = base.scrWidth
local scrHeight = base.scrHeight
local button = base.button
local tutorial = base.tutorial
local applic = base.applic

local company = nil 
local bankApp = nil
local windowLoan = nil
local bankWindow = nil

function Show()
	bankApp = applic:GetBank()
	company = applic:GetPlayerCompany()
	
	if bankWindow then
		bankWindow:SetVisible( true )
	else
		bankWindow = guienv:AddWindow( "media/bank_normal.png", 0, 0, scrWidth, scrHeight, -1, guienv:GetRootGUIElement() )
		bankWindow:SetDraggable( false )
		bankWindow:GetCloseButton():SetVisible( false )
		
		--adding closeButton
		button.Stretch( scrWidth - 80, scrHeight - 80, scrWidth, scrHeight, 
		 			    "button_down", bankWindow:Self(), -1, "",
						"./bank.Hide()" )
	end
	
	--get loan
	button.EqualeTexture( 80, 402, "loans", bankWindow:Self(), -1, "", "./bank.ShowLoans()" )
	--deposit	
	button.EqualeTexture( 258, 301, "deposit", bankWindow:Self(), -1, "", "./bank.ShowDeposits()" )
	
	guienv:FadeAction( base.FADE_TIME, false, false )			
	guienv:AddTimer( base.AFADE_TIME, "bank.FadeEnterAction()" )
end

function FadeEnterAction()
	bankWindow:SetVisible( true )
	guienv:FadeAction( base.FADE_TIME, true, true )
end

function ShowLoans( tabler )
	local tbl = base.CLuaTable( tabler )
	tbl:ClearRows()
	local loansNumber = bank:GetLoansNumber()
	if loansNumber > 0 then
		for i=0, loansNumber-1 do
			local loanID = bank:GetLoanID( i )
			
			if bankApp:GetLoanCompanyName( loanID ) == company:GetName() then
				local idx = tbl:AddRow( tbl:GetRowCount() )
				tbl:SetCellText( idx, 0, loanID, 0xff, 0xff, 0, 0 )
				tbl:SetCellText( idx, 1, bank:GetLoanStartSumm( loanID ), 0xff, 0xff, 0, 0 )
				tbl:SetCellText( idx, 2, bank:GetLoanMoneyToClose( loanID ), 0xff, 0xff, 0, 0 )
				tbl:SetCellText( idx, 3, bank:GetLoanMoneyPerMonth( loanID ), 0xff, 0xff, 0, 0 )
				tbl:SetCellText( idx, 4, bank:GetLoanMonthToEnd( loanID ), 0xff, 0xff, 0, 0 )
			end
		end
	end
end

function Hide()
	bankWindow:Remove()
	bankWindow = nil
end

function HideLoans()
	windowLoan:Remove()
	windowLoan = nil
end

function ShowLoans()
	
	if windowLoan == nil then
		windowLoan = guienv:AddWindow( "media/bank_select.png", 0, 0, scrWidth, scrHeight, -1, guienv:GetRootGUIElement() )
		windowLoan:SetDraggable( false )
		windowLoan:GetCloseButton():SetAction( "./bank.HideLoans()" )
	else
		windowLoan:SetVisible( true )	
	end
	
	local summ = bank:GetMaxCompanyLoan( company:GetName() )
	Log({src=SCRIPT, dev=ODS|CON}, summ )
	
	local edit = guienv:AddEdit(  summ, 10, 20, 190, 40,
								  -1, windowLoan:Self() )
	
	local button = guienv:AddButton( 10, 80, 10 + 140, 80 + 20, windowLoan:Self(), -1, "Взять кредит" )
	button:SetAction( "sworkGetLoan" )
	
	button = guienv:AddButton( 160, 80, 160 + 140, 80 + 20, windowLoan:Self(), -1, "Вернуть кредит" )
	button:SetAction( "sworkReturnLoan" )
	
	local label = guienv:AddLabel( "Доступная сумма: "..summ, 10, 110, 10 + 280, 110 + 20, -1, windowLoan:Self() )
	label:SetName( WNDLOANACTION_MAXSUM_LABEL )
	
	local image = guienv:AddImage( 10, 140, scrWidth - 10, scrHeight - 60, windowLoan:Self(), -1, "" )
	image:SetImage( "media/tableLoanBg.png" )
	image:SetScaleImage( true )
	image:SetUseAlphaChannel( true )

	local width = 0
	local height = 0
	local tabler = guienv:AddTable( 10, 140, scrWidth - 10,  scrHeight - 60, -1, windowLoan:Self() )
	tabler:SetName( WNDLOANACTION_TABLE )
	width, height = tabler:GetSize()
	
	tabler:AddColumn( "ID", -1 );
	tabler:SetColumnWidth( 0, 20 );	
	
	tabler:AddColumn( "Старт", -1 );
	tabler:SetColumnWidth( 1, width / 4 );	
	
	tabler:AddColumn( "Осталось", -1 );
	tabler:SetColumnWidth( 2, width / 4 );	
	
	tabler:AddColumn( "В месяц", -1 );
	tabler:SetColumnWidth( 3, width / 4 );	
	
	tabler:AddColumn( "Месяцев", -1 );
	tabler:SetColumnWidth( 4, width / 4 );	
	
	sworkShowLoans( tabler:Self() )
end

function GetLoan()

	local edit = base.CLuaEdit( guienv:GetElementByName( WNDLOANACTION_GETLOAN_EDIT ) )
	bank:CreateLoan( company:GetName(), edit:GetText(), 14, 10 )
	
	sworkShowLoans( guienv:GetElementByName( WNDLOANACTION_TABLE ) )
	
	local label = base.CLuaLabel( guienv:GetElementByName( WNDLOANACTION_MAXSUM_LABEL ) )
	local summ = bank:GetMaxCompanyLoan( company:GetName() )
	Log({src=SCRIPT, dev=ODS|CON}, summ )
	label:SetText(  "Доступная сумма: "..summ )
end