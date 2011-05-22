local base = _G

module( "tutorialProjectManager" )

function ShowProjects()	base.tutorial.ArrowToElm( base.projectManager.comboxProjects ) end
function ShowProjectComponents() base.tutorial.ArrowToElm( base.projectManager.lbxComponents ) end
function ShowUserType()	base.tutorial.ArrowToElm( base.projectManager.layoutUserTypes ) end
function ShowUsers() base.tutorial.ArrowToElm( base.projectManager.comboxUsers ) end
function ShowUserInfo() base.tutorial.ArrowToElm( base.projectManager.windowUserInfo ) end
function ShowUserComponents() base.tutorial.ArrowToElm( base.projectManager.lbxWorks ) end

