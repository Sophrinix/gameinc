local base = _G



module( "worldMap" )

local window = base.window
local guienv = base.guienv
local button = base.button
local applic = base.applic
local questenv = base.questenv

local _mainWindow = nil
local _mapWindow = nil

local function _Hide()

end

function Show()
	local txsBlur = base.driver:CreateBlur( "director_cabinet.png", 2, 4 )
	_mainWindow = window.fsWindow( txsBlur.path, _Hide )
	
	_mapWindow = guienv:AddWindow( "worldMap.png", 0, 89, "0e", "89e", -1, _mainWindow )
	
	for i=1, questenv.numberActives do
		local qst = questenv.GetActiveQuest( i-1 )
		
		if not qst.finished then
			local btn = button.EqualeTexture( qst.x, qst.y, "button_quest", _mapWindow, -1, qst.name, qst.func )
		end
	end
end
