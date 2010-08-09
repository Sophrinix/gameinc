local links = { }

function GetHelpLinkForObject( name )
	return links[ name ]
end

function SetHelpLinkForObject( name, link )
	links[ name ] = link
end