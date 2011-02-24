plant = applic.plant
labelSpeed = nil
balanceLabel = nil

function sworkAppDayChange( ptr )
	balanceLabel.text = "$"..applic.playerCompany.balance
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
	for i=1, company.userNumber do
		user = company:GetUser( i-1 )
		salary = user:GetParam( "salary" )
		user:AddParam( "balance", salary )
		needMoney = needMoney + salary
	end
	
	company:AddBalance( -needMoney )
end

function PayMoneyToInventions()
	local company = applic.playerCompany
	for index=1, company.inventionNumber do
		local invention = company:GetInvention( index-1 )
					
		company:AddBalance( -invention:GetMonthPay() )
		invention:ClearMonthPay()
	end
end

function sworkAppYearChange( ptr )

end

function sworkInventionFinished( ptr )
	local inv = CLuaTech( ptr )
	pda.Show( "Закончена работа над изобретением "..inv.name )
end

function sworkPlayerCompanyReadyProject( ptr )
	local game = CLuaGame( ptr )
	pda.Show( "Закончена работа над проектом "..game.name )
end

function sworkMainLoop( ptr )
	applic:UpdateGameTime( ID_DATETIME_LABEL )	
end

function sworkApplicationClose( ptr )
	NrpApplicationSave()
	applic:SaveBoxAddonsPrice()
	applic.pda:Save()
	plant:Save()
	NrpApplicationClose()
end

function sworkModuleFinished( ptrModule )
	local mod = CLuaDevelopModule( ptrModule )
	pda.Show( "Завершена работа над модулем "..mod.name.." проекта "..mod.parent.name )
end

function sworkUserMarketUpdated()
	pda.Show( "Обновление на рынке труда" )
end

function sworkReklameFinished( ptrReklame )
	local reklame = CLuaReklame( ptrReklame )
	pda.Show( "Закончилась рекламная кампания "..reklame.name )
end

local function localChangeSpeed( keyInput )
	if labelSpeed then labelSpeed:Remove() end
	
	if keyInput == 0xBB then 
		if applic.speed > 100 then applic.speed = applic.speed - 100 end
	else
		if applic.speed < 1000 then applic.spped = applic.speed + 100 end
	end
	
	local dd = ( 1000 - applic.speed ) / 100
	labelSpeed = guienv:AddLabel( "Скорость игры " .. dd, "40%", "45%", "20%+", "10%+", -1, guienv.root )
								  
	guienv:AddBlenderAnimator( labelSpeed, 255, 10, 2000, false, true, false )
end

function sworkKeyboardEvent( ptr )
	local event = CLuaEvent( ptr )
	local keyInput = event.key

	LogScript( "key event "..keyInput )
	if  event.keyDown and ( keyInput == 0xBD or keyInput == 0xBB ) then
		localChangeSpeed( keyInput )
	end
end

sceneManager:AddSceneFunction( SCENE_AFTER_END, sworkMainLoop )
sceneManager:AddSceneFunction( SCENE_KEY_INPUT_EVENT, sworkKeyboardEvent )

applic:AddLuaFunction( APP_DAY_CHANGE, sworkAppDayChange )
applic:AddLuaFunction( APP_MONTH_CHANGE, sworkAppMonthChange )
applic:AddLuaFunction( APP_YEAR_CHANGE, sworkAppYearChange )
applic:AddLuaFunction( APP_INVENTION_FINISHED, sworkInventionFinished )
applic:AddLuaFunction( APP_MODULE_FINISHED, sworkModuleFinished )
applic:AddLuaFunction( APP_REKLAME_FINISHED, sworkReklameFinished )
applic:AddLuaFunction( APP_USER_MARKETUPDATE, sworkUserMarketUpdated )
applic:AddLuaFunction( APP_PROJECT_FINISHED, sworkPlayerCompanyReadyProject )
