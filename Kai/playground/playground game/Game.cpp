#include "Game.h"
global_variable Shader shader;

global_variable Camera Cam;

global_variable Mesh Player;
global_variable Mesh Ground;
global_variable Mesh Wall;
global_variable Mesh brick;
global_variable Mesh ball;





// intilize player 
int num = 0.1;
vec2 playervelocity = vec2(15, 15);
vec2 ballVelocity = vec2(3, 3);
vec2  playersize;
vec2 bricksize;
vec3 playerpos;
vec3 brickpos;
vec3 groundpos;
vec2 groundsize;
vec3 ballpos;
vec2 ballsize;
float ballRaduis = 25;

// handle movement variables 

vec2 MovableVelocity=vec2 (0,0);
vec2 goalvelocity;

float jumpSpeed = 150, inair , maxinair=.3;
bool Jump= false , groundCheck=false ;

int flag = 0;
//Initialize the game
GAME_DLL GAME_INIT(Game_Init)
{
	InitShaders();

	CalculateOrthoProjectionMatrix(&Cam, dimensions.Width, dimensions.Height, -0.1f, 500.0f);
	//CalculatePerspectiveProjection(&Cam, 80.0f, dimensions.Width, dimensions.Height, -0.1f, 500.0f);
	CalculateLookAtViewMatrix(&Cam, vec3(0, 0, -1), vec3(0, 1, 0));
	brickpos = vec3(0, -150, 0);
	bricksize = vec2(100, 100);

	brick = CreateSprite(bricksize, brickpos, &SetTexture("resources\\textures\\empty.png"), &Color(0, 0.5f, .3, 1), 1);
	
	brick.collider.pos = brickpos;
	brick.collider.size = bricksize;
	brick.collider.detected = false;


	playersize = vec2(100, 100);
	playerpos = vec3(0, 0, 0);
	Player = CreateSprite(playersize, playerpos, &SetTexture("resources\\textures\\empty.png"), &Color(0, .3, 1, 1), 1);

	Player.collider.pos = playerpos;
	Player.collider.size = playersize;
    Player.collider.detected = false;


	ballpos = vec3(0, 150, 0);
	ballsize = vec2(ballRaduis * 2, ballRaduis * 2);
	ball = CreateSprite(ballsize, ballpos, &SetTexture("resources\\textures\\awesomeface.png"), &Color(1, 1, 1, 1), 1);
	
	ball.collider.pos = ballpos;
	ball.collider.size = ballsize;
	ball.collider.detected = false;


	Ground = CreateSprite(vec2(dimensions.Width, 100), vec3(-dimensions.Width / 2.0f, -dimensions.Height / 2.0f, 0), &SetTexture("resources\\textures\\empty.png"), &Color(0, 1, 1, 1), 1);
	groundpos = vec3(-dimensions.Width / 2.0f, -dimensions.Height / 2.0f, 0);
	groundsize = vec2(dimensions.Width, 100);
	Ground.collider.pos = groundpos;
	Ground.collider.size = groundsize;
	Ground.collider.detected = false;

	Wall = CreateSprite(vec2(dimensions.Width, dimensions.Height), vec3(-dimensions.Width / 2.0f, -dimensions.Height / 2.0f, 0), &SetTexture("resources\\textures\\empty.png"), &Color(0.2f, 0.2f, 0.2f, 1), 1);

	
	
}


//Render the game
GAME_DLL GAME_RENDER(Game_Render)
{
	CalculateModelMatrix(&Player.GlobalTransform);
	CalculateModelMatrix(&ball.GlobalTransform);
	CalculateModelMatrix(&brick.GlobalTransform);

	//Activate the shader
	shader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shader.GetProgram(), "modelMatrix"), 1, false, CalculateMVP(&Wall.GlobalTransform, &Cam).GetElemets());
	DrawMesh(&Wall);
	glUniformMatrix4fv(glGetUniformLocation(shader.GetProgram(), "modelMatrix"), 1, false, CalculateMVP(&Ground.GlobalTransform, &Cam).GetElemets());
	DrawMesh(&Ground);
	
	glUniformMatrix4fv(glGetUniformLocation(shader.GetProgram(), "modelMatrix"), 1, false, CalculateMVP(&ball.GlobalTransform, &Cam).GetElemets());
    DrawMesh(&ball);

	glUniformMatrix4fv(glGetUniformLocation(shader.GetProgram(), "modelMatrix"), 1, false, CalculateMVP(&brick.GlobalTransform, &Cam).GetElemets());
	DrawMesh(&brick);

	glUniformMatrix4fv(glGetUniformLocation(shader.GetProgram(), "modelMatrix"), 1, false, CalculateMVP(&Player.GlobalTransform, &Cam).GetElemets());
	DrawMesh(&Player);

}

Transform testTrans;





void jumpHandle(Game_Input *input, float delta)
{

	if (input->space.KeyDown&&groundCheck)
	{   
		
		Player.GlobalTransform.Position.y += jumpSpeed;
		playerpos.y +=jumpSpeed;
		Player.collider.pos = playerpos;

		Jump = true;
		groundCheck = false;


	}
	if (Jump&&inair<maxinair){

		inair += delta/1.35;

		
	}
	else  if (Jump && !groundCheck && maxinair<=inair){
		
		Player.GlobalTransform.Position.y -= jumpSpeed;
		playerpos.y -= jumpSpeed;
		Player.collider.pos = playerpos;
		inair = maxinair;
		Jump = false;

	}
}

//Update the game
GAME_DLL GAME_UPDATE(Game_Update)
{
        	groundCheck = false;
	        Move(input, dimensions,delta);
			AABBvsAABB(&Player,&brick);

			checkBoxCircleCollision(&ball, &Player);

			//keep it last thing in update !!
			AABBvsAABB(&Player, &Ground);
			jumpHandle(input, delta);

		
}
float Approach(float flGoal, float flCurrent, float delta ){
	
	//delta = .02;
	float flDifference = flGoal - flCurrent;

	if (flDifference > delta)
		return flCurrent + delta * 25;
	if (flDifference < -delta)
		return flCurrent - delta*25;

	return flGoal;


}

void Move(Game_Input *input, Window_Dimensions dimensions, float delta){

	double difference;
	Mesh  movable;
	vec3 MovablePos;
	vec2 MovableSize;
	vec3 backandforth;
	// if released 
	if (input->LEFT.KeyUp){
		MovableVelocity = 0;

	}
	if (input->RIGHT.KeyUp){
		MovableVelocity = 0;

	}
	if (input->UP.KeyUp){
		MovableVelocity = 0;

	}if (input->DOWN.KeyUp){
		MovableVelocity = 0;

	}

	if (input->LEFT.KeyDown || input->_A.KeyDown)

	{

		if (input->LEFT.KeyDown){

			movable = Player;
			MovablePos = playerpos;
			goalvelocity = playervelocity;
			MovableSize = playersize;

			flag = 1;
		}
		else if (input->_A.KeyDown){
			movable = brick;
			MovablePos = brickpos;
			goalvelocity = ballVelocity;
			MovableSize = bricksize;
			flag = 2;

		}

		difference = (-1 * .5*dimensions.Width - MovablePos.x)*-1;

		if (difference < goalvelocity.x)
		{
			goalvelocity.x = difference;
			MovableVelocity.x = Approach(goalvelocity.x, MovableVelocity.x,delta);
			movable.GlobalTransform.Position.x -= MovableVelocity.x;
			MovablePos.x -= MovableVelocity.x;
		}
		else
		{
			goalvelocity = playervelocity;
			MovableVelocity.x = Approach(goalvelocity.x, MovableVelocity.x,delta);

			movable.GlobalTransform.Position.x -= MovableVelocity.x;
			MovablePos.x -= MovableVelocity.x;

		}

		if (flag == 1)
		{
			Player = movable;
			playerpos = MovablePos;
			Player.collider.pos = playerpos;
		}
		else if (flag == 2){
			brick = movable;
			brickpos = MovablePos;
			brick.collider.pos = brickpos;

		}
	}
	
	if (input->RIGHT.KeyDown || input->_D.KeyDown)

	{

		if (input->RIGHT.KeyDown){

			movable = Player;
			MovablePos = playerpos;
			goalvelocity = playervelocity;
			MovableSize = playersize;
			flag = 1;
			
		}
		else if (input->_D.KeyDown){
			movable = brick;
			MovablePos = brickpos;
			MovableVelocity = ballVelocity;
			MovableSize = bricksize;
			flag = 2;
		}
		difference = (MovablePos.x + MovableSize.x + 18 - .5*dimensions.Width)*-1;

		if (difference < goalvelocity.x)
		{
			goalvelocity.x = difference;
			MovableVelocity.x = Approach(goalvelocity.x, MovableVelocity.x, delta);
			movable.GlobalTransform.Position.x += MovableVelocity.x;
			MovablePos.x += MovableVelocity.x;
		}
		else
		{
			goalvelocity.x = playervelocity.x;
			MovableVelocity.x = Approach(goalvelocity.x, MovableVelocity.x, delta);
			movable.GlobalTransform.Position.x += MovableVelocity.x;
			MovablePos.x += MovableVelocity.x;
		}

		if (flag == 1)
		{
			Player = movable;
			playerpos = MovablePos;
			Player.collider.pos = playerpos;

		}
		else if (flag == 2){
			brick = movable;
			brickpos = MovablePos;
			brick.collider.pos = brickpos;

		}

	}

	if (input->UP.KeyDown||input->_W.KeyDown)

	{

		if (input->UP.KeyDown){

			movable = Player;
			MovablePos = playerpos;
			goalvelocity = playervelocity;
			MovableSize = playersize;
			flag = 1;
		}
		else if (input->_W.KeyDown){
			movable = brick;
			MovablePos = brickpos;
			MovableVelocity = ballVelocity;
			MovableSize = bricksize;
			flag = 2;
		}
		difference = (MovablePos.y + MovableSize.y + 25 - .5*dimensions.Height)*-1;

		if (difference <goalvelocity.y)
		{
			goalvelocity.y = difference;
			MovableVelocity.y = Approach(goalvelocity.y, MovableVelocity.y, delta);

			movable.GlobalTransform.Position.y += MovableVelocity.y;
			MovablePos.y += MovableVelocity.y;
		}
		else
		{
			goalvelocity.y = playervelocity.y;
			MovableVelocity.y = Approach(goalvelocity.y, MovableVelocity.y, delta);

			movable.GlobalTransform.Position.y += MovableVelocity.y;
			MovablePos.y += MovableVelocity.y;
		}

		if (flag == 1)
		{
			Player = movable;
			playerpos = MovablePos;
			Player.collider.pos = playerpos;

		}
		else if (flag == 2){
			brick = movable;
			brickpos = MovablePos;
			brick.collider.pos = brickpos;


		}


	}

	if (input->DOWN.KeyDown||input->_S.KeyDown)

	{
		if (input->DOWN.KeyDown){

			movable = Player;
			MovablePos = playerpos;
			goalvelocity = playervelocity;
			MovableSize = playersize;
			flag = 1;
		}
		else if (input->_S.KeyDown){
			movable = brick;
			MovablePos = brickpos;
			MovableVelocity = ballVelocity;
			MovableSize = bricksize;
			flag = 2;
		}
		difference = (-1 * .5*dimensions.Height - MovablePos.y)*-1;

		//check if collide with ground so cant move down 
		/*if (checkBoxescollision(&movable,&Ground)){
			movable.GlobalTransform.Position.y -= 0;
			MovablePos.y -= 0;
		}*/
		
		  if (difference < goalvelocity.y)
		{
			goalvelocity.y = difference;
			MovableVelocity.y = Approach(goalvelocity.y, MovableVelocity.y, delta);
			movable.GlobalTransform.Position.y -= MovableVelocity.y;
			MovablePos.y -= MovableVelocity.y ;
		}
		else
		{
			goalvelocity.y = playervelocity.y;
			MovableVelocity.y = Approach(goalvelocity.y, MovableVelocity.y, delta);
			movable.GlobalTransform.Position.y -= MovableVelocity.y;
			MovablePos.y -= MovableVelocity.y;
		}

		if (flag == 1)
		{
			Player = movable;
			playerpos = MovablePos;
			Player.collider.pos = playerpos;
		}
		else if (flag == 2){
			brick = movable;
			brickpos = MovablePos;
			brick.collider.pos = brickpos;


		}
	}
	std::cout << " velocity X" << MovableVelocity.x << " velocity Y"<<MovableVelocity.y<<std::endl;


}



bool AABBvsAABB(Mesh *A, Mesh *B)
{
	

	// Vector from A to B
    vec3 n = B->collider.pos - A->collider.pos;
	//std::cout << "A-pos     " << A->pos.x <<" "<< A->pos.y << "    B-pos  " << B->pos.x <<	B->pos.y << std::endl;

	//std::cout << "length.x  " << n.x << std::endl;

		// Calculate half extents along x axis for each object
	float a_extent = A->collider.pos.x+ (A->collider.size.x) / 2;
	float b_extent = B->collider.pos.x + B->collider.size.x / 2;
	vec2 diffrence;
	diffrence.x	=  (b_extent - a_extent);
	
		// Calculate overlap on x axis
	float x_overlap = ((A->collider.size.x) / 2 + (B->collider.size.x / 2)) - abs (diffrence.x);
		//std::cout << "x-overlap" <<x_overlap<< std::endl;
		// SAT test on x axis

		 if (x_overlap >=0)
		 {

			 // Calculate half extents along x axis for each object
			 float a_extent = A->collider.pos.y + (A->collider.size.y / 2);
			 float b_extent = B->collider.pos.y+ (B->collider.size.y / 2);
			 diffrence.y = (b_extent - a_extent);

			 // Calculate overlap on y axis
			 float y_overlap = (A->collider.size.y / 2 + (B->collider.size.y / 2)) - abs (diffrence.y);
			//std::cout << "y-overlap" << y_overlap << std::endl;

			 // SAT test on y axis
			 if (y_overlap >=0)
			 {
				 // so collisoion exist 
				 if (B == &Ground || A == &Ground){
					 groundCheck = true;
					 inair = 0;
				 }
				 
				 // Find out which axis is axis of least penetration
				 if (x_overlap < y_overlap)
				 {

					 // Point towards B knowing that n points from A to B
					 if (diffrence.x<0)
						 // left A - RIgth B
					 {
						 A->collider.normal = vec2(-1, 0);
						 B->collider.normal = vec2(1, 0);


						 A->GlobalTransform.Position.x += x_overlap;
						 playerpos.x += x_overlap;
						 A->collider.pos.x += x_overlap;

						
					 }
					 else 
						
					 {
						 //rigth A - LEFT B
						 A->collider.normal = vec2(1, 0);
						 B->collider.normal = vec2(-1, 0);
						 A->GlobalTransform.Position.x -= x_overlap;
						 playerpos.x -= x_overlap;
						 A->collider.pos.x -= x_overlap;
						// std::cout << "Rigth " << std::endl;
						/* B->GlobalTransform.Position.x += x_overlap;
						 brickpos.x += x_overlap;
						 B->collider.pos.x += x_overlap;*/
					 }

					A->collider.penetration = x_overlap;
					B->collider.penetration = x_overlap;
                    
					A->collider.detected = true;
					B->collider.detected = true;

				//	brick = CreateSprite(bricksize, brickpos, &SetTexture("resources\\textures\\empty.png"), &Color(1, 0.5f, .3, 1), 1);

					std::cout << "player collider detected  :     " << A->collider.detected << "depth" << A->collider.penetration<< std::endl;
					if (A->collider.normal == vec2(-1, 0)) std::cout << " player face collided   : left " << std::endl;
					else std::cout << " player face collided   : rigth " << std::endl;
					std::cout << "Brick collider detected  :     " << B->collider.detected << "depth" << B->collider.penetration << std::endl;
					if (B->collider.normal == vec2(-1, 0)) std::cout << " brick face collided   : left " << std::endl;
					else std::cout << " brick face collided   : rigth " << std::endl;
					 return true;

				 }

				 else 
				 {
					 // Point toward B knowing that n points from A to B
					 if (diffrence.y<0)
					 { 
						 
						 //down A - UP B 
						 
					A->GlobalTransform.Position.y += y_overlap;
					playerpos.y += y_overlap;
				    A->collider.pos.y += y_overlap;
						 
						 A->collider.normal = vec2(0, -1);
						 B->collider.normal = vec2(0, 1);
						
						// std::cout << "Down " << std::endl;

					 }
					 else 
						 //up A - DOWN B 
					 {
						 A->collider.normal = vec2(0, 1);
						 B->collider.normal = vec2(0, -1);
						 A->GlobalTransform.Position.y -= y_overlap;
						 playerpos.y -= y_overlap;
						 A->collider.pos.y -= y_overlap;
						// std::cout << "Up " << std::endl;

					 }
					 A->collider.penetration = y_overlap;
					 B->collider.penetration = y_overlap;
					

					 A->collider.detected = true;
					 B->collider.detected = true;
				//	 brick = CreateSprite(bricksize, brickpos, &SetTexture("resources\\textures\\empty.png"), &Color(1, 0.5f, .3, 1), 1);

				//	 std::cout << "true " << std::endl;
					 std::cout << "player collider detected  :    " << A->collider.detected << "Depth" << A->collider.penetration << std::endl;
					 if (A->collider.normal == vec2(0, -1)) std::cout << " player face collided   : Down " << std::endl;
					 else std::cout << "   player face collided   : UP " << std::endl;
					 std::cout << "Brick collider detected  :    " << B->collider.detected << "depth   " << B->collider.penetration << std::endl;
					 if (B->collider.normal == vec2(0, -1)) std::cout << " brick face collided   : Down " << std::endl;
					 else std::cout << "    brick face collided   : Up "<<std::endl ;

					 return true;


				 }

			 }
			 
			 
		 }
		// brick = CreateSprite(bricksize, brickpos, &SetTexture("resources\\textures\\empty.png"), &Color(0, 0.5f, .3, 1), 1);

		 A->collider.detected = false;
		 B->collider.detected = false;

		 /*std::cout << "player collider detected  :    " << A->collider.detected << "Depth" << A->collider.penetration << std::endl;
		 if (A->collider.normal == vec2(0, 0)) std::cout << "     player face collided   : Nothing " << std::endl;;
		
		 std::cout << "Brick collider detected  :    " << B->collider.detected << "depth" << B->collider.penetration << std::endl;
		 if (B->collider.normal == vec2(0, 0)) std::cout << "    brick face collided   : Nothing " << std ::endl;;*/
		
		 return false;
		
}



bool checkBoxCircleCollision(Mesh *circle, Mesh *box){
	

	// Get center point circle first 
	vec3 center(circle->collider.pos.x + circle->collider.size.x / 2, circle->collider.pos.y + circle->collider.size.y / 2, 0);
	// Calculate AABB info (center, half-extents)
	vec3 aabb_half_extents(box->collider.size.x / 2, box->collider.size.y / 2);
	vec3 aabb_center(
		box->collider.pos.x + aabb_half_extents.x,
		box->collider.pos.y + aabb_half_extents.y
		);

	vec3 difference = aabb_center - center;
	//std::cout << "centers diff    " << difference.x << "  " << difference.y << std::endl;

	vec3 clamped = clamp(difference, aabb_half_extents*-1, aabb_half_extents);
	// Add clamped value to AABB_center and we get the value of box closest to circle
	vec3 closest = aabb_center - clamped;
	//std::cout << "closest   " << closest.x << "  "<<closest.y  << std::endl;
	// Retrieve vector between center circle and closest point AABB and check if length < radius
	difference = closest - center;
	float pentration;
	//std::cout << "difference  " << difference.x << "      "<<difference.y<< std:: endl;
	
	//if(abs(difference.x) < circlesize.x/2&& abs(difference.y) < circlesize.y/2)
		if (difference.Length() < circle->collider.size.x/2)
	{
			// check the min pentartaion in which axis x or y 
			if ((circle->collider.size.x / 2 - abs(difference.x))<(circle->collider.size.y / 2 - abs(difference.y)))
		{

			if (difference.x < 0)
			{
				// circle left box rigth 
				pentration = circle->collider.size.x / 2 - abs(difference.x);

                   circle->collider.normal = vec2(-1, 0);
				   box->collider.normal = vec2(1, 0);

				   box->GlobalTransform.Position.x -= pentration;
					playerpos.x -= pentration;
					Player.collider.pos.x -= pentration;


					std::cout << "true ball" << std::endl;
					std::cout << "left" << std::endl;
					return true;
			}
			
			else
			{
				// circle rigth box left 
				pentration = circle->collider.size.x / 2 - abs(difference.x);

				circle->collider.normal = vec2(1, 0);
				box->collider.normal = vec2(-1, 0);
				Player.GlobalTransform.Position.x += pentration;
				playerpos.x += pentration;
				Player.collider.pos.x += pentration;

				std::cout << "true ball" << std::endl;
				std::cout << "rigth" << std::endl;
				return true;

			}
			circle->collider.detected = true;
			box->collider.detected = true;

		}
			// y ovelap > x - overlap 
		else
		{
		if (difference.y < 0)
		{
			// circle down box up 
			pentration = circle->collider.size.y / 2 - abs(difference.y);

			circle->collider.normal = vec2(0, -1);
			box->collider.normal = vec2(0, 1);
			Player.GlobalTransform.Position.y -= pentration;
			playerpos.y -= pentration;
			Player.collider.pos.y -= pentration;

			std::cout << "true ball" << std::endl;
			std::cout << "down" << std::endl;
			return true;
		}

		else {
			// circle up box down 
			pentration = circle->collider.size.y / 2 - abs(difference.y);

			circle->collider.normal = vec2(0, 1);
			box->collider.normal = vec2(0, -1);
			Player.GlobalTransform.Position.y+= pentration;
			playerpos.y += pentration;
			Player.collider.pos.y  += pentration;

			std::cout << "true ball " << std::endl;
			std::cout << "up" << std::endl;
			return true;
		}
		circle->collider.detected = true;
		box->collider.detected = true;

	}

}
	
	
	else {
		circle->collider.detected = false;
		box->collider.detected = false;
		std::cout << "false ball" << std::endl;
		return false;
	}
}

























































void computetime(clock_t start, clock_t end){


	double elapsedtime = ((double)(end - start) / (double)(CLOCKS_PER_SEC)) * 1000.0f;

	std::cout << "game render time is : " << elapsedtime << " ms" << std::endl;

}

void InitShaders()
{
#if GLSL_VERSION == ANCIENT_VERSION
	shader = Shader("resources\\shaders\\vertex shader 120.vert", "resources\\shaders\\fragment shader 120.frag");
#elif GLSL_VERSION == MODERN_VERSION	//Use modern shaders with modern GLSL
	shader = Shader("resources\\shaders\\vertex shader.vert", "resources\\shaders\\fragment shader.frag");
#endif
}

// max & min are macros in windows.h
vec3 clamp(vec3 value, vec3 minimum, vec3 maximum){
	maximum.x = max(minimum.x, min(maximum.x, value.x));
	maximum.y = max(minimum.y, min(maximum.y, value.y));
	return maximum;

}

//*******************************************************<<< prevoius trials  >>>*********************************************************************

//bool AABBvsAABB(Mesh *one, Mesh *two){
//
//	vec3 onepos, twopos;
//	vec2 onesize, twosize;
//
//	vec3 length, difference;
//	if (one == &Player){
//		onepos = playerpos;
//		onesize = playersize;
//	}
//
//	if (two == &brick){
//		twopos = brickpos;
//		twosize = bricksize;
//	}
//
//
//	// Collision x-axis?
//	bool collisionX = onepos.x + onesize.x >= twopos.x &&
//		twopos.x + twosize.x >= onepos.x;
//
//	// Collision y-axis?
//	bool collisionY = onepos.y + onesize.y >= twopos.y &&
//		twopos.y + twosize.y >= onepos.y;
//
//	vec3 one_half_extents(onesize.x / 2, onesize.y / 2);
//	vec3 one_center(onepos.x + one_half_extents.x, onepos.y + one_half_extents.y);
//	vec3 two_half_extents(twosize.x / 2, twosize.y / 2);
//	vec3 two_center(twopos.x + two_half_extents.x, twopos.y + two_half_extents.y);
//
//	if (collisionX&&collisionY)
//	{
//		length.x = abs(two_center.x - one_center.x);
//		length.y = abs(two_center.y - one_center.y);
//		diff_box_box.x = difference.x = one_half_extents.x + two_half_extents.x - length.x;
//		diff_box_box.y = difference.y = one_half_extents.y + two_half_extents.y - length.y;
//
//		diff_x_left = onepos.x + onesize.x - twopos.x;
//		diff_y_down = (onepos.y + onesize.y - twopos.y);
//
//		diff_x_rigth = onesize.x + twosize.x - (twopos.x + twosize.x - onepos.x);
//		diff_y_up = onesize.y + twosize.y - (twopos.y + twosize.y - onepos.y);
//
//		//	std::cout << "diff x is " << difference.x << std::endl;
//		//	std::cout << "diff y is " << difference.y << std::endl;
//		compass[0] = compass[1] = compass[2] = compass[3] = 0;
//		double leftvalue = 0, rightvalue = 0, upvalue = 0, downvalue = 0;
//
//		if (onepos.x < twopos.x) {
//			//std::cout << "left" << std::endl;
//
//			leftvalue = twopos.x - onepos.x;
//			compass[3] = true;
//
//		}
//
//		if (onepos.y < twopos.y) {
//			downvalue = twopos.y - onepos.y;
//
//			//std::cout << "down" << std::endl;
//			compass[2] = true;
//		}
//
//		if (onepos.y > twopos.y){
//			upvalue = onepos.y - twopos.y;
//			//std::cout << "up" << std::endl;
//			compass[0] = true;
//
//		}
//
//		if (onepos.x > twopos.x)
//		{
//			rightvalue = onepos.x - twopos.x;
//			//	std::cout << "right" << std::endl;
//			compass[1] = true;
//		}
//		GLfloat maximum = 0, index = -1;
//		double all_directions[] = { leftvalue, rightvalue, upvalue, downvalue };
//
//		for (int i = 0; i < 4; i++){
//			if (all_directions[i]>maximum){
//				maximum = all_directions[i];
//				index = i;
//			}
//		}
//			if (index == 0)
//			{
//				one->GlobalTransform.Position.x -= diff_box_box.x / 100;
//				onepos.x -= diff_box_box.x / 100;
//
//				//std::cout << "left value is :: " <<leftvalue<< std::endl;
//				std::cout << "left" << std::endl;
//			}
//			if (index == 1)			std::cout << "rigth" << std::endl;
//			if (index == 2)			std::cout << "up" << std::endl;
//			if (index == 3)			std::cout << "down" << std::endl;
//			playerpos = onepos;
//			brickpos = twopos;
//
//			brick = CreateSprite(vec2(100, 100), vec3(0, -150, 0), &SetTexture("resources\\textures\\empty.png"), &Color(1, 0.5f, .3, 1), 1);
//			return true;
//		}
//		else {
//			brick = CreateSprite(vec2(100, 100), vec3(0, -150, 0), &SetTexture("resources\\textures\\empty.png"), &Color(0, 0.5f, .3, 1), 1);
//			return false;
//		}
//
//	}
//
//bool   checkBoxescollision(Mesh *one, Mesh *two){
//
//	vec3 onepos, twopos;
//	vec2 onesize, twosize;
//	
//	if (one == &Player){
//		onepos = playerpos;
//		onesize = playersize;
//	}
//	if (one == &ball){
//		onepos = ballpos;
//		onesize = ballsize;
//	}
//	
//	if (two == &brick){
//		twopos = brickpos;
//		twosize = bricksize;
//	}
//
//	else if (two == &Ground){
//
//		twopos = groundpos;
//		twosize = groundsize;
//
//	}
//	// Collision x-axis?
//	bool collisionX = onepos.x + onesize.x >= twopos.x &&
//		twopos.x + twosize.x >= onepos.x;
//	if (collisionX)
//	{
//		diff_x_left = onesize.x+twosize.x- (twopos.x + twosize.x - onepos.x);
//		diff_x_rigth = onepos.x + onesize.x - twopos.x;
//	}
//	// Collision y-axis?
//	bool collisionY = onepos.y + onesize.y >= twopos.y &&
//		twopos.y + twosize.y >= onepos.y;
//
//	if (collisionY)
//	{
//		diff_y_down = onesize.y + twosize.y - (twopos.y + twosize.y - onepos.y);
//        diff_y_up = (onepos.y + onesize.y - twopos.y);
//
//	}
//	// Collision only if on both axes
//	if (collisionX && collisionY)  {
//		float leftvalue=0 , rightvalue=0 , upvalue=0,downvalue=0 ;
//		compass[0] = compass[1] = compass[2] = compass[3] = 0;
//		if (onepos.x < twopos.x) {
//			
//			leftvalue = twopos.x - onepos.x;
//			std::cout << "left" << std::endl;
//			compass[3] = true;
//			
//		}
//
//		if (onepos.y < twopos.y) {
//			
//			downvalue = twopos.y - onepos.y;
//			std::cout << "down" << std::endl;
//			compass[2] = true;
//		}
//		/*if (leftvalue>downvalue) {
//			std::cout << "left" << std::endl;
//			compass[3] = true;
//
//
//		}
//		else if (leftvalue<downvalue){
//			std::cout << "down" << std::endl;
//			compass[2] = true;
//
//		}
//		else if (leftvalue==downvalue>0){
//			std::cout << "left" << std::endl;
//			compass[3] = true;
//			std::cout << "down" << std::endl;
//			compass[2] = true;
//		}*/
//		if (onepos.y > twopos.y){
//			std::cout << "up" << std::endl;
//			compass[0] = true;
//
//		}
//
//		if (onepos.x > twopos.x) {
//
//			std::cout << "right" << std::endl;
//			compass[1] = true;
//		}
//
//		if (two == &brick)
//			brick = CreateSprite(vec2(100, 100), vec3(0, -150, 0), &SetTexture("resources\\textures\\empty.png"), &Color(1, 0.5f, .3, 1), 1);
//		if (two == &Ground) num = 0;
//		return collision = true;
//
//	}
//
//
//	else {
//		if (two == &brick)
//			brick = CreateSprite(vec2(100,100), vec3(0, -150, 0), &SetTexture("resources\\textures\\empty.png"), &Color(0, 0.5f, .3, 1), 1);
//		
//		return collision = false;
//
//	
//	}
//
//	
//}

//void DoBoxCircleCollision(){
//
//	Direction dir = VectorDirection(diff_circle_box);
//	
//
//		if (dir== LEFT){
//			std::cout << "left" << std::endl;
//		}
//		if (dir == RIGHT){
//
//			std::cout << "right" << std::endl;
//
//		}
//		if (dir == UP){
//			std::cout << "up" << std::endl;
//
//		}
//
//		if (dir == DOWN){
//			std::cout << "down" << std::endl;
//
//		}
//	}
//
//void DoAABB_AABB(){
//	if (AABBvsAABB(&Player,&brick)){
//		Direction_1 dir_1 = VectorDirection_1(vec3(diff_x_rigth, diff_y_up));
//		Direction_2 dir_2 = VectorDirection_2(vec3(diff_x_left, diff_y_down));
//
//		if (max > max_2)
//		{
//			if (dir_1 == RIGHT)		 std::cout << "direction is R " << std::endl;
//			else  if (dir_1 == UP)		 std::cout << "direction is UP " << std::endl;
//		}
//		else {
//			if (dir_2 == LEFT)		 std::cout << "direction is L " << std::endl;
//			else  if (dir_2 == DOWN)		 std::cout << "direction is Down " << std::endl;
//
//		}
//
//		//reset max 
//		max =max_2= 0;
//
//	}
//
//
//}
//void DoBoxesCollision(){
//	// 1- define the 6 possible direction 
//	if (checkBoxescollision(&Player, &brick))
//	
//	{
//		//UP only 
//		if (compass[0] && !compass[1] && !compass[2] && !compass[3]){
//
//
//			Player.GlobalTransform.Position.y += diff_y_up;
//			playerpos.y += diff_y_up;
//
//
//		}
//		//down only 
//		else if (!compass[0] && !compass[1] && compass[2] && !compass[3]){
//			Player.GlobalTransform.Position.y -=diff_y_down;
//			playerpos.y -= diff_y_down;
//
//
//		}
//		//right only 
//		else if (!compass[0] && compass[1] && !compass[2] && !compass[3]){
//
//			Player.GlobalTransform.Position.x += diff_x_rigth;
//			playerpos.x += diff_x_rigth;
//
//		}
//		//left only 
//		else if (!compass[0] && !compass[1] && !compass[2] && compass[3]){
//
//		Player.GlobalTransform.Position.x -= diff_x_left;
//			playerpos.x -= diff_x_left;
//
//		}
//		//UP-LEFT 
//		else if (compass[0] && !compass[1] && !compass[2] && compass[3]){
//
//			//Player.GlobalTransform.Position.x -= difference.x;
//			//Player.GlobalTransform.Position.y += difference.y;
//			////playerpos.x -= difference.x;
//			//playerpos.y += difference.y;
//
//		}
//		//DOWN-LEFT
//
//		else if (!compass[0] && !compass[1] && compass[2] && compass[3]){
//
//
//
//		}
//		//UP-RIGHT
//		else if (compass[0] && compass[1] && !compass[2] && !compass[3]){
//
//
//
//		}
//		//DOWN-RIGTH
//		else if (!compass[0] && compass[1] && compass[2] && !compass[3]){
//
//
//
//		}
//
//
//
//	}
//}
//double dotProduct(vec3 a, vec3 b) {
//	double runningSum;
//	runningSum = a.x *b.x + a.y*b.y + a.z *b.z;
//
//	return runningSum;
//}
//// Calculates which direction a vector is facing (N,E,S or W)
//Direction VectorDirection(vec3 target)
//{
//	vec3 compass[] = {
//		vec3(0.0f, 1.0f),	// up
//		vec3(1.0f, 0.0f),	// right
//		vec3(0.0f, -1.0f),	// down
//		vec3(-1.0f, 0.0f)	// left
//	};
//	GLfloat max = 0.0f;
//	GLuint best_match = -1;
//	for (GLuint i = 0; i < 4; i++)
//	{
//		GLfloat dot_product = abs(dotProduct((target.Normalize()), compass[i]));
//		if (dot_product > max)
//		{
//			max = dot_product;
//			best_match = i;
//		}
//	}
//	return (Direction)best_match;
//}
//
//// Calculates which direction a vector is facing (N,E,S or W)
//Direction_1 VectorDirection_1(vec3 target)
//{
//	vec3 compass[] = {
//		vec3(0.0f, 1.0f),	// up
//		vec3(1.0f, 0.0f), };	// right
//	
//
//	GLuint best_match = -1;
//	for (GLuint i = 0; i < 2; i++)
//	{
//		GLfloat dot_product = (dotProduct((target.Normalize()), compass[i]));
//		if (dot_product > max)
//		{
//			max = dot_product;
//			best_match = i;
//		}
//	}
//	
//	
//	return    Direction_1( best_match);
//	
//
//}
//
//Direction_2 VectorDirection_2(vec3 target){
//
//	vec3 compass_2[] = {
//		vec3(0.0f, -1.0f),	// down
//		vec3(-1.0f, 0.0f)	// left
//	};
//
//	GLuint best_match_2 = -1;
//
//	for (GLuint i = 0; i < 2; i++)
//	{
//		GLfloat dot_product = (dotProduct((target.Normalize()), compass_2[i]));
//		if (dot_product > max_2)
//		{
//			max_2 = dot_product;
//			best_match_2 = i;
//		}
//	}
//
//	return (Direction_2)best_match_2;
//}

