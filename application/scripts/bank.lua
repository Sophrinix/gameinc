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
local tblLoans = nil
local lbMaxLoan = nil
local btnGetLoan = nil
local btnPayLoan = nil
local edLoan = nil

btnLoan = nil
btnDebt = nil
btnBridge = nil

function Show()
	bankApp = applic.bank
	company = applic.playerCompany
	
	if bankWindow then
		bankWindow.visible = true
	else
		bankWindow = window.fsWindow( "media/maps/bank_normal.png", Hide )
	end
	
	tutorial.Update( tutorial.STEP_OVERVIEW_BANK )
	
	--get loan
	btnLoan = button.EqualeTexture( 80, 402, "loans", bankWindow, -1, "", ShowLoans )
	--deposit	
	btnDebt = button.EqualeTexture( 258, 301, "deposit", bankWindow, -1, "", ShowDeposits )
	
	btnBridge = button.EqualeTexture( 300, 300, "", bankWindow, -1, "", base.bridge.Show )
	
	guienv:FadeAction( base.FADE_TIME, false, false )			
	guienv:AddTimer( base.AFADE_TIME, FadeEnterAction )
end

function FillLoansTable( tabler )
	tabler:ClearRows()

	for i=1, bankApp.loansNumber do
		local loanID = bankApp:GetLoanID( i-1 )
		
		if bankApp:GetLoanCompanyName( loanID ) == company.name then
			local idx = tabler:AddRow( tabler.rowCount )
			tabler:SetCellText( idx, 0, loanID, 0xff, 0xff, 0, 0 )
			tabler:SetCellText( idx, 1, bankApp:GetLoanStartSumm( loanID ), 0xff, 0xff, 0, 0 )
			tabler:SetCellText( idx, 2, bankApp:GetLoanMoneyToClose( loanID ), 0xff, 0xff, 0, 0 )
			tabler:SetCellText( idx, 3, bankApp:GetLoanMoneyPerMonth( loanID ), 0xff, 0xff, 0, 0 )
			tabler:SetCellText( idx, 4, bankApp:GetLoanMonthToEnd( loanID ), 0xff, 0xff, 0, 0 )
		end
	end
end

function HideLoans()
	windowLoan:Remove()
	windowLoan = nil
end

function ShowLoans()
	
	if windowLoan == nil then
		windowLoan = window.fsWindow( "media/maps/bank_select.png", HideLoans )
		
		local summ = bankApp:GetMaxCompanyLoan( company.name )
		base.LogScript( "loan for "..company.name .. " is "..summ )
		
		edLoan = guienv:AddEdit( summ, 10, 20, 190, 40,  -1, windowLoan )
		
		btnGetLoan = guienv:AddButton( 10, 80, "140+", "20+", windowLoan, -1, "Взять кредит" )
		btnGetLoan.action = GetLoan
		
		btnPayLoan = guienv:AddButton( 160, 80, "140+", "20+", windowLoan, -1, "Вернуть кредит" )
		btnPayLoan.action = ReturnLoan
		
		lbMaxLoan = guienv:AddLabel( "Доступная сумма: "..summ, 10, 110, "280+", "20+", -1, windowLoan )
		
		local image = guienv:AddImage( 10, 140, "60e", "60e", windowLoan, -1, "" )
		image:SetImage( "media/tableLoanBg.png" )
		image.scaleImage = true
		image.useAlphaChannel = true

		tblLoans = guienv:AddTable( 10, 140, "10e", "60e", -1, windowLoan )
		local width, _ = tblLoans:GetSize()
		
		tblLoans:AddColumn( "ID", -1 );
		tblLoans:SetColumnWidth( 0, 20 );	
		
		tblLoans:AddColumn( "Старт", -1 );
		tblLoans:SetColumnWidth( 1, width / 5 );	
		
		tblLoans:AddColumn( "Осталось", -1 );
		tblLoans:SetColumnWidth( 2, width / 5 );	
		
		tblLoans:AddColumn( "В месяц", -1 );
		tblLoans:SetColumnWidth( 3, width / 5 );	
		
		tblLoans:AddColumn( "Месяцев", -1 );
		tblLoans:SetColumnWidth( 4, width / 5 );	
	else
		windowLoan.visible = true
	end
	
	FillLoansTable( tblLoans )
end

function GetLoan()
	bankApp:CreateLoan( company.name, edLoan.text, 14, 10 )
	FillLoansTable( tblLoans )
	
	local summ = bankApp:GetMaxCompanyLoan( company.name )
	base.LogScript( "loan for "..company.name.. " is $"..summ )
	lbMaxLoan.text = "Доступная сумма: $"..summ
end

function FadeEnterAction()
	bankWindow.visible = true
	guienv:FadeAction( base.FADE_TIME, true, true )
end

function FadeExitAction()
	bankWindow:Remove()
	bankWindow = nil
	guienv:FadeAction( base.FADE_TIME, true, true )
end

function Hide()
	guienv:FadeAction( base.FADE_TIME, false, false )			
	guienv:AddTimer( base.AFADE_TIME, FadeExitAction )	
end