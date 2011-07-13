local base = _G

module( "tutorialBridge" )

function ShowSharesChange()	base.tutorial.ArrowToElm( base.bridge.btnChangeShare ) end
function ShowControl() base.tutorial.ArrowToElm( base.bridge.btnTryControl ) end
function ShowCompaniesList() base.tutorial.ArrowToElm( base.bridge.tblCompanies ) end