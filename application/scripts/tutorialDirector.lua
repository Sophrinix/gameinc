local base = _G

module( "tutorialDirector" )

function ShowEmployers() base.tutorial.ArrowToElm( base.director.btnEmployer ) end
function ShowComp() base.tutorial.ArrowToElm( base.director.btnComp ) end
function ShowProject() base.tutorial.ArrowToElm( base.director.btnProject ) end
function ShowCase()	base.tutorial.ArrowToElm( base.director.btnCase ) end

