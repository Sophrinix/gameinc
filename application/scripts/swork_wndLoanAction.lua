local guienv = CLuaGuiEnvironment( NrpGetGuiEnvironment() )
local app = CLuaApplication( NrpGetApplication() )
local company = CLuaCompany( app:GetPlayerCompany() )
local bank = CLuaBank( app:GetBank() )

function sworkShowLoans( tabler )
	local tbl = CLuaTable( tabler )
	tbl:ClearRows()
	local loansNumber = bank:GetLoansNumber()
	if loansNumber > 0 then
		for i=0, loansNumber-1 do
			local loanID = bank:GetLoanID( i )
			
			if bank:GetLoanCompanyName( loanID ) == company:GetName() then
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

function sworkCreateWindowLoanAction()

	local windowg = CLuaWindow( guienv:AddWindow( "Loan", 0, 0, 800, 600, -1, guienv:GetRootGUIElement() ) )
	windowg:SetName( WINDOW_LOAN_ACTION_NAME )
	
	local summ = bank:GetMaxCompanyLoan( company:GetName() )
	Log({src=SCRIPT, dev=ODS|CON}, summ )
	
	local edit = CLuaEdit( guienv:AddEdit(  summ,
				 						    10, 20, 190, 40,
											-1,
											windowg:Self() ) )
	edit:SetName( WNDLOANACTION_GETLOAN_EDIT )
	
	local button = CLuaButton( guienv:AddButton( 10, 80, 10 + 140, 80 + 20, windowg:Self(), -1, "Взять кредит" ) )
	button:SetAction( "sworkGetLoan" )
	
	button:SetObject( guienv:AddButton( 160, 80, 160 + 140, 80 + 20, windowg:Self(), -1, "Вернуть кредит" ) )
	button:SetAction( "sworkReturnLoan" )
	
	local label = CLuaLabel( guienv:AddLabel( "Доступная сумма: "..summ, 10, 110, 10 + 280, 110 + 20, -1, windowg:Self() ) )
	label:SetName( WNDLOANACTION_MAXSUM_LABEL )
	
	local width = 0
	local height = 0
	local tabler = CLuaTable( guienv:AddTable( 10, 140, 790, 600 - 50, -1, windowg:Self() ) )
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

function sworkGetLoan( ptr )

	local edit = CLuaEdit( guienv:GetElementByName( WNDLOANACTION_GETLOAN_EDIT ) )
	bank:CreateLoan( company:GetName(), edit:GetText(), 14, 10 )
	
	sworkShowLoans( guienv:GetElementByName( WNDLOANACTION_TABLE ) )
	
	local label = CLuaLabel( guienv:GetElementByName( WNDLOANACTION_MAXSUM_LABEL ) )
	local summ = bank:GetMaxCompanyLoan( company:GetName() )
	Log({src=SCRIPT, dev=ODS|CON}, summ )
	label:SetText(  "Доступная сумма: "..summ )
end