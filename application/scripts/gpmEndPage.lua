local base = _G

module( "gpmEndPage" )

function Show()
	localUpdateProjectWindow( "end" )

	local flick = guienv:AddFlick( "55%", "5%", "95%", "95%", 3, -1, projectWindow )
	
	FlickLinkBox( flick, base.STR_ENGINE, 0, project.gameEngine,
				  NDRG, DSBL, nil, nil, nil, nil )
	
	FlickLinkBox( flick, base.STR_GENRE, 0, project:GetTech( 0, base.PT_GENRE ), 
				  NDRG, DSBL, nil, nil, nil, nil ) 
	
	local lnk = FlickLinkBox( flick, base.STR_CONTENT, 0, nil,
							  NDRG, DSBL, nil, nil, nil, nil )
							   
	if project.haveLicense or project.haveScenario then 
	   if project.haveLicense then
			lnk.texture = project.license.texture
	   else
			lnk.texture = project.scenario.texture
	   end
	end
	
	lnk = FlickLinkBox( flick, base.STR_PLATFORMS, 0, nil, 
						 NDRG, DSBL, nil, nil, nil, nil )
						 
	if project:GetNumber( base.PT_PLATFORM ) > 0 then 
		lnk.texture = "media/textures/havePlatform.png"
	end
	
	lnk = FlickLinkBox( flick, base.STR_LOCALIZATION, 0, nil, 
						 NDRG, DSBL, nil, nil, nil, nil )
	if project:GetNumber( base.PT_LANGUAGE ) > 0 then 
		lnk.texture = "media/textures/haveLangs.png"
	end
	
	local button = guienv:AddButton( "55%", "80%", "95%", "90%", projectWindow, -1, base.STR_FINISH )
	button.action = CreateProjectGame
	button.enabled = project.projectReady
end
