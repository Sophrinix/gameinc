local plant = NrpGetPlant()

function sworkAppDayChange( ptr )
	local company = applic:GetPlayerCompany()
	
	local userLabel = CLuaLabel( guienv:GetElementByID( ID_USERNAME_LABEL ) )
	local name = company:GetName()
	local summ = company:GetBalance()
	userLabel:SetText( name..":   $"..summ)
end

function sworkAppMonthChange()
	applic:CreateNewFreeUsers()
	updates.CheckNewReklames( true )
	updates.CheckNewTechs() 
	updates.CheckGameBoxAddons()
	updates.CheckNewGames()
	
	PayMoneyToInventions()
	PaySalaryToWorkers()
end

function PaySalaryToWorkers()
	local company = applic:GetPlayerCompany()
	
	local needMoney = 0
	local user = nil
	local salary = 0
	for i=1, company:GetUserNumber() do
		user = company:GetUser( i-1 )
		salary = user:GetParam( "salary" )
		user:AddParam( "balance", salary )
		needMoney = needMoney + salary
	end
	
	company:AddBalance( -needMoney )
end

function PayMoneyToInventions()
	local company = applic:GetPlayerCompany()
	for index=1, company:GetInventionNumber() do
		local invention = company:GetInvention( index-1 )
					
		company:AddBalance( -invention:GetMonthPay() )
		invention:ClearMonthPay()
	end
end

function sworkAppYearChange( ptr )

end

function sworkInventionFinished( ptr )
	local inv = CLuaTech( ptr )
	pda.Show( "��������� ������ ��� ������������ "..inv:GetName() )
end

function sworkPlayerCompanyReadyProject( ptr )
	local game = CLuaGame( ptr )
	pda.Show( "��������� ������ ��� �������� "..game:GetName() )
end

function sworkMainLoop( ptr )
	applic:UpdateGameTime( ID_DATETIME_LABEL )	
end

function sworkApplicationClose( ptr )
	NrpApplicationSave()
	applic:SaveBoxAddonsPrice()
	applic:GetPda():Save()
	plant:Save()
	NrpApplicationClose()
end

function sworkDrawOnTopWindows( ptr )
	for i=1, #alwaysOnTopWindow do
		alwaysOnTopWindow[ i ]:Draw()
		
		guienv:BringToFront( alwaysOnTopWindow[ i ]:Self() )
	end
end

function sworkModuleFinished( ptrModule )
	local mod = CLuaDevelopModule( ptrModule )
	pda.Show( "��������� ������ ��� ������� "..mod:GetName().." ������� "..mod:GetParent():GetName() )
end

function sworkUserMarketUpdated()
	pda.Show( "���������� �� ����� �����" )
end

function sworkReklameFinished( ptrReklame )
	local reklame = CLuaReklame( ptrReklame )
	pda.Show( "����������� ��������� �������� "..reklame:GetName() )
end

function sworkKeyboardEvent( ptr )
	local event = CLuaEvent( ptr )
	local charInput = event:GetChar()
	local keyDown = event:IsKeyDown()
	if  keyDown and charInput == "+" then
		local spd = applic:GetPauseBetweenStep()
		if spd > 100 then
			applic:SetPauseBetweenStep( spd - 100 )
			local lb = guienv:AddLabel( "�������� ���� "..spd, scrWidth / 2 - 100, scrHeight / 2 - 50, 
										 scrWidth / 2 + 100, scrHeight / 2 + 50, -1, guienv:GetRootGUIElement() )
			guienv:AddBlenderAnimator( lb:Self(), 255, 10, 2000, false, false, true )
		end 
	elseif  keyDown and charInput == "-" then
		local spd = applic:GetPauseBetweenStep()
		if spd < 1000 then
			applic:SetPauseBetweenStep( spd + 100 )
			local lb = guienv:AddLabel( "�������� ���� "..spd, scrWidth / 2 - 100, scrHeight / 2 - 50, 
					  					scrWidth / 2 + 100, scrHeight / 2 + 50, -1, guienv:GetRootGUIElement() )
			guienv:AddBlenderAnimator( lb:Self(), 255, 10, 2000, false, false, true )
		end 
	end
end

sceneManager:AddSceneFunction( SCENE_AFTER_END, "sworkMainLoop" )
sceneManager:AddSceneFunction( SCENE_AFTER_RENDER, "sworkDrawOnTopWindows" )
sceneManager:AddSceneFunction( SCENE_KEY_INPUT_EVENT, "sworkKeyboardEvent" )

applic:AddLuaFunction( APP_DAY_CHANGE, "sworkAppDayChange" )
applic:AddLuaFunction( APP_MONTH_CHANGE, "sworkAppMonthChange" )
applic:AddLuaFunction( APP_YEAR_CHANGE, "sworkAppYearChange" )
applic:AddLuaFunction( APP_INVENTION_FINISHED, "sworkInventionFinished" )
applic:AddLuaFunction( APP_MODULE_FINISHED, "sworkModuleFinished" )
applic:AddLuaFunction( APP_REKLAME_FINISHED, "sworkReklameFinished" )
applic:AddLuaFunction( APP_USER_MARKETUPDATE, "sworkUserMarketUpdated" )