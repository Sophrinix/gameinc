local base = _G

module( "tutorialProjectManager" )

function ShowProjects()
	local elm = base.button.ArrowToElement( base.projectManager.comboxProjects, "media/buttons/mouse_left.png", 5000 )
	base.browser:Move( elm.left, elm.bottom )
end

function ShowProjectComponents()
	local elm = base.button.ArrowToElement( base.projectManager.lbxComponents, "media/buttons/mouse_left.png", 5000 )
	base.browser:Move( elm.left, elm.bottom )
end

function ShowUserType()
	local elm = base.button.ArrowToElement( base.projectManager.layoutUserTypes, "media/buttons/mouse_left.png", 5000 )
	base.browser:Move( elm.left, elm.bottom )
end

function ShowUsers()
	local elm = base.button.ArrowToElement( base.projectManager.comboxUsers, "media/buttons/mouse_left.png", 5000 )
	base.browser:Move( elm.left, elm.bottom )
end

function ShowUserInfo()
	local elm = base.button.ArrowToElement( base.projectManager.windowUserInfo, "media/buttons/mouse_left.png", 5000 )
	base.browser:Move( elm.left, elm.bottom )
end

function ShowUserComponents()
	local elm = base.button.ArrowToElement( base.projectManager.lbxWorks, "media/buttons/mouse_left.png", 5000 )
	base.browser:Move( elm.left, elm.bottom )
end

