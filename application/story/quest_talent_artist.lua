local base = _G

IncludeScript( "windowTalk" )

module( "quest_talent_artist" )

local questenv = base.questenv
local window = base.window
local applic = base.applic
local windowTalk = base.windowTalk
local guienv = base.guienv
local _mainWindow = nil
local _this = nil
local _gameSpeed = nil

local _oldMenPassedCount = 0
local _babkiPassedCount = 0
local _percentApply = 0
local _alhocol = 0

local function _Exit()
	_this.obsolete = true
	windowTalk.Hide()
	
	--����������� ����� ����
	applic.speed = _gameSpeed
	questenv:SetResult( "general", "quest_talent_artist", "kistochkin_is_enemy" )
end

local function _Apply()
	questenv:AddActiveQuest( "quest_talent_hack" )
	questenv:SetResult( "general", "quest_talent_artist", "kistochkin_is_friend" )
	windowTalk.Message( "��� ������ ��� ������� ����� ������ ��������, �� ����� ���������� �� ���� "
						.." � ������ ��������, � ����������� ��� ����� ��� ��� ����� ��� ����� ������",
						{ "��������� � �������", _Exit } )
end

local function _RandomMonetka()
	local prc = base.math.random(50 + _percentApply - _alhocol )
	if prc > 90 then
		windowTalk.Message( "������� ��������� �� ����� � ������� � �������, �������� ����������"
							.."������. �������� ������� ����������� �� ������� ��� �������", 
							{ "�� ������ �������� ��������. ", _Apply } )
	elseif prc > 70 and prc <= 90 then
		windowTalk.Message( "������ ��������� �� ����, ��� ���� ������ �� ����. ����� ������ ����� "
							.."������ ��������. ������", 
							{ "�� ����, �����", _Exit } )
	elseif prc > 30 and prc <= 70 then
		windowTalk.Message( "������ � ��� ������� � �����, ��� 500$ � �������� ��������, �������?", 
							{ "�����, �����", _Exit,
							  "�� �����", _Apply  } )
	elseif prc <= 30 then
		windowTalk.Message( "��������� ����, �������� ������ � �������� � ����� � �������, �������?", 
							{ "�����, �����", _Exit,
							  "�� �����", _Apply  } )
	end
end

local function _RandomExit()
	local text = "��� ��� ����� - � ������� �� ���, �� ��� ��� �����... ��� � ����� ������� ������"
				 .."������, � ����� ������� �����"
				 .."\n����� - ��� �� �... �� ����� �������" 
				 .."\n���� - ���� �����, �� ���� � ������ ���������� ����� � ����� 2000$ ��� ������"
				 .."\n�� ����� ������� - ����� ����� �����"
				 .."\n� ������� �������� - ���� �������� �� ���"
	
	windowTalk.Dialog( text,
	                   { "�����", _RandomMonetka,
					     "�� ����� ������������, ����� � ������ ���", _Exit } )
end

local function _BlackHorse()
	local text = "�� ��� ��, ��� ��... ��� ������� ���� ��� ��� ������ �� ����"
				 .."����� � � �� ������� �������, ��� �������� �������. ������� ���"
				 .."�� � ����� � ������, ����� ���� ��, � ��� ��������� �� ����"
				 .."�� ���� ������ ��? ����� ����� � ���� ���������, ��� ����� �����?"
	
	windowTalk.Dialog( text, { "�� ������ �� ��� �������?", _RandomExit,
							   "�� ����� �����, �� �� ������. �����", _Exit } )
end

local function _WhiteHorse()
	_percentApply = _percentApply + 10
	_alhocol = _alhocol + 10
	local text = "��� � � ������, ��� ����� ������ �������� �����. �� ����� �� �������� ���..."
	             .."��� ����� �� �������� ������???"
	
	windowTalk.Dialog( text, { "��������� ������� ������ ��������?", _RandomExit,
							   "�� ���, ���������� �����. ������� �� ��������.", _Exit } )
end

local function _BadQ_2()
	local text = "������ �� �� �� �������, �� ����� ��� �������� ��� �������� ������"
	             .."� � ���� ��� ��� ������ ������ ���� �������� ���������, ����� ���"
	             .."������� ������ ��� �����"
	
	windowTalk.Dialog( text, { "������", _BlackHorse,
							   "�����", _WhiteHorse } )
end

local function _GoodQ_1()
	_alhocol = _alhocol + 10
	local text = "� ������ �� �������� �� ������� � ������������ �����. ���������� ���� �� ���..."
				 .." �����, ������, � ��� � ���������� - �� ������� �������"
				 .." �� ��� � �������� ���������, � ��� ����� ������ �� ���� ��������?"
				 .." �����, ����� ������ � ������, ���������� ���������"
	
	windowTalk.Dialog( text, { "�� ��� ����� ����� ������� ������?", _RandomExit,
							   "�� � ���������� �����. �����", _Exit} )
end

local function _GoodQ_2_M()
	_percentApply = _percentApply + 10
	local text = "��, ������� ����� ���� - ���������, ���� �� ����������� ����� �������"
	             .." � � ��� ������� �����, ���-�� ������� ����������, ���-�� ������"
	             .." ������� ��� � ��������� ����� ���������, �������� ������ �� ����� ���������"
	             .." ����� � ���� ������� ��������, �� ���� ���� � ������ ������"
	             
	windowTalk.Dialog( text, { "����� ����� ��������� ������������?", _RandomExit,
	                           "������ ����� �� ���������", _Exit } )
end

local function _BadQ_1()
	local text = "�������� �� �, � �� ���? ��... �� ��� �� "..base.applic.profileName
	             .." ������ ���������, ��������� ��������. � � ��� ������� ������ ������ �� ������."
	             .." �� ������ ��� �� ���������. � �� ���� �� �����, ��� ��������� �� ������� ��������"
	             .." �����������. � ������� �� �� ��� ������� �������� ��������?"
	             
	windowTalk.Dialog( text, { "���, Metallica �����", _GoodQ_2_M,
	                           "Scorpions forever", _BadQ_2,
	                           "� �������, �������� ������ ����", _GoodQ_1 } )
end

local function _EnterHouse()
	local text = "���������� �� �����, ����� ������ �� �� ���������, �������� ��� �������� ��������"
	             .." ������ ������ ����, ������ � �������� ����� �� ���������, ������� �� ������� ������"
	             .." � �����, ������������ ������ ������ ������������ � �������������� ������������ - ���"
	             .." ����� � �����, ���� ������� ���� ��� ������-�� �������. �� ��� ����� �� ������������� "
	             .." ������ ����������, ������ ��� �����. �� ���������, �����."
	             
    windowTalk.Show( "media/story/talentArtist/AntonKistochkinHouse.png", "�����", "10%", "10%" )
	windowTalk.Dialog( text, 
					   { "�������, ������, ��� �� ��� �������� �����?", _BadQ_1, 
					     "����� ������ �������������� ��������?", _GoodQ_1,
					     "��� ������������ ��������� ��� ���������� ������", _BadQ_2 } )	
end

local function _GoArtistHome()
	local text = "�������, ����� �������� � ������. ����������� �����,"
	             .." ��������� �� ��������� ����, ������ ����� �������� ���� � ���,"
	             .." ����� �������� � ������. ���������� �� ������ ���� �� ������������"
	             .." ����� ������, ������ ����� �������� ���� �� ��������, ����� ��������"
	             .." � ��� �������. "
	             
	windowTalk.Message( text, { "����� � ��������", _EnterHouse, 
								"��������� �� ����", _GoBabki } )
end

local function _BabkiTalkAboutArtist()
	local text
	if _oldMenPassedCount == 0 then
		text = "��� ������ ����� ���������, � �� ��� ������� �������"
	else
		text = "� �� ����� �� �� ������� �������? � �� ������ ������ �������� ���������"
		       .." ��� ���������� ���� ��������, ���� �������� � �� ������������� �� ����, �� ��� � ��������� �����"
		       .." �� ��� ��� �� �������� ������� �����, �� �� ������ ������ ����� �� �������� �� ������."  
		       .." ����� �� - ��� �����... �� ���� ������ ������� �� ��������, �� ��� �����-�� �� ����..."
		       .." �� �������� ����� ������ ����������, ���� ���� ����� �� ����...\n"
		       .." � ����� �� ���� �� ���� (������� �������� �� �����)? �����, � ���� ���������..." 
		       .." ������� ����� �����? ��� ������ ����� ���������"
	end
	
	_babkiPassedCount = _babkiPassedCount + 1
	
	local tableAnser = { "����������� � ����", _GoOldMan,
	                     "����� � �������", _GoArtistHome,
					     "�������� � ��������� ��� ���"..aboutArtistYet, _BabkiTalkAboutArtist,
					     "����������� � ������", _OnEnter }
	
	windowTalk.Show( "media/story/talentArtist/babki.png", "�����", "10%", "10%" )
	windowTalk.Dialog( text, tableAnswer )	
end


local function _GoBabki()
    local text, aboutArtistYet = ""
	if _oldMenPassedCount > 0 then
		text = "�������� �� �������� �������� �� ���"
		aboutArtistYet = " ��� ���"
	else
		text = "����� �� ��������� �������� �������� ���������, ������� �� ��� � ���������, �� ������"
	end
	
	--��� ��������� � ���� � ���������� ������ ���������
	local tableAnser = { "����������� � ����", _GoOldMan,
	                     "����� � �������", _GoArtistHome,
					     "�������� � ���������"..aboutArtistYet, _BabkiTalkAboutArtist,
					     "����������� � ������", _OnEnter }

	windowTalk.Show( "media/story/talentArtist/babki.png", "�����", "10%", "10%" )
	windowTalk.Dialog( prefText..text, tableAnswer )	
end

local function _OldMenTalkAboutArtist()
	local prefText = ""
	if _oldMenPassedCount > 0 then
		prefText = "��� ����� ������ ��������."
	end
	
	_oldMenPassedCount = _oldMenPassedCount + 1
	
	local text = "� ��� ������ ������� ���� ��������, �� ������ ��������� ����� �������."
	             .." �� ������� ����� ��� ���� - ����� �� ������ �����. ��� �����-�� ������������ �������"
	             .." �� ��� � ������� ��������, � ���� � ���� ������, �� ��� ��������. ����� ��� �� ���������,"
	             .." �� ��� ������� � ��� ����� �����, �� �������...\n - ������, ���� ���������, � �� ��������..."
	             .."\n - ��� ���� �� ��� ���� �������, � �� ��� ��� ������� �����������"
	
	--��� ��������� � ���� � ���������� ������ ���������
	local tableAnser = { "����������� � ���������", _GoBabki,
					     "�������� � ��������� ��� ���", _OldMenTalkAboutArtist,
					     "����������� � ������", _OnEnter }

	windowTalk.Show( "media/story/talentArtist/oldMen.png", "������", "10%", "10%" )
	windowTalk.Dialog( prefText..text, tableAnswer )	
end

local function _GoOldMan()
	local text = "������ ���� ���������, ��� �� �������� ��� ��������� ��� ��������������� ����������"
	
	--��� ��������� � ���� � ���������� ������ ���������
	local tableAnser = nil
	if _oldMenPassedCount > 0 then		
		tableAnswer { "����������� � ���������", _GoBabki,
					  "�������� � ���������", _OldMenTalkAboutArtist,
					  "����������� � ������", _OnEnter }
	else
		tableAnswer { "����������� � ���������", _GoBabki,
					  "�������� � ��������� ��� ���", _OldMenTalkAboutArtist,
					  "����������� � ������", _OnEnter }
	end
	
	windowTalk.Show( "media/story/talentArtist/oldMen.png", "������", "10%", "10%" )
	windowTalk.Dialog( text, tableAnswer )	
end

local function _OnEnter()
	windowTalk.Message( "�� ������, ������� ����� ��������� ���������� ������ ������, ��� �� ���� ��� ����������� �������."
						.."����� ��������� ����� �������������� ������ ���������� ��������, ������� ����� ����� ���������� ������, ��-�� ��������� ������ "
						.."����� � �������� � �����-�������",
						{ "���� � ����", _GoOldMan,
						  "����������� � ���������", _GoBabki,
						  "����� � �������", _GoArtistHome,
						  "���� �� ����� �������� ��������� ���������.(��������� � �������)", _Exit } )
end

function Start()
	base.LogScript( "Start quest_talent_artist" )
	--���������� ���� �� ����� ������
	_gameSpeed = applic.speed
	applic.speed = 1000
	
	_mainWindow = windowTalk.ShowFs( "media/story/talentArtist/AntonKistochkinHouse.png", "������ ����", "25%", "25%" )
	_OnEnter()
end

local function _Register()
	_this = questenv:GetQuest( "quest_talent_artist" )
	_this.x = 50
	_this.y = 50
	_this.name = "����������� ��������"
	_this.start = Start
end

_Register()