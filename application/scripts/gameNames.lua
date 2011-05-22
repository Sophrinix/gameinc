local base = _G

module( "gameNames" )
--Skill games
local names = { "������", "T�����������", "����� � �����", "������", "�������",
				"���", "������ � ������", "�������", "�������", "�����", "������",
				"����", "�������� �����", "�������", "����", "���� � ������", "����",
				"���� � ���", "����������", "��������", "������� �� �������", "���� � ����",
				"��������� �������", "�������", "�������� �����", "����� �����", "�������� �����",
				"���� � ����", "�����", "������ �� ��������", "�����", "����������", "������ ��������",
				"�����������", "�������� �����", "�����������", "�������", "��������", "����� �������",
				"��� ���� ���", "��������", "������ �����", "���", "������� �������", "�������", "������������ ���������",
				"��������� �����", "������� ��������", "�������������", "�������-�����", 
--Arcade-------------------------------------------
				"�������� ��������", "�����-��������", "����� �����������", "S-���", "���������",
				"������ ����", "�������", "������", "���������", "�������� X12", "�������", "�������",
				"�������� �����", "������ ������", "������ ���������", "������", "������ ���", "����-���������", 
				"�����", "�������� �����", "�������", "��������", "�����", "����� 2182", "�����", "����� �� ������ ����",
				"��������� �����","������� X","�������","�������� �����","����� ����","����������� �������",
				"������-�����������","������ ����","������ ��������","�����","�������� ������","���������-������",
				"������","������ ����","�����-������","���","�������� ����","����� ��������","��� ��� ����",
				"��� ��� ���� 2","���� ������������� ������","���������","����������","����",
--Brainteaser-------------------------------------------,
				"����� Y","��� ������","������������","��������� ������","�� ������?","������ ������",
				"����������� ����","������� � ������","����� Y 2","����","����� Y? 2","����������","���� ���������",
				"������","���������","������","����������� �����","���� ��������� 2","������ �����������",
				"������� IQ","������ ����������� 2","�������� �����","������� X","���� ��������� 3",
				"������������","�����","IQ ������ ��","������� 2000","������� �����","��������","��� ������",
				"������� �������","������� 2001","�����������","��� ������ 2","��������","���� ������� �����",
				"�������� 2","������","���� ������� ����� 2","������","�������� �����������","��� ������ 3",
				"������������ IQ","������� 2002","������� ����","�����������","�������� 2","�������","������ 2",
--Jump & Run-------------------------------------------,
				"���� � ������ �����","������ � ������","��������� ������-������","������� �����","��������� �����",
				"������ �� ���� ����","������ �����","������� ����� 2","�������� ������","������������","������� �������",
				"��� ����������","������������ 2","�����������","��������","������ ����","������������ 3","���������� ����",
				"������ ����� 2","����","�����������","���� �������������","���������","������� �� ����������","��������� ���",
				"������","������ ������","���� � ���","����� ������","������� �� ��������","������ ������ 2",
				"����������� ����","�������� �������","�������-������","����������� �����","��������������","����������� ���������",
				"��������� ���","�������� ������� 2","������� ��������","�������������� 2","�����������",
				"��� �������","�������������� 3","������ �����","����� Z","�������-������ 2","��� �����",
				"������������ ����","������ ����� 2",
--Role Playing-------------------------------------------,
				"��� ������","����� �����","����������","������� ������","������ ����","�������������� ���",
				"��������� ���","����� �������","���������� 2","������","������ � �������","������ ����������",
				"��������","��������� ��� 2","������� �������","����� �����","������ 2","������� ������",
				"��� ����","����� �������","�����������","����� ������","������ 3","������ ������",
				"��������� ����","������ ������","������ �����","������","������ ������ 2","�����������",
				"�������� ������","����� �����","�������� �������","��������� �����","����� �� ��������",
				"������ ������ 2","������ 4","���� �������","���������� �����","������ ������",
				"���������� ����� 2","������� ������","������� �� ��������","���� �����","��� ����",
				"���������� ����� 3","�������","������� 2","�������� �����","������ ����������",
--Adventure-------------------------------------------,
				"������ �������","���","���� ������","������ ������� 2","�������� � �������","����",
				"������� �� �����","��������� ���","����","������� ��������","���� 2","����� � ����",
				"������ � �����","����������� �����������","������ ������","����������� ������","����������� ��������������",
				"����������� �������","����������� ������ 2","������� ������","����������� ������� 2","�����",
				"����������� ������ 2","��������","����������� �������������� 2","����������� �� ����",
				"�������","������-������","����������� �� ���� 2","�����","���� �������","������� 2",
				"������ �����","��� � ����","��������","�����","������ ����� 2","����� ������","��������� �����",
				"�������","������� 2","�������� ����","����� 2","����","����������� �� ���� 2","���������� ������",
				"�������","��������� ����","���� 2","������ ����� 3",
--Strategy-------------------------------------------,
				"����� 2010","�����","�������� ������","������� �����","������� ����� 2","������� ��������",
				"�����","����� ���","������� ����� 3","������ ���������","�����������","����� ��� 2",
				"������������ ������","����������� �������","����������� 2","����� 2020 - ������",
				"������ ��������� 2","���","��������","��������� �����","���� ��������","������� �����",
				"���������","��� 2","���� �������� 2","�����","������� �������","�������� ������",
				"����� 2","������","������","������� ������� 2","���������","������� �����","����� �����",
				"������� ���������� �����","�������� ��� ������","3-� ������� �����","����������� �������",
				"������� ���������� ����� 2","�������� ��� ������ 2","���������� ������","����������� �����",
				"����������� ������� 2","�������","��������","�������� ��� ������ 3","������","������� 2","�������",
--Sports-------------------------------------------,
				"�����-������","��� ��������","������� ���������","������� ������","����� 2000",
				"�����-������ 2","����������� ���������","�������� ������","������","������ 2",
				"������� �����","��� ������","������ 2046","��������� ������","������� ������ 2",
				"������� ������","�������� ����","��� ������ 2","����� �����","������ ����",
				"������ ���� 2","������ ��������","���������","��������� 2","������ ���","����������",
				"���������� 2","��������� ���","��������� ����","�����������","����������� ���� 2000",
				"��������� ��� 2","��������","��������� ���� 2","������ ��������","���� �����","������� ��������",
				"������ �������� 2","����� ����-����","���� ����� 2","�����","������� �� �����",
				"������� �������� 2","���������� �������","������������ ���������","����� �����������",
				"������������ ��������� 2","������� �� ����� 2","������� �������� 3","������ �������",
--Martial Arts------------------------------------------,
				"���� ������","���������","������� �� �����","������ �������","���� ������ 2","�������",
				"��� �������","������� 2","�����-����","�������� �����","�������� �����","������� 2",
				"������ ������� 2","������� ������","������� ���","������ ������","�������������",
				"�������� ����� 2","�������������� �����","������������� 2","���� � �������",
				"�������� ����� 3","�������������� ����� 2","���������","����� 2172","���������",
				"������ ������","������� ������","����� ������","������ ������ 2","��� �� �����",
				"����� ������ 2","���� � �����","������ �����","������ �� ������","���� � ����� 2",
				"������","������� ������ 2","�������� ���","������ 2","������ �����","������ �����",
				"��������� �������","�������� ��� 2","���","��������� ������� 2","������ ������",
				"�����","��� 2","��� 3",
--Simulation------------------------------------------,
				"����������� F16","����� � �������","��������� ������","����� ����������",
				"����������� F162","����� �������","����� ������� 2","��������","������������ ��������",
				"����������� ������","������������ �������� 2","����� ������","����� ����������",
				"�������� 2","������� ������","��������� ������ 2","�������� �� �������",
				"�������� �� ������� 2","������� �����","�����","�������� �� ������� 3",
				"������� ������","������� ������ 2","��� ������","����� 2","������� ����",
				"����� �����","F22 ��������","�������� ��������","���������� ��������","������� ��������� �����",
				"������� ���� 2","������� ��������� ����� 2","������ ����","�����������","F1 ��� �������",
				"�������� ������","����������� 2","�������� ������ 2","����������� ��������� ��������",
				"������ ���� 2","������� ������","F1 ��� ������� 2","������","�������� ������ 3",
				"����� �������","������� ����-�������","������ 2","������� ������ 2","Mi-24 ��������",
--Economy------------------------------------------,
				"����-�����","���������� ��������","�������� ������","���������� �������� 2",
				"��������� ��������","������ ������������ �����������++",
				"����� - ������ ������","����������","�������� ������ 2","������������ ��������",
				"��������������","���������� 2","������ ������������ ����������� 2++","�����","���������",
				"������������� �����","���������������","����������� ���������","�������������� 2",
				"��������������� 2","������","����������� ��������","��������","����������� �������� 2",
				"������ ������������ ����������� 3++","����������� ��������","�������� ����",
				"������������","�������� ���� 2","������","�������� ���������",
				"������������� ��������","������","���������� �������� 2","������������� �������� 2",
				"������������ ������","��� ������","�������� ��������� 2","��� ����������",
				"��� ������ 2","��� ���������� 2","������������ ������ 2",
				"�������� ����-�������","�����-��������","������������� ���������",
				"�����-�������� 2","������������� ��������� 2","����","���� 2","�������������� �������� 2",
--Building Sim------------------------------------------,
				"������� �����","������� �����","������� ����� 2","�����","������� ����� 2",
				"����������� �������","����� 2","����� ������","������","����������� ������� 2",
				"���","������� ���","������ 2","��������","������� ����","������� ����� 3",
				"������� ���� 2","����� �������","����������� ������� 3","��� 2","�������� 2",
				"������� ����","������� ���� 2","������� ��� 2","��� 3","�����",
				"����� 2000","������� �����","�����","�������� ������",
				"������� ����� 2","�������� ������ 2","����� ������","����� 2",
				"�������","������","����� ������ 2","������� 2","������ 2",
				"1492","1493","���","����� 3000","��� 2","������ ������������ �����",
				"������� ������","������ ������������ ����� 2","������� ������ 2",
				"�������� ����","�������� ���� 2",
--Real-Time Strategy------------------------------------------,
				"�������� ����","���� � �����","�����������","�������� ���� 2","��������","�������",
				"����������� 2","���� � ����� 2","��� 2","�������� 2","������� ���������",
				"����� ����","����� ���� 2","������� ��������� 2","����������� ����","�������� �����������",
				"����������� ���� 2","�������� ����������� 2","���� � ����� 2","������ 2034","������ 2035",
				"������� �����","�������","��������� �����","�������� ���� 2","��������������",
				"�������������� 2","��� - ������","������� ���","��� - ������ 2","������� ��� 2",
				"����� �������","����� �����","����� ������� 2","������� 2","������������ �������",
				"�������������� 3","������������ ������� 2","���","��� 2","9-�� ������",
				"����� ������� 3","�������� �������","9-�� ������ 2","�������� ������� 2",
				"������� �������","�������� ������� 3","������� ������� 2","������","������ 2",
--3D Action------------------------------------------,
				"����","������������ �����","������ 2","������� �����","������� ����� 2",
				"������� ����� 3","��75 - ��������","��","������������ ����� 2","��� 45",
				"���������","�� 2","���� ������","�.�.�.�.","�.�.�.�. 2","�� 3","����� ����",
				"����� ���� 2","����� ���� 3","�����","����� ���-�����","����������� ������","����� 2",
				"����������� ������ 2","���� 2","������","������ 2","����","������","������","�����",
				"����� 2","���� 2","������ 2","������ ���","������ ��� 2","������ ����","������ ��� 3",
				"������ ���� 2","���� �","���� � 2","���� � 3","����","����","���� 2","������ ���� 3",
				"�����������","������ ������ 2","������ �������","������ ������� 2",
--Interactive Film------------------------------------------,
				"�������� ����� �����","����� �����","����� ����� 2","����� 14",
				"������� �����","�������","�� ����","������� ����� 2","�� ���� 2","���",
	 			"������� ����� 3","�������","�������� - ������","�������� - ������ 2",
				"�������� - ������ 3","�������� �����","������","�������� ����� 2",
				"��������������� �� �������","������ 2","�����","����� �����","������� 2",
				"��� �����","��� ����� 2","����� ����","RAX","RAX 2","�������� ����������",
				"�������� ���������� 2","��������","RAX 3","�������� 2","�����","����� 2",
				"����� 3","�����","�������� 3","����� 2","�����������","������� ����",
				"�����","������ ����","����� �������","����� 2","������ �������","��������� �����","������ ���� 2",
				"��������� ����� 2","�����"
}
				
function GetName()
	for i=0, 9 do
		local index = base.math.random( 1, #names )
		local game = base.applic:GetGame( names[ index ] )
				
		if game == nil or game.empty then
			return names[ index ]
		end
	end
	
	return "gameNames:GetName:Error"
end