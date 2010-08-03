require("elmid")

local wndHeight = 180
local portfelleWnd = CLuaWindow( nil )
local maxObjectNumber = 0							--число объектов, которые отображаются в окне
local startObjectNumber = 0							--номер стартового объекта

function AddPortfelleWindow()

	portfelleWnd:SetObject( guienv:AddWindow( "portfelleWindow", wndHeight, scrHeight - wndHeight, scrWidth, scrHeight, -1, guienv:GetRootGUIElement() ) )
	portfelleWnd:SetName( NAME_PORTFELLE_WINDOW )
	maxObjectNumber = ( scrWidth - wndHeight ) / wndHeight
	
	sworkUpdateCompanyPortfelle()
end

function sworkUpdateCompanyPortfelle()
	local company = CLuaCompany( applic:GetPlayerCompany() )
	for i=1, company:GetObjectsInPortfelle() do
		if i-1 > maxObjectNumber then 
		   return 
		end
		
		local config = CLuaProject( company:GetFromPortfelle( i - 1 ) )
		local linkModule = CLuaLinkBox( guienv:AddLinkBox( config:GetName(), 10, 50, 10 + 50, 50 + 50, -1, portfelleWnd:Self() ) )
		linkModule:SetData( config:Self() )
		--linkModule:
		--linkModule:AddLuaFunction( GUIELEMENT_LMOUSE_LEFTUP, "sworkLeftMouseButtonUp" )
		--linkModule:AddLuaFunction( GUIELEMENT_RMOUSE_LEFTUP, "sworkRigthMouseButtonUp" )
	end
end