require("elmid")

local wndHeight = 180
local portfelleWnd = nil
local maxObjectNumber = 0							--����� ��������, ������� ������������ � ����
local startObjectNumber = 0							--����� ���������� �������

function AddPortfelleWindow()

	portfelleWnd = guienv:AddWindow( "portfelleWindow", wndHeight, scrHeight - wndHeight, scrWidth, scrHeight, -1, guienv.root  )
	maxObjectNumber = ( scrWidth - wndHeight ) / wndHeight
	
	sworkUpdateCompanyPortfelle()
end

function sworkUpdateCompanyPortfelle()
	local company = applic.playerCompany
	for i=1, company:GetObjectsInPortfelle() do
		if i-1 > maxObjectNumber then 
		   return 
		end
		
		local config = company:GetFromPortfelle( i - 1 )
		local linkModule = guienv:AddLinkBox( config.name, 10, 50, 10 + 50, 50 + 50, -1, portfelleWnd )
		linkModule.data = config.object
		--linkModule:
		--linkModule:AddLuaFunction( GUIELEMENT_LMOUSE_LEFTUP, "sworkLeftMouseButtonUp" )
		--linkModule:AddLuaFunction( GUIELEMENT_RMOUSE_LEFTUP, "sworkRigthMouseButtonUp" )
	end
end