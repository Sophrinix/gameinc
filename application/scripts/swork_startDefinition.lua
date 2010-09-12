local company = nil
local user = CLuaUser( nil )

function sloginResetDataForNewGame()
	applic:ResetData()
    user:Create( "RealPlayer", applic:GetCurrentProfile() )
    company = applic:GetPlayerCompany()
	company:Create( applic:GetCurrentProfileCompany(), applic:GetCurrentProfile() ) 
end

function sloginAddStartCompanyTechnology()
	
	local ge = CLuaGameEngine( nil )
	ge:Create( "simpleEngine" )
	ge:Load( "xtras/engines/simpleEngine" )
	company:AddGameEngine( ge:Self() )
	
	Log({src=SCRIPT, dev=ODS|CON}, "SCRIPT-TEST:������ ������ simpleEngine")
end

function sloginAddStartPlayerDef()
	company:SetCEO( user:Self() )
end

function sloginAddStartPublicTechnology()

	local tech = CLuaTech( nil )
	tech:Create( PT_GENRE )
	tech:SetStatus( TS_READY )
	tech:Load( "xtras/technology/genre_skill/item.tech" )
	
	local tech2 = CLuaTech( nil )
	tech2:Create( PT_GENRE )
	tech2:SetStatus( TS_READY )
	tech2:Load( "xtras/technology/genre_contact/item.tech" )
	
	Log({src=SCRIPT, dev=ODS|CON}, "SCRIPT-TEST:������� ���������� GT_SKILL")
	applic:AddPublicTechnology( tech:Self() )
	applic:AddPublicTechnology( tech2:Self() )
	
	local sndTech = CLuaTech( nil )
	sndTech:Create( PT_SOUNDTECH )
	sndTech:SetStatus( TS_READY )
	sndTech:Load( "xtras/technology/sound_speaker/item.tech" )

	Log({src=SCRIPT, dev=ODS|CON}, "SCRIPT-TEST:������� ���������� SND_SPEAKER_SOUND")
	applic:AddPublicTechnology( sndTech:Self() )
	
	local videoTech = CLuaTech( nil )
	videoTech:Create( PT_VIDEOTECH )
	videoTech:SetTechType( VDT_TEXT_OUTPUT )
	videoTech:SetName( "����� ������� � ���� ������" )
	videoTech:SetBaseCode( 0.4 )
	videoTech:SetAddingEngineCode( 0.2 )
	videoTech:SetEngineTechRequire( VDT_TEXT_OUTPUT, 10 )
	videoTech:SetEmployerSkillRequire( SKL_VIDEO_TECH, 10 )
	
	Log({src=SCRIPT, dev=ODS|CON}, "SCRIPT-TEST:������� ���������� VIDEO_TEXT_OUTPUT")
	applic:AddPublicTechnology( videoTech:Self() )
	
	local advTech = CLuaTech( nil )
	advTech:Create( PT_ADVTECH )
	advTech:SetTechType( ADV_MEMORY )
	advTech:SetName( "������� ����������" )
	advTech:SetBaseCode( 0.4 )
	advTech:SetAddingEngineCode( 0.2 )
	advTech:SetEmployerSkillRequire( SKL_CODING, 5 )
	
	Log({src=SCRIPT, dev=ODS|CON}, "SCRIPT-TEST:������� ���������� ADV_MEMORY")
	applic:AddPublicTechnology( advTech:Self() )
	
	advTech:Create( PT_ADVTECH )
	advTech:SetTechType( ADV_JOYSTIK )
	advTech:SetName( "��������" )
	advTech:SetBaseCode( 0.4 )
	advTech:SetAddingEngineCode( 0.2 )
	advTech:SetEmployerSkillRequire( SKL_CODING, 5 )
	
	Log({src=SCRIPT, dev=ODS|CON}, "SCRIPT-TEST:������� ���������� ADV_MEMORY")
	applic:AddPublicTechnology( advTech:Self() )
	
	local scriptTech = CLuaTech( nil )
	scriptTech:Create( PT_SCRIPTS )
	scriptTech:SetTechType( SCRLVL_SIMPLE )
	scriptTech:SetName( "����� ������������" )
	scriptTech:SetBaseCode( 1 )
	scriptTech:SetAddingEngineCode( 0.3 )
	scriptTech:SetEmployerSkillRequire( SKL_CODING, 10 )

	Log({src=SCRIPT, dev=ODS|CON}, "SCRIPT-TEST:������� ���������� SCRLVL_SIMPLE")
	applic:AddPublicTechnology( scriptTech:Self() )
	
	local mgTech = CLuaTech( nil )
	mgTech:Create( PT_MINIGAME )
	mgTech:SetTechType( MNGM_TEXTQUEST )
	mgTech:SetName( "��������� ������" )
	mgTech:SetBaseCode( 1 )
	mgTech:SetAddingEngineCode( 0.4 )
	mgTech:SetEmployerSkillRequire( SKL_CODING, 10 )

	Log({src=SCRIPT, dev=ODS|CON}, "SCRIPT-TEST:������� ���������� MNGM_TEXTQUEST")
	applic:AddPublicTechnology( mgTech:Self() )
	
	local phTech = CLuaTech( nil )
	phTech:Create( PT_PHYSIC )
	phTech:SetTechType( PHYS_2DCONCLUSION )
	phTech:SetName( "������� ������������" )
	phTech:SetBaseCode( 1 )
	phTech:SetAddingEngineCode( 0.5 )
	phTech:SetEmployerSkillRequire( SKL_CODING, 10 )

	Log({src=SCRIPT, dev=ODS|CON}, "SCRIPT-TEST:������� ���������� SCRLVL_SIMPLE")
	applic:AddPublicTechnology( phTech:Self() )
end

function sloginAddStartScenarioContentTechnology( ptr )
	local vScn = CLuaTech( nil )
	vScn:Create( PT_SCENARIOQUALITY ) 
	vScn:SetStatus( TS_READY )
	vScn:Load( "xtras/technology/scnq_writeself/item.tech" )
	
	Log({src=SCRIPT, dev=ODS|CON}, "SCRIPT-TEST:������� ���������� VSQ_SELFWRITE")
	applic:AddPublicTechnology( vScn:Self() )
end

function sloginAddStartSoundContentTechnology( ptr )
	local vSn = CLuaTech( nil )
	vSn:Create( PT_SOUNDQUALITY )
	vSn:SetStatus( TS_READY )
	vSn:Load( "xtras/technology/sndq_writeself/item.tech" )
    	
	Log({src=SCRIPT, dev=ODS|CON}, "SCRIPT-TEST:������� ���������� VSQ_SELFCOMPOSE")
	applic:AddPublicTechnology( vSn:Self() )
end

function sloginAddStartVideoContentTechnology( ptr )
	local vCn = CLuaTech( nil )
	vCn:Create( PT_VIDEOQUALITY )

	vCn:SetTechType( VDQ_SELFRENDER )
	vCn:SetName( "�������� �����" )
	vCn:SetBaseCode( 1 )			--depended from engine code volume
	vCn:SetEmployerSkillRequire( SKL_DRAWING, 10 )
	
	Log({src=SCRIPT, dev=ODS|CON}, "SCRIPT-TEST:������� ���������� VDQ_SELFRENDER")
	applic:AddPublicTechnology( vCn:Self() )
end