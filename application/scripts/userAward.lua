local base = _G

module( "userAward" )

local emp = nil

local function _GetAward( start, stop )
	if emp ~= nil then
		emp:AddParam( "lastAward", stop, true )
		
		local salary = emp:GetParam( "salary" )
		local lastAward = emp:GetParam( "lastAward" )
		
		--��������� ���������� ���������� �������� ��� ������� ������
		emp:AddModificator( "mood", 14, false, 100 * stop / ( award + salary ), "������" )	
		base.LogScript( "!!!!!!!!!!!!!! Award to "..emp.name.." is $"..stop )
		--currentEmployer:AddModificator( "effectively", 1.2, "7 day" )
		
		base.applic.playerCompany:AddBalance( "������ $"..award.." ��� "..emp.name, -award )
	end 
end

function Show()
	emp = base.userManager.currentEmployer
	if emp ~= nil then
		local curSalary = emp:GetParam( "salary" )
		local interval = base.math.ceil( curSalary / 10 )
		base.moneySelect.Show( "���������� ������ ������", 0, interval, _GetAward )
	end
end