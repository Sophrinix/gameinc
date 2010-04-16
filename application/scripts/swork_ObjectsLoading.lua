local sceneManager = CLuaSceneManager( NrpGetSceneManager() )

function AddObjectsToCityScene()
    
	local x = 1
	local y = 1
	local delta = 800
	local office = CLuaSceneNode( sceneManager:AddCubeSceneNode( "officeNode" ) )
	office:SetScale( 30, 40, 30 )
	office:SetPosition( x * delta, 800, y *delta )
	office:SetMaterialTexture( 0, "media/t351sml.jpg" )
	office:SetMaterialFlag( EMF_LIGHTING, false )
	local selector = sceneManager:CreateTriangleSelectorFromBoundingBox( office:Self() )
	office:SetTriangleSelector( selector )
	sceneManager:SetMarkText( office:Self(), "office" )
	citySceneObjects[ 0 ] = office:Self()				
				
	x = x + 1
	local univer = CLuaSceneNode( sceneManager:AddCubeSceneNode( "univerNode" ) )
	univer:SetScale( 40, 40, 40 )
	univer:SetPosition( x * delta, 1000, y * delta )
	univer:SetMaterialTexture( 0, "media/t351sml.jpg" )
	univer:SetMaterialFlag( EMF_LIGHTING, false )
	selector = sceneManager:CreateTriangleSelectorFromBoundingBox( univer:Self() ) 
	univer:SetTriangleSelector( selector )
	sceneManager:SetMarkText( univer:Self(), "univer" )
	citySceneObjects[ 1 ] = univer:Self()	
	
	x = x + 1; y = y + 1
	local bank = CLuaSceneNode( sceneManager:AddCubeSceneNode( "bankNode" ) )
	bank:SetScale( 40, 50, 40 )
	bank:SetPosition( x * delta, 1000, y * delta )
	bank:SetMaterialTexture( 0, "media/t351sml.jpg" )
	bank:SetMaterialFlag( EMF_LIGHTING, false )
	selector = sceneManager:CreateTriangleSelectorFromBoundingBox( bank:Self() ) 
	bank:SetTriangleSelector( selector )
	sceneManager:SetMarkText( bank:Self(), "Bank" )
	citySceneObjects[ 2 ] = bank:Self()
	
	x = x + 1; y = y - 1
	local gameShop = CLuaSceneNode( sceneManager:AddCubeSceneNode( "gameShopNode" ) )
	gameShop:SetScale( 40, 20, 40 )
	gameShop:SetPosition( x * delta, 1000, y * delta )
	gameShop:SetMaterialTexture( 0, "media/t351sml.jpg" )
	gameShop:SetMaterialFlag( EMF_LIGHTING, false )
	selector = sceneManager:CreateTriangleSelectorFromBoundingBox( gameShop:Self() ) 
    gameShop:SetTriangleSelector( selector )
	sceneManager:SetMarkText( gameShop:Self(), "gameShop" )
	citySceneObjects[ 3 ] = gameShop:Self()
	
	x = x + 1; y = y + 1
	local labor = CLuaSceneNode( sceneManager:AddCubeSceneNode( "laborNode" ) )
	labor:SetScale( 40, 20, 40 )
	labor:SetPosition( x * delta, 1000, y * delta )
	labor:SetMaterialTexture( 0, "media/t351sml.jpg" )
	labor:SetMaterialFlag( EMF_LIGHTING, false )
	selector = sceneManager:CreateTriangleSelectorFromBoundingBox( labor:Self() ) 
    labor:SetTriangleSelector( selector )
	sceneManager:SetMarkText( labor:Self(), "laboratory" )
	citySceneObjects[ 4 ] = labor:Self()
	
	x = x + 1; y = y - 1
	local pr = CLuaSceneNode( sceneManager:AddCubeSceneNode( "prNode" ) )
	pr:SetScale( 40, 20, 40 )
	pr:SetPosition( x * delta, 1000, y * delta )
	pr:SetMaterialTexture( 0, "media/t351sml.jpg" )
	pr:SetMaterialFlag( EMF_LIGHTING, false )
	selector = sceneManager:CreateTriangleSelectorFromBoundingBox( pr:Self() ) 
    pr:SetTriangleSelector( selector )
	sceneManager:SetMarkText( pr:Self(), "PR kontora" )
	citySceneObjects[ 5 ] = pr:Self()
	
	x = x + 1; y = y + 1
	local cinema = CLuaSceneNode( sceneManager:AddCubeSceneNode( "cinemaNode" ) )
	cinema:SetScale( 40, 20, 40 )
	cinema:SetPosition( x * delta, 1000, y * delta )
	cinema:SetMaterialTexture( 0, "media/t351sml.jpg" )
	cinema:SetMaterialFlag( EMF_LIGHTING, false )
	selector = sceneManager:CreateTriangleSelectorFromBoundingBox( cinema:Self() ) 
    cinema:SetTriangleSelector( selector )
	sceneManager:SetMarkText( cinema:Self(), "Cinema" )
	citySceneObjects[ 6 ] = cinema:Self()
	
	x = x + 1; y = y - 1
	local pizza = CLuaSceneNode( sceneManager:AddCubeSceneNode( "pizzaNode" ) )
	pizza:SetScale( 40, 20, 40 )
	pizza:SetPosition( x * delta, 1000, y * delta )
	pizza:SetMaterialTexture( 0, "media/t351sml.jpg" )
	pizza:SetMaterialFlag( EMF_LIGHTING, false )
	selector = sceneManager:CreateTriangleSelectorFromBoundingBox( pizza:Self() ) 
    pizza:SetTriangleSelector( selector )
	sceneManager:SetMarkText( pizza:Self(), "Sabotaje" )
	citySceneObjects[ 7 ] = pizza:Self()
	
	x = x + 1; y = y + 1
	local plant = CLuaSceneNode( sceneManager:AddCubeSceneNode( "plantNode" ) )
	plant:SetScale( 40, 20, 40 )
	plant:SetPosition( x * delta, 1000, y * delta )
	plant:SetMaterialTexture( 0, "media/t351sml.jpg" )
	plant:SetMaterialFlag( EMF_LIGHTING, false )
	selector = sceneManager:CreateTriangleSelectorFromBoundingBox( plant:Self() ) 
	plant:SetTriangleSelector( selector )
	sceneManager:SetMarkText( plant:Self(), "Proizvodstvo" )
	citySceneObjects[ 8 ] = plant:Self()

end

function AddObjectsToOfficeScene()
	local x = 1
	local y = 1
	local delta = 800
	
	local newProj = CLuaSceneNode( sceneManager:AddCubeSceneNode( "createNewProjectNode" ) )
	newProj:SetScale( 30, 40, 30 )
	newProj:SetVisible( false )
	newProj:SetPosition( x * delta, 800, y *delta )
	newProj:SetMaterialTexture( 0, "media/t351sml.jpg" )
	newProj:SetMaterialFlag( EMF_LIGHTING, false )
	local selector = sceneManager:CreateTriangleSelectorFromBoundingBox( newProj:Self() )
	newProj:SetTriangleSelector( selector )
	sceneManager:SetMarkText( newProj:Self(), "createNewProject" )
	officeSceneObjects[ 0 ] = newProj:Self()	
	x = x + 1
	
	local showEmployers = CLuaSceneNode( sceneManager:AddCubeSceneNode( "employerManageNode" ) )
	showEmployers:SetScale( 30, 40, 30 )
	showEmployers:SetVisible( false )
	showEmployers:SetPosition( x * delta, 800, y *delta )
	showEmployers:SetMaterialTexture( 0, "media/t351sml.jpg" )
	showEmployers:SetMaterialFlag( EMF_LIGHTING, false )
	local selector = sceneManager:CreateTriangleSelectorFromBoundingBox( showEmployers:Self() )
	showEmployers:SetTriangleSelector( selector )
	sceneManager:SetMarkText( showEmployers:Self(), "employerManageNode" )
	officeSceneObjects[ 1 ] = showEmployers:Self()	
	x = x + 1
	
	local exitN = CLuaSceneNode( sceneManager:AddCubeSceneNode( "exitOfficeNode" ) )
	exitN:SetScale( 30, 40, 30 )
	exitN:SetVisible( false )
	exitN:SetPosition( x * delta, 800, y *delta )
	exitN:SetMaterialTexture( 0, "media/t351sml.jpg" )
	exitN:SetMaterialFlag( EMF_LIGHTING, false )
	selector = sceneManager:CreateTriangleSelectorFromBoundingBox( exitN:Self() )
	exitN:SetTriangleSelector( selector )
	sceneManager:SetMarkText( exitN:Self(), "exit" )
	officeSceneObjects[ 2 ] = exitN:Self()	

end

function AddObjectsToUniverScene()
	local x = 1
	local y = 1
	local delta = 800
	
	local stuffNode = CLuaSceneNode( sceneManager:AddCubeSceneNode( "stuffNode" ) )
	stuffNode:SetScale( 30, 40, 30 )
	stuffNode:SetVisible( false )
	stuffNode:SetPosition( x * delta, 800, y *delta )
	stuffNode:SetMaterialTexture( 0, "media/t351sml.jpg" )
	stuffNode:SetMaterialFlag( EMF_LIGHTING, false )
	local selector = sceneManager:CreateTriangleSelectorFromBoundingBox( stuffNode:Self() )
	stuffNode:SetTriangleSelector( selector )
	sceneManager:SetMarkText( stuffNode:Self(), "stuff" )
	univerSceneObjects[ 0 ] = stuffNode:Self()	
	x = x + 1
	
	local exitN = CLuaSceneNode( sceneManager:AddCubeSceneNode( "exitUniverNode" ) )
	exitN:SetScale( 30, 40, 30 )
	exitN:SetVisible( false )
	exitN:SetPosition( x * delta, 800, y *delta )
	exitN:SetMaterialTexture( 0, "media/t351sml.jpg" )
	exitN:SetMaterialFlag( EMF_LIGHTING, false )
	local selector = sceneManager:CreateTriangleSelectorFromBoundingBox( exitN:Self() )
	exitN:SetTriangleSelector( selector )
	sceneManager:SetMarkText( exitN:Self(), "exit" )
	univerSceneObjects[ 1 ] = exitN:Self()	
	x = x + 1
end

function AddObjectsToBankScene()
	
	local x = 1
	local y = 1
	local delta = 800
	
	local loan = CLuaSceneNode( sceneManager:AddCubeSceneNode( "loanNode" ) )
	loan:SetScale( 30, 40, 30 )
	loan:SetVisible( false )
	loan:SetPosition( x * delta, 800, y *delta )
	loan:SetMaterialTexture( 0, "media/t351sml.jpg" )
	loan:SetMaterialFlag( EMF_LIGHTING, false )
	local selector = sceneManager:CreateTriangleSelectorFromBoundingBox( loan:Self() )
	loan:SetTriangleSelector( selector )
	sceneManager:SetMarkText( loan:Self(), "loan" )
	bankSceneObjects[ 0 ] = loan:Self()	
	x = x + 1
	
	local deposit = CLuaSceneNode( sceneManager:AddCubeSceneNode( "depositNode" ) )
	deposit:SetScale( 30, 40, 30 )
	deposit:SetVisible( false )
	deposit:SetPosition( x * delta, 800, y *delta )
	deposit:SetMaterialTexture( 0, "media/t351sml.jpg" )
	deposit:SetMaterialFlag( EMF_LIGHTING, false )
	local selector = sceneManager:CreateTriangleSelectorFromBoundingBox( deposit:Self() )
	deposit:SetTriangleSelector( selector )
	sceneManager:SetMarkText( deposit:Self(), "deposit" )
	bankSceneObjects[ 1 ] = deposit:Self()	
	x = x + 1
	
	local akcii = CLuaSceneNode( sceneManager:AddCubeSceneNode( "akciiNode" ) )
	akcii:SetScale( 30, 40, 30 )
	akcii:SetVisible( false )
	akcii:SetPosition( x * delta, 800, y *delta )
	akcii:SetMaterialTexture( 0, "media/t351sml.jpg" )
	akcii:SetMaterialFlag( EMF_LIGHTING, false )
	local selector = sceneManager:CreateTriangleSelectorFromBoundingBox( akcii:Self() )
	akcii:SetTriangleSelector( selector )
	sceneManager:SetMarkText( akcii:Self(), "akcii" )
	bankSceneObjects[ 2 ] = akcii:Self()	
	x = x + 1
	
	local exitN = CLuaSceneNode( sceneManager:AddCubeSceneNode( "exitBankNode" ) )
	exitN:SetScale( 30, 40, 30 )
	exitN:SetVisible( false )
	exitN:SetPosition( x * delta, 800, y *delta )
	exitN:SetMaterialTexture( 0, "media/t351sml.jpg" )
	exitN:SetMaterialFlag( EMF_LIGHTING, false )
	local selector = sceneManager:CreateTriangleSelectorFromBoundingBox( exitN:Self() )
	exitN:SetTriangleSelector( selector )
	sceneManager:SetMarkText( exitN:Self(), "exit" )
	bankSceneObjects[ 3 ] = exitN:Self()	
	x = x + 1
end

function SetVisibleObjects( array, visible )
	for i=0, #array do
		local node = CLuaSceneNode( array[ i ] )
		node:SetVisible( visible )
	end
end