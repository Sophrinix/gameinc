local app = CLuaApplication( NrpGetApplication() )
local company = CLuaCompany( app:CreateCompany( "DaleTeam" ) )
local user = CLuaUser( app:CreateUser( "player", "Dalerank" ) 

function AddStartCompanyTechnology()
	local ge = CLuaGameEngine( app:CreateGameEngine( "simpleEngine" ) )
	Log({src=SCRIPT, dev=ODS|CON}, "SCRIPT-TEST:Создан движок simpleEngine")
	ge:AddGenre( GT_SKILL )
	ge:SetGenreModuleNumber( 2 )
	ge:SetCodeVolume( 10000 )
	company:AddGameEngine( ge:Self() )
end

function AddStartPlayerDef()

	company:SetCEO( user:Self() )
	
end

function AddStartPublicTechnology()

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
	
	local advTech = CLuaTech( app:CreateTechnology( PT_ADVANCED ) )
	advTech:SetTechType( ADV_MEMORY )
	advTech:SetName( "Функции сохранения" )
	advTech:SetBaseCode( 0.4 )
	advTech:SetAddingEngineCode( 0.2 )
	advTech:SetEmployerSkillRequie( SKL_CODING, 5 )
	
	Log({src=SCRIPT, dev=ODS|CON}, "SCRIPT-TEST:Создана технология ADV_MEMORY")
	app:AddPublicTechnology( advTech:Self() )
end