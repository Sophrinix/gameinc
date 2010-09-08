cityWindow = nil
laborWindow = nil
receptionWindow = nil
directorCabinetWindow = nil

function ApplicationLoadLaborScene()
	--[[sceneManager:SetSelectedNode( nil )
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
	--]]
end

function ApplicationLoadCityScene()
	if cityWindow then
		cityWindow:SetVisible( true )
	else
		cityWindow = guienv:AddWindow( "media/city_map.tga", 0, 0, scrWidth, scrHeight, -1, guienv:GetRootGUIElement() )
		cityWindow:SetDraggable( false )
		local closeBtn = cityWindow:GetCloseButton()
		closeBtn:SetVisible( false )
	end
	
	sceneManager:SetSelectedNode( nil )
	sceneManager:RemoveAllNodes()
	
	local btnOffice = guienv:AddButton( 730, 43, 730 + 229, 43 + 215, cityWindow:Self(), -1, "")
	btnOffice:SetImage( 0, 0, 229, 215, "media/buttons/office_normal.tga" )
	btnOffice:SetHoveredImage( 0, 0, 229, 215, "media/buttons/office_select.tga" )	
	btnOffice:SetPressedImage( 0, 0, 229, 215, "media/buttons/office_select.tga" )	
	btnOffice:SetAction( "ApplicationLoadOfficeScene" )
	
	local btnMedia = guienv:AddButton( 417, 82, 417 + 216, 82 + 226, cityWindow:Self(), -1, "")
	btnMedia:SetImage( 0, 0, 216, 226, "media/buttons/media_normal.tga" )
	btnMedia:SetHoveredImage( 0, 0, 216, 226, "media/buttons/media_select.tga" )	
	btnMedia:SetPressedImage( 0, 0, 216, 226, "media/buttons/media_select.tga" )	
	btnMedia:SetAction( "ApplicationLoadMediaScene" )
	
	local btnLabor = guienv:AddButton( 811, 198, 811 + 179, 198 + 154, cityWindow:Self(), -1, "")
	btnLabor:SetImage( 0, 0, 179, 154, "media/buttons/labor_normal.tga" )
	btnLabor:SetHoveredImage( 0, 0, 179, 154, "media/buttons/labor_select.tga" )	
	btnLabor:SetPressedImage( 0, 0, 179, 154, "media/buttons/labor_select.tga" )	
	btnLabor:SetAction( "ApplicationLoadLaborScene" )
	
	local btnBank = guienv:AddButton( 323, 389, 323 + 237, 389 + 271, cityWindow:Self(), -1, "")
	btnBank:SetImage( 0, 0, 237, 271, "media/buttons/bank_normal.tga" )
	btnBank:SetHoveredImage( 0, 0, 237, 271, "media/buttons/bank_select.tga" )	
	btnBank:SetPressedImage( 0, 0, 237, 271, "media/buttons/bank_select.tga" )	
	btnBank:SetAction( "ApplicationLoadBankScene" )
	
	local btnPlant = guienv:AddButton( 517, 326, 517 + 212, 326 + 161, cityWindow:Self(), -1, "")
	btnPlant:SetImage( 0, 0, 212, 161, "media/buttons/plant_normal.tga" )
	btnPlant:SetHoveredImage( 0, 0, 212, 161, "media/buttons/plant_select.tga" )	
	btnPlant:SetPressedImage( 0, 0, 212, 161, "media/buttons/plant_select.tga" )	
	btnPlant:SetAction( "ApplicationLoadPlantScene" )
	
	local btnPizza = guienv:AddButton( 105, 42, 105 + 219, 42 + 171, cityWindow:Self(), -1, "")
	btnPizza:SetImage( 0, 0, 219, 171, "media/buttons/pizza_normal.tga" )
	btnPizza:SetHoveredImage( 0, 0, 219, 171, "media/buttons/pizza_select.tga" )	
	btnPizza:SetPressedImage( 0, 0, 219, 171, "media/buttons/pizza_select.tga" )	
	btnPizza:SetAction( "ApplicationLoadPizzaScene" )
end

function ApplicationLoadBankScene()
	--[[sceneManager:RemoveAllNodes()
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
	--]]
end

function ApplicationLoadShopScene()
    --[[sceneManager:SetSelectedNode( nil )
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
	--]]
end

function ApplicationLoadOfficeScene()
	cityWindow:SetVisible( false )
	
	if receptionWindow then
		receptionWindow:SetVisible( true )
	else
		receptionWindow = guienv:AddWindow( "media/reception.tga", 0, 0, scrWidth, scrHeight, -1, guienv:GetRootGUIElement() )
		local closeBtn = receptionWindow:GetCloseButton()
		closeBtn:SetVisible( false )
	end	
	
	local btnDirector = guienv:AddButton( 448, 242, 448 + 85, 242 + 175, receptionWindow:Self(), -1, "")
	btnDirector:SetImage( 0, 0, 85, 175, "media/buttons/director_normal.tga" )
	btnDirector:SetHoveredImage( 0, 0, 85, 175, "media/buttons/director_select.tga" )	
	btnDirector:SetPressedImage( 0, 0, 85, 175, "media/buttons/director_select.tga" )	
	btnDirector:SetAction( "ApplicationLoadDirectorCabinetScene" )
		
	--[[local newProj = sceneManager:GetSceneNodeByName( "createNewProjectNode" )
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
	
	local showInventionManager = sceneManager:GetSceneNodeByName( "inventionManagerNode" )
	selector = sceneManager:CreateTriangleSelectorFromBoundingBox( showInventionManager:Self() )
	showInventionManager:SetTriangleSelector( selector )
	sceneManager:SetMarkText( showInventionManager:Self(), "inventionManagerNode" )
		
	local exitN = sceneManager:GetSceneNodeByName( "exitOfficeNode" )
	selector = sceneManager:CreateTriangleSelectorFromBoundingBox( exitN:Self() )
	exitN:SetTriangleSelector( selector )
	sceneManager:SetMarkText( exitN:Self(), "exit" )
	
	sceneManager:AddSceneFunction( SCENE_LMOUSE_DOUBLE_CLICK, "sworkSelectObjectOnOfficeScene" )
	--]]
end

function ApplicationLoadPlantScene()
	--[[sceneManager:SetSelectedNode( nil )
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
	--]]
end

function ApplicationLoadUniverScene()
	--[[sceneManager:SetSelectedNode( nil )
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
	--]]
end

function ApplicationLoadDirectorCabinetScene( ptr )
	receptionWindow:SetVisible( false )
	
	if directorCabinetWindow then
		directorCabinetWindow:SetVisible( true )
	else
		directorCabinetWindow = guienv:AddWindow( "media/director_cabinet.tga", 0, 0, scrWidth, scrHeight, -1, guienv:GetRootGUIElement() )
		local closeBtn = directorCabinetWindow:GetCloseButton()
		closeBtn:SetVisible( false )
	end	
	
	local btnNewProject = guienv:AddButton( 0, 227, 0 + 197, 227 + 165, directorCabinetWindow:Self(), -1, "")
	btnNewProject:SetImage( 0, 0, 197, 165, "media/buttons/newProject_normal.png" )
	btnNewProject:SetHoveredImage( 0, 0, 197, 165, "media/buttons/newProject_select.png" )	
	btnNewProject:SetPressedImage( 0, 0, 197, 165, "media/buttons/newProject_select.png" )	
	btnNewProject:SetAction( "sworkCreateWindowWizardProject" )

end
