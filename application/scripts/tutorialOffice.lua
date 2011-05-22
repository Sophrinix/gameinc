local base = _G

module( "tutorialOffice" )

function ShowDirector()	base.tutorial.ArrowToElm( base.office.btnDirector ) end
function ShowDevRoom() base.tutorial.ArrowToElm( base.office.btnDevRoom ) end
function ShowResearch() base.tutorial.ArrowToElm( base.office.btnResearch ) end