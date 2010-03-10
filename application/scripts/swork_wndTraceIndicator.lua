require("elmid")
require("NerpaActionType")

local guienv = CLuaGuiEnvironment( NrpGetGuiEnvironment() )
local sceneManager = CLuaSceneManager( NrpGetSceneManager() )
local driver = CLuaDriver( NrpGetVideoDriver() )

local NUMBER_CHANNEL_TO_VIEW = 5
local TEXTURE_WIDTH = 128
local TEXTURE_HEIGTH = 512

local HEADER_HEIGHT = 20
local FOOTER_HEIGHT = 10
local INDICATOR_WIDTH = TEXTURE_WIDTH * NUMBER_CHANNEL_TO_VIEW + NUMBER_CHANNEL_TO_VIEW
local INDICATOR_HEIGHT = TEXTURE_HEIGTH + HEADER_HEIGHT + FOOTER_HEIGHT
local STEP = 5
local INDICATOR_TEXTURE_NAME = "_IndicatorTexture_"
local IMAGE_ID = ID_WINDOW_TRACE_INDICATOR + 10
--sequence - x-2 x-1 x x+1 x+2 (последовательный порядок)
--random - (произвольный порядок)
local modeChannelView = "sequence" 
local channel_number = { }
local selectedSonar

function sworkToggleIndicatorWindowVisible( ptr )

	local windowg = CLuaWindow( guienv:GetElementByName( WINDOW_TRACE_INDICATOR_NAME ) )
	
	if windowg:Empty() == 1 then	
		sworkIndicatorWindowCreate( nil )
	else
		sworkIndicatorWindowClose( nil )
	end

end

function sworkIndicatorWindowCreate( ptr )

	local windowg = CLuaWindow( guienv:AddWindow( "", 
	                                              -INDICATOR_WIDTH, 100, 
												  0 + 10, 100 + INDICATOR_HEIGHT + 10, 
												  -1, 
												  guienv:GetRootGUIElement() ) )
	
	Log({src=SCRIPT, dev=ODS}, "Окна индикатора нет, создаем заново" )
	
	windowg:SetDraggable( true )
	windowg:SetName( WINDOW_TRACE_INDICATOR_NAME )
			
	local button = CLuaButton( windowg:GetCloseButton() )
	button:SetVisible( false )
	
	button:SetObject( guienv:AddButton( INDICATOR_WIDTH + 10 - 15, 0, INDICATOR_WIDTH + 10, 15, windowg:Self(), -1, "X") )
	button:SetAction( "sworkIndicatorWindowClose" )
	
	button:SetObject( guienv:AddButton( INDICATOR_WIDTH + 10 - 15, 20, INDICATOR_WIDTH + 10, 35, windowg:Self(), -1, "M") )
	button:SetAction( "sworkIndicatorToggleRejimChannelView" )
		
	for i=0, NUMBER_CHANNEL_TO_VIEW-1 do
		local xBegin = (TEXTURE_WIDTH + 1)*i
		local image = CLuaImage( guienv:AddImage( xBegin, HEADER_HEIGHT, 
												  xBegin + TEXTURE_WIDTH, HEADER_HEIGHT + TEXTURE_HEIGTH,
												  windowg:Self(),
												  IMAGE_ID+i, 
												  "" ) )
												  
		image:SetImage( driver:AddTexture( TEXTURE_WIDTH, TEXTURE_HEIGTH, INDICATOR_TEXTURE_NAME..i ) )
		
		channel_number[ i ] = i % 4
	end
	
	--создадим аниматор, который переместит окно в новые координаты, оставит его видимым, а потом удалится сам
	guienv:AddMoveAnimator( windowg:Self(), 0, 150, STEP, true, true, false )
	--добавим обработчик обновления данных после рендеринга сцены
	sceneManager:AddSceneFunction( SCENE_AFTER_END, "sworkUpdateIndicatorData" )

end

function sworkIndicatorWindowClose( ptr )

	local windowg = CLuaWindow( guienv:GetElementByName( WINDOW_TRACE_INDICATOR_NAME ) )
	
	if windowg:Empty() == 1 then
		Log({src=SCRIPT, dev=ODS}, "Нет окна индикатора чтобы его закрывать" )
	else
		for i=0, NUMBER_CHANNEL_TO_VIEW-1 do
			driver:RemoveTexture( INDICATOR_TEXTURE_NAME..i )
		end
		
		windowg:RemoveAllAnimators() 
		guienv:AddMoveAnimator( windowg:Self(), -INDICATOR_WIDTH, 150, STEP, false, true, true )
	end
	
	sceneManager:RemoveSceneFunction( SCENE_AFTER_END, "sworkUpdateIndicatorData" )
	
end

function sworkUpdateIndicatorData( ptr )
	
	local selNode = CLuaSceneNode( sceneManager:GetSelectedNode() )
	local windowg = CLuaWindow( guienv:GetElementByName( WINDOW_TRACE_INDICATOR_NAME ) )

	if selNode:Empty() == 0 and selNode:GetTypeName() == "Sonar" then	
		local snrNode = CLuaSonarNode( selNode:Self() )
		local snr = CLuaSonar( snrNode:GetSonar() )
		
		for i=0, NUMBER_CHANNEL_TO_VIEW-1 do		
			if channel_number[ i ] > -1	and snr:IsDataUpdated( windowg:Self(), channel_number[ i ], CHNL_IMAGE ) then
				Log({src=SCRIPT, dev=ODS|CON}, "sworkUpdateIndicatorData stepupdate "..channel_number[ i ] )
				snr:UpdateTexture( windowg:Self(), 
								   channel_number[ i ], 
								   driver:GetTexture( INDICATOR_TEXTURE_NAME..i ) ) 
			
			end
		end
	end	
	
end

function sworkIndicatorToggleRejimChannelView( ptr )

	--функция для смены режимы отображения канала

end