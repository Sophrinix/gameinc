local sceneManager = CLuaSceneManager( NrpGetSceneManager() )
local guienv = CLuaGuiEnvironment( NrpGetGuiEnvironment() )

function ApplicationLoadCityScene()
	--guienv:FadeAction( 3000, false )
	sceneManager:RemoveAllNodes()
	
	ApplicationAddCityCamera()
	sceneManager:LoadIrrlichtScene( "scene/nrpCityScene.irr" )	
	
	local office = CLuaSceneNode( sceneManager:GetSceneNodeByName( "officeNode" ) )
	local selector = sceneManager:CreateTriangleSelectorFromBoundingBox( office:Self() )
	office:SetTriangleSelector( selector )
	sceneManager:SetMarkText( office:Self(), "office" )
	LogScript( "officeNode find" )	
	
	local univer = CLuaSceneNode( sceneManager:GetSceneNodeByName( "univerNode" ) )
	selector = sceneManager:CreateTriangleSelectorFromBoundingBox( univer:Self() ) 
	univer:SetTriangleSelector( selector )
	sceneManager:SetMarkText( univer:Self(), "univer" )
	LogScript( "univerNode find" )	
	
	local bank = CLuaSceneNode( sceneManager:GetSceneNodeByName( "bankNode" ) )
	selector = sceneManager:CreateTriangleSelectorFromBoundingBox( bank:Self() ) 
	bank:SetTriangleSelector( selector )
	sceneManager:SetMarkText( bank:Self(), "Bank" )
	LogScript( "bankNode find" )	
	
	local gameShop = CLuaSceneNode( sceneManager:GetSceneNodeByName( "gameShopNode" ) )
	selector = sceneManager:CreateTriangleSelectorFromBoundingBox( gameShop:Self() ) 
    gameShop:SetTriangleSelector( selector )
	sceneManager:SetMarkText( gameShop:Self(), "gameShop" )
	LogScript( "gameShopNode find" )	
	
	local labor = CLuaSceneNode( sceneManager:GetSceneNodeByName( "laborNode" ) )
	selector = sceneManager:CreateTriangleSelectorFromBoundingBox( labor:Self() ) 
    labor:SetTriangleSelector( selector )
	sceneManager:SetMarkText( labor:Self(), "laboratory" )
	LogScript( "laborNode find" )	
	
	local pr = CLuaSceneNode( sceneManager:GetSceneNodeByName( "prNode" ) )
	selector = sceneManager:CreateTriangleSelectorFromBoundingBox( pr:Self() ) 
    pr:SetTriangleSelector( selector )
	sceneManager:SetMarkText( pr:Self(), "PR kontora" )
	LogScript( "prNode find" )	
	
	local cinema = CLuaSceneNode( sceneManager:GetSceneNodeByID( 1001001 ) )
	selector = sceneManager:CreateTriangleSelectorFromBoundingBox( cinema:Self() ) 
    cinema:SetTriangleSelector( selector )
	sceneManager:SetMarkText( cinema:Self(), "Cinema" )
	LogScript( "cinemaNode find" )	
		
	local pizza = CLuaSceneNode( sceneManager:GetSceneNodeByName( "pizzaNode" ) )
	selector = sceneManager:CreateTriangleSelectorFromBoundingBox( pizza:Self() ) 
    pizza:SetTriangleSelector( selector )
	sceneManager:SetMarkText( pizza:Self(), "Sabotaje" )
	LogScript( "pizzaNode find" )	
	
	local plant = CLuaSceneNode( sceneManager:GetSceneNodeByName( "plantNode" ) )
	selector = sceneManager:CreateTriangleSelectorFromBoundingBox( plant:Self() ) 
	plant:SetTriangleSelector( selector )
	sceneManager:SetMarkText( plant:Self(), "Производство" )
	LogScript( "plantNode find" )	
	--guienv:FadeAction( 3000, true )
end

function ApplicationLoadBankScene()
	sceneManager:RemoveAllNodes()
	ApplicationAddCityCamera()
	sceneManager:LoadIrrlichtScene( "scene/nrpBankScene.irr" )
	
	local loan = CLuaSceneNode( sceneManager:GetSceneNodeByName( "loanNode" ) )
	local selector = sceneManager:CreateTriangleSelectorFromBoundingBox( loan:Self() )
	loan:SetTriangleSelector( selector )
	sceneManager:SetMarkText( loan:Self(), "loan" )
	
	local deposit = CLuaSceneNode( sceneManager:GetSceneNodeByName( "depositNode" ) )
	local selector = sceneManager:CreateTriangleSelectorFromBoundingBox( deposit:Self() )
	deposit:SetTriangleSelector( selector )
	sceneManager:SetMarkText( deposit:Self(), "deposit" )
	
	local akcii = CLuaSceneNode( sceneManager:GetSceneNodeByName( "acciiNode" ) )
	local selector = sceneManager:CreateTriangleSelectorFromBoundingBox( akcii:Self() )
	akcii:SetTriangleSelector( selector )
	sceneManager:SetMarkText( akcii:Self(), "akcii" )
	
	local exitN = CLuaSceneNode( sceneManager:GetSceneNodeByName( "exitBank" ) )
	local selector = sceneManager:CreateTriangleSelectorFromBoundingBox( exitN:Self() )
	exitN:SetTriangleSelector( selector )
	sceneManager:SetMarkText( exitN:Self(), "exit" )
end

function ApplicationLoadShopScene()
	sceneManager:RemoveAllNodes()
	sceneManager:LoadIrrlichtScene( "scene/nrpShopScene.irr" )

	--игры в продаже
	--топ-лист месяца
	--топ-лист за все время
	--игровые журналы
	local gameInSale = CLuaSceneNode( sceneManager:GetSceneNodeByName( "gameInSaleNode" ) )
	local selector = sceneManager:CreateTriangleSelectorFromBoundingBox( gameInSale:Self() )
	gameInSale:SetTriangleSelector( selector )
	sceneManager:SetMarkText( gameInSale:Self(), "gameInSale" )
	
	local topListMonth = CLuaSceneNode( sceneManager:GetSceneNodeByName( "topListMonthNode" ) )
	local selector = sceneManager:CreateTriangleSelectorFromBoundingBox( topListMonth:Self() )
	topListMonth:SetTriangleSelector( selector )
	sceneManager:SetMarkText( topListMonth:Self(), "topListMonth" )

	local topListAllTime = CLuaSceneNode( sceneManager:GetSceneNodeByName( "allTimeTopListNode" ) )
	local selector = sceneManager:CreateTriangleSelectorFromBoundingBox( topListAllTime:Self() )
	topListAllTime:SetTriangleSelector( selector )
	sceneManager:SetMarkText( topListAllTime:Self(), "topListAllTime" )

	local gameJournals = CLuaSceneNode( sceneManager:GetSceneNodeByName( "gameJournalsNode" ) )
	local selector = sceneManager:CreateTriangleSelectorFromBoundingBox( gameJournals:Self() )
	gameJournals:SetTriangleSelector( selector )
	sceneManager:SetMarkText( gameJournals:Self(), "gameJournals" )
	
	local exitN = CLuaSceneNode( sceneManager:GetSceneNodeByName( "exitShopNode" ) )
	selector = sceneManager:CreateTriangleSelectorFromBoundingBox( exitN:Self() )
	exitN:SetTriangleSelector( selector )
	sceneManager:SetMarkText( exitN:Self(), "exit" )
end

function ApplicationLoadOfficeScene()
	sceneManager:RemoveAllNodes()
	sceneManager:LoadIrrlichtScene( "scene/nrpOfficeScene.irr" )
	
	local newProj = CLuaSceneNode( sceneManager:GetSceneNodeByName( "createNewProjectNode" ) )
	local selector = sceneManager:CreateTriangleSelectorFromBoundingBox( newProj:Self() )
	newProj:SetTriangleSelector( selector )
	sceneManager:SetMarkText( newProj:Self(), "createNewProject" )
	
	local showEmployers = CLuaSceneNode( sceneManager:GetSceneNodeByName( "employerManageNode" ) )
	local selector = sceneManager:CreateTriangleSelectorFromBoundingBox( showEmployers:Self() )
	showEmployers:SetTriangleSelector( selector )
	sceneManager:SetMarkText( showEmployers:Self(), "employerManageNode" )
	
	local showProjectManager = CLuaSceneNode( sceneManager:GetSceneNodeByName( "projectManagerNode" ) )
	local selector = sceneManager:CreateTriangleSelectorFromBoundingBox( showProjectManager:Self() )
	showProjectManager:SetTriangleSelector( selector )
	sceneManager:SetMarkText( showProjectManager:Self(), "projectManagerNode" )
	
	local exitN = CLuaSceneNode( sceneManager:GetSceneNodeByName( "exitOfficeNode" ) )
	selector = sceneManager:CreateTriangleSelectorFromBoundingBox( exitN:Self() )
	exitN:SetTriangleSelector( selector )
	sceneManager:SetMarkText( exitN:Self(), "exit" )
end

function ApplicationLoadPlantScene()
	sceneManager:RemoveAllNodes()
	sceneManager:LoadIrrlichtScene( "scene/nrpPlantScene.irr" )

	local createBoxNode = CLuaSceneNode( sceneManager:GetSceneNodeByName( "gameBoxManagerNode" ) )
	local selector = sceneManager:CreateTriangleSelectorFromBoundingBox( createBoxNode:Self() )
	createBoxNode:SetTriangleSelector( selector )
	sceneManager:SetMarkText( createBoxNode:Self(), "createBoxNode" )
	
	local plnatNode = CLuaSceneNode( sceneManager:GetSceneNodeByName( "plantCeNode" ) )
	local selector = sceneManager:CreateTriangleSelectorFromBoundingBox( plnatNode:Self() )
	plnatNode:SetTriangleSelector( selector )
	sceneManager:SetMarkText( plnatNode:Self(), "plantCeNode" )
	
	local exitN = CLuaSceneNode( sceneManager:GetSceneNodeByName( "exitPlantNode" ) )
	local selector = sceneManager:CreateTriangleSelectorFromBoundingBox( exitN:Self() )
	exitN:SetTriangleSelector( selector )
	sceneManager:SetMarkText( exitN:Self(), "exit" )
end

function ApplicationLoadUniverScene()
	sceneManager:RemoveAllNodes()
	sceneManager:LoadIrrlichtScene( "scene/nrpUniverScene.irr" )
	
	local stuffNode = CLuaSceneNode( sceneManager:GetSceneNodeByName( "stuffNode" ) )
	local selector = sceneManager:CreateTriangleSelectorFromBoundingBox( stuffNode:Self() )
	stuffNode:SetTriangleSelector( selector )
	sceneManager:SetMarkText( stuffNode:Self(), "stuff" )
	
	local exitN = CLuaSceneNode( sceneManager:GetSceneNodeByName( "exitUniverNode" ) )
	local selector = sceneManager:CreateTriangleSelectorFromBoundingBox( exitN:Self() )
	exitN:SetTriangleSelector( selector )
	sceneManager:SetMarkText( exitN:Self(), "exit" )
end