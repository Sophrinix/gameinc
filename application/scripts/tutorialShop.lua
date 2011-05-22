local base = _G 

module( "tutorialShop" )

function ShowChartAllTime()	base.tutorial.ArrowToElm( base.shop.btnTopAllTime ) end
function ShowChartMonth() base.tutorial.ArrowToElm( base.shop.btnTopMonth ) end
function ShowGameInSale() base.tutorial.ArrowToElm( base.shop.btnInSale ) end
function ShowJournals()	base.tutorial.ArrowToElm( base.shop.btnJournals ) end
function ShowGamesList() base.tutorial.ArrowToElm( base.saleManager.listboxGames ) end
function ShowParams() base.tutorial.ArrowToElm( base.saleManager.prgRating ) end
function ShowJournalsList() base.tutorial.ArrowToElm( base.journals.lbxGames ) end
