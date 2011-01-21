local plant = NrpGetPlant()
local labelSpeed = nil

function sworkAppDayChange( ptr )
	local company = applic.playerCompany
	
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
	local company = applic.playerCompany
	
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
	local company = applic.playerCompany
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
	pda.Show( "Закончена работа над изобретением "..inv:GetName() )
end

function sworkPlayerCompanyReadyProject( ptr )
	local game = CLuaGame( ptr )
	pda.Show( "Закончена работа над проектом "..game:GetName() )
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
	pda.Show( "Завершена работа над модулем "..mod:GetName().." проекта "..mod:GetParent():GetName() )
end

function sworkUserMarketUpdated()
	pda.Show( "Обновление на рынке труда" )
end

function sworkReklameFinished( ptrReklame )
	local reklame = CLuaReklame( ptrReklame )
	pda.Show( "Закончилась рекламная кампания "..reklame:GetName() )
end

function sworkKeyboardEvent( ptr )
	local event = CLuaEvent( ptr )
	local charInput = event:GetChar()
	local keyDown = event:IsKeyDown()
	if  keyDown and ( charInput == "+" or charInput == "-" ) then
		if labelSpeed then labelSpeed:Remove() end
		local spd = applic:GetPauseBetweenStep()
		
		if charInput == "+" then 
			if spd > 100 then applic:SetPauseBetweenStep( spd - 100 ) end
		else 
			if spd < 1000 then applic:SetPauseBetweenStep( spd + 100 ) end
		end
		
		local dd = ( 1000 - applic:GetPauseBetweenStep() ) / 100
		labelSpeed = guienv:AddLabel( "Скорость игры " .. dd,
									  scrWidth / 2 - 100, scrHeight / 2 - 50, 
									  scrWidth / 2 + 100, scrHeight / 2 + 50, -1, 
									  guienv:GetRootGUIElement() )
									  
		guienv:AddBlenderAnimator( labelSpeed, 255, 10, 2000, false, true, false )
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