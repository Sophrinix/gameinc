local base = _G

module( "year2003" )

local events = {}

events[ 0102 ] = function()
	base.applic.CompanyBancrupt( "Data East" )
end
