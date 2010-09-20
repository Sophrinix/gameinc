local company = nil 
local bank = applic:GetBank()
local windowLoan = nil

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

function sworkCloseWindowLoanAction( ptr )
	windowLoan:Remove()
	windowLoan = nil
end

function sworkCreateWindowLoanAction()
	company = applic:GetPlayerCompany()
	if windowLoan == nil then
		windowLoan = guienv:AddWindow( "media/bank_select.png", 0, 0, scrWidth, scrHeight, -1, guienv:GetRootGUIElement() )
		windowLoan:SetDraggable( false )
		local closeBtn = windowLoan:GetCloseButton()
		closeBtn:SetAction( "sworkCloseWindowLoanAction" )
	else
		windowLoan:SetVisible( true )	
	end
	
	local summ = bank:GetMaxCompanyLoan( company:GetName() )
	Log({src=SCRIPT, dev=ODS|CON}, summ )
	
	local edit = guienv:AddEdit(  summ, 10, 20, 190, 40,
								  -1, windowLoan:Self() )
	edit:SetName( WNDLOANACTION_GETLOAN_EDIT )
	
	local button = guienv:AddButton( 10, 80, 10 + 140, 80 + 20, windowLoan:Self(), -1, "����� ������" )
	button:SetAction( "sworkGetLoan" )
	
	button = guienv:AddButton( 160, 80, 160 + 140, 80 + 20, windowLoan:Self(), -1, "������� ������" )
	button:SetAction( "sworkReturnLoan" )
	
	local label = guienv:AddLabel( "��������� �����: "..summ, 10, 110, 10 + 280, 110 + 20, -1, windowLoan:Self() )
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
	
	tabler:AddColumn( "�����", -1 );
	tabler:SetColumnWidth( 1, width / 4 );	
	
	tabler:AddColumn( "��������", -1 );
	tabler:SetColumnWidth( 2, width / 4 );	
	
	tabler:AddColumn( "� �����", -1 );
	tabler:SetColumnWidth( 3, width / 4 );	
	
	tabler:AddColumn( "�������", -1 );
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
	label:SetText(  "��������� �����: "..summ )
end