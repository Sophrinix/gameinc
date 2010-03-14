local guienv = CLuaGuiEnvironment( NrpGetGuiEnvironment() )
local company = CLuaCompany( NrpGetCompany() )
local bank = CLuaBank( NrpGetBank() )

function sworkCreateWindowLoanAction()

	local windowg = CLuaWindow( guienv:AddWindow( "Loan", 0, 0, 800, 600, -1, guienv:GetRootGUIElement() ) )
	
	local nameEdit = CLuaEdit( guienv:AddEdit( "", 30, 25, 100, 25 + 20, -1, windowg:Self() ) )
	nameEdit:SetName( WNDLOANACTION_GETLOAN_EDIT )
	
	local btn = CLuaButton( guienv:AddButton( 10, 80, 10 + 100, 80 + 20, windowg:Self(), -1, "Взять кредит" ) )
	button:SetAction( "sworkGetLoan" )
	
	btn:SetObject( guienv:AddButton( 10, 80, 10 + 100, 80 + 20, windowg:Self(), -1, "Взять кредит" ) )
	button:SetAction( "sworkReturnLoan" )
	
	guienv:AddLabel( "Кредит:"..company:GetLoan(), 5, 30, 5 + 40, 30 + 20, -1, windowg:Self() )
	guienv:AddLabel( "Доступная сумма: "..bank:GetMaximumLoan() - company:GetLoan(), 10, 110, 10 + 80, 110 + 20, -1, windowg:Self() )

	btn:SetObject( guienv:AddButton( 800 - 60, 600 - 30, 800 - 5, 600 - 10, windowg:Self(), -1, "Закрыть" ) )
	button:SetAction( "sworkWindowLoadActionClose" )

end
