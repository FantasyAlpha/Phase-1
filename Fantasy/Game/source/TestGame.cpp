#include <TestGame.h>
#include <PauseScreen.h>
#include <PauseGame.h>
#include <MainMenu.h>
#include <ResetCamera.h>
#include <ShowHide.h>
#include <PlayBGM.h>

void TestGame::Init()
{
	// textures 
	/*AddTexture(&ResourceManager, LoadTexture("resources\\textures\\empty.png"), "Empty");
	AddTexture(&ResourceManager, LoadTexture("resources\\textures\\back.png"), "back");*/

	AddTexture(&ResourceManager, LoadTexture("resources\\textures\\back.png"), "back");
	AddTexture(&ResourceManager, LoadTexture("resources\\textures\\new grass1.png"), "grass forward");
	AddTexture(&ResourceManager, LoadTexture("resources\\textures\\new grass22.png"), "grass back");
	AddTexture(&ResourceManager, LoadTexture("resources\\textures\\double2.png"), "back trees");
	AddTexture(&ResourceManager, LoadTexture("resources\\textures\\land piece2.png"), "floor");
	AddTexture(&ResourceManager, LoadTexture("resources\\textures\\sky with clouds.png"), "sky");
	AddTexture(&ResourceManager, LoadTexture("resources\\textures\\bar.png"), "black");
	AddTexture(&ResourceManager, LoadTexture("resources\\textures\\left.png"), "left");
	AddTexture(&ResourceManager, LoadTexture("resources\\textures\\middle.png"), "middle");
	AddTexture(&ResourceManager, LoadTexture("resources\\textures\\right.png"), "right");
	//AddTexture(&ResourceManager, LoadTexture("resources\\textures\\player_2.png"), "PLAYER");
	AddTexture(&ResourceManager, LoadTexture("resources\\textures\\Neo In.png"), "neo");

	AddTexture(&ResourceManager, LoadTexture("resources\\textures\\Noe X BasicAttack.png"), "noe basicattack");
	AddTexture(&ResourceManager, LoadTexture("resources\\textures\\Noe X Idle.png"), "noe idle");
	AddTexture(&ResourceManager, LoadTexture("resources\\textures\\Noe X Summon.png"), "noe summon");
	AddTexture(&ResourceManager, LoadTexture("resources\\textures\\Noe X Walk.png"), "noe walk");



	AddTexture(&ResourceManager, LoadTexture("resources\\textures\\main Canvas.png"), "main Canvas");


	AddTexture(&ResourceManager, LoadTexture("resources\\textures\\BG0.png"), "BG0");
	AddTexture(&ResourceManager, LoadTexture("resources\\textures\\PlatForm2.png"), "PlatForm2");


	AddTexture(&ResourceManager, LoadTexture("resources\\textures\\Asato Attack.png"), "straight_Mon");
	AddTexture(&ResourceManager, LoadTexture("resources\\textures\\Weboo Attack.png"), "arc_Mon");
	AddTexture(&ResourceManager, LoadTexture("resources\\textures\\Hook PushBack.png"), "hook");
	AddTexture(&ResourceManager, LoadTexture("resources\\textures\\Jumpo Jump.png"), "jumpo");


	AddTexture(&ResourceManager, LoadTexture("resources\\textures\\Warrior.png"), "Warrior");
	AddTexture(&ResourceManager, LoadTexture("resources\\textures\\Warrior Idle.png"), "warrior idle");
	AddTexture(&ResourceManager, LoadTexture("resources\\textures\\Warrior Walk.png"), "warrior walk");
	AddTexture(&ResourceManager, LoadTexture("resources\\textures\\Warrior BasicAttack.png"), "wAttack");


	AddTexture(&ResourceManager, LoadTexture("resources\\textures\\Wizard.png"), "Wizard");
	AddTexture(&ResourceManager, LoadTexture("resources\\textures\\Wizard Idle.png"), "wizard idle");
	AddTexture(&ResourceManager, LoadTexture("resources\\textures\\Wizzard Walk.png"), "wizard walk");
	AddTexture(&ResourceManager, LoadTexture("resources\\textures\\Wizard Attack.png"), "wizard attack");
	AddTexture(&ResourceManager, LoadTexture("resources\\textures\\Water Attack.png"), "wizard Projectile");

	AddTexture(&ResourceManager, LoadTexture("resources\\textures\\Rufus Idle.png"), "rufus idle");
	AddTexture(&ResourceManager, LoadTexture("resources\\textures\\Rufus Walk.png"), "rufus walk");
	AddTexture(&ResourceManager, LoadTexture("resources\\textures\\Rufus Attack1.png"), "rufus attack basic");
	AddTexture(&ResourceManager, LoadTexture("resources\\textures\\Rufus Attack2.png"), "rufus attack advance");

	AddBGM(&ResourceManager, "resources\\audio\\Game-Menu_Looping.wav", "menu");
	AddBGM(&ResourceManager, "resources\\audio\\Forest-Chase_Looping.wav", "game");
	
	Scene->Init(1);
	// camera 
	Scene->MainCamera = BasicCamera(vec2f(Dimensions.Width, Dimensions.Height));
	Scene->MainCamera.Type = CameraType::ORTHOGRAPHIC;
	//Scene->MainCamera.WindowSize = Scene->MainCamera.Size / 2.0f;
	//Scene->MainCamera.Size *= 2.5f;
	Scene->MainCamera.WindowSize = vec2f(320, 286);
	//Scene->MainCamera.Scale = Scene->MainCamera.Scale / 2.5f;
   
	Scene->REngine.InitShader("resources\\shaders\\vertex shader 120.vert", "resources\\shaders\\fragment shader 120.frag");
	Scene->REngine.InitDebugShader("resources\\shaders\\vertex shader 120_2.vert", "resources\\shaders\\fragment shader 120_2.frag");

	/*
	*/
	GameObject *state1 = Scene->AddSubRoot("State 1");
	state1->AddComponent(&ResetCamera(Scene->MainCamera.Size * 2.5f));
	state1->AddComponent(&PlayBGM(&ResourceManager));
	GameObject *state2 = Scene->AddSubRoot("State 2");
	state2->AddComponent(&ResetCamera(Scene->MainCamera.Size));
	state2->AddComponent(&PlayBGM(&ResourceManager));

	GameObject *state3 = Scene->AddSubRoot("State 3");
	state3->AddComponent(&ResetCamera(Scene->MainCamera.Size));

	/*
	*/
	Scene->EnableSubRoot("State 1");

	state1->AddComponent(&SwitchState("switch", "State 3"));
	state1->AddComponent(&PauseGame((SwitchState *)state1->GetComponent("switch", "SWITCH")));

	GameObject *tillingsky1 = Scene->AddObject("sky1");
	GameObject *tillingsky2 = Scene->AddObject("sky2");
	tillingsky1->AddComponent(&Tilling("tilling4", &this->ResourceManager, "sky1", "sky2", vec3f(0.0f, 400.0f, -20.0f), vec2f((Dimensions.Width * 2.5 + 400.0f), 2085), "sky", Dimensions.Width, 0.0f));

	GameObject *tillingtrees1 = Scene->AddObject("trees1");
	GameObject *tillingtrees2 = Scene->AddObject("trees2");
	tillingtrees1->AddComponent(&Tilling("tilling3", &this->ResourceManager, "trees1", "trees2", vec3f(0.0f, 400.0f, -20.0f), vec2f((Dimensions.Width * 2.5 + 400.0f), 3085), "back trees", Dimensions.Width, 0.6f));

	GameObject *tillingBack1 = Scene->AddObject("back1");
	GameObject *tillingBack2 = Scene->AddObject("back2");
	tillingBack1->AddComponent(&Tilling("tilling1", &this->ResourceManager, "back1", "back2", vec3f(0.0f, -350.0f, -1.0f), vec2f((Dimensions.Width * 2.5 + 400.0f), 300), "grass back", Dimensions.Width, 0.4f));

	// collider (colliderName , pos * vec3 , size vec2 , offest vec2 , ground , trigger)


	// platform area
	{
		/*GameObject *tillingFloor1 = Scene->AddObject("floor1");
		GameObject *tillingFloor2 = Scene->AddObject("floor2");
		tillingFloor1->AddComponent(&Tilling("tilling8", &this->ResourceManager, "floor1", "floor2", vec3f(0.0f, -350.0f, -1.0f), vec2f((Dimensions.Width * 2.5 + 400.0f), 300), "grass back", Dimensions.Width, 0.4f));
		*/
		GameObject *tillingfloor1 = Scene->AddObject("floor1");
		GameObject *tillingfloor2 = Scene->AddObject("floor2");
		tillingfloor1->AddComponent(&Tilling("tillingf", &this->ResourceManager, "floor1", "floor2", vec3f(0.0f, -600.0f, -1.0f), vec2f((Dimensions.Width * 2.5 + 400.0f), 300), "floor", Dimensions.Width, 0.0f));

		/*GameObject *objp0 = Scene->AddObject("objp0");
		objp0->AddComponent(&SpriteRenderer("r1", Sprite{ vec3f(0, 0, 0), vec2f(2035, 300), Material{ GetTexture(&ResourceManager, "floor"), vec4f(1, 1, 1, 1) } }));
		objp0->AddComponent(&Collider("c1", &objp0->ObjectTransform.Position, vec2f(9000, 200), vec2f(0, 0), 1, false));
		objp0->ObjectTransform.Position.Y = -600;*/

		GameObject *objp1 = Scene->AddObject("objp1");
		objp1->AddComponent(&SpriteRenderer("r1", Sprite{ vec3f(0, 0, 0), vec2f(2035, 300), Material{ GetTexture(&ResourceManager, "left"), vec4f(1, 1, 1, 1) } }));
		objp1->ObjectTransform.Position.Y = -600;
		objp1->AddComponent(&Collider("c1", &objp1->ObjectTransform.Position, vec2f(9000, 200), vec2f(0, 0), 1, false));





	}

	GameObject *MainPlayer = Scene->AddObject("player");
	//GameObject *enemy_warrior1 = Scene->AddObject("warrior1");


	// render component 
	vec2f playerSize = vec2f(800, 700);


	MainPlayer->AddComponent(&SpriteRenderer("r1", Sprite{ vec3f(0, 0, 0), playerSize, Material{ GetTexture(&ResourceManager, "neo"), vec4f(1, 1, 1, 1) } }));
	MainPlayer->ObjectTransform.Position.X = -500.0f;
	MainPlayer->AddComponent(&Animator("animator", (SpriteRenderer *)MainPlayer->GetComponent("r1", "RENDERABLE")));

	GameObject *enemyw1 = Scene->AddObject("enemyw1");
	enemyw1->AddComponent(&SpriteRenderer("enemy1w_sprite", Sprite{ vec3f(0, 0, 0), vec2f(350, 466), Material{ GetTexture(&this->ResourceManager, "Warrior"), vec4f(1, 1, 1, 1) } }));
	enemyw1->ObjectTransform.Position.X = 100.0f;
	enemyw1->AddComponent(&Collider("enemyw1Collider", &enemyw1->ObjectTransform.Position, vec2f(300, 466), vec2f(0, 0), false, 1));
	enemyw1->AddComponent(&EnemyWarriorMind("myMind", &ResourceManager, "enemyw1", vec3f(100, 0, 0), "enemyw1Attack", "enemyw1HP", "warrior idle clip", "warrior walk clip", "warrior attack clip", "enemyw1Collider", "enemy1w_sprite"));
	enemyw1->AddComponent(&Animator("animatorWarriorEnemy", (SpriteRenderer *)enemyw1->GetComponent("enemy1w_sprite", "RENDERABLE")));
	//enemyw1->AddComponent(&ApplyGravity("enemyw1", "enemyw1Collider");
	//enemyw1->Layer = "warrior";

	GameObject *enemywizard1 = Scene->AddObject("enemywizard1");
	enemywizard1->AddComponent(&SpriteRenderer("enemy1wizard_sprite", Sprite{ vec3f(0, 0, 0), vec2f(350, 566), Material{ GetTexture(&this->ResourceManager, "Wizard"), vec4f(1, 1, 1, 1) } }));
	enemywizard1->AddComponent(&Collider("enemywizard1Collider", &enemywizard1->ObjectTransform.Position, vec2f(300, 566), vec2f(0, 0), false, 1));
	enemywizard1->AddComponent(&EnemyWizardMind("myMind", &ResourceManager, "enemywizard1", vec3f(300, 0, 0), "enemywizard1Attack", "enemywizard1HP", "wizard idle clip", "wizard walk clip", "wizard attack clip", "enemywizard1Collider", "enemy1wizard_sprite"));
	enemywizard1->AddComponent(&Animator("animatorWizardEnemy", (SpriteRenderer *)enemywizard1->GetComponent("enemy1wizard_sprite", "RENDERABLE")));
	//enemywizard1->Layer = "wizard";

	GameObject *enemyRufus = Scene->AddObject("enemyRufus");
	enemyRufus->AddComponent(&SpriteRenderer("enemyrufus_sprite", Sprite{ vec3f(0, 0, 0), vec2f(300 * 3.0f, 266 * 4.0f), Material{ GetTexture(&this->ResourceManager, "Wizard"), vec4f(1, 1, 1, 1) } }));
	enemyRufus->ObjectTransform.Position.X = 900.0f;
	//enemyRufus->ObjectTransform.Position.Y = 900.0f;
	enemyRufus->AddComponent(&Collider("enemyrufusCollider", &enemyRufus->ObjectTransform.Position, vec2f(300 * 2, 266 * 3.0f), vec2f(0, 0), false, true));
	enemyRufus->AddComponent(&EnemyRufusMind("myMind", &ResourceManager, "enemyRufus", vec3f(900, 0, 0), "enemyrufusAttack", "enemyrufusAttackAdvance", "enemyrufusHP", "wizard idle clip", "rufus walk clip", "rufus attack clip", "rufus attack advance clip", "enemyrufusCollider", "enemyrufus_sprite"));
	enemyRufus->AddComponent(&Animator("animatorRufusEnemy", (SpriteRenderer *)enemyRufus->GetComponent("enemyrufus_sprite", "RENDERABLE")));
	//enemyRufus->Layer = "rufus";




	{
		/*obj3->AddComponent(&SpriteRenderer("r2", Sprite{ vec3f(0, 0, 0), vec2f(300, 50), Material{ GetTexture(&ResourceManager, "Empty"), vec4f(1, 0, 0, 1) } }));
		obj3->AddComponent(&Collider("c2", &obj3->ObjectTransform.Position, vec2f(300, 50), vec2f(), 1, false));
		obj3->AddComponent(&ApplyGravity("A1", "c2"));*/

		//blue 
		/*obj4->AddComponent(&SpriteRenderer("r2", Sprite{ vec3f(0, 0, 0), vec2f(30, 150), Material{ GetTexture(&ResourceManager, "Empty"), vec4f(0, 0, 1, 1) } }));
		obj4->AddComponent(&Collider("c2", &obj4->ObjectTransform.Position, vec2f(30, 150), vec2f(), 1, false));
		obj4->ObjectTransform.Position.X += 50;
		obj4->ObjectTransform.Position.Y += 50;*/

	}

	MainPlayer->AddComponent(&Collider("c1", &MainPlayer->ObjectTransform.Position, vec2f(300, 500), vec2f(0, -50), 0, false));
	MainPlayer->AddComponent(&ArcMonster("ArcMonster_attack", &this->ResourceManager));
	MainPlayer->AddComponent(&StraightMonster("StraightMonster_attack", &this->ResourceManager));
	MainPlayer->AddComponent(&PullBackMonster("PullBackMonster_attack", &this->ResourceManager));
	MainPlayer->AddComponent(&BasicAttack("playerBasic_attack", &this->ResourceManager));
	MainPlayer->AddComponent(&PlayerMove("m1", "c1", 500.0f, false));
	MainPlayer->AddComponent(&Jump("j1", 13, &this->ResourceManager));
	MainPlayer->AddComponent(&playerCollide("checkCollide", &this->ResourceManager, "c1"));

	//tiling area

	GameObject *tillingforword1 = Scene->AddObject("forword1");
	GameObject *tillingforword2 = Scene->AddObject("forword2");
	tillingforword1->AddComponent(&Tilling("tilling2", &this->ResourceManager, "forword1", "forword2", vec3f(0.0f, -630.0f, 0.0f), vec2f((Dimensions.Width * 2.5 + 400.0f), 300), "grass forward", Dimensions.Width, -0.8f));

	//black area
	{

		GameObject *tillingblack1 = Scene->AddObject("black1");
		GameObject *tillingblack2 = Scene->AddObject("black2");
		tillingblack1->AddComponent(&Tilling("tillingb", &this->ResourceManager,
			"black1", "black2", vec3f(0.0f, -800.0f, 0.0f), vec2f((Dimensions.Width * 2.5 + 400.0f), 100),
			"black", Dimensions.Width, 0.0f));



		GameObject *tillingblack11 = Scene->AddObject("black11");
		GameObject *tillingblack22 = Scene->AddObject("black22");
		tillingblack11->AddComponent(&Tilling("tillingbb", &this->ResourceManager,
			"black11", "black22", vec3f(0.0f, 800.0f, 0.0f), vec2f((Dimensions.Width * 2.5 + 400.0f), 100),
			"black", Dimensions.Width, 0.0f));


	}


	Animator *animator = (Animator *)MainPlayer->GetComponent("animator", "ANIMATOR");

	animator->AddClip("noe idle clip", &AnimationClip(GetTexture(&ResourceManager, "noe idle"), 4, 5, 1.0f / 60.0f, true));
	animator->AddClip("noe walk clip", &AnimationClip(GetTexture(&ResourceManager, "noe walk"), 3, 5, 1.0f / 60.0f, true));
	animator->AddClip("noe basicattack clip", &AnimationClip(GetTexture(&ResourceManager, "noe basicattack"), 3, 5, 1.0f / 60.0f, false));
	animator->AddClip("noe summon clip", &AnimationClip(GetTexture(&ResourceManager, "noe summon"), 4, 5, 1.0f / 60.0f, false));

	Animator *animatorWarriorEnemy = (Animator *)enemyw1->GetComponent("animatorWarriorEnemy", "ANIMATOR");

	animatorWarriorEnemy->AddClip("warrior idle clip", &AnimationClip(GetTexture(&ResourceManager, "warrior idle"), 2, 5, 1.0f / 60.0f, true));
	animatorWarriorEnemy->AddClip("warrior walk clip", &AnimationClip(GetTexture(&ResourceManager, "warrior walk"), 3, 5, 1.0f / 60.0f, true));
	animatorWarriorEnemy->AddClip("warrior basicattack clip", &AnimationClip(GetTexture(&ResourceManager, "wAttack"), 4, 5, 1.0f / 60.0f, false));

	Animator *animatorWizardEnemy = (Animator *)enemywizard1->GetComponent("animatorWizardEnemy", "ANIMATOR");

	animatorWizardEnemy->AddClip("wizard idle clip", &AnimationClip(GetTexture(&ResourceManager, "wizard idle"), 2, 5, 1.0f / 60.0f, true));
	animatorWizardEnemy->AddClip("wizard walk clip", &AnimationClip(GetTexture(&ResourceManager, "wizard walk"), 3, 5, 1.0f / 60.0f, true));
	animatorWizardEnemy->AddClip("wizard basicattack clip", &AnimationClip(GetTexture(&ResourceManager, "wizard attack"), 3, 5, 1.0f / 60.0f, false));

	Animator *animatorRufusEnemy = (Animator *)enemyRufus->GetComponent("animatorRufusEnemy", "ANIMATOR");

	animatorRufusEnemy->AddClip("Rufus idle clip", &AnimationClip(GetTexture(&ResourceManager, "rufus idle"), 2, 5, 1.0f / 60.0f, true));
	animatorRufusEnemy->AddClip("Rufus walk clip", &AnimationClip(GetTexture(&ResourceManager, "rufus walk"), 3, 5, 1.0f / 60.0f, true));
	animatorRufusEnemy->AddClip("Rufus basicaAttack clip", &AnimationClip(GetTexture(&ResourceManager, "rufus attack basic"), 3, 5, 1.0f / 60.0f, false));

	animatorRufusEnemy->AddClip("Rufus AdvanceAttack clip", &AnimationClip(GetTexture(&ResourceManager, "rufus attack advance"), 4, 5, 1.0f / 60.0f, false));


	//	animator->SetStartClip("walk");

	PauseScreen ps = PauseScreen(Scene, &ResourceManager, "State 3");
	MainMenu m = MainMenu(Scene, &ResourceManager, "State 2");
	m.InitMainMenu();
	ps.InitPauseGame();

	/*Scene->EnableSubRoot("State 1");

	GameObject *obj1 = Scene->AddObject("obj1");
	GameObject *obj2 = Scene->AddObject("obj2");

	obj1->AddComponent(&SpriteRenderer("r1", Sprite{ vec3f(), vec3f(50, 50), Material{ GetTexture(&ResourceManager, "Empty"), vec4f(1, 1, 1, 1) } }));

	obj2->AddComponent(&SpriteRenderer("r1", Sprite{ vec3f(70, 70), vec3f(50, 50), Material{ GetTexture(&ResourceManager, "Empty"), vec4f(1, 1, 1, 1) } }));

	*/
	
	Scene->EnableSubRoot("State 2");
	/*
	GameObject *o1 = Scene->AddObject("o1");
	GameObject *o2 = Scene->AddObject("test1");
	o1->AddChild(o2);
	o1->AddComponent(&ShowHide("s1", "test1", 'S'));
	o2->AddComponent(&SpriteRenderer("r1", Sprite{ vec3f(), vec3f(1366, 786)
		, Material{ GetTexture(&ResourceManager, "back"), vec4f(1, 1, 1, 1) } }));

	GameObject *o3 = Scene->AddObject("o3");
	GameObject *o4 = Scene->AddObject("test2");
	o3->AddChild(o4);
	o3->AddComponent(&ShowHide("s1", "test2", 'A'));
	o4->AddComponent(&SpriteRenderer("r1", Sprite{ vec3f(100, 100), vec3f(80, 100), Material{ GetTexture(&ResourceManager, "Empty"), vec4f(1, 1, 1, 1) } }));

	GameObject *o5 = Scene->AddObject("o5");
	GameObject *o6 = Scene->AddObject("test3");
	o5->AddChild(o6);
	o5->AddComponent(&ShowHide("s1", "test3", 'D'));
	
	o6->AddComponent(&Move("m", 15, 150, 0));
	o6->AddComponent(&Collider("c1", &o6->ObjectTransform.Position, vec2f(80, 120), vec2f(), 1, 0));
	o6->AddComponent(&SpriteRenderer("r1", Sprite{ vec3f(), vec3f(80, 120)
		, Material{ GetTexture(&ResourceManager, "Empty"), vec4f(0.2f, 0.31f, 0.91, 1) } }));

	GameObject *o7 = Scene->AddObject("o7");
	GameObject *o8 = Scene->AddObject("test4");
	GameObject *o9 = Scene->AddObject("test5");
	GameObject *o10 = Scene->AddObject("test6");
	GameObject *o11 = Scene->AddObject("test7");
	GameObject *o12 = Scene->AddObject("test8");

	o7->AddChild(o8);
	o7->AddChild(o9);
	o7->AddChild(o10);
	o7->AddChild(o11);
	o7->AddChild(o12);

	o8->AddComponent(&SpriteRenderer("r1", Sprite{ vec3f(), vec3f(80, 80), Material{ GetTexture(&ResourceManager, "Empty"), vec4f(0.3f, 0.4f, 1, 1) } }));
	o8->AddComponent(&Collider("c1", &o8->ObjectTransform.Position, vec2f(80, 80), vec2f(), 0, 0));
	o8->ObjectTransform.Position = vec3f(-200, -100);

	o9->AddComponent(&SpriteRenderer("r1", Sprite{ vec3f(), vec3f(80, 80), Material{ GetTexture(&ResourceManager, "Empty"), vec4f(0.8f, 0.4f, 0.2f, 1) } }));
	o9->AddComponent(&Collider("c1", &o9->ObjectTransform.Position, vec2f(80, 80), vec2f(), 0, 0));
	o9->ObjectTransform.Position = vec3f(200, 100);

	o10->AddComponent(&SpriteRenderer("r1", Sprite{ vec3f(), vec3f(80, 80), Material{ GetTexture(&ResourceManager, "Empty"), vec4f(1, 0.4f, 0, 1) } }));
	o10->AddComponent(&Collider("c1", &o10->ObjectTransform.Position, vec2f(80, 80), vec2f(), 0, 0));
	o10->ObjectTransform.Position = vec3f(300);

	o11->AddComponent(&SpriteRenderer("r1", Sprite{ vec3f(), vec3f(80, 80), Material{ GetTexture(&ResourceManager, "Empty"), vec4f(1, 0.4f, 1, 1) } }));
	o11->AddComponent(&Collider("c1", &o11->ObjectTransform.Position, vec2f(80, 80), vec2f(), 0, 0));
	o11->ObjectTransform.Position = vec3f(0, -100);

	o12->AddComponent(&SpriteRenderer("r1", Sprite{ vec3f(), vec3f(80, 80), Material{ GetTexture(&ResourceManager, "Empty"), vec4f(0.6f, 0, 1, 1) } }));
	o12->AddComponent(&Collider("c1", &o12->ObjectTransform.Position, vec2f(80, 80), vec2f(), 0, 0));
	o12->ObjectTransform.Position = vec3f(0, -200);

	o7->AddComponent(&ShowHide("s1", "test4", 'W'));
	o7->AddComponent(&ShowHide("s2", "test5", 'W'));
	o7->AddComponent(&ShowHide("s3", "test6", 'W'));
	o7->AddComponent(&ShowHide("s4", "test7", 'W'));
	o7->AddComponent(&ShowHide("s5", "test8", 'W'));

	GameObject *o13 = Scene->AddObject("o13");
	o13->AddComponent(&SpriteRenderer("r1", Sprite{ vec3f(), vec3f(80, 80), Material{ GetTexture(&ResourceManager, "Empty"), vec4f(0.6f, 1, 1, 1) } }));
	o12->AddChild(o13);
	o12->AddComponent(&Move("m", 15, 150, 0));
	o1->Enabled = true;
	o2->Enabled = false;

	o3->Enabled = true;
	o4->Enabled = false;

	o5->Enabled = true;
	o6->Enabled = false;

	o7->Enabled = true;
	o8->Enabled = false;
	o9->Enabled = false;
	o10->Enabled = false;
	o11->Enabled = false;
	o12->Enabled = false;
	*/
}
