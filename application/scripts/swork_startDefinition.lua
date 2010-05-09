local app = CLuaApplication( NrpGetApplication() )
local company = CLuaCompany( nil )
local user = CLuaUser( nil )

function sloginResetDataForNewGame()
	app:ResetData()
	company:SetObject( app:CreateCompany( app:GetCurrentProfileCompany() ) )
	user:SetObject( app:CreateUser( "RealPlayer", app:GetCurrentProfile() ) )
end

function sloginAddStartCompanyTechnology()
	local ge = CLuaGameEngine( app:CreateGameEngine( "simpleEngine" ) )
	ge:AddGenre( GT_SKILL )
	ge:SetGenreModuleNumber( 2 )
	ge:SetCodeVolume( 10000 )
	company:AddGameEngine( ge:Self() )
	
	Log({src=SCRIPT, dev=ODS|CON}, "SCRIPT-TEST:Создан движок simpleEngine")
end

function sloginAddStartPlayerDef()

	company:SetCEO( user:Self() )
	
end

function sloginAddStartPublicTechnology()

	local tech = CLuaTech( app:CreateTechnology( PT_GENRE ) )
	tech:SetTechType( GT_SKILL )
	tech:SetName( "Игра на реакцию" )
	tech:SetBaseCode( 0.3 )			--depended from engine code volume
	tech:SetAddingEngineCode( 0.3 )  --depended from tech code volume
	tech:SetEngineTechRequire( GT_SKILL, 10 )
	tech:SetEmployerSkillRequire( SKL_VIDEO_TECH, 10 )
	tech:SetEmployerSkillRequire( SKL_CODING, 10 )
	
	Log({src=SCRIPT, dev=ODS|CON}, "SCRIPT-TEST:Создана технология GT_SKILL")
	app:AddPublicTechnology( tech:Self() )
	
	local sndTech = CLuaTech( app:CreateTechnology( PT_SOUNDTECH ) )
	sndTech:SetTechType( SND_SPEAKER_SOUND )
	sndTech:SetName( "PC speaker sound" ) 
	sndTech:SetBaseCode( 0.2 )
	sndTech:SetAddingEngineCode( 0.3 )
	sndTech:SetEmployerSkillRequire( SKL_SOUND, 10 )

	Log({src=SCRIPT, dev=ODS|CON}, "SCRIPT-TEST:Создана технология SND_SPEAKER_SOUND")
	app:AddPublicTechnology( sndTech:Self() )
	
	local videoTech = CLuaTech( app:CreateTechnology( PT_VIDEOTECH ) )
	videoTech:SetTechType( VDT_TEXT_OUTPUT )
	videoTech:SetName( "Вывод графики в виде текста" )
	videoTech:SetBaseCode( 0.4 )
	videoTech:SetAddingEngineCode( 0.2 )
	videoTech:SetEngineTechRequire( VDT_TEXT_OUTPUT, 10 )
	videoTech:SetEmployerSkillRequire( SKL_VIDEO_TECH, 10 )
	
	Log({src=SCRIPT, dev=ODS|CON}, "SCRIPT-TEST:Создана технология VIDEO_TEXT_OUTPUT")
	app:AddPublicTechnology( videoTech:Self() )
	
	local advTech = CLuaTech( app:CreateTechnology( PT_ADVTECH ) )
	advTech:SetTechType( ADV_MEMORY )
	advTech:SetName( "Функции сохранения" )
	advTech:SetBaseCode( 0.4 )
	advTech:SetAddingEngineCode( 0.2 )
	advTech:SetEmployerSkillRequire( SKL_CODING, 5 )
	
	Log({src=SCRIPT, dev=ODS|CON}, "SCRIPT-TEST:Создана технология ADV_MEMORY")
	app:AddPublicTechnology( advTech:Self() )
	
	advTech:SetObject( app:CreateTechnology( PT_ADVTECH ) )
	advTech:SetTechType( ADV_JOYSTIK )
	advTech:SetName( "Джойстик" )
	advTech:SetBaseCode( 0.4 )
	advTech:SetAddingEngineCode( 0.2 )
	advTech:SetEmployerSkillRequire( SKL_CODING, 5 )
	
	Log({src=SCRIPT, dev=ODS|CON}, "SCRIPT-TEST:Создана технология ADV_MEMORY")
	app:AddPublicTechnology( advTech:Self() )
	
	local scriptTech = CLuaTech( app:CreateTechnology( PT_SCRIPTS ) )
	scriptTech:SetTechType( SCRLVL_SIMPLE )
	scriptTech:SetName( "Файлы конфигурации" )
	scriptTech:SetBaseCode( 1 )
	scriptTech:SetAddingEngineCode( 0.3 )
	scriptTech:SetEmployerSkillRequire( SKL_CODING, 10 )

	Log({src=SCRIPT, dev=ODS|CON}, "SCRIPT-TEST:Создана технология SCRLVL_SIMPLE")
	app:AddPublicTechnology( scriptTech:Self() )
	
	local mgTech = CLuaTech( app:CreateTechnology( PT_MINIGAME ) )
	mgTech:SetTechType( MNGM_TEXTQUEST )
	mgTech:SetName( "Текстовые квесты" )
	mgTech:SetBaseCode( 1 )
	mgTech:SetAddingEngineCode( 0.4 )
	mgTech:SetEmployerSkillRequire( SKL_CODING, 10 )

	Log({src=SCRIPT, dev=ODS|CON}, "SCRIPT-TEST:Создана технология MNGM_TEXTQUEST")
	app:AddPublicTechnology( mgTech:Self() )
	
	local phTech = CLuaTech( app:CreateTechnology( PT_PHYSIC ) )
	phTech:SetTechType( PHYS_2DCONCLUSION )
	phTech:SetName( "Простые столкновения" )
	phTech:SetBaseCode( 1 )
	phTech:SetAddingEngineCode( 0.5 )
	phTech:SetEmployerSkillRequire( SKL_CODING, 10 )

	Log({src=SCRIPT, dev=ODS|CON}, "SCRIPT-TEST:Создана технология SCRLVL_SIMPLE")
	app:AddPublicTechnology( phTech:Self() )
	
end

function sloginAddStartScenarioContentTechnology( ptr )
	local vScn = CLuaTech( app:CreateTechnology( PT_SCENARIOQUALITY ) )

	vScn:SetTechType( SCNQ_SELFWRITE )
	vScn:SetName( "Написать самому" )
	vScn:SetBaseCode( 0.2 )			--depended from engine code volume
	vScn:SetQuality( 10 )
	
	Log({src=SCRIPT, dev=ODS|CON}, "SCRIPT-TEST:Создана технология VSQ_SELFWRITE")
	app:AddPublicTechnology( vScn:Self() )
end

function sloginAddStartSoundContentTechnology( ptr )
	local vSn = CLuaTech( app:CreateTechnology( PT_SOUNDQUALITY ) )

	vSn:SetTechType( VSQ_SELFCOMPOSE )
	vSn:SetName( "Писать ноты" )
	vSn:SetBaseCode( 1 )			--depended from engine code volume
	vSn:SetEmployerSkillRequire( SKL_SOUND, 10 )
	
	Log({src=SCRIPT, dev=ODS|CON}, "SCRIPT-TEST:Создана технология VSQ_SELFCOMPOSE")
	app:AddPublicTechnology( vSn:Self() )
end

function sloginAddStartVideoContentTechnology( ptr )
	local vCn = CLuaTech( app:CreateTechnology( PT_VIDEOQUALITY ) )

	vCn:SetTechType( VDQ_SELFRENDER )
	vCn:SetName( "Рисовать самим" )
	vCn:SetBaseCode( 1 )			--depended from engine code volume
	vCn:SetEmployerSkillRequire( SKL_DRAWING, 10 )
	
	Log({src=SCRIPT, dev=ODS|CON}, "SCRIPT-TEST:Создана технология VDQ_SELFRENDER")
	app:AddPublicTechnology( vCn:Self() )
end