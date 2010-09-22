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
		windowLabor:GetCloseButton():SetVisible( false )
		
		--adding closeButton
		button.Stretch( scrWidth - 80, scrHeight - 80, scrWidth, scrHeight, 
		 			    "button_down", windowLabor:Self(), -1, "",
						"./windowLabor:Remove(); windowLabor = nil" )
	end	
	
	tutorial.Update( tutorial.STEP_OVERVIEW_LABORATORY )
	
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
		cityWindow:GetCloseButton():SetVisible( false )
	end	
	
	--Office
	button.EqualeTexture( 730, 43, "office", cityWindow:Self(), -1, "", "ApplicationLoadOfficeScene" )
	--shop	
	button.EqualeTexture( 417, 82, "media", cityWindow:Self(), -1, "", "ApplicationLoadShopScene" )
	--labor
	button.EqualeTexture( 811, 198, "labor", cityWindow:Self(), -1, "", "ApplicationLoadLaborScene" )
	--bank
	button.EqualeTexture( 323, 389, "bank", cityWindow:Self(), -1, "", "ApplicationLoadBankScene" )
	--plant	
	button.EqualeTexture( 517, 326, "plant", cityWindow:Self(), -1, "", "ApplicationLoadPlantScene" )
	--pizza	
	button.EqualeTexture( 105, 42, "pizza", cityWindow:Self(), -1, "", "ApplicationLoadPizzaScene" )
	--univer	
	button.EqualeTexture( 0, 287, "univer", cityWindow:Self(), -1, "", "ApplicationLoadUniverScene" )
end

function ApplicationLoadBankScene()
	if bankWindow then
		bankWindow:SetVisible( true )
	else
		bankWindow = guienv:AddWindow( "media/bank_normal.png", 0, 0, scrWidth, scrHeight, -1, guienv:GetRootGUIElement() )
		bankWindow:SetDraggable( false )
		bankWindow:GetCloseButton():SetVisible( false )
		
		--adding closeButton
		button.Stretch( scrWidth - 80, scrHeight - 80, scrWidth, scrHeight, 
		 			    "button_down", bankWindow:Self(), -1, "",
						"./bankWindow:Remove(); bankWindow = nil" )
	end
	
	--get loan
	button.EqualeTexture( 80, 402, "loans", bankWindow:Self(), -1, "", "sworkCreateWindowLoanAction" )
	--deposit	
	button.EqualeTexture( 258, 301, "deposit", bankWindow:Self(), -1, "", "sworkCreateWindowDepositAction" )
end

function ApplicationLoadShopScene()  
    if shopWindow then
		shopWindow:SetVisible( true )
	else
		shopWindow = guienv:AddWindow( "media/windowShop_normal.png", 0, 0, scrWidth, scrHeight, -1, guienv:GetRootGUIElement() )
		shopWindow:SetDraggable( false )
		shopWindow:GetCloseButton():SetVisible( false )
		
		--adding closeButton
		button.Stretch( scrWidth - 80, scrHeight - 80, scrWidth, scrHeight, 
		 			    "button_down", shopWindow:Self(), -1, "",
						"./shopWindow:Remove(); shopWindow = nil" )
	end

	--игры в продаже
	button.EqualeTexture( 147, 333, "gameInSale", shopWindow:Self(), -1, "", "sworkCreateGameInSaleWindow")
	
	--топ-лист месяца
	button.EqualeTexture( 703, 67, "toplistmonth", shopWindow:Self(), -1, "", "sworkCreateMonthTopListWindow" )

	--топ-лист за все время
	button.Equaletexture( 119, 94, "toplisttime", shopWindow:Self(), -1, "", "sworkCreateAllTimeTopListWindow")

	--игровые журналы
	button.EqualeTexture( 861, 268, "showMagazines", shopWindow:Self(), -1, "", "sworkCreateGameJournals" )
end

function ApplicationLoadOfficeScene()
	if receptionWindow then
		receptionWindow:SetVisible( true )
	else
		receptionWindow = guienv:AddWindow( "media/reception.tga", 0, 0, scrWidth, scrHeight, -1, guienv:GetRootGUIElement() )
		receptionWindow:GetCloseButton():SetVisible( false )
		receptionWindow:SetDraggable( false )
		
		--adding closeButton
		button.Stretch( scrWidth - 80, scrHeight - 80, scrWidth, scrHeight, 
		 			    "button_down", receptionWindow:Self(), -1, "",
						"./receptionWindow:Remove(); receptionWindow = nil" )
	end	
	
	tutorial.Update( tutorial.STEP_OVERVIEW_RECEPTION )
	--directors room
	button.EqualeTexture( 448, 242, "director", receptionWindow:Self(), -1, "", "ApplicationLoadDirectorCabinetScene" )
		
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
		
		--adding closeButton
		button.Stretch( scrWidth - 80, scrHeight - 80, scrWidth, scrHeight, 
		 			    "button_down", plantWindow:Self(), -1, "",
						"./plantWindow:Remove(); plantWindow = nil" )
	end	
	
	tutorial.Update( tutorial.STEP_OVERVIEW_PLANT )
	
	--box manager
	button.EqualeTexture( 94, 29, "boxManager", plantWindow:Self(), -1, "", "sworkCreateGameBoxManagerWindow" )
	--produce
	button.EqualeTexture( 407, 1, "produce", plantWindow:Self(), -1, "", "sworkCreateDiskProducePlantWindow")
end

function ApplicationLoadUniverScene()
	cityWindow:SetVisible( false )
	
	if univerWindow then
		univerWindow:SetVisible( true )
	else
		univerWindow = guienv:AddWindow( "media/univer_dvor.tga", 0, 0, scrWidth, scrHeight, -1, guienv:GetRootGUIElement() )
		univerWindow:GetCloseButton():SetVisible( false )
		univerWindow:SetDraggable( false )
		
		--adding closeButton
		button.Stretch( scrWidth - 80, scrHeight - 80, scrWidth, scrHeight, 
		 			    "button_down", univerWindow:Self(), -1, "",
						"./univerWindow:Remove(); univerWindow = nil" )
	end	
	
	tutorial.Update( tutorial.STEP_OVERVIEW_UNIVER )

	--stuff plate	
	button.EqualeTexture( 122, 320, "stuffPlate", univerWindow:Self(), -1, "", "sworkCreateEmployersWindow" )
	
	--outsourcing
	button.EqualeTexture( 612, 300, "outsorcing", univerWindow:Self(), -1, "", "sworkCreateWindowOutsorcing" )
end

function ApplicationLoadDirectorCabinetScene( ptr )
	if directorCabinetWindow then
		directorCabinetWindow:SetVisible( true )
	else
		directorCabinetWindow = guienv:AddWindow( "media/director_cabinet.tga", 0, 0, scrWidth, scrHeight, -1, guienv:GetRootGUIElement() )
		directorCabinetWindow:SetDraggable( false )
		directorCabinetWindow:GetCloseButton():SetVisible( false )
		
		--adding closeButton
		button.Stretch( scrWidth - 80, scrHeight - 80, scrWidth, scrHeight, 
		 			    "button_down", directorCabinetWindow:Self(), -1, "",
						"./directorCabinetWindow:Remove(); directorCabinetWindow = nil" )
	end	
	
	tutorial.Update( tutorial.STEP_OVERVIEW_DIRECTORS_ROOM )
	
	button.EqualeTexture( 0, 227, "newProject", directorCabinetWindow:Self(), -1, "", "sworkCreateWindowWizardProject")
end
