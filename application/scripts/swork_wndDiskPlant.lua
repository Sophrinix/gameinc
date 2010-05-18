local guienv = CLuaGuiEnvironment( NrpGetGuiEnvironment() ) 
local applic = CLuaApplication( NrpGetApplication() )
local compamy = CLuaCompany( nil )
local wndDPP = CLuaWindow( nil )

local width = 800
local height = 600

function sworkCreateDiskProducePlantWindow( ptr )
	company:SetObject( applic:GetPlayerCompany() )
	wndDPP:SetObject( guienv:GetElementByName( WINDOW_DISKPRODUCEPLANT_NAME ) )
	wndDPP:Remove()
	
	wndDPP:SetObject( guienv:AddWindow( "GameBoxWizzard", 0, 0, width, height, WINDOW_DISKPRODUCEPLANT_ID, guienv:GetRootGUIElement() ) )
	wndDPP:SetName( WINDOW_DISKPRODUCEPLANT_NAME )
	
	--добавим выпадающий список типа аппаратов
	local cmbxProduceType = CLuaComboBox( guienv:AddComboBox( "", 10, 20, width / 2 - 10, 40, WNDDPP_CMBX_PRODUCE_TYPE, wndDPP:Self() ) )
	
	local diskMachine = CLuaDiskMachine( nil )
	for i=1, applic:GetDiskMachineNumber() do
		diskMachine:SetObject( applic:GetDiskMachine( i-1 ) )
		cmbxProduceType:AddItem( diskMachine:GetName(), diskMachine:Self() )		
	end
	wndDPP:AddLuaFunction( GUIELEMENT_CMBXITEM_SELECTED, "" )
	
	--добавим метку цены за наем одного аппарата
	
	
	--добавим кнопки изменения количества аппаратов для производства дисков
	--добавим кнопки изменения количества дней производства
	--добавим метку количества произведенных дисков
	--добавим метку общей цены за производство
	--добавим метку цены одного диска
end