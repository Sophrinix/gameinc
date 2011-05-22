labelSpeed = nil

function sworkAppDayChange( ptr )
	updates.CheckNewEvents()
end

function sworkAppMonthChange()
	applic:CreateNewFreeUsers()
	updates.CheckNewReklames( true )
	updates.CheckNewTechs() 
	updates.CheckGameBoxAddons()
	updates.CheckNewGames()
	updates.CheckNewCompanies()
	
	PayMoneyToInventions()
	PaySalaryToWorkers()
end

function PaySalaryToWorkers()
	local company = applic.playerCompany
	
	local needMoney = 0
	local user = nil
	local salary = 0
	
	--подсчет суммы для выплаты зарплаты
	for i=1, company.userNumber do
		user = company:GetUser( i-1 )
		--выплатим зарплату
		salary = user:GetParam( "salary" )
		user:AddParam( "balance", salary, false )
		needMoney = needMoney + salary
		
		--снижение ожидания премии
		salary = user:GetParam( "lastAward" )
		user:SetParam( "lastAward", salary / 2 )
	end
	
	company:AddBalance( "Выплата зарплат сотрудникам", -needMoney )
end

function PayMoneyToInventions()
	local company = applic.playerCompany
	for index=1, company.inventionNumber do
		local invention = company:GetInvention( index-1 )
					
		company:AddBalance( "Инвестиции в развитие", -invention:GetMonthPay() )
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

function sworkApplicationClose( ptr )
	NrpApplicationSave()
	applic:SaveBoxAddonsPrice()
	applic.pda:Save()
	applic.plant:Save()
	NrpApplicationClose()
end

function sworkModuleFinished( project, ptrModule )
	local mod = CLuaDevelopModule( ptrModule )
	pda.Show( "Завершена работа над модулем "..mod.name.." проекта "..mod.parent.name )
end

function sworkModuleTestedFinished( project, ptrModule )
	local mod = CLuaDevelopModule( ptrModule )
	pda.Show( "Завершено тестирование модуля "..mod.name.." проекта "..mod.parent.name )	
end

function sworkUserMarketUpdated()
	pda.Show( "Обновление на рынке труда" )
end

function sworkReklameFinished( ptrReklame )
	local reklame = CLuaReklame( ptrReklame )
	local rtobject = CLuaProject( reklame.reklameObject )
	pda.Show( "Закончилась рекламная кампания " .. reklame.name .. " для " .. rtobject.name )
end

local function localChangeSpeed( keyInput )
	if labelSpeed then labelSpeed:Remove() end
	
	if keyInput == 0xBB then 
		if applic.speed > 100 then applic.speed = (applic.speed - 100) end
	else
		if applic.speed < 1000 then applic.speed = (applic.speed + 100) end
	end
	
	local dd = ( 1000 - applic.speed ) / 100
	labelSpeed = guienv:AddLabel( "Скорость игры " .. dd, "25%", "45%", "50%+", "20%+", -1, guienv.root )
	labelSpeed:SetTextAlignment( EGUIA_CENTER, EGUIA_CENTER )
	labelSpeed.font = "font_28"
							
	LogScript( "labelSpeed:Update to "..dd )
	guienv:AddBlenderAnimator( labelSpeed, 255, 10, 2000, false, true, false )
end

function sworkKeyboardEvent( _, ptr )
	local event = CLuaEvent( ptr )
	local keyInput = event.key

	--LogScript( "key event "..keyInput )
	if  event.keyDown and ( keyInput == 0xBD or keyInput == 0xBB ) then
		localChangeSpeed( keyInput )
	end
end

function sworkNewGameOnMarket( _, ptr )
	local game = CLuaGame( ptr )
	--покажем пользователю что вышла новая игра
	pda.Show( "На рынке появилась новая игра "..game.name )
	
	--выход игры повышает инвестиционные ожидания компании на 10%
	if game.company then
		game.company.invexp = game.company.invexp * 1.1 
	end
end