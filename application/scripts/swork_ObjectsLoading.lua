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
				
				
	x = x + 1
	local univer = CLuaSceneNode( sceneManager:AddCubeSceneNode( "univerNode" ) )
	univer:SetScale( 40, 40, 40 )
	univer:SetPosition( x * delta, 1000, y * delta )
	univer:SetMaterialTexture( 0, "media/t351sml.jpg" )
	univer:SetMaterialFlag( EMF_LIGHTING, false )
	selector = sceneManager:CreateTriangleSelectorFromBoundingBox( univer:Self() ) 
	univer:SetTriangleSelector( selector )
	sceneManager:SetMarkText( univer:Self(), "univer" )
	
	x = x + 1; y = y + 1
	local bank = CLuaSceneNode( sceneManager:AddCubeSceneNode( "bankNode" ) )
	bank:SetScale( 40, 50, 40 )
	bank:SetPosition( x * delta, 1000, y * delta )
	bank:SetMaterialTexture( 0, "media/t351sml.jpg" )
	bank:SetMaterialFlag( EMF_LIGHTING, false )
	selector = sceneManager:CreateTriangleSelectorFromBoundingBox( bank:Self() ) 
	bank:SetTriangleSelector( selector )
	sceneManager:SetMarkText( bank:Self(), "Bank" )
	
	x = x + 1; y = y - 1
	local gameShop = CLuaSceneNode( sceneManager:AddCubeSceneNode( "gameShopNode" ) )
	gameShop:SetScale( 40, 20, 40 )
	gameShop:SetPosition( x * delta, 1000, y * delta )
	gameShop:SetMaterialTexture( 0, "media/t351sml.jpg" )
	gameShop:SetMaterialFlag( EMF_LIGHTING, false )
	selector = sceneManager:CreateTriangleSelectorFromBoundingBox( gameShop:Self() ) 
    gameShop:SetTriangleSelector( selector )
	sceneManager:SetMarkText( gameShop:Self(), "gameShop" )
	
	x = x + 1; y = y + 1
	local labor = CLuaSceneNode( sceneManager:AddCubeSceneNode( "laborNode" ) )
	labor:SetScale( 40, 20, 40 )
	labor:SetPosition( x * delta, 1000, y * delta )
	labor:SetMaterialTexture( 0, "media/t351sml.jpg" )
	labor:SetMaterialFlag( EMF_LIGHTING, false )
	selector = sceneManager:CreateTriangleSelectorFromBoundingBox( labor:Self() ) 
    labor:SetTriangleSelector( selector )
	sceneManager:SetMarkText( labor:Self(), "laboratory" )
	
	x = x + 1; y = y - 1
	local pr = CLuaSceneNode( sceneManager:AddCubeSceneNode( "prNode" ) )
	pr:SetScale( 40, 20, 40 )
	pr:SetPosition( x * delta, 1000, y * delta )
	pr:SetMaterialTexture( 0, "media/t351sml.jpg" )
	pr:SetMaterialFlag( EMF_LIGHTING, false )
	selector = sceneManager:CreateTriangleSelectorFromBoundingBox( pr:Self() ) 
    pr:SetTriangleSelector( selector )
	sceneManager:SetMarkText( pr:Self(), "PR kontora" )
	
	x = x + 1; y = y + 1
	local cinema = CLuaSceneNode( sceneManager:AddCubeSceneNode( "cinemaNode" ) )
	cinema:SetScale( 40, 20, 40 )
	cinema:SetPosition( x * delta, 1000, y * delta )
	cinema:SetMaterialTexture( 0, "media/t351sml.jpg" )
	cinema:SetMaterialFlag( EMF_LIGHTING, false )
	selector = sceneManager:CreateTriangleSelectorFromBoundingBox( cinema:Self() ) 
    cinema:SetTriangleSelector( selector )
	sceneManager:SetMarkText( cinema:Self(), "Cinema" )
	
	x = x + 1; y = y - 1
	local pizza = CLuaSceneNode( sceneManager:AddCubeSceneNode( "pizzaNode" ) )
	pizza:SetScale( 40, 20, 40 )
	pizza:SetPosition( x * delta, 1000, y * delta )
	pizza:SetMaterialTexture( 0, "media/t351sml.jpg" )
	pizza:SetMaterialFlag( EMF_LIGHTING, false )
	selector = sceneManager:CreateTriangleSelectorFromBoundingBox( pizza:Self() ) 
    pizza:SetTriangleSelector( selector )
	sceneManager:SetMarkText( pizza:Self(), "Sabotaje" )
	
	x = x + 1; y = y + 1
	local plant = CLuaSceneNode( sceneManager:AddCubeSceneNode( "plantNode" ) )
	plant:SetScale( 40, 20, 40 )
	plant:SetPosition( x * delta, 1000, y * delta )
	plant:SetMaterialTexture( 0, "media/t351sml.jpg" )
	plant:SetMaterialFlag( EMF_LIGHTING, false )
	selector = sceneManager:CreateTriangleSelectorFromBoundingBox( plant:Self() ) 
	plant:SetTriangleSelector( selector )
	sceneManager:SetMarkText( plant:Self(), "Proizvodstvo" )

end