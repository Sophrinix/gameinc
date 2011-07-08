
NrpInitializeLoginScene()

IncludeScript( "LoginFunctions" )
IncludeScript( "button" )
IncludeScript( "window" )

local splashWnd = nil

function ShowMenu()
	button.StretchGrayed( "64e", "64e", "0e", "0e", "vopros", guienv.root, -1, "", LoginFunctions.About )

	--создание кнопоку входа 
	button.StretchGrayed( "10.9%", "57.4%", "580+", "267+", "btnNewGame", guienv.root, -1, "", LoginFunctions.NewGame )
	button.StretchGrayed( "4.6%", "0", "41%", "55%", "btnContinueGame", guienv.root, -1, "",	LoginFunctions.Continue )
	button.StretchGrayed( "51.3%", "51%", "74.3%", "69%", "btnOptionsGame", guienv.root, -1, "", LoginFunctions.ShowOptions )
	button.StretchGrayed( "29.5%", "6%", "96%", "51.6%", "btnExitGame", guienv.root, -1, "", LoginFunctions.Quit )
end

function SplashEnd()
	guienv:AddBlenderAnimator( splashWnd, 255, 0, 1000, false, true, false )
	guienv:AddTimer( 1050, function() splashWnd:Remove() end, nil )
end

function Splash2()
	rdtImg = guienv:AddImage( "256e", "128e", "256+", "128+", splashWnd, -1, "" )
	rdtImg.texture = "rdt32.png"
	rdtImg.alphaChannel = true
	guienv:AddTimer( 1050, function() 
								guienv:AddBlenderAnimator( rdtImg, 255, 0, 1000, false, true, false ) 
						   end, nil )
	guienv:AddTimer( 2150, SplashEnd, nil )
end

function Splash1()	
	nrpImg = guienv:AddImage( "33%", "33%", "256+", "128+", splashWnd, -1, "" )
	nrpImg.texture = "nerpaEngine.png"
	nrpImg.alphaChannel = true
	guienv:AddTimer( 1050, function() guienv:AddBlenderAnimator( nrpImg, 255, 0, 4000, false, true, false ) end, nil )
	
	guienv:AddTimer( 2150, Splash2, splashWnd )	
end

function ShowSplash()
	--переходим на следующую сцену
	splashWnd =  guienv:AddImage( 0, 0, "0e", "0e", guienv.root, -1, "" )
	splashWnd.texture = "blackScr.png"
	
	powImg = guienv:AddImage( "33%", "33%", "256+", "128+", splashWnd, -1, "" )
	powImg.texture = "powerBy.png"
	powImg.alphaChannel = true
	guienv:AddTimer( 1050, function() guienv:AddBlenderAnimator( powImg, 255, 0, 4000, false, true, false ) end, nil )
	guienv:AddTimer( 2150, Splash1, splashWnd )	
end

ShowMenu()
ShowSplash()