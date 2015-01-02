#include <iostream>
#include <map>
#include <FlyWin32.h>
#include "local/Character.h"
#include "local/CharacterManageSystem.h"
#include "local/Camera.h"
#include "net/game_client.h"
#include "net/game_updater_real.h"

#define window_w 1024
#define window_h 768
#define sight_w 83
#define sight_h 66


GmClient game_client;
GmUpdaterReal game_updater;

VIEWPORTid viewportID;	//major viewe port
SCENEid sceneID;	//3d scene
OBJECTid cameraID, cameraBaseID, terrainID, lightID;
CHARACTERid actorID;
ACTIONid idleID, runID, curPoseID;

OBJECTid spID0 = FAILED_ID;

ROOMid terrainRoomID = FAILED_ID;
TEXTid textID = FAILED_ID;
TEXTid textCharID = FAILED_ID;
TEXTid textHP_vID = FAILED_ID;
TEXTid textInfo_vID = FAILED_ID;
Camera camera;

SCENEid sID2;                // the 2D scene

CharacterManageSystem chrMgtSystem(&game_updater);

BOOL4 DIR_KEYDOWN[4] = {FALSE, FALSE, FALSE, FALSE};
BOOL4 first_switch_action = FALSE;

char dbg_msgS[256];

//global value
int frame = 0;

int oldX, oldY, oldXM, oldYM, oldXMM, oldYMM;
std::map<MotionState, ACTIONid> state2ActionTable;
//BOOL4 poseChange = FALSE;

//hotkey callback
void QuitGame(BYTE, BOOL4);
void Movement(BYTE, BOOL4);

//timer callback
void GameAI(int);
void RenderIt(int);

//mouse callback
void InitPivot(int, int);
void PivotCam(int, int);
void InitMove(int, int);
void MoveCam(int, int);
void InitZoom(int, int);
void ZoomCam(int, int);
void ChangeActor(BYTE code, BOOL4 value);
void setCamera();

void FyMain(int argc, char **argv) {
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);
	string ip, port;
	cout << "Enter ip   [127.0.0.1]: ";getline(cin, ip);
	cout << "Enter port [8976     ]: ";getline(cin, port);

	if(ip.empty()) {ip = "127.0.0.1";}
	if(port.empty()) {port = "8976";}

	game_client.initialize(ip, port, &game_updater);
	game_client.connectServer();
	

	std::cout<<"Start Game" << std::endl;
	//create a new window
	FyStartFlyWin32("HomeWork 3 - with Fly2", 0, 0, 1024, 768, FALSE);
	//std::cout << "Window initiated" << std::endl; system("pause");
	//set up path
	FySetShaderPath("Data\\NTU5\\Shaders");
	FySetModelPath("Data\\NTU5\\Scenes");
	FySetTexturePath("Data\\NTU5\\Scenes\\Textures");
	FySetScenePath("Data\\NTU5\\Scenes");
	//std::cout << "Path set" << std::endl; system("pause");
	//create a viewport
	viewportID = FyCreateViewport(0, 0, 1024, 768);
	FnViewport viewport(viewportID);

	//create 3D scene
	sceneID = FyCreateScene(10);
	FnScene scene(sceneID);

	scene.Load("gameScene01");
	scene.SetAmbientLights(1.0f, 1.0f, 1.0f, 0.6f, 0.6f, 0.6f);

	// create a 2D scene for sprite rendering which will be rendered on the top of 3D
    FnScene scene2D;
    sID2 = FyCreateScene(1);
    scene2D.Object(sID2);
    scene2D.SetSpriteWorldSize(1024, 768);         // 2D scene size in pixels

	FnSprite sp;
    spID0 = scene2D.CreateObject(SPRITE);
    sp.Object(spID0);
	sp.SetSize(sight_w, sight_h);
    sp.SetImage("spiner", 0, NULL, FALSE, NULL, 2, TRUE, FILTER_LINEAR);
	sp.SetPosition(window_w/2-sight_w/2, window_h/2-sight_h/2, 0);


//	std::cout << "GameScene loaded" << std::endl; system("pause");
	//load the terrain
	terrainID = scene.CreateObject(OBJECT);
	FnObject terrain;
	terrain.ID(terrainID);
	terrain.Load("terrain");
	terrain.Show(FALSE);

	//set terrain environment
	terrainRoomID = scene.CreateRoom(SIMPLE_ROOM, 10);
	FnRoom room;
	room.ID(terrainRoomID);
	room.AddObject(terrainID);

   // put the character on terrain
   float pos[3], fDir[3], uDir[3];
	pos[0] = 3569.0, pos[1] = -3108; pos[2] = 0;
	fDir[0] = 1, fDir[1] = 0; fDir[2] = 0;
	uDir[0] = 0, uDir[1] = 0, uDir[2] = 1;

	//load the character
	FySetModelPath("Data\\NTU5\\Characters");
	FySetTexturePath("Data\\NTU5\\Characters");
	FySetCharacterPath("Data\\NTU5\\Characters");

	//std::cout << "Path set again" << std::endl; system("pause");
	//依據GameTree來新增
	// 目前只有兩個腳色，之後需要把enemy換成vector
	Character actor, ememy;
	map<int, GmCharacter*> charNode = game_client.getGmTree().getCharacterNode();
	cout << "Number of Character: " << charNode.size() << endl;
    for (map<int, GmCharacter*>::iterator it = charNode.begin(); it != charNode.end(); it++) {
		cout << it->second->game_id << ", is_main_character?" << it->second->is_main_actor << endl;
		if(it->second->is_main_actor) {
			cout << "main character" << endl;
			actor.setMeshFileName("Lyubu2");
			actor.setCharacterName("Lyubu2");
			actor.initialize(sceneID, NULL, terrainRoomID, it->second->fdir,  it->second->udir,  it->second->pos);
			actorID = actor.getCharacterId();
			chrMgtSystem.addCharacter(actor, true);
			game_updater.registerCharacter(it->second->game_id, actor.getCharacterId());
		} else {
			cout << "Not main character" << endl;
			ememy.setMeshFileName("Donzo2");
			ememy.setCharacterName("Donzo2");
			ememy.initialize(sceneID, NULL, terrainRoomID, it->second->fdir,  it->second->udir,  it->second->pos);
			chrMgtSystem.addCharacter(ememy, false);
			game_updater.registerCharacter(it->second->game_id, ememy.getCharacterId());
		}
    }
	//std::cout << "Character set." << std::endl; system("pause");
	//初始化人物 注意cameraID要重設
	setCamera();
	//std::cout << "Camera set." << std::endl; system("pause");
   // setup a point light
   /*
   FnLight light;
   lightID = scene.CreateObject(LIGHT);
   light.ID(lightID);
   light.Translate(70.0f, -70.0f, 70.0f, REPLACE);
   light.SetColor(1.0f, 1.0f, 1.0f);
   light.SetIntensity(1.0f);
   *//*
   //create a text object for display message on screen
   textID = FyCreateText("Trebuchet MS", 18, FALSE, FALSE);
   textCharID = FyCreateText("Trebuchet MS", 40, TRUE, FALSE);
   */
   // set Hotkeys
   /*
   FyDefineHotKey(FY_ESCAPE, QuitGame, FALSE);  // escape for quiting the game
   FyDefineHotKey(FY_UP, Movement, FALSE);      // Up for moving forward
   FyDefineHotKey(FY_RIGHT, Movement, FALSE);   // Right for turning right
   FyDefineHotKey(FY_LEFT, Movement, FALSE);    // Left for turning left
   FyDefineHotKey(FY_DOWN, Movement, FALSE);    // Down for moving backward
	*//*
   FyDefineHotKey(FY_TAB, ChangeActor, FALSE);
   //define some mouse function
   FyBindMouseFunction(LEFT_MOUSE, InitPivot, PivotCam, NULL, NULL);
   FyBindMouseFunction(MIDDLE_MOUSE, InitZoom, ZoomCam, NULL, NULL);
   FyBindMouseFunction(RIGHT_MOUSE, InitMove, MoveCam, NULL, NULL);
   */
   //bind timers, frame rate = 30 fps
   FyBindTimer(0, 30.0f, GameAI, TRUE);
   FyBindTimer(1, 30.0f, RenderIt, TRUE);
   
	//invoke the system
   FyInvokeFly(TRUE);
   
   system("pause");
}



/*-------------------------------------------------------------
  30fps timer callback in fixed frame rate for major game loop
  C.Wang 1103, 2007
 --------------------------------------------------------------*/
void GameAI(int skip){
	chrMgtSystem.update(skip); //人物狀態的更新
	actorID = chrMgtSystem.getActorID();
   //Camera狀態的更新
	camera.GameAIupdate(skip);
	//camera.resetCamera();
	//game_updater.updateCharacterPush(actorID);
	game_client.update();
}

void RenderIt(int skip){
	float pos[3], fDir[3], uDir[3];

	FnViewport vp;

	//render the whole scene
	vp.ID(viewportID);
	vp.Render3D(cameraID, TRUE, TRUE);
	vp.RenderSprites(sID2, FALSE, TRUE);  // no clear the background but clear the z buffer

	//show frame rate
	static char string[128];
	if(frame == 0){
		FyTimerReset(0);
	}

	if((frame/10)*10 == frame){
		float curTime;
		curTime = FyTimerCheckTime(0);
		sprintf_s(string, "Fps: %6.2f", frame/curTime);
	}

	frame += skip;
	if(frame >= 1000){
		frame = 0;
	}

	FnText text,charactorInfo,char_HP,info;
	text.ID(textID);
	charactorInfo.ID(textCharID);
	char_HP.ID(textHP_vID);
	info.ID(textInfo_vID);

	text.Begin(viewportID);
	charactorInfo.Begin(viewportID);
	text.Write(string, 20, 20, 255, 0, 0);

	//get camera's data
	camera.getCamera().GetPosition(pos);
	camera.getCamera().GetDirection(fDir, uDir);

	char posS[256], fDirS[256], uDirS[256];
	sprintf_s(posS, "pos: %8.3f %8.3f %8.3f", pos[0], pos[1], pos[2]);
	sprintf_s(fDirS, "facing: %8.3f %8.3f %8.3f", fDir[0], fDir[1], fDir[2]);
	sprintf_s(uDirS, "up: %8.3f %8.3f %8.3f", uDir[0], uDir[1], uDir[2]);

    text.Write(posS, 20, 35, 255, 255, 0);
    text.Write(fDirS, 20, 50, 255, 255, 0);
    text.Write(uDirS, 20, 65, 255, 255, 0);

	//get camera base's data
	camera.getCameraBase().GetPosition(pos);
	camera.getCameraBase().GetDirection(fDir, uDir);
	sprintf_s(posS, "pos: %8.3f %8.3f %8.3f", pos[0], pos[1], pos[2]);
	sprintf_s(fDirS, "facing: %8.3f %8.3f %8.3f", fDir[0], fDir[1], fDir[2]);
	sprintf_s(uDirS, "up: %8.3f %8.3f %8.3f", uDir[0], uDir[1], uDir[2]);
    
	text.Write(posS, 20, 80, 255, 255, 0);
    text.Write(fDirS, 20, 95, 255, 255, 0);
    text.Write(uDirS, 20, 110, 255, 255, 0);

	FnCharacter actor;
	actor.ID(actorID);
	//get actor's data
	actor.GetPosition(pos);
	actor.GetDirection(fDir, uDir);
	sprintf_s(posS, "pos: %8.3f %8.3f %8.3f", pos[0], pos[1], pos[2]);
	sprintf_s(fDirS, "facing: %8.3f %8.3f %8.3f", fDir[0], fDir[1], fDir[2]);
	sprintf_s(uDirS, "up: %8.3f %8.3f %8.3f", uDir[0], uDir[1], uDir[2]);
    
	text.Write(posS, 20, 125, 255, 255, 0);
    text.Write(fDirS, 20, 140, 255, 255, 0);
    text.Write(uDirS, 20, 155, 255, 255, 0);

   text.End();

   	sprintf_s(posS, "HEALTH", chrMgtSystem.getCharacterblood(actorID));
	charactorInfo.Write(posS, 50, 700, 255, 255, 100);
	sprintf_s(posS, "ROUND", chrMgtSystem.getCharacterblood(actorID));
	charactorInfo.Write(posS, 400, 700, 255, 255, 100);
	sprintf_s(posS, "TIME", chrMgtSystem.getCharacterblood(actorID));
	charactorInfo.Write(posS, 515, 700, 255, 255, 100);
	charactorInfo.End();

	sprintf_s(posS, "%d", chrMgtSystem.getCharacterblood(actorID));
	char_HP.Write(posS, 110, 672, 255, 255, 255);
	sprintf_s(posS, "%d", chrMgtSystem.getCharacterblood(actorID));
	info.Write(posS, 460, 694 ,255, 255, 255);
	sprintf_s(posS, "%d", chrMgtSystem.getCharacterblood(actorID));
	info.Write(posS, 560, 694, 255, 255, 255);
	info.End();
   FySwapBuffers();
}



void Movement(BYTE code, BOOL4 value){

}

/*------------------
  quit the demo
  C.Wang 0327, 2005
 -------------------*/
void QuitGame(BYTE code, BOOL4 value)
{
   if (code == FY_ESCAPE) {
      if (value) {
         FyQuitFlyWin32();
      }
   }
}

/*-----------------------------------
  initialize the pivot of the camera
  C.Wang 0329, 2005
 ------------------------------------*/
void InitPivot(int x, int y)
{
   oldX = x;
   oldY = y;
   frame = 0;
}


/*------------------
  pivot the camera
  C.Wang 0329, 2005
 -------------------*/
void PivotCam(int x, int y)
{
   FnObject model;

   if (x != oldX) {
      model.ID(cameraID);
      model.Rotate(Z_AXIS, (float) 0.2*(x - oldX), GLOBAL);
      oldX = x;
   }

   if (y != oldY) {
      model.ID(cameraID);
      model.Rotate(X_AXIS, (float) 0.2*(y - oldY), GLOBAL);
      oldY = y;
   }
}


/*----------------------------------
  initialize the move of the camera
  C.Wang 0329, 2005
 -----------------------------------*/
void InitMove(int x, int y)
{
   oldXM = x;
   oldYM = y;
   frame = 0;
}


/*------------------
  move the camera
  C.Wang 0329, 2005
 -------------------*/
void MoveCam(int x, int y)
{
   if (x != oldXM) {
      FnObject model;

      model.ID(cameraID);
      model.Translate((float)(x - oldXM)*2.0f, 0.0f, 0.0f, LOCAL);
      oldXM = x;
   }
   if (y != oldYM) {
      FnObject model;

      model.ID(cameraID);
      model.Translate(0.0f, (float)(oldYM - y)*2.0f, 0.0f, LOCAL);
      oldYM = y;
   }
}


/*----------------------------------
  initialize the zoom of the camera
  C.Wang 0329, 2005
 -----------------------------------*/
void InitZoom(int x, int y)
{
   oldXMM = x;
   oldYMM = y;
   frame = 0;
}


/*------------------
  zoom the camera
  C.Wang 0329, 2005
 -------------------*/
void ZoomCam(int x, int y)
{
   if (x != oldXMM || y != oldYMM) {
      FnObject model;

      model.ID(cameraID);
      model.Translate(0.0f, 0.0f, (float)(x - oldXMM)*10.0f, LOCAL);
      oldXMM = x;
      oldYMM = y;
   }
}

void ChangeActor(BYTE code, BOOL4 value)
{
	if (value)
	{
		chrMgtSystem.changActorByTAB();
		actorID = chrMgtSystem.getActorID();
		setCamera();
	}
}

void setCamera() {
	//初始化攝影機
	//std::cout << "setCamera()" << std::endl; system("pause");
	camera.initialize(sceneID, terrainID, chrMgtSystem.getCameraActor());
	//std::cout << "Camera initialize" << std::endl; system("pause");
	cameraID = camera.getCameraId();
	//std::cout << "getCameraId()" << std::endl; system("pause");
	cameraBaseID = camera.getCameraBaseId();
	//std::cout << "cameraBaseID" << std::endl; system("pause");
	//放好相機
	camera.resetCamera();
	//std::cout << "resetCamera()" << std::endl; system("pause");
}