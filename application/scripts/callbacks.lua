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
	guienv:MessageBox( "Закончена работа над изобретением "..inv:GetName(), false, false, "", "" )
end

function sworkPlayerCompanyReadyProject( ptr )
	local game = CLuaGame( ptr )
	guienv:MessageBox( "Закончена работа над проектом "..game:GetName(), false, false, "", "" )
end

function sworkMainLoop( ptr )
	applic:UpdateGameTime( ID_DATETIME_LABEL )	
end

function sworkSelectObjectOnOfficeScene( ptr )
	local node = CLuaSceneNode( ptr )
	local nodeName = node:GetName()
	
	
	if nodeName == "inventionManagerNode" then
		sworkCreateWindowCompanyInventionManager()
		return 0
	end
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
	local reklame = CLuaReklame( ptrModule )
	pda.Show( "Завершена работа над модулем "..reklame:GetName() )
end

sceneManager:AddSceneFunction( SCENE_AFTER_END, "sworkMainLoop" )
sceneManager:AddSceneFunction( SCENE_AFTER_RENDER, "sworkDrawOnTopWindows" )
applic:AddLuaFunction( APP_DAY_CHANGE, "sworkAppDayChange" )
applic:AddLuaFunction( APP_MONTH_CHANGE, "sworkAppMonthChange" )
applic:AddLuaFunction( APP_YEAR_CHANGE, "sworkAppYearChange" )
applic:AddLuaFunction( APP_INVENTION_FINISHED, "sworkInventionFinished" )
applic:AddLuaFunction( APP_MODULE_FINISHED, "sworkModuleFinished" )
applic:AddLuaFunction( APP_REKLAME_FINISHED, "sworkReklameFinished" )
applic:AddLuaFunction( APP_USER_MARKETUPDATE, "sworkUserMarketUpdated" )