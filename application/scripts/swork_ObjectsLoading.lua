function ApplicationLoadLaborScene()
	sceneManager:SetSelectedNode( nil )
	--guienv:FadeAction( 3000, false )
	sceneManager:RemoveAllNodes()
	
	ApplicationAddCityCamera()
	sceneManager:LoadIrrlichtScene( "scene/nrpLaborScene.irr" )	

	local videoTech = sceneManager:GetSceneNodeByName( "graphicTechsNode" )
	local selector = sceneManager:CreateTriangleSelectorFromBoundingBox( videoTech:Self() )
	videoTech:SetTriangleSelector( selector )
	sceneManager:SetMarkText( videoTech:Self(), "Video Technologies" )
	LogScript( "videoTechsNode find" )	
	
	local soundTech = sceneManager:GetSceneNodeByName( "soundTechsNode" )
	selector = sceneManager:CreateTriangleSelectorFromBoundingBox( soundTech:Self() )
	soundTech:SetTriangleSelector( selector )
	sceneManager:SetMarkText( soundTech:Self(), "Sound Technologies" )
	LogScript( "soundTechsNode find" )	
	
	local genreTech = sceneManager:GetSceneNodeByName( "genreTechsNode" )
	selector = sceneManager:CreateTriangleSelectorFromBoundingBox( genreTech:Self() )
	genreTech:SetTriangleSelector( selector )
	sceneManager:SetMarkText( genreTech:Self(), "Genres" )
	LogScript( "genreTechsNode find" )
	
	local otherTech = sceneManager:GetSceneNodeByName( "otherTechsNode" )
	selector = sceneManager:CreateTriangleSelectorFromBoundingBox( otherTech:Self() )
	otherTech:SetTriangleSelector( selector )
	sceneManager:SetMarkText( otherTech:Self(), "Other" )
	LogScript( "otherTechsNode find" )
	
	local exitN = sceneManager:GetSceneNodeByName( "exitLaborNode" )
	selector = sceneManager:CreateTriangleSelectorFromBoundingBox( exitN:Self() )
	exitN:SetTriangleSelector( selector )
	sceneManager:SetMarkText( exitN:Self(), "exit" )
	LogScript( "exitLaborNode find" )	
	
	sceneManager:SetSelectedNode( nil )
	sceneManager:AddSceneFunction( SCENE_LMOUSE_DOUBLE_CLICK, "sworkSelectObjectOnLaborScene" )
end

function ApplicationLoadCityScene()
	sceneManager:SetSelectedNode( nil )
	--guienv:FadeAction( 3000, false )
	sceneManager:RemoveAllNodes()
	
	ApplicationAddCityCamera()
	sceneManager:LoadIrrlichtScene( "scene/nrpCityScene.irr" )	
	
	local office = sceneManager:GetSceneNodeByName( "officeNode" )
	local selector = sceneManager:CreateTriangleSelectorFromBoundingBox( office:Self() )
	office:SetTriangleSelector( selector )
	sceneManager:SetMarkText( office:Self(), "office" )
	SetHelpLinkForObject( "officeNode", "media/html/officeNode.htm" )
	LogScript( "officeNode find" )	
	
	local univer = sceneManager:GetSceneNodeByName( "univerNode" )
	selector = sceneManager:CreateTriangleSelectorFromBoundingBox( univer:Self() ) 
	univer:SetTriangleSelector( selector )
	sceneManager:SetMarkText( univer:Self(), "univer" )
	LogScript( "univerNode find" )	
	
	local bank = sceneManager:GetSceneNodeByName( "bankNode" )
	selector = sceneManager:CreateTriangleSelectorFromBoundingBox( bank:Self() ) 
	bank:SetTriangleSelector( selector )
	sceneManager:SetMarkText( bank:Self(), "Bank" )
	LogScript( "bankNode find" )	
	
	local gameShop = sceneManager:GetSceneNodeByName( "gameShopNode" )
	selector = sceneManager:CreateTriangleSelectorFromBoundingBox( gameShop:Self() ) 
    gameShop:SetTriangleSelector( selector )
	sceneManager:SetMarkText( gameShop:Self(), "gameShop" )
	LogScript( "gameShopNode find" )	
	
	local labor = sceneManager:GetSceneNodeByName( "laborNode" )
	selector = sceneManager:CreateTriangleSelectorFromBoundingBox( labor:Self() ) 
    labor:SetTriangleSelector( selector )
	sceneManager:SetMarkText( labor:Self(), "laboratory" )
	LogScript( "laborNode find" )	
	
	local pr = sceneManager:GetSceneNodeByName( "prNode" )
	selector = sceneManager:CreateTriangleSelectorFromBoundingBox( pr:Self() ) 
    pr:SetTriangleSelector( selector )
	sceneManager:SetMarkText( pr:Self(), "PR kontora" )
	LogScript( "prNode find" )	
	
	local cinema = sceneManager:GetSceneNodeByID( 1001001 )
	selector = sceneManager:CreateTriangleSelectorFromBoundingBox( cinema:Self() ) 
    cinema:SetTriangleSelector( selector )
	sceneManager:SetMarkText( cinema:Self(), "Cinema" )
	LogScript( "cinemaNode find" )	
		
	local pizza = sceneManager:GetSceneNodeByName( "pizzaNode" )
	selector = sceneManager:CreateTriangleSelectorFromBoundingBox( pizza:Self() ) 
    pizza:SetTriangleSelector( selector )
	sceneManager:SetMarkText( pizza:Self(), "Sabotaje" )
	LogScript( "pizzaNode find" )	
	
	local plant = sceneManager:GetSceneNodeByName( "plantNode" )
	selector = sceneManager:CreateTriangleSelectorFromBoundingBox( plant:Self() ) 
	plant:SetTriangleSelector( selector )
	sceneManager:SetMarkText( plant:Self(), "Производство" )
	LogScript( "plantNode find" )	
	--guienv:FadeAction( 3000, true )
	sceneManager:AddSceneFunction( SCENE_LMOUSE_DOUBLE_CLICK, "sworkSelectObjectOnCityScene" )
end

function ApplicationLoadBankScene()
	sceneManager:RemoveAllNodes()
	ApplicationAddCityCamera()
	sceneManager:LoadIrrlichtScene( "scene/nrpBankScene.irr" )
	
	local loan = sceneManager:GetSceneNodeByName( "loanNode" )
	local selector = sceneManager:CreateTriangleSelectorFromBoundingBox( loan:Self() )
	loan:SetTriangleSelector( selector )
	sceneManager:SetMarkText( loan:Self(), "loan" )
	
	local deposit = sceneManager:GetSceneNodeByName( "depositNode" )
	selector = sceneManager:CreateTriangleSelectorFromBoundingBox( deposit:Self() )
	deposit:SetTriangleSelector( selector )
	sceneManager:SetMarkText( deposit:Self(), "deposit" )
	
	local akcii = sceneManager:GetSceneNodeByName( "acciiNode" )
	selector = sceneManager:CreateTriangleSelectorFromBoundingBox( akcii:Self() )
	akcii:SetTriangleSelector( selector )
	sceneManager:SetMarkText( akcii:Self(), "akcii" )
	
	local exitN = sceneManager:GetSceneNodeByName( "exitBank" )
	selector = sceneManager:CreateTriangleSelectorFromBoundingBox( exitN:Self() )
	exitN:SetTriangleSelector( selector )
	sceneManager:SetMarkText( exitN:Self(), "exit" )
	
	sceneManager:SetSelectedNode( nil )
	sceneManager:AddSceneFunction( SCENE_LMOUSE_DOUBLE_CLICK, "sworkSelectObjectOnBankScene" )
end

function ApplicationLoadShopScene()
    sceneManager:SetSelectedNode( nil )
	sceneManager:RemoveAllNodes()
	ApplicationAddCityCamera()
	sceneManager:LoadIrrlichtScene( "scene/nrpShopScene.irr" )

	--игры в продаже
	--топ-лист месяца
	--топ-лист за все время
	--игровые журналы
	local gameInSale = sceneManager:GetSceneNodeByName( "gameInSaleNode" )
	local selector = sceneManager:CreateTriangleSelectorFromBoundingBox( gameInSale:Self() )
	gameInSale:SetTriangleSelector( selector )
	sceneManager:SetMarkText( gameInSale:Self(), "gameInSale" )
	LogScript( "gameInSale find" )	
	
	local topListMonth = sceneManager:GetSceneNodeByName( "topListMonthNode" )
	selector = sceneManager:CreateTriangleSelectorFromBoundingBox( topListMonth:Self() )
	topListMonth:SetTriangleSelector( selector )
	sceneManager:SetMarkText( topListMonth:Self(), "topListMonth" )
	LogScript( "topListMonth find" )	

	local topListAllTime = sceneManager:GetSceneNodeByName( "allTimeTopListNode" )
	selector = sceneManager:CreateTriangleSelectorFromBoundingBox( topListAllTime:Self() )
	topListAllTime:SetTriangleSelector( selector )
	sceneManager:SetMarkText( topListAllTime:Self(), "topListAllTime" )
	LogScript( "topListAllTime find" )	

	local gameJournals = sceneManager:GetSceneNodeByName( "gameJournalsNode" )
	selector = sceneManager:CreateTriangleSelectorFromBoundingBox( gameJournals:Self() )
	gameJournals:SetTriangleSelector( selector )
	sceneManager:SetMarkText( gameJournals:Self(), "gameJournals" )
	LogScript( "gameJournals find" )	
	
	local exitN = sceneManager:GetSceneNodeByName( "exitShopNode" )
	selector = sceneManager:CreateTriangleSelectorFromBoundingBox( exitN:Self() )
	exitN:SetTriangleSelector( selector )
	sceneManager:SetMarkText( exitN:Self(), "exit" )
	LogScript( "exitN find" )	
	
	sceneManager:AddSceneFunction( SCENE_LMOUSE_DOUBLE_CLICK, "sworkSelectObjectOnShopScene" )
end

function ApplicationLoadOfficeScene()
	sceneManager:SetSelectedNode( nil )
	sceneManager:RemoveAllNodes()
	ApplicationAddCityCamera()
	sceneManager:LoadIrrlichtScene( "scene/nrpOfficeScene.irr" )
	
	local newProj = sceneManager:GetSceneNodeByName( "createNewProjectNode" )
	local selector = sceneManager:CreateTriangleSelectorFromBoundingBox( newProj:Self() )
	newProj:SetTriangleSelector( selector )
	sceneManager:SetMarkText( newProj:Self(), "createNewProject" )
	
	local showEmployers = sceneManager:GetSceneNodeByName( "employerManageNode" )
	selector = sceneManager:CreateTriangleSelectorFromBoundingBox( showEmployers:Self() )
	showEmployers:SetTriangleSelector( selector )
	sceneManager:SetMarkText( showEmployers:Self(), "employerManageNode" )
	
	local showProjectManager = sceneManager:GetSceneNodeByName( "projectManagerNode" )
	selector = sceneManager:CreateTriangleSelectorFromBoundingBox( showProjectManager:Self() )
	showProjectManager:SetTriangleSelector( selector )
	sceneManager:SetMarkText( showProjectManager:Self(), "projectManagerNode" )
	
	--[[local showInventionManager = CLuaSceneNode( sceneManager:GetSceneNodeByName( "inventionManagerNode" ) )
	selector = sceneManager:CreateTriangleSelectorFromBoundingBox( showInventionManager:Self() )
	showInventionManager:SetTriangleSelector( selector )
	sceneManager:SetMarkText( showInventionManager:Self(), "inventionManagerNode" )
	--]]
	
	local exitN = sceneManager:GetSceneNodeByName( "exitOfficeNode" )
	selector = sceneManager:CreateTriangleSelectorFromBoundingBox( exitN:Self() )
	exitN:SetTriangleSelector( selector )
	sceneManager:SetMarkText( exitN:Self(), "exit" )
	
	sceneManager:AddSceneFunction( SCENE_LMOUSE_DOUBLE_CLICK, "sworkSelectObjectOnOfficeScene" )
end

function ApplicationLoadPlantScene()
	sceneManager:SetSelectedNode( nil )
	sceneManager:RemoveAllNodes()
	ApplicationAddCityCamera()
	sceneManager:LoadIrrlichtScene( "scene/nrpPlantScene.irr" )

	local createBoxNode = sceneManager:GetSceneNodeByName( "gameBoxManagerNode" )
	local selector = sceneManager:CreateTriangleSelectorFromBoundingBox( createBoxNode:Self() )
	createBoxNode:SetTriangleSelector( selector )
	sceneManager:SetMarkText( createBoxNode:Self(), "createBoxNode" )
	
	local plnatNode = sceneManager:GetSceneNodeByName( "plantCeNode" )
	selector = sceneManager:CreateTriangleSelectorFromBoundingBox( plnatNode:Self() )
	plnatNode:SetTriangleSelector( selector )
	sceneManager:SetMarkText( plnatNode:Self(), "plantCeNode" )
	
	local exitN = sceneManager:GetSceneNodeByName( "exitPlantNode" )
	selector = sceneManager:CreateTriangleSelectorFromBoundingBox( exitN:Self() )
	exitN:SetTriangleSelector( selector )
	sceneManager:SetMarkText( exitN:Self(), "exit" )
	sceneManager:AddSceneFunction( SCENE_LMOUSE_DOUBLE_CLICK, "sworkSelectObjectOnPlantScene" )
end

function ApplicationLoadUniverScene()
	sceneManager:SetSelectedNode( nil )
	sceneManager:RemoveAllNodes()
	ApplicationAddCityCamera()
	sceneManager:LoadIrrlichtScene( "scene/nrpUniverScene.irr" )
	
	local stuffNode = sceneManager:GetSceneNodeByName( "stuffNode" )
	local selector = sceneManager:CreateTriangleSelectorFromBoundingBox( stuffNode:Self() )
	stuffNode:SetTriangleSelector( selector )
	sceneManager:SetMarkText( stuffNode:Self(), "stuff" )
	
	local exitN = sceneManager:GetSceneNodeByName( "exitUniverNode" )
	selector = sceneManager:CreateTriangleSelectorFromBoundingBox( exitN:Self() )
	exitN:SetTriangleSelector( selector )
	sceneManager:SetMarkText( exitN:Self(), "exit" )
	
	sceneManager:AddSceneFunction( SCENE_LMOUSE_DOUBLE_CLICK, "sworkSelectObjectOnUniverScene" )
end