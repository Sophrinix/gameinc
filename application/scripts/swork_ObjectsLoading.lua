cityWindow = nil
windowLabor = nil
receptionWindow = nil
directorCabinetWindow = nil
shopWindow = nil

function ApplicationLoadLaborScene()	
	if windowLabor then
		windowLabor:SetVisible( true )
	else
		windowLabor = guienv:AddWindow( "media/laboratory_normal.png", 0, 0, scrWidth, scrHeight, -1, guienv:GetRootGUIElement() )
		windowLabor:SetDraggable( false )
		windowLabor:SetVisible( false )
		local closeBtn = windowLabor:GetCloseButton()
		closeBtn:SetVisible( false )
		
		closeBtn = button.Stretch( scrWidth - 80, scrHeight - 80, scrWidth, scrHeight, 
								   "button_down", windowLabor:Self(), -1, "",
								   "sworkCloseLaboratoryWindow" )
	end	
	
	UpdateTutorialState( STEP_OVERVIEW_LABORATORY )
	
	button.EqualeTexture( 545, 330, "techMapVideo", windowLabor:Self(), -1, "", "sworkCreateVideoTechMapWindow" )
	button.EqualeTexture( 372, 213, "techMapSound", windowLabor:Self(), -1, "", "sworkCreateSoundTechMapWindow" )
	button.EqualeTexture( 749, 222, "techMapAdvTech", windowLabor:Self(), -1, "", "sworkCreateAdvancedMapWindow" )
	button.EqualeTexture( 73, 202,	"techMapGenre", windowLabor:Self(), -1, "", "sworkCreateGenreTechMapWindow" )

	guienv:FadeAction( 1000, false, false )			
	guienv:AddTimer( 1005, "windowLabor:SetVisible( true ); guienv:FadeAction( 1000, true, true )" )
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
	
	local btnOffice = guienv:AddButton( 730, 43, 730 + 229, 43 + 215, cityWindow:Self(), -1, "")
	btnOffice:SetImage( 0, 0, 229, 215, "media/buttons/office_normal.tga" )
	btnOffice:SetHoveredImage( 0, 0, 229, 215, "media/buttons/office_select.tga" )	
	btnOffice:SetPressedImage( 0, 0, 229, 215, "media/buttons/office_select.tga" )	
	btnOffice:SetAction( "ApplicationLoadOfficeScene" )
	
	local btnMedia = guienv:AddButton( 417, 82, 417 + 216, 82 + 226, cityWindow:Self(), -1, "")
	btnMedia:SetImage( 0, 0, 216, 226, "media/buttons/media_normal.tga" )
	btnMedia:SetHoveredImage( 0, 0, 216, 226, "media/buttons/media_select.tga" )	
	btnMedia:SetPressedImage( 0, 0, 216, 226, "media/buttons/media_select.tga" )	
	btnMedia:SetAction( "ApplicationLoadShopScene" )
	
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
	
	local btnUniver = guienv:AddButton( 0, 287, 287, 287 + 273, cityWindow:Self(), -1, "" )
	btnUniver:SetImage( 0, 0, 287, 273, "media/buttons/univer_normal.tga" )
	btnUniver:SetHoveredImage( 0, 0, 287, 273, "media/buttons/univer_select.tga" )	
	btnUniver:SetPressedImage( 0, 0, 287, 273, "media/buttons/univer_select.tga" )	
	btnUniver:SetAction( "ApplicationLoadUniverScene" )

end

function ApplicationLoadBankScene()
	if bankWindow then
		bankWindow:SetVisible( true )
	else
		bankWindow = guienv:AddWindow( "media/bank_normal.png", 0, 0, scrWidth, scrHeight, -1, guienv:GetRootGUIElement() )
		bankWindow:SetDraggable( false )
		local closeBtn = bankWindow:GetCloseButton()
		closeBtn:SetVisible( false )
	end
	
	local btnLoan = guienv:AddButton( 80, 402, 80 + 223, 402 + 171, bankWindow:Self(), -1, "")
	btnLoan:SetImage( 0, 0, 223, 171, "media/buttons/loans_normal.png" )
	btnLoan:SetHoveredImage( 0, 0, 223, 171, "media/buttons/loans_select.png" )	
	btnLoan:SetPressedImage( 0, 0, 223, 171, "media/buttons/loans_select.png" )	
	btnLoan:SetAction( "sworkCreateWindowLoanAction" )
	
	local btnDeposit = guienv:AddButton( 258, 301, 258 + 183, 301 + 208, bankWindow:Self(), -1, "")
	btnDeposit:SetImage( 0, 0, 183, 208, "media/buttons/deposit_normal.png" )
	btnDeposit:SetHoveredImage( 0, 0, 183, 208, "media/buttons/deposit_select.png" )	
	btnDeposit:SetPressedImage( 0, 0, 183, 208, "media/buttons/deposit_select.png" )	
	btnDeposit:SetAction( "sworkCreateWindowDepositAction" )
end

function ApplicationLoadShopScene()
    
    if shopWindow then
		shopWindow:SetVisible( true )
	else
		shopWindow = guienv:AddWindow( "media/windowShop_normal.png", 0, 0, scrWidth, scrHeight, -1, guienv:GetRootGUIElement() )
		shopWindow:SetDraggable( false )
		local closeBtn = shopWindow:GetCloseButton()
		closeBtn:SetVisible( false )
	end

	--игры в продаже
	local gameInSale = guienv:AddButton( 147, 333, 147 + 338, 333 + 294, shopWindow:Self(), -1, "")
	gameInSale:SetImage( 0, 0, 338, 294, "media/buttons/gameInSale_normal.png" )
	gameInSale:SetHoveredImage( 0, 0, 338, 294, "media/buttons/gameInSale_select.png" )	
	gameInSale:SetPressedImage( 0, 0, 338, 294, "media/buttons/gameInSale_select.png" )	
	gameInSale:SetAction( "sworkCreateGameInSaleWindow" )
	
	--топ-лист месяца
	local topListMonth = guienv:AddButton( 703, 67, 703 + 138, 67 + 150, shopWindow:Self(), -1, "")
	topListMonth:SetImage( 0, 0, 138, 150, "media/buttons/toplistmonth_normal.png" )
	topListMonth:SetHoveredImage( 0, 0, 138, 150, "media/buttons/toplistmonth_select.png" )	
	topListMonth:SetPressedImage( 0, 0, 138, 150, "media/buttons/toplistmonth_select.png" )	
	topListMonth:SetAction( "sworkCreateMonthTopListWindow" )

	--топ-лист за все время
	local topListAllTime = guienv:AddButton( 119, 94, 119 + 172, 94 + 226, shopWindow:Self(), -1, "")
	topListAllTime:SetImage( 0, 0, 172, 226, "media/buttons/toplisttime_normal.png" )
	topListAllTime:SetHoveredImage( 0, 0, 172, 226, "media/buttons/toplisttime_select.png" )	
	topListAllTime:SetPressedImage( 0, 0, 172, 226, "media/buttons/toplisttime_select.png" )	
	topListAllTime:SetAction( "sworkCreateAllTimeTopListWindow" )

	--игровые журналы
	local gameJournals = guienv:AddButton( 861, 268, 861 + 163, 268 + 222, shopWindow:Self(), -1, "")
	gameJournals:SetImage( 0, 0, 163, 222, "media/buttons/showMagazines_normal.png" )
	gameJournals:SetHoveredImage( 0, 0, 163, 222, "media/buttons/showMagazines_select.png" )	
	gameJournals:SetPressedImage( 0, 0, 163, 222, "media/buttons/showMagazines_select.png" )	
	gameJournals:SetAction( "sworkCreateGameJournals" )
	
	local exitN = guienv:AddButton( 700, 500, 700 + 183, 500 + 208, shopWindow:Self(), -1, "")
	exitN:SetImage( 0, 0, 183, 208, "media/buttons/deposit_normal.png" )
	exitN:SetHoveredImage( 0, 0, 183, 208, "media/buttons/deposit_select.png" )	
	exitN:SetPressedImage( 0, 0, 183, 208, "media/buttons/deposit_select.png" )	
	exitN:SetAction( "sworkCloseWindowShop" )
end

function ApplicationLoadOfficeScene()
	cityWindow:SetVisible( false )
	
	if receptionWindow then
		receptionWindow:SetVisible( true )
	else
		receptionWindow = guienv:AddWindow( "media/reception.tga", 0, 0, scrWidth, scrHeight, -1, guienv:GetRootGUIElement() )
		receptionWindow:GetCloseButton():SetVisible( false )
		receptionWindow:SetDraggable( false )
		
		--local btn = guienv:AddButton()
	end	
	
	UpdateTutorialState( STEP_OVERVIEW_RECEPTION )
	
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
	cityWindow:SetVisible( false )
	
	if plantWindow then
		plantWindow:SetVisible( true )
	else
		plantWindow = guienv:AddWindow( "media/plant_normal.tga", 0, 0, scrWidth, scrHeight, -1, guienv:GetRootGUIElement() )
		plantWindow:GetCloseButton():SetVisible( false )
		plantWindow:SetDraggable( false )
	end	
	
	UpdateTutorialState( STEP_OVERVIEW_PLANT )
	
	local BoxButton = guienv:AddButton( 94, 29, 94 + 241, 29 + 129, plantWindow:Self(), -1, "")
	BoxButton:SetImage( 0, 0, 241, 129, "media/buttons/boxManager_normal.png" )
	BoxButton:SetHoveredImage( 0, 0, 241, 129, "media/buttons/boxManager_select.png" )	
	BoxButton:SetPressedImage( 0, 0, 241, 129, "media/buttons/boxManager_select.png" )	
	BoxButton:SetAction( "sworkCreateGameBoxManagerWindow" )
	
	local ProduceButton = guienv:AddButton( 407, 1, 407 + 558, 1 + 234, plantWindow:Self(), -1, "")
	ProduceButton:SetImage( 0, 0, 558, 234, "media/buttons/produce_normal.png" )
	ProduceButton:SetHoveredImage( 0, 0, 558, 234, "media/buttons/produce_select.png" )	
	ProduceButton:SetPressedImage( 0, 0, 558, 234, "media/buttons/produce_select.png" )	
	ProduceButton:SetAction( "sworkCreateDiskProducePlantWindow" )
end

function ApplicationLoadUniverScene()
	cityWindow:SetVisible( false )
	
	if univerWindow then
		univerWindow:SetVisible( true )
	else
		univerWindow = guienv:AddWindow( "media/univer_dvor.tga", 0, 0, scrWidth, scrHeight, -1, guienv:GetRootGUIElement() )
		univerWindow:GetCloseButton():SetVisible( false )
		univerWindow:SetDraggable( false )
	end	
	
	UpdateTutorialState( STEP_OVERVIEW_UNIVER )
	
	local btnStuffUp = guienv:AddButton( 122, 320, 122 + 324, 329 + 130, univerWindow:Self(), -1, "")
	btnStuffUp:SetImage( 0, 0, 324, 130, "media/buttons/stuffPlate_normal.png" )
	btnStuffUp:SetHoveredImage( 0, 0, 324, 130, "media/buttons/stuffPlate_select.png" )	
	btnStuffUp:SetPressedImage( 0, 0, 324, 130, "media/buttons/stuffPlate_select.png" )	
	btnStuffUp:SetAction( "sworkCreateEmployersWindow" )
	
	local btnOutsourcing = guienv:AddButton( 612, 300, 612 + 211, 300 + 156, univerWindow:Self(), -1, "")
	btnOutsourcing:SetImage( 0, 0, 211, 156, "media/buttons/outsorcing_normal.png" )
	btnOutsourcing:SetHoveredImage( 0, 0, 211, 156, "media/buttons/outsorcing_select.png" )	
	btnOutsourcing:SetPressedImage( 0, 0, 211, 156, "media/buttons/outsorcing_select.png" )	
	btnOutsourcing:SetAction( "sworkCreateWindowOutsorcing" )
	
	local btnExit = guienv:AddButton( 455, 315, 455 + 211, 315 + 156, univerWindow:Self(), -1, "")
	btnExit:SetImage( 0, 0, 137, 187, "media/buttons/univerExit_normal.png" )
	btnExit:SetHoveredImage( 0, 0, 137, 187, "media/buttons/univerExit_select.png" )	
	btnExit:SetPressedImage( 0, 0, 137, 187, "media/buttons/univerExit_select.png" )	
	btnExit:SetAction( "sworkWindowUpEmployerClose" )

end

function ApplicationLoadDirectorCabinetScene( ptr )
	receptionWindow:SetVisible( false )
	
	if directorCabinetWindow then
		directorCabinetWindow:SetVisible( true )
	else
		directorCabinetWindow = guienv:AddWindow( "media/director_cabinet.tga", 0, 0, scrWidth, scrHeight, -1, guienv:GetRootGUIElement() )
		directorCabinetWindow:SetDraggable( false )
		
		local closeBtn = directorCabinetWindow:GetCloseButton()
		closeBtn:SetVisible( false )
	end	
	
	UpdateTutorialState( STEP_OVERVIEW_DIRECTORS_ROOM )
	
	local btnNewProject = guienv:AddButton( 0, 227, 0 + 197, 227 + 165, directorCabinetWindow:Self(), -1, "")
	btnNewProject:SetImage( 0, 0, 197, 165, "media/buttons/newProject_normal.png" )
	btnNewProject:SetHoveredImage( 0, 0, 197, 165, "media/buttons/newProject_select.png" )	
	btnNewProject:SetPressedImage( 0, 0, 197, 165, "media/buttons/newProject_select.png" )	
	btnNewProject:SetAction( "sworkCreateWindowWizardProject" )

end
