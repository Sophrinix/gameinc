-- Start up a new LuaGameObject wrapper class and pass the global gameobject
-- C++ lightuserdata pointer into it
terrain = NrpTerrain(globalTerrain, "media/thm_demo.bmp")

-- Call a Lua function on this object
terrain:setPosition( 0, 0, 0 )
terrain:setScale( 40, 5, 40 )
