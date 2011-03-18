local base = _G

module( "userRecognition" )

local emp = nil
local mainWindow = nil
local guienv = base.guienv
local button = base.button

local rank = { "Лучший работник", "Золотой человек", "Злющий тестер", 
			   "Бродячий кот", "Подниматель пингвинов", "Главный идеолог",
			   "Геймер-испытатель", "Параноик", "Аналитик" }

local function _Apply( sender )
	sender = base.CLuaButton( sender )
	--получим коэффициент подъема отношения к компании,
	--который уменьшается с каждой добавкой к параметру
	
    if not emp:HaveModificator( sender.Text ) then
		local addingValue = base.math.random( 0, 20 )
		local addingKoeff = 1
		
		if emp.qualityAward > addingValue then
			addingKoeff = addingValue / emp.qualityAward
			addingValue = addingValue * addingKoeff
		end
		   
		
		emp:AddModificator( "codeQuality", base.math.random( 5, 30 ),
		  					false, addingValue, sender.text )
		emp.qualityAward = emp.qualityAward + addingValue
	else
		guienv:MessageBox( "Уже есть такой модификатор", false, false, button.CloseParent, nil )
	end
	
	--изменим отношение работника к компании
	local relation = emp:GetRelation( emp.company.name )
	relation.value = relation.value * 1.1
	--relation.value = realtion.value * ( 1 + emp:GetParam( "codeQualityLast" ) )
	
	--надо снизить удовольствие работников с уровнем выше
	--чем у награжденного
end

function Show()
	emp = base.userManager.currentEmployer
	if emp ~= nil then
		mainWindow = guienv:AddWindow( "recognition.png", "25%", "25%", "50%+", "50%+", -1, guienv.root )
		mainWindow.closeButton.visible = false
		
		local layout = guienv:AddLayout( "5%", "5%", "95%", "95%", 3, -1, mainWindow ) 
		
		for i=1, #rank do
			button.LayoutButton( "", layout, -1, rank[ i ], _Apply )
		end
		
		button.Stretch( "30e", "30e", "0e", "0e", "button_down", mainWindow, -1, "", button.CloseParent )
	end	
end