local base = _G

IncludeScript( "magazine" )

module( "year1983" )

local events = {}
local paper = base.magazine
local rbank = base.applic.bank
local applic = base.applic

events[ 102 ] = function()
	local header = "����� ������� �������"
	local text = "ARPANET ������ �������� �������� � NCP �� TCP/IP."
	
	paper.Show( header, text, "font_10" )
end

events[ 107 ] = function()
	local header = "��� �������"
	local text = "���������� ������ ���������� ���������� �� ���������"
	
	paper.Show( header, text, "font_10" )
end

events[ 113 ] = function()
	local header = "���������� ��������"
	local text = "���������� ������ ��������������� ��������������� ��������� � ������"
	
	paper.Show( header, text, "font_10" )
end

events[ 119 ] = function()
	local header = "��������������"
	local text = "�������� Apple ��������� ������������ ��������� Apple Lisa. "
	text = text .. "� ������� ������������ �������� ����������� ����� � ����������� ��������� ������������"
	
	paper.Show( header, text, "font_10" )
	rbank:ChangePieCost( "apple_company", "2%+" )
end

events[ 120 ] = function()
	local header = "������� �� ��������"
	local text = "B ������� ��������� ������������������ ���������� ������� ���������� ����� �����"
	
	paper.Show( header, text, "font_10" )
end

events[ 122 ] = function()
	local text = "����� ���� ���� �� �������� ������� ����� 5 ���������� ������ ������������ ��������."
	--��������� �������� � ���������� �����
end

events[ 126 ] = function()
	local header = "1-2-3-�����"
	local text =  "Lotus ��������� ����������� ������� Lotus 1-2-3"
	
	paper.Show( header, text, "font_10" )
	rbank:ChangePieCost( "lotus_company", "3%+" )
end

events[ 127 ] = function()
	local header = "Best of the IT 1982"
	local text = "������� ������ ������� �������� ���� � ����� IT. \"InfoWorld\" �������� Multiplan ������������ Microsoft ����������� ��������� ����, � IBM PC - ����������� ����"
	
	paper.Show( header, text, "font_10" )
	rbank:ChangePieCost( "microsoft_company", "4%+" )
	rbank:ChangePieCost( "ibm_company", "3%+" )
end

events[ 126 ] = function()
	local text = "��� IBM PC ����������� ����������� �������� ��������� Lotus 1-2-3."
	--���������� ������ ��
end

events[ 127 ] = function ()
	local header = "������ ��� �����"
	local text = "� ������ �������������� ���������-��������� ����������. ����������� ����������� ���� � �������� ����������� ���� � ����������� ������."
	
	paper.Show( header, text )
	applic:GrowInterest( "genre_war", 0.1 )
end 

events[ 212 ] = function()
	local header = "����� � ������"
	local text = "�� ����� ������ � ���������� � ������ ������� 64 ��������"
	
	paper.Show( header, text )
end

events[ 213 ] = function()
	local header = "1983 - ��� ������"
	local text = "��������� ��� ������� ������ ������� 1983 ������ ������"
	
	paper.Show( header, text )
end

events[ 217 ] = function()
	local header = "������ � �������"
	local text = "�������� �������� ����� ADA ANSI/MIL-STD-1815-A-1983. �� ��� ������� ������������ �Ada� ������������������ �������� ������,"
	text = text .. "� �������� ��������� ����������� �����, �� ��������� ����������� ��������� ������������ �� ������������ ����������"
	
	paper.Show( header, text, "font_10" )
end

events[ 218 ] = function()
	local header = "����� ��-��" 
	local text = "�� ����� ������� ���������� � ������( ��� ) ���� ����� 13 �������."
	
	paper.Show( header, text )
end

events[ 224 ] = function ()
--1983.02.24	
	local text = "�������� ����� ���� �������� ������� �� �������������� ����� � ������� 5 ���������� ��������."
	text = text .. "����� ����, ���� ������ ��������������� ������������� ���� � ������� 433 ��������� ��������."
	--���� ����� ������������ �������� �� 2%
end

events[ 301 ] = function()
	local header = "�������"
	local text = "���������� ������� � ������ ����� ����������� ��������� �������"
	
	paper.Show( header, text )
end

events[ 307 ] = function()
	local header = "������� ���"
	local text = "��������� ��� ������� ������ ������ ���� ��������� ���"
	
	paper.Show( header, text )
end

events[ 308 ] = function ()
	local header = "����� ������ - ����� �����������"
	local text = "������������ ������ ��������� IBM PC �� ���� ������������������ ���������� Intel 8088. "
	text = text .. "MFM-������ ���� � ����������� ST-412, ������� 10 �����, ��� �������� 128. � IBM PC/XT ����� "
	text = text .. "�������� ������ �������� 5\" ������ ����. ���������� ����� 84 �������."
	
	paper.Show( header, text, "font_8" )
	rbank:ChangePieCost( "ibm_company", "2%+" )
end

events[ 309 ] = function()
	local header = "����������� ������"
	local text = "����� ���������� � �������� ������ �������������� ���� ��� ����� ������� V. "
	text = text .. "��� ��������� � ����� ����� ���������� ������������ �� ������, ����� ������ �� ����� ���� ���� �� ����� ���������� ��������������� "
	text = text .. "������ �����. ������� ��������� ��������� ��������� ����� ������ ������� ������, "
	text = text .. "� �� ����� ��� ���� ���������� ������������� ��������� ����� �� ����������� �������. "
	text = text .. "��� ��������� ������������ ������ ����, ����� �������, � �������� ����� ���� ����������� ����������."
	
	paper.Show( header, text, "font_8" )
	applic:GrowInterest( "genre_war", 0.1 )
	applic:GrowInterest( "genre_space", 0.1 )
end

events[ 314 ] = function ()
--1983.03.14	
	local text = "����������� �����-����������� ����� (����) ������� ����������� ��������� ���� �� ����� �����. ���� �� ������������������ "
	text = text .. "����� �� ���������� ������ ����� ������ - � 34 �� 19 �������� �� �������."
	--
end

events[ 322 ] = function()
	local header = "������"
	local text = "������� �������, ��������� ����������� ���������������� ��������"
	
	paper.Show( header, text )
end

events[ 323 ] = function ()
--1983.03.23	
	local text = "��������� ��� ������ ������������� ������ ����� �� �������� ������������ ������� ��������������� ������� � ������ ��������� \"�������� �����\"," 
	text = text .. "� ������� �������������� ������������� ������������� ��������� ��� ����������� � ������������ ����������� ����� ����������."
	--���� �������� � ����� ��������� � �������
end

events[ 407 ] = function()
	local header = "���������� � �������� �������"
	local text = "���� ������������ ������� ���������� STS-6. ���������� ����� �������� � ������� ��������, "
	text = text .. "������� �� ��������� ������ ����� ��������� ����� � �������� ������ (������������������ 4 ���� 10 �����)"
	
	paper.Show( header, text )
end

events[ 415 ] = function()
	local header = "���������� � �����"
	local text = "������ ������ ���������� �� ��������� ��� � �����"
	
	paper.Show( header, text )
end

events[ 501 ] = function()
	local header = "����� ��������� Apple"
	local text = "���� ������ ���������� � ������� Apple Computer, ������� ����� ��������"
	
	paper.Show( header, text )
end

events[ 525 ] = function()
	local header = "������� ����������"
	local text = "��� ��������� � �������� �� ������� ����� �������������������� ������������"
	
	paper.Show( header, text )
end

events[ 526 ] = function() 
	local header = "������ � ������"
	local text = "������ ������������� � ������ �� ������ �����(������), ������� 104 ��������, ������ 163"
end

events[ 601 ] = function()
	local header = "���������"
	local text = "C� ��������� ����� Apple ������ ���������� ��������������"
	
	paper.Show( header, text )
end

events[ 605 ] = function()
	local header = "������� ���������� ����"
	local text = "� ������ ��������� ������� ���������� ������� ��������� � ����� �����, �� �������� ��� �������� �����."
	text = text .. "���� ����� �� ��� ������������ ��� ����������� (�� ���� ����������� ������ ��� ������� ��� ��� �����) � ������ ������ ��������� ��������� ������."
	text = text .. "�������, �� ������ ������, �� 176 �� 600 �������"
	
	paper.Show( header, text )
end

events[ 613 ] = function()
	local text = "�������� ������"
	local header = "������-10 ���� ������ ������������� �������� ���������� ������� ��������� �������"
	
	paper.Show( header, text )
end

events[ 618 ] = function()
	local header = "������ ���������� � �������"
	local text = "���������� STS-7: ����� ���� ����� ������ ����������� ����������� ����������� ����"	
	
	paper.Show( header, text )
end

events[ 617 ] = function()
--1983.06.17	
	local text = "� �������������� ������� ������� �������� ������ �� 3,7 �������� - ��� ����� ������ ������� �� ��������� 15 ���."
	--���� ��������� ����� ���������� ��������
end

events[ 701 ] = function()
	local header = "����� ������������ ������������"
	local text = "������� ��������� ������� ��������-9�: ������ ������������ ������-1 �� �������� ����������� ���������"
	
	paper.Show( header, text )
end

events[ 706 ] = function ()
--1983.07.06	
	local text = "���������� ������������� ��������� ����� ����� �� �������� ������� \"������ �� ������ ������������������\", "
	text = text .. " � ������� ������������ ���� ��������� ���������� �������� ������ �� ������� ����� � ������-������� � ���������"
end

events[ 715 ] = function()
	local header = "����� �������"
	local text = "� ������ �������� ������� ������� ��������� Nintendo Entertainment System"
	
	paper.Show( header, text )
end

events[ 716 ] = function()
	local header = "�������������� �� �����"
	local text = "�� �������� ����� �������� ���������� ������� Sikorsky S-61, ������� 20 �������"
	
	paper.Show( header, text )
end

events[ 718 ] = function()
	local header = "������� � �������"
	local text = "���� ������ � ���� �������-���������� �������� �������� �� �� ������-7� � ������� ������� �� 5 �������"
	
	paper.Show( header, text )
end

events[ 721 ] = function()
	local header = "-90 �� �������"
	local text = "���������������� ����� ������ ����������� �� ����� �� ��� ������� ����������������� ����������. "
	text = " �� ��������� �������������� ������� ������� ��������� �� ������������� ������� ?89,2 C"
	
	paper.Show( header, show )
end

events[ 722 ] = function()
	local header = "������ ����� �� ���������"
	local text = "���������� ��� ���� �������� ��� ��������� ������������ ����������� �� ��������"
	
	paper.Show( header, text )
end

events[ 723 ] = function()
	local header = "����� �����"
	local text = "��-�� ������ ���������� ������������ ������ ����������� �� ������� ���� �����, ���� ��������, ��� ������ ������������"
	
	paper.Show( header, text )
end

events[ 724 ] = function()
	local header = "�������� � ������"
	local text = "������� ����� � �������� ������� � 117 ������������ ������� � ���������� ������(������)"
	
	paper.Show( header, text )
end

events[ 820 ] = function()
	local header = "������� ��� �����"
	local text = "��������� ��� ������ ������ ������ �� �������� � ���� ������������ ��� ������������� �������������"
	
	paper.Show( header, text )
end

events[ 822 ] = function()
	local header = "����� ������"
	local text = "����� �������� ���������� ������� ������ Depeche Mode ��� ��������� Construction Time Again"
	
	paper.Show( header, text )
end 

events[ 830 ] = function()
	local header = "�������������� � �������"
	local text = "���������� STS-8: ������� � ������ ������� ��������� ����������������� ������������� ����� �������"
	
	paper.Show( header, text )
end

events[ 901 ] = function()
--1983.09.01	
	local text = "� ���������� ���������� �������������� ������������� ����������� '�����-747', ������� ��������� "
	text = text .. " ������������ � ������ ��������� ���������� ���������� ������������ � ������ ������� �������, �������� 269 �������"
end

function FindEvent( month, day )
    base.LogScript( "find for event "..month * 100 + day )
	if events[ month * 100 + day ] ~= nil then
		base.DebugFunctionCall( events[ month * 100 + day ] )
	end
end
