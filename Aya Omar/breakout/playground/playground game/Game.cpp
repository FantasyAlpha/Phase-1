#include "Game.h"
#include<iostream>


global_variable Mesh background_mesh, player_mesh, level_mesh, ball_mesh;
global_variable Shader shader;
global_variable Transform transform_player;
global_variable Transform transform_ball;

global_variable Camera Cam;
global_variable std::vector<Mesh> level_Meshes;
GameObject    player;
Gamelevel     level;
GameObject   ball;
ballobject  ball_;

Texture background_text, player_text, ball_text;

float Count = 0;




//Initialize the game
GAME_DLL GAME_INIT(Game_Init)
{
	//Use older shaders with old GLSL
#if GLSL_VERSION == ANCIENT_VERSION

	shader = Shader("resources\\shaders\\vertex shader 120.vert", "resources\\shaders\\fragment shader 120.frag");
#elif GLSL_VERSION == MODERN_VERSION	//Use modern shaders with modern GLSL
		shader = Shader("resources\\shaders\\vertex shader.vert", "resources\\shaders\\fragment shader.frag");

#endif

	//the mesh i will use
	SetTexture(&background_text, "resources\\textures\\background.png");
	SetTexture(&player_text, "resources\\textures\\paddle.png");
	SetTexture(&ball_text, "resources\\textures\\awesomeface.png");

	background_mesh = CreateSprite(vec2(screenWidth, screenHeight), vec3(.5 *-1 * screenWidth, .5*-1 * screenHeight, 0), &background_text, &Color(1, 1, 1, 1), 1);


	// intilize player game object 
	initGameObject(&player, &player_mesh, playerSize, vec3(0 - (playerSize.x) / 2, (-1 * .5*screenHeight), 0), &Color(1, 1, 1, 1), &player_text);

	//player_mesh.GlobalTransform.Position = vec3(0 - (playerSize.x) / 2, (-1 * .5*screenHeight ), 0);
	player_mesh.GlobalTransform.ModelMatrix = mat4(1);
	createobjectSprite(&player);

	// intilaize ball position on the paddel 
	initGameObject(&ball, &ball_mesh, vec2(ballraduis * 2, ballraduis * 2),
		vec3(player.Position.x + playerSize.x / 2 - ballraduis, player.Position.y + 1.5*ballraduis, 0), &Color(1, 1, 1, 1), &ball_text);
	ball.velocity = ballvelocity;
	ball.ball_obj = &ball_;
	ball.ball_obj->raduis = ballraduis;

	//ball_mesh.GlobalTransform.Position = vec3(player.Position.x + playerSize.x / 2 - ballraduis, player.Position.y + 1.5*ballraduis, 0);
	ball_mesh.GlobalTransform.ModelMatrix = mat4(1);
    createobjectSprite(&ball);

	// load level 
	loadLevel(&level, &level_mesh, "resources\\levels\\one.lvl", screenWidth, screenHeight* 0.5);

	level_Meshes = level.levelMeshes;



	//CalculatePerspectiveProjection(&Cam, 80.0f, screenWidth, screenHeight, -0.1f, 500.0f);
	CalculateOrthoProjectionMatrix(&Cam, screenWidth, screenHeight, -0.1f, 500.0f);


}


//Update the game
GAME_DLL GAME_UPDATE(Game_Update)
{	
	

	vec2 velocity = vec2 (playerVelocity.x,0);
	float handelleftpos = player.Position.x , handlerightPos = player.Position.x , difference;
	if (input->LEFT.KeyDown)
	{
		difference =( -1 * .5*screenWidth - handelleftpos)*-1;

		if (difference < velocity.x)
		{
			player_mesh.GlobalTransform.Position.x -= difference;
			player.Position.x -= difference;

			//	 if stuck move at x with velocity 

			if (ball.ball_obj->stuck == true)
			{
				ball_mesh.GlobalTransform.Position.x -= difference;;

				ball.Position.x -= difference;
			}


		}
			else
			{
				player_mesh.GlobalTransform.Position.x -= velocity.x;
				player.Position.x -= velocity.x;
				//	 if stuck move at x with velocity 

				if (ball.ball_obj->stuck == true)
				{
					ball_mesh.GlobalTransform.Position.x -= velocity.x;

					ball.Position.x -= velocity.x;
				}

			}
		
		}
		
	else if (input->RIGHT.KeyDown)
	{

		difference = (handlerightPos + playerSize.x - .5*screenWidth)*-1;
		if (difference < velocity.x)
		{
			player_mesh.GlobalTransform.Position.x += difference;
			player.Position.x += difference;

			//	 if stuck move at x with velocity 

			if (ball.ball_obj->stuck == true)
			{
				ball_mesh.GlobalTransform.Position.x += difference;;

				ball.Position.x += difference;
			}


		}
		else
		{
			player_mesh.GlobalTransform.Position.x += velocity.x;
			player.Position.x += velocity.x;
			//	 if stuck move at x with velocity 

			if (ball.ball_obj->stuck == true)
			{
				ball_mesh.GlobalTransform.Position.x += velocity.x;

				ball.Position.x += velocity.x;
			}

		}
	}


		// handeling the ball for the  start 

		 if (input->UP.KeyDown)
		{
		if (ball.ball_obj->stuck)
		ball.ball_obj->stuck = false;
		}
		// call moveball
		moveball(&ball, &ball_,& ball_mesh,screenWidth, screenHeight);
		/*if (ball.Position.y < -1 * (.5*screenHeight+ball.Size.y))
		{
			ball.Position = vec3(player.Position.x + playerSize.x / 2 - ballraduis, player.Position.y + 1.5*ballraduis, 0);
			ball.ball_obj->stuck = true;
		}*/
		 DoCollisions();

	}


//Render the game
GAME_DLL GAME_RENDER(Game_Render)
{
	shader.Activate();

	CalculateLookAtViewMatrix(&Cam, vec3(0, 0, -1), vec3(0, 1, 0));

	//Activate the shader
	Transform t1,t2;
	t1.Position = 0;
	CalculateModelMatrix(&t1);
    glUniformMatrix4fv(glGetUniformLocation(shader.GetProgram(), "modelMatrix"), 1, false, CalculateMVP(&t1, &Cam).GetElemets());
	DrawMesh(&background_mesh);

	GLuint flag = 0;

	for (unsigned int i = 0; i < level_Meshes.size(); i++)
	{
		if (!level.Bricks[i].Destroyed)
		{

			DrawMesh(&level_Meshes[i]);
		}
			
		flag++;
	}

	CalculateModelMatrix(&player_mesh.GlobalTransform);

	glUniformMatrix4fv(glGetUniformLocation(shader.GetProgram(), "modelMatrix"), 1, false, CalculateMVP(&player_mesh.GlobalTransform, &Cam).GetElemets());
	DrawMesh(&player_mesh);
	
	CalculateModelMatrix(&ball_mesh.GlobalTransform);

	glUniformMatrix4fv(glGetUniformLocation(shader.GetProgram(), "modelMatrix"), 1, false, CalculateMVP(&ball_mesh.GlobalTransform, &Cam).GetElemets());
	DrawMesh(&ball_mesh);


	

}

//------------------- Math Functions i need---------------------
double getmax(double a , double b){
	if (a > b) return a;
	else return b;
}
double getmin(double a, double b){
	if (a < b) return a;
	else return b;
}
double dotProduct(vec3 a, vec3 b) {
	double runningSum;
	runningSum = a.x *b.x + a.y*b.y + a.z *b.z;
		
	return runningSum;
}

// max & min are macros in windows.h
vec3 clamp(vec3 value, vec3 minimum, vec3 maximum){
	maximum.x = max(minimum.x, min(maximum.x, value.x));
	maximum.y = max(minimum.y, min(maximum.y, value.y));
	return maximum;

}

vec3 normalize3( vec3 v)
{
	float length_of_v = sqrt((v.x * v.x) + (v.y * v.y) + (v.z * v.z));
	return vec3(v.x / length_of_v, v.y / length_of_v, v.z / length_of_v);
}
vec2 normalize2(vec2 v)
{
	float length_of_v = sqrt((v.x * v.x) + (v.y * v.y));
	return vec2(v.x / length_of_v, v.y / length_of_v);
}
//--------------------------------------------------
double lengthVector(vec2 v){
	double length_of_v = sqrt((v.x * v.x) + (v.y * v.y));
	return length_of_v;

}
Collision CheckBoxesCollision(GameObject &one, GameObject &two) // AABB - AABB collision
{
	double diff_x;
	double diff_y;

	// Collision x-axis?
	bool collisionX = one.Position.x + one.Size.x >= two.Position.x &&
		two.Position.x + two.Size.x >= one.Position.x;
	if (collisionX)
	{
		diff_x = one.Position.x + one.Size.x - two.Position.x;

	}
	// Collision y-axis?
	bool collisionY = one.Position.y + one.Size.y >= two.Position.y &&
		two.Position.y + two.Size.y >= one.Position.y;
	if (collisionY)
	{
		diff_y = one.Position.y + one.Size.y - two.Position.y;

	}
	// Collision only if on both axes
	if (collisionX && collisionY){
		return  std::make_tuple(GL_TRUE, VectorDirection(vec3(diff_x, diff_y)), vec3(diff_x, diff_y));

	}
	else {
	 return std::make_tuple(GL_FALSE, UP, vec3(0, 0, 0));

	}
	/*if (one.Position.x + one.Size.x < two.Position.x ||
		two.Position.x + two.Size.x < one.Position.x)
		{
		return false;
		}
		if (one.Position.y + one.Size.y < two.Position.y ||
		two.Position.y + two.Size.y < one.Position.y)
		{
		return false;
		}
		else true;
		*/
}
Collision checkBallBoxCollisions(GameObject &ball, GameObject & box){


	// Get center point circle first 
	vec3 center(ball.Position.x + ballraduis,ball.Position.y+ballraduis,0);
	// Calculate AABB info (center, half-extents)
	vec3 aabb_half_extents(box.Size.x / 2, box.Size.y / 2);
	vec3 aabb_center(
		box.Position.x + aabb_half_extents.x,
		box.Position.y + aabb_half_extents.y
		);
	// Get difference vector between both centers
	vec3 difference = center - aabb_center;
	/*difference.x = abs(difference.x);
	difference.y = abs(difference.y);*/
    vec3 clamped = clamp(difference,aabb_half_extents*-1, aabb_half_extents);
	// Add clamped value to AABB_center and we get the value of box closest to circle
	vec3 closest = aabb_center + clamped;
	// Retrieve vector between center circle and closest point AABB and check if length <= radius
	difference = closest - center;
	difference.x = abs(difference.x);
	difference.y = abs(difference.y);
	if (difference.x <ball.ball_obj->raduis&&difference.y < ball.ball_obj->raduis)
	//if (lengthVector(difference) < ballraduis)
	{
     
	return  std::make_tuple(GL_TRUE, VectorDirection(difference), difference);


	}
	else return std::make_tuple(GL_FALSE,UP, vec3(0,0,0));

}

// Calculates which direction a vector is facing (N,E,S or W)
Direction VectorDirection(vec3 target)
{
	vec3 compass[] = {
		vec3(0.0f, 1.0f),	// up
		vec3(1.0f, 0.0f),	// right
		vec3(0.0f, -1.0f),	// down
		vec3(-1.0f, 0.0f)	// left
	};
	GLfloat max = 0.0f;
	GLuint best_match = -1;
	for (GLuint i = 0; i < 4; i++)
	{
		GLfloat dot_product = dotProduct(normalize3(target), compass[i]);
		if (dot_product > max)
		{
			max = dot_product;
			best_match = i;
		}
	}
	return (Direction)best_match;
}






void DoCollisions()
{
	/*for (GameObject &box : level.Bricks)
	{
		if (CheckBoxesCollision(ball,box))
		{
		if (!box.Destroyed)
		{

			if (!box.IsSolid)
				box.Destroyed = GL_TRUE;
		}
	}
}*/
	Collision result = checkBallBoxCollisions(ball, player);
	if (!ball.ball_obj->stuck && std::get<0>(result))
	{
		// Check where it hit the board, and change velocity based on where it hit the board
		GLfloat centerBoard = player.Position.x + player.Size.x / 2;
		GLfloat distance = (ball.Position.x +ballraduis) - centerBoard;
		GLfloat percentage = distance / (player.Size.x / 2);
		// Then move accordingly
		GLfloat strength = 2.0f;
		vec2 oldVelocity = ball.velocity;
		ball.velocity.x = -1*abs (ballvelocity.x * percentage * strength);
		ball.velocity.y = -1*abs (ball.velocity.y);
		// not too large or too small velocity 
		ball.velocity = normalize2(ballvelocity);
		ball.velocity.x = ball.velocity.x *lengthVector(oldVelocity);
		ball.velocity.y = ball.velocity.y * lengthVector(oldVelocity);
		
	}

	for (GameObject &box : level.Bricks)
	{
		if (!box.Destroyed)
		{
			Collision collision = checkBallBoxCollisions(ball, box);
			if (std::get<0>(collision)) // If collision is true
			{
				if (!box.IsSolid)
				box.Destroyed = GL_TRUE;
				// Collision resolution
				Direction dir = std::get<1>(collision);
			    vec3 diff_vector = std::get<2>(collision);
				if (dir == LEFT || dir == RIGHT)
				{
					//1- reverse 
					ball.velocity.x = -ball.velocity.x;
					//2-re position the ball out of the box 
					GLfloat penetration = ballraduis  -std::abs(diff_vector.x);
					if (dir == LEFT){
						//transform out 
						ball_mesh.GlobalTransform.Position.x += penetration;
						ball.Position.x += penetration;
					}
					else
					{
						ball_mesh.GlobalTransform.Position.x -= penetration;
                        ball.Position.x -= penetration; 
					}
				}
				else if (dir == UP || dir == DOWN)
				{
					//1- reverse 
					ball.velocity.y = -ball.velocity.y;
					//2-re position the ball out of the box 
					GLfloat penetration = ballraduis -std::abs(diff_vector.y);
					if (dir == UP){
						//transform out 
						ball_mesh.GlobalTransform.Position.y += penetration;
						ball.Position.y += penetration;
					}
					else 
					{
						ball_mesh.GlobalTransform.Position.y -= penetration;
						ball.Position.y -= penetration;
					}

				}

				checkballpos(&ball, &ball_mesh, screenWidth, screenHeight);

			}

		}
	}
	
	
}





