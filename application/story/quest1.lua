local base = _G

IncludeScript( "windowTalk" )

module( "quest1" )

local questenv = base.questenv
local window = base.window
local applic = base.applic
local button = base.button
local windowTalk = base.windowTalk
local guienv = base.guienv
local _mainWindow = nil
local _this = nil
local _gameSpeed = nil

local function _Exit()
	_this.obsolete = true
	windowTalk.Hide()
	
	--����������� ����� ����
	applic.speed = _gameSpeed
	
	_this:SetResult( "test", "test" )
end

local function _AddQuestFirstOutsource()
	questenv:AddActiveQuest( "quest_first_outsource" )
	_Exit()
end

local function _AddAnotherQuest()
	windowTalk.Dialog( "���� ��� ������, ������� ������� ������ �������"
					   .." ��� �������� �������� ������� ��������� ������ ��� ����, ��� ����� ����� ������� � ����"
					   .." ����� �������� ������, ������ �� ����� ��� ���������. � ���������, ���� ����� ���������� ���������"
					   .." �����, �� ���������, ������� ��� ����� �������. ����� �����.",
					   { "� ��� ��� �������, �� �������.", _Exit,
					     "��������� ��� � ����� ������� ��� ����", _AddQuestFirstOutsource } )
					   					   
	questenv:AddActiveQuest( "quest_get_money" )
end

local function _AddTalentArtistQuest()
	questenv:AddActiveQuest( "quest_talent_artist" )
	_AddAnotherQuest()
end

local function _Scenario()
	windowTalk.Dialog( "���� ��� ������������ �������, ���� ���������� - � ��� ������� ������ �� ������ �����. "..
					   "�������� �� ������� ��������� ��� ������ ����� ���."..
					   "��� ������ ������ �����, � ������ � ������ ���� ��������, "..
					   "�� ������ �������� ����������� �������. ���-�� � ���� ��� ����� ���..."..
					   "���, ��� � ������ ������. ������� �� ��� �� ������� ����������� �� �����������. ��� �� �� ���� �������?",
					   { "� ��������� � ���", _AddTalentArtistQuest,	
					     "�� ����� ����� ���������� ��������������", _AddAnotherQuest } )
end

local function _Prof( index )
	if index == 0 then
		
	elseif index == 1 then
	
	elseif index == 2 then
	
	elseif index == 3 then
	
	end

	_Scenario()
end

local function _Difq( levelDificult )
    base.applic.difficult = levelDificult

	windowTalk.Dialog( "� ���� ���������, ����������� ������ � ����������, ��� ������?", 
					   { "���������������� (�������� ��������� ����������)", function() _Prof( 0 ) end,
					     "������ (�������� ��������� ���������)", function() _Prof( 1 ) end,
					     "���������� ������������ (���� ��������� ����������)", function() _Prof( 2 ) end,
					     "� ����� ������� ��������� � ������ (����� ��� ��������� ����)", function() _Prof( 3 ) end } )		
end

local function _DifficultQ()
	windowTalk.Dialog( "��� ���������� �� ��������� ����� �������� ��������� ������������"
					   .."��� ���� ����� � ����� �������� �� �������� ��������", 
					   { "������ ���������� ��� (��� �������� � ����)", function() _Difq( 1 ) end,
					     "�������� ��� ����� (����� ������ ������� ����, ������ �����)", function() _Difq( 2 ) end,
					     "� ����������� �� ����������� (���� ��������� �������, ���� ������ �������)", function() _Difq( 3 ) end,
					     "��� ������ ��� � ��������, ������, ����� - ��� ����� �����", function() _Difq( 4 ) end } )	
end

local function _FreeGame()
	windowTalk.Dialog( "����� ����� ����� ������� ��������.",
					   { "��������� � �����", _Exit }  )	
end

function Start()
	base.LogScript( "Start quest1" )
	--���������� ���� �� ����� ������
	_gameSpeed = applic.speed
	applic.speed = 1000
	
	_mainWindow = windowTalk.Show( "media/story/uncleSven.png", "���� �������", "10%", "10%" )
	windowTalk.Dialog( "��� ���� ����� � ������������??? �������, ��� ��������... � ������ �����"
					   .." ������ ���� � �������, ���� ������ ��������"
					   .." ����� ������� ���� � ����� ����. ������ ��������� ���� �� ��������� ���.���������."
					   .." ���� �������� ��������� ���������� - ������� ���� �� ���������� ������,  "
					   .." ����������� - ����� �� ����, ������� � ������ ������ ������ �����. �� ���, �� �����???", 
					   { "��������.(�������� ������)", _DifficultQ, 
						 "�� ������ ����� �����.(��������� ����)", _FreeGame } )
end

local function _Register()
	_this = questenv:GetQuest( "quest1" )
	_this.x = 50
	_this.y = 50
	_this.name = "����������� �����"
	_this.start = Start
end

_Register()