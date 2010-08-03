local terrainHeightMap = "media/terrain-heightmap.bmp"
local terrainName = "terrain"
local terrainTexture = "media/terrain-texture.jpg"
local terrainDetailMap = "media/detailmap3.jpg"

--создаем объект поверхности по карте высот
function AddTerrain()

	--получаем доступ к функциям поверхности
	local terrain = CLuaTerrain( sceneManager:AddTerrainSceneNode( terrainHeightMap, terrainName ) )

	--позиция
	terrain:SetPosition( 0, 0, 0 )

	--масштаб
	terrain:SetScale( 40, 5, 40 )

	--текстура поверхности и детализации
	terrain:SetMaterialTexture( 0, terrainTexture )
	terrain:SetMaterialTexture( 1, terrainDetailMap )

	--установить масштаб текстуры
	terrain:ScaleTexture( 1, 20 )

	--установить необходимость источника света для ландшафта
	local EMF_LIGHTING = 9
	terrain:SetMaterialFlag( EMF_LIGHTING, false )
																			
	terrain:SetMaterialType( 9 )		

	
	local selector = sceneManager:CreateTerrainTriangleSelector( terrain:Self(), 0 )
	terrain:SetTriangleSelector( selector ) 
								   
	local camera = CLuaCamera( sceneManager:GetActiveCamera() )
	camera:AddTerrain( terrain:Self() )
	
	sceneManager:LinkTerrain( terrain:Self() )
end