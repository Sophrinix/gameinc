local guienv = CLuaGuiEnvironment( NrpGetGuiEnvironment() ) 
local applic = CLuaApplication( NrpGetApplication() )
local company = CLuaCompany( nil )
local wndDPP = CLuaWindow( nil )
local produceDiskWork = CLuaPlantWork( nil )
local dayOfProduce = 0
local numberMachine = 0
local labelPricePrint = CLuaLabel( nil )
local currentDiskMachine = CLuaDiskMachine( nil )
local labelPerdomance = CLuaLabel( nil )
local labelPriceHour = CLuaLabel( nil )
local labelNumberMachine = CLuaLabel( nil )
local labelNumberDay = CLuaLabel( nil )
local labelDiskNumber = CLuaLabel( nil )
local labelFinalPrice = CLuaLabel( nil )
local labelDiskPrice = CLuaLabel( nil )

local width = 800
local height = 600

function sworkCreateDiskProducePlantWindow( ptr )
	numberMachine = 0
	dayOfProduce = 0
	produceDiskWork:Create()
	company:SetObject( applic:GetPlayerCompany() )
	wndDPP:SetObject( guienv:GetElementByName( WINDOW_DISKPRODUCEPLANT_NAME ) )
	wndDPP:Remove()
	
	wndDPP:SetObject( guienv:AddWindow( "GameBoxWizzard", 0, 0, width, height, WINDOW_DISKPRODUCEPLANT_ID, guienv:GetRootGUIElement() ) )
	wndDPP:SetName( WINDOW_DISKPRODUCEPLANT_NAME )
	
	--добавим выпадающий список типа аппаратов
	local cmbxProduceType = CLuaComboBox( guienv:AddComboBox( "", 10, 20, width / 2 - 10, 40, WNDDPP_CMBX_PRODUCE_TYPE, wndDPP:Self() ) )
	
	for i=1, applic:GetDiskMachineNumber() do
		local dm = CLuaDiskMachine( applic:GetDiskMachine( i-1 ) )
		cmbxProduceType:AddItem( dm:GetName(), dm:Self() )		
	end
	wndDPP:AddLuaFunction( GUIELEMENT_CMBXITEM_SELECTED, "sworkWndDiskProducePlantCmbxItemSelected" )
	
	--добавим метку цены за наем одного аппарата
	labelPricePrint:SetObject( guienv:AddLabel( "Плата за размещение:", 
												width / 2 + 10, 20, width - 10, 40, 
												-1, wndDPP:Self() ) )
	--Добавим метку производительности аппарата
	labelPerdomance:SetObject( guienv:AddLabel( "Производительность (коробок\час):", 
												width / 2 + 10, 50, width - 10, 70, 
												-1, wndDPP:Self() ) )
	--добавим метку стоимости работы в час
	labelPriceHour:SetObject( guienv:AddLabel( "Стоимость работы ($\час):", 
											   width / 2 + 10, 80, width - 10, 100, 
											   -1, wndDPP:Self() ) )
	
	--добавим кнопки изменения количества аппаратов для производства дисков
	local btn = CLuaButton( guienv:AddButton( 10, 50, 60, 100, wndDPP:Self(), -1, "+" ) )
	btn:SetAction( "sworkWndDiskProducePlantIncMachineNumber" )
	
	labelNumberMachine:SetObject( guienv:AddLabel( "Количество линий сборки:",
												   100, 50, width - 60, 100, 
												   -1, wndDPP:Self() ) )
	btn:SetObject( guienv:AddButton( width - 60, 50, width - 10, 160, wndDPP:Self(), -1, "-" ) )
	btn:SetAction( "sworkWndDiskProducePlantDecMachineNumber" )
	
	--добавим кнопки изменения количества дней производства
	btn:SetObject( guienv:AddButton( 10, 110, 60, 160, wndDPP:Self(), -1, "+" ) )
	btn:SetAction( "sworkWndDiskProducePlantIncDayNumber" )
	
	labelNumberDay:SetObject( guienv:AddLabel( "Дней производства:", 
											   100, 110, width - 60, 160, 
											   -1, wndDPP:Self() ) )
	btn:SetObject( guienv:AddButton( width - 60, 110, width - 10, 160, wndDPP:Self(), -1, "-" ) )
	btn:SetAction( "sworkWndDiskProducePlantDecDayNumber" )
	
	--добавим метку количества произведенных дисков
	labelDiskNumber:SetObject( guienv:AddLabel( "Количество дисков:", 
												width / 2 + 10, 170, width - 10, 220, 
												-1, wndDPP:Self() ) )
	
	--добавим метку общей цены за производство
	labelFinalPrice:SetObject( guienv:AddLabel( "Общая цена:", 
											    width / 2 + 10, 230, width - 10, 280, 
											    -1, wndDPP:Self() ) )
	
	--добавим метку цены одного диска
	labelDiskPrice:SetObject( guienv:AddLabel( "Цена одного диска:", 
											   width / 2 + 10, 290, width - 10, 340, 
											   -1, wndDPP:Self() ) )
											   
	local createBtn = CLuaButton( guienv:AddButton( 10, height - 50, width / 2 - 10, height - 10, 
													wndDPP:Self(), -1, "Запустить" ) )
	createBtn:SetAction( "sworkWndDiskProducePlantLocate" )
	
	local closeBtn = CLuaButton( guienv:AddButton( width / 2 + 10, height - 50, width - 10, height - 10, 
												   wndDPP:Self(), -1, "Выход" ) )
	closeBtn:SetAction( "sworkWndDiskProducePlantClose" )
end

function sworkWndDiskProducePlantLocate( ptr )
	company:AddProduceWork( produceDiskWork:Self() )
	wndDPP:Remove()
end

function sworkWndDiskProducePlantClose( ptr )
	produceDiskWork:Remove()
	produceDiskWork:SetObject( nil )
	
	wndDPP:Remove()
end

local function UpdateLabels()
	labelPricePrint:SetText( "Плата за размещение:"..produceDiskWork:GetRentPrice().."$" )
	labelPerdomance:SetText( "Производительность:"..produceDiskWork:GetHourPerfomance().."(коробок\час)" )
	labelPriceHour:SetText( "Стоимость работы:"..produceDiskWork:GetHourPrice().."($\час)" )
	labelNumberMachine:SetText( "Количество линий сборки:"..produceDiskWork:GetNumberMachine().."(шт)" )
	labelNumberDay:SetText( "Дней производства:"..produceDiskWork:GetNumberDay().."(дн)" )
	labelDiskNumber:SetText(  "Количество дисков:"..produceDiskWork:GetNumberDisk().."(шт)" ) 
	labelDiskPrice:SetText( "Цена одного диска:"..produceDiskWork:GetDiskPrice().."$" )  
end

function sworkWndDiskProducePlantIncDayNumber( ptr )
	dayOfProduce = dayOfProduce + 1
	produceDiskWork:SetDayOfProduce( dayOfProduce )
	
	UpdateLabels()
end

function sworkWndDiskProducePlantDecDayNumber( ptr )
	dayOfProduce = dayOfProduce - 1
	produceDiskWork:SetDayOfProduce( dayOfProduce )
	
	UpdateLabels()
end

function sworkWndDiskProducePlantIncMachineNumber( ptr )
	numberMachine = numberMachine + 1
	produceDiskWork:SetNumberMachine( numberMachine )
	
	UpdateLabels()
end

function sworkWndDiskProducePlantDecMachineNumber( ptr )
	numberMachine = numberMachine - 1
	produceDiskWork:SetNumberMachine( numberMachine )
	
	UpdateLabels()
end

function sworkWndDiskProducePlantCmbxItemSelected( ptr )
	local cmbx = CLuaComboBox( ptr )
	
	currentDiskMachine:SetObject( cmbx:GetSelectedObject() )
	produceDiskWork:SetProduceType( currentDiskMachine:Self() )

	UpdateLabels()
end