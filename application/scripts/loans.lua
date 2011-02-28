local base = _G

module( "loans" )

local window = base.window
local button = base.button
local guienv = base.guienv
local tutorial = base.tutorial
local applic = base.applic

local company = nil 
local bankApp = nil
local windowLoan = nil
local tblLoans = nil
local lbMaxLoan = nil
local btnGetLoan = nil
local btnPayLoan = nil
local edLoan = nil

local function _Hide()
end


local function _FillLoansTable( tabler )
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

local function _GetLoan()
	bankApp:CreateLoan( company.name, edLoan.text, 14, 10 )
	_FillLoansTable( tblLoans )
	
	local summ = bankApp:GetMaxCompanyLoan( company.name )
	base.LogScript( "loan for "..company.name.. " is $"..summ )
	lbMaxLoan.text = "Доступная сумма: $"..summ
end

local function _ReturnLoan()

end

function Show()
	bankApp = applic.bank
	company = applic.playerCompany
	
	windowLoan = window.fsWindow( "media/maps/bank_select.png", _Hide )
	
	local summ = bankApp:GetMaxCompanyLoan( company.name )
	base.LogScript( "loan for "..company.name .. " is "..summ )
	
	edLoan = guienv:AddEdit( summ, 10, 20, 190, 40,  -1, windowLoan )
	
	btnGetLoan = guienv:AddButton( 10, 80, "140+", "20+", windowLoan, -1, "Взять кредит" )
	btnGetLoan.action = _GetLoan
	
	btnPayLoan = guienv:AddButton( 160, 80, "140+", "20+", windowLoan, -1, "Вернуть кредит" )
	btnPayLoan.action = _ReturnLoan
	
	lbMaxLoan = guienv:AddLabel( "Доступная сумма: "..summ, 10, 110, "280+", "20+", -1, windowLoan )
	
	local tmpWnd = guienv:AddWindow( "media/tableLoanBg.png", 10, 140, "60e", "60e", -1, windowLoan )
	tmpWnd.drawBody = false 
	tmpWnd.dragable = false

	tblLoans = guienv:AddTable( 0, 0, "0e", "0e", -1, tmpWnd )
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
	
	_FillLoansTable( tblLoans )
end
