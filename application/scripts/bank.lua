local base = _G

IncludeScript( "bridge" )
IncludeScript( "loans" )
IncludeScript( "deposit" )

module( "bank" )

local guienv = base.guienv

local window = base.window
local button = base.button
local tutorial = base.tutorial
local applic = base.applic

local bankWindow = nil

btnLoan = nil
btnDebt = nil
btnBridge = nil

local function _Hide()
	base.package.loaded[ "bridge" ] = false
	base.package.loaded[ "loans" ] = false
	base.package.loaded[ "deposit" ] = false
end

function Show()
	bankWindow = window.fsWindow( "bank.png", _Hide )
	
	tutorial.Update( tutorial.STEP_OVERVIEW_BANK )
	
	--get loan
	btnLoan = button.EqualeTexture( 80, 402, "loans", bankWindow, -1, "", base.loans.Show )
	--deposit	
	btnDebt = button.EqualeTexture( 258, 301, "deposit", bankWindow, -1, "", base.deposit.Show )
	
	btnBridge = button.EqualeTexture( 425, 174, "bridge", bankWindow, -1, "", base.bridge.Show )
end

