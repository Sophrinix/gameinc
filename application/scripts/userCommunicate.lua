local base = _G

IncludeScript( "userRecognition" )

module( "userCommunicate" )

local guienv = base.guienv
local button = base.button

local emp = nil
local mainWindow = nil

local function _Hide()
	base.package.loaded[ "userRecognition" ] = false
end

local function _GrantWork()
	--  emp:AddTimeWork( <days for work >, <name for work>, <duration of effect>, <param name to update>, <value to update> )
	local ret = emp:AddTimeWork( 3, "Ответственное задание", 14, "mood", 10 )
	
	--ret - это коэффициент прироста уважения к конторе
	local relation = emp:GetRelation( emp.company.name )
	relation.value = realtion.value * ( 1 + ret )
	guienv:MessageBox( "Работник отправлен на ответственное задание", false, false, button.CloseParent, nil )
end

function Show()
	emp = base.userManager.currentEmployer
	if emp ~= nil then
		mainWindow = guienv:AddWindow( "communicate.png", "25%", "25%", "50%+", "50%+", -1, guienv.root )
		mainWindow.closeButton.visible = false
		
		local layout = guienv:AddLayout( "5%", "5%", "95%", "95%", 3, -1, mainWindow ) 
		
		button.LayoutButton( "medal", layout, -1, "Признание заслуг", base.userRecognition.Show )
		button.LayoutButton( "grant_work", layout, -1, "Отвественное задание", _GrantWork )
		button.LayoutButton( "discuss", layout, -1, "Обсудить состояние проекта", _Discuss )		
		button.LayoutButton( "leadtalk", layout, -1, "Беседа по душам", _LeadTalk )	
		button.LayoutButton( "document", layout, -1, "Грамота", _Document )	
		button.LayoutButton( "workspace_upgrade", layout, -1, "Улучшить рабочее место", _WorkPlaceUpgrade )	
		button.LayoutButton( "opcion", layout, -1, "Подарить акции", _CompanyOpcion )	
		button.LayoutButton( "medical", layout, -1, "Медстраховка", _Medical )	

		button.Stretch( "30e", "30e", "0e", "0e", "button_down", mainWindow, -1, "", _Hide )
	end
end
