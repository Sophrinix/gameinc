local base = _G

IncludeScript( "windowTalk" )

module( "quest1" )

local questenv = base.questenv
local window = base.window
local applic = base.applic
local windowTalk = base.windowTalk
local guienv = base.guienv
local _mainWindow = nil
local _this = nil

local function _Exit()
	_this.obsolete = true
	windowTalk.Hide()
	
	--����������� ����� ����
	applic.speed = 500
end

local function _AddAnotherQuest()
	windowTalk.Dialog( "������ �����. ��� �������� �������� ������� ��������� ������ ��� ����, ��� ����� ����� ������� � ����"
					   .." ����� �������� ������, ������ �� ����� ��� ���������. � ���������, ���� ����� ���������� ���������"
					   .." �����, �� ���������, ������� ��� ����� �������. ����� �����.",
					   { "��������� � �����", _Exit } )
					   
	questenv:SetActiveQuest( "quest_get_money" )
	questenv:SetActiveQuest( "quest_first_outsource" )
end

local function _AddOldFriendQuest()
	questenv:SetActiveQuest( "quest_old_friend" )
	_AddAnotherQuest()
end


local function _TalkOldFriend()	
	windowTalk.Dialog( "� ��� � ������ ���� ��������, �� ������ �������� ����������� �������. ���-�� � ���� ��� ����� ���..."
					   .." ���, ��� � ������ ������. ������� �� ��� �� ������� ����������� �� �����������. ��� �� �� ���� �������?",
					   { "� ��������� � ���", _AddOldFriendQuest,	
					     "�� ����� ����� ���������� ��������������", _AddAnotherQuest } )			   					   
end

local function _Scenario()
	windowTalk.Dialog( "������� ���������. ��� ������ ������ �����, ����� ��������� � �������� ���� ������ "..
					   "����. �������� �� ������� ��������� ��� ������ ����� ���.\n\t� ��� ������� ������ �� ���� "..
					   "�����, �� ������ �����. ����� �����...",
					   { "������", _TalkOldFriend } )						
end

local function _FreeGame()
	windowTalk.Dialog( "������� ���������. ��� ������ ������ �����, ����� ��������� � �������� ���� ������ "..
					   "����. �������� �� ������� ��������� ��� ������ ����� ���.\n\t� ��� ������� ������ �� ���� "..
					   "�����, �� ������ �����. ��� ���� ������...",
					   { "��������� � �����", _Exit }  )	
end

function Start()
	base.LogScript( "Start quest1" )
	--���������� ���� �� ����� ������
	applic.speed = 1000
	
	_mainWindow = windowTalk.Show( "media/story/uncleSven.png", "���� �������", "10%", "10%" )
	windowTalk.Dialog( "��� ���� ����� � ������������??? �������, ��� ��������... � ������ �����"
					   .." ������ ���� � �������, ���� ������ �� ������������ ��������"
					   .." ����� ����� ���� � ����. ������ ��������� ���� �� ��������� ���.���������."
					   .." ���� �������� ��������� ����������, ������� ���� �� ���������� ������. "
					   .." ���� ����������� - ����� �� ����, ��������� � �����. �� ���, �� �����???", 
							{ "��������.(�������� ������)", _Scenario, 
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