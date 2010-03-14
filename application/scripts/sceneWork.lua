-- �������� ������ �����
require("logdefs")
require("elmid")
require("swork_terrainConfig")
require("swork_MainMenu")
require("swork_minimap")
require("swork_formular")
require("swork_camera")
require("swork_ObjectsLoading")

Log({src=SCRIPT, dev=ODS|CON}, "SCRIPT-TEST:������������� ������ � ��")
-- �������� ��������������� �������
LogVideo("���� �������� ���������", 0xDEADBEEF)

local sceneManager = CLuaSceneManager( NrpGetSceneManager() 
local app = CLuaConfig( NrpGetApplication() )

citySceneObjects = { } 
bankSceneObjects = { }

--�������� ������� ����� 
NrpInitializeWorkScene()
sceneManager:DrawProgress( 0 )

--���������� ������ � �����
AddCamera()
sceneManager:DrawProgress( 25 )

--���������� �������� � �����
AddTerrain()
sceneManager:DrawProgress( 50 )

--�������� �������� ����
AddMenuWindow()
sceneManager:DrawProgress( 70 )

--�������� ������� �����
AddStorePanel()
sceneManager:DrawProgress( 77 )

--�������� ���������
AddMiniMap()
sceneManager:DrawProgress( 85 )

--�������� ����
sceneManager:AddSkyDomeSceneNode( "media/sky/panorama.jpg", 20, 12, 1, 2.0, 10000 )
sceneManager:DrawProgress( 90 )

--�������� ��������� �����
--AddGlobalMap()
sceneManager:DrawProgress( 95 )

AddObjectsToCityScene()
AddObjectsToBankScene()
sceneManager:DrawProgress( 100 )

sceneManager:AddSceneFunction( SCENE_LMOUSE_DOUBLE_CLICK, "sworkSelectObjectOnCityScene" )
local company = CLuaCompany( app:CreateCompany( "DaleTeam" ) )