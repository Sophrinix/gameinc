local base = _G

module( "gameRecense" )

local window = base.window
local mainWindow = nil
local guienv = base.guienv
local game = nil
local lbVideo = nil
local lbSound = nil
local lbGameplay = nil
local lbRating = nil
local lbBugs = nil
local lbBestsaller = nil
local layoutAppRec = nil

local layoutRecenses = nil

local function _Hide()

end

local keyb = {} 
keyb[ 1 ] = "���������� ����������������. � ��� ���������� ���������� ������."
keyb[ 2 ] = "���������� ����� ���� � �����. ��������� ��������� ������ ����� ������� ��� �� �������� ������."
keyb[ 3 ] = "���������� �������� �������. ��� ��������� ��-�� ��������� ������, ������� ������� ��������."
keyb[ 4 ] = "������� ����������. ��� ������ �����������, ������� �� ������� ����� �����������."
keyb[ 5 ] = "�����������! ��������� ����������. ��� �� ����� �����������. ������ �� ��� ���� ���� ������!"

local graphic = {}
graphic[ 1 ] = "���� ��� �����, � ��� ������ ������������.\n ������� ������."
graphic[ 2 ] = "������������ ������� �����������\n ������ ���������� �����!"
graphic[ 3 ] = "������� ������ ���� �����.\n ������� ��� �������� �������������."
graphic[ 4 ] = "���� �� � ��� ��������� � ���������,\n ������� ���������� �� ���������."
graphic[ 5 ] = "������� �� ������� ������."
graphic[ 6 ] = "������� ������ ������� ������, �� � ���������� ����." 
graphic[ 7 ] = "������������� ��������� �� ������� ������ �������,\n ����� ������� ������� �� ���."
graphic[ 8 ] = "������� �������� ������. �� ����������� �� ����\n ��������� �� ������ ����."
graphic[ 9 ] = "� �����, ������� ������, �� �������� ���\n ��������� ��� �����."
graphic[ 10 ] = "��������� - ����� � ������� ������ ����."
graphic[ 11 ] = "��� ������� ��������� �����\n ��������� ��� ������� ���."
graphic[ 12 ] = "������������ ���������� ��������������� �����!"

local sound = {}
sound[ 1 ] = "� ���� �� ����???"
sound[ 2 ] = "�� ����� ����� ����� ������!"
sound[ 3 ] = "��������� ����, ����� �� ���������� ���� ����������."
sound[ 4 ] = "� �������� �� ������ � ��������� �����."
sound[ 5 ] = "������� ������ � ������� ������ ���� ������������."
sound[ 6 ] = " ��������� ����������� ������� � ����,\n ��� ���� ������� ������ ������."
sound[ 7 ] = "������ ������� ���-���� ������ � �����."
sound[ 8 ] = "��������� ����������� �� ���� �������� �������"
sound[ 9 ] = "���� �����������!"
sound[ 10 ] = "� ���� ������ � ���� ������, �����\n ������ ��������� ������. ��� ���������!"

local bugs = {}
bugs[ 1 ] = "� ����-������ ��� �����������???"
bugs[ 2 ] = "����� ��������, �� ��������"
bugs[ 3 ] = "����� ��������"
bugs[ 4 ] = "����� ������..."
bugs[ 5 ] = "������ ��������� ���������"
bugs[ 6 ] = "�������� �������"
bugs[ 7 ] = "����� �� ��������"
bugs[ 8 ] = "��������!!!"
bugs[ 9 ] = "��� �� ��� ������ ��� ������"
bugs[ 10 ] = "���� �������� �� �� �����!!!"

local rating = {}
rating[ 1 ] = "���� ��� �� ����������� ����������."
rating[ 2 ] = "� ���� ����� �����������."
rating[ 3 ] = "���� �� �� ������, ������� �� ����."
rating[ 4 ] = "�������� ���, ����� ��� ���� �� ������ � ��� � ����."
rating[ 5 ] = "��� ���� ������������� ���� ��� ����� ���� �����������."
rating[ 6 ] = "� ��� ����� ��������� � ����� ��������� ����������."
rating[ 7 ] = "� ��� ������� ����� ������������."
rating[ 8 ] = "���� ������ �� ���������."
rating[ 9 ] = "���� �� ������ � �� �������."
rating[ 10 ] = "���� �� ��������� �������� ������ ����� � �������, ��� ����� �� ����� �����"
rating[ 11 ] = "���� ��� ��������� �����."
rating[ 12 ] = "����� ���������� �������� ��� ���� ����������� �����."
rating[ 13 ] = "��� ���� ����������."
rating[ 14 ] = "��� ���� ����� ��� ����� �����"
rating[ 15 ] = "��� ��� ��! ��� ���� ������ �����������!"
rating[ 16 ] = "������ ������ � ������� � ���������, ���� ��� ��� ��� ����"
rating[ 17 ] = "��� ���� - ��������� ������."
rating[ 18 ] = "����� ���� ���������� �����!"

local function _UpdateRatings()
	lbVideo.text = graphic[ base.math.floor( game.graphikRating / 100 * (#graphic) ) + 1 ]
	lbSound.text = sound[ base.math.floor( game.soundRating / 100 * (#sound) ) + 1 ]
	lbGameplay.text = "not yet work"
	lbBugs.text = bugs[ base.math.floor( game.bugsRating / 100 * (#bugs) ) + 1 ]
	lbRating.text = rating[ base.math.floor( game.rating / 100 * (#rating) ) + 1 ]
	
	lbBestsaller.text = "not yet work"
end

local function _AddLabel( text )
	local ret = guienv:AddLabel( text, 0, 0, 0, 0, -1, layoutAppRec )
	ret.wordWrap = true
	ret.image = "media/textures/recenseJournal.png"
	
	return ret
end

function Show()
	local txsBlur = base.driver:CreateBlur( "windowShop.png", 2, 4 )
	mainWindow = window.fsWindow( txsBlur.path, _Hide )
	game = base.journals.currentGame
	
	layoutRecenses = guienv:AddLayout( "5%", "15%", "95%", "65%", 2, -1, mainWindow )
	
	for i=1, 6 do
		local ret = guienv:AddLabel( game.randomRecense, 0, 0, 0, 0, -1, layoutRecenses )
		ret.image = "media/textures/recenseGame.png"
	end	
	
	layoutAppRec = guienv:AddLayout( "5%", "70%", "95%", "95%", 2, -1, mainWindow ) 	
	lbVideo = _AddLabel( "����� �����" )
	lbSound = _AddLabel( "����� ����" )
	lbGameplay = _AddLabel( "����� ��������" )
	lbBugs = _AddLabel( "����� ����" )
	lbRating = _AddLabel( "����� ����� ������� � �����" )
	lbBestsaller = _AddLabel( "����� �����" )

	_UpdateRatings()
end

local skill = {}
skill[ 1 ] = "������..."
skill[ 2 ] = "��������� ���� �� ��������"
skill[ 3 ] = "������� ���� �� ��������"
skill[ 4 ] = "������� �������������������"
skill[ 5 ] = "���� �� �������� ��� ������"
skill[ 6 ] = "����� �������������� ��������"
skill[ 7 ] = "��������, �� ������ ������"
skill[ 8 ] = "��, ������� �����, ����������"
skill[ 9 ] = "��������� ������ ������� ���"
skill[ 10 ] = "������ ���� �� ��������"

local arcade = {}
arcade[ 1 ] = "�������������"
arcade[ 2 ] = "������"
arcade[ 3 ] = "������� ������"
arcade[ 4 ] = "����� ���������"
arcade[ 5 ] = "������ �� ������� ������"
arcade[ 6 ] = "������������� ������������"
arcade[ 7 ] = "��������� ���������� ����"
arcade[ 8 ] = "�������� �����"
arcade[ 9 ] = "������� ��� ��������"
arcade[ 10 ] = "������ ������ � �������"

local puzzle = {}
puzzle[ 1 ] = "� �� ������� �� ����"
puzzle[ 2 ] = "FAQ 1: ������ ���� ������?"
puzzle[ 3 ] = "������ ���������� ����������"
puzzle[ 4 ] = "������� ��� ������ ��������"
puzzle[ 5 ] = "���� �� �����������������"
puzzle[ 6 ] = "������ ��� ������"
puzzle[ 7 ] = "��������� ���� ��� �����"
puzzle[ 8 ] = "������ ������ ���������"
puzzle[ 9 ] = "�������� ��� ���"
puzzle[ 10 ] = "����� ��� ��� ��� �����!"

local platformer = {}
platformer[ 1 ] = "�������� ��� �������"
platformer[ 2 ] = "������� ������� ����"
platformer[ 3 ] = "��������� ��������"
platformer[ 4 ] = "��� � �������������"
platformer[ 5 ] = "��������� ���� ��� ��������"
platformer[ 6 ] = "���������� ������"
platformer[ 7 ] = "��������� �������"
platformer[ 8 ] = "����, �������, �����"
platformer[ 9 ] = "������������ �������"
platformer[ 10 ] = "�������� ����� ���� � ������"

local rpg = {}
rpg[ 1 ] = "�������� �����"
rpg[ 2 ] = "������, �������� � ���������"
rpg[ 3 ] = "����������� ����������"
rpg[ 4 ] = "������� ����� �� ����������"
rpg[ 5 ] = "�������� ������� ����"
rpg[ 6 ] = "������� ����� �� ���������"
rpg[ 7 ] = "�������������� ���"
rpg[ 8 ] = "������� ��������� �����"
rpg[ 9 ] = "��� ����� ��������� ���"
rpg[ 10 ] = "������ ������� ����!"

local quest = {}
quest[ 1 ] = "�������������: �����������"
quest[ 2 ] = "����������� �� ���� �����"
quest[ 3 ] = "����������� �� ������������"
quest[ 4 ] = "����������� �� ����"
quest[ 5 ] = "�������������� �����������"
quest[ 6 ] = "������� � ���������� �����"
quest[ 7 ] = "������� �����������"
quest[ 8 ] = "������ �����������"
quest[ 9 ] = "��������� �����������"
quest[ 10 ] = "������� ����� ��������!"

local strategy = {}
strategy[ 1 ] = "������ ���������"
strategy[ 2 ] = "���������� ���������!"
strategy[ 3 ] = "�������������� �������"
strategy[ 4 ] = "���� �"
strategy[ 5 ] = "������ ��� ���������"
strategy[ 6 ] = "�������� ���������"
strategy[ 7 ] = "��������� ���������"
strategy[ 8 ] = "������� ���������, �������!"
strategy[ 9 ] = "������������� ���������"
strategy[ 10 ] = "������ ���������"

local sport = {}
sport[ 1 ] = "������������ ���������"
sport[ 2 ] = "����� ����� ��� �� �����"
sport[ 3 ] = "������������ �������"
sport[ 4 ] = "������ �������������"
sport[ 5 ] = "�������� �����"
sport[ 6 ] = "������� ������"
sport[ 7 ] = "�������� �����"
sport[ 8 ] = "��������� �����"
sport[ 9 ] = "����������� �������"
sport[ 10 ] = "������� ������"

local fight = {}
fight[ 1 ] = "������ � ������ ������"
fight[ 2 ] = "������ ��������� ���������"
fight[ 3 ] = "�������������"
fight[ 4 ] = "������� �����"
fight[ 5 ] = "������� ���"
fight[ 6 ] = "������������ �������"
fight[ 7 ] = "������������� �������"
fight[ 8 ] = "���������� � �����������"
fight[ 9 ] = "�������� ����"
fight[ 10 ] = "������� �����"

local sim = {}
sim[ 1 ] = "����������"
sim[ 2 ] = "���������� ������"
sim[ 3 ] = "������� ���������"
sim[ 4 ] = "������� ���������"
sim[ 5 ] = "������ ���������"
sim[ 6 ] = "������"
sim[ 7 ] = "������� ��� ��������"
sim[ 8 ] = "������������� ��������"
sim[ 9 ] = "������������ ���������"
sim[ 10 ] = "��������� ����!"

local econom = {}
econom[ 1 ] = "������� ���������..."
econom[ 2 ] = "�������� ����"
econom[ 3 ] = "������ - ��� �� ���"
econom[ 4 ] = "������������� ������"
econom[ 5 ] = "��� ���������� �����������..."
econom[ 6 ] = "������� ��� �����������"
econom[ 7 ] = "��� ����� ����� �����������?"
econom[ 8 ] = "�������� ���� �� ���������"
econom[ 9 ] = "������������� ���������"
econom[ 10 ] = "������������� ���!"

local bilder = {}
bilder[ 1 ] = "����������!"
bilder[ 2 ] = "������� ����"
bilder[ 3 ] = "��������� ��������"
bilder[ 4 ] = "�������������� ������..."
bilder[ 5 ] = "������� �������������"
bilder[ 6 ] = "��������� �������������"
bilder[ 7 ] = "������� �������"
bilder[ 8 ] = "������ ��������"
bilder[ 9 ] = "��������� ����� �� ������..."
bilder[ 10 ] = "��� ����!"

local war = {}
war[ 1 ] = "�������������� �����������"
war[ 2 ] = "������������� �����"
war[ 3 ] = "��������� ����� �������"
war[ 4 ] = "������� ���������"
war[ 5 ] = "�������������� ���������"
war[ 6 ] = "��� ��������� ���������"
war[ 7 ] = "������������� ����� �����"
war[ 8 ] = "������� ���������"
war[ 9 ] = "��������� ������� ������"
war[ 10 ] = "������, � �������� �������!"

local action = {}
action[ 1 ] = "����������"
action[ 2 ] = "������ ������"
action[ 3 ] = "������ ���������"
action[ 4 ] = "������� ������"
action[ 5 ] = "���������� ��������"
action[ 6 ] = "������ ����������"
action[ 7 ] = "������"
action[ 8 ] = "������� � ������� ��������"
action[ 9 ] = "����, ������ ��������"
action[ 10 ] = "������������ ��������!"

local scenario = {}
scenario[ 1 ] = "���������������� �����"
scenario[ 2 ] = "�������������� �����"
scenario[ 3 ] = "������������ �������"
scenario[ 4 ] = "������� �����"
scenario[ 5 ] = "������� ����������"
scenario[ 6 ] = "������������� ������������"
scenario[ 7 ] = "������������� �������"
scenario[ 8 ] = "��������� ���"
scenario[ 9 ] = "����������� ����"
scenario[ 10 ] = "�������� ������"