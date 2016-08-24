#include<Jump.h>


Jump::Jump(char *name, float speed, Game_Resources *resources, char *type)
{
	this->Name = name;
	this->Type = type;
	this->Speed = speed;
	this->Resources = resources;
	this->Size = sizeof(Jump);
}

 void Jump :: Init()
{

	this->jumpflag = false;
	this->fallSpeed = 0.0f;
	this->fallSpeed_jump = 0.0f;
	doubleJumpFlag = false;
	onair = false;
	doublejump = 0;
	waitBeforeDouble_flag = false;
	waitDouble = 121;
	controlWait = 0;


}


 void Jump :: Update(float delta, Game_Input *input)
 {
	 
	 if (this->Scene->GetGameObject("jumboo")!=NULL&&!animator->IsRunning("jumboo monster clip"))
	 {
		 this->Scene->RemoveObject("jumboo");
	 }
	 bool groundCheck = false;

	 bool Jump;

	 controlWait++;
	 Collider *collider = (Collider *)Owner->GetComponent("c1", "COLLIDER");

	 Jump = collider->jump;

	 if (waitBeforeDouble_flag == true && waitDouble > 0)
		 waitDouble--;

	 if (waitDouble == 0)
	 {
		 waitBeforeDouble_flag = false;
		 waitDouble = 121;

	 }
	 if (Jump)
	 {

		 groundCheck = true;
	 }

	 if (groundCheck)

	 {
		 doubleJumpFlag = false;
		 onair = false;
		 fallSpeed_jump = 0;
		 collider->velocity.Y = 0;

	 }
	 Animator *animatorPlayer = (Animator *)Owner->GetComponent("animator", "ANIMATOR");
	 playerCollide *PlayerCollide = (playerCollide *)Owner->GetComponent("checkCollide", "player collide");

	 if (input->Space.KeyDown && groundCheck)
	 {
		 // reset 
		 Speed = 17.0f;
		 onair = true;
		 jumpflag = true;
		 groundCheck = false;
		 controlWait = 0;
	 }

	 else if (input->Space.KeyDown && onair && waitBeforeDouble_flag == false)
	 {
		 if (controlWait >= 17)
		 {
			 doubleJumpFlag = true;
			 onair = false;
			 jumpflag = true;
			 waitBeforeDouble_flag = true;
			 controlWait = 0;
		 }
	 }

	 if (collider->up)
	 {
		 jumpflag = false;
	 }

	 if (jumpflag)
	 {
		 fallSpeed_jump = 0;

		 Owner->ObjectTransform.Position.Y += Speed;
		 //	Game_Scene.CollisionManager.GetCollider(ActorName)->pos->Y += jumpSpeed;
		 collider->velocity.Y = Speed;

		 Speed -= collider->Scene->PEngine.ph.Gravity*delta * 10.0f;

		 if (Speed <= 0){
			 if (doubleJumpFlag == true && onair == false)
			 {
				 Speed = 0;
				 
				 jumbooHide_flag = false;
				 PlayerCollide->playerHp -= 20;
				 jumboo = this->Scene->AddObject("jumboo");
				 jumboo->ObjectTransform.Position = Owner->ObjectTransform.Position;
				 jumboo->ObjectTransform.Position.X -= 50.0f*Owner->ObjectTransform.Scale.X;
				 jumboo->ObjectTransform.Position.Y -= 300.0f;
				 jumboo->ObjectTransform.Scale.X = Owner->ObjectTransform.Scale.X;
				 jumboo->AddComponent(&SpriteRenderer("jumboo_sprite", Sprite{ vec3f(0, 0, 0), vec2f(150*1.5f, 133*1.5f), Material{ GetTexture(Resources, "jumpo"), vec4f(1, 1, 1, 1) } }));
				 jumboo->AddComponent(&Collider("pullBack", &jumboo->ObjectTransform.Position, vec2f(300, 266), vec2f(0, 0), 0, true));
				 jumboo->AddComponent(&Animator("animator", (SpriteRenderer *)jumboo->GetComponent("jumboo_sprite", "RENDERABLE")));
				 //	Game_Scene.ActorManager.GetTransform("straight_Mon")->Position = vec3f(200.0f, -200.0f, 0);

				 animator = (Animator *)jumboo->GetComponent("animator", "ANIMATOR");
				 animator->AddClip("jumboo monster clip", &AnimationClip(GetTexture(Resources, "jumpo"), 2, 5, (1.0f / 60.0f), true));
				 animator->StopWhenDone();

				 Speed += 13.0f;
				 doubleJumpFlag = false;

			 }
			 else {
				 Speed = 0;
				 jumpflag = false;
			 }
		 }
	 }
	 else if (!groundCheck)
	 {

		 fallSpeed_jump += (collider->Scene->PEngine.ph.Gravity*delta*10.0f);

		 Owner->ObjectTransform.Position.Y -= fallSpeed_jump;
		 collider->velocity.Y = -fallSpeed_jump;

	 }
 }