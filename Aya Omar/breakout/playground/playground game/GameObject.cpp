#include "GameObject.h"
// should be a value not refernce * 


void initGameObject(GameObject *object, Mesh *sprite, vec2 size, vec3 Position, Color *color,
	Texture *text)
{

	object->Sprite = sprite;
	object->Size = size;
	object->Position = Position;
	object->Color = color;
	object->Text = text;

}

Mesh*   createobjectSprite(GameObject *object){

	*object->Sprite = CreateSprite(object->Size, object->Position, object->Text, object->Color, 1);
	return object->Sprite;


}

void moveball(GameObject * ball, ballobject*ball_, Mesh *ballmesh, float screenwidth, float screenheight){
	// If not stuck to player board
	if (!ball->ball_obj->stuck)
	{
		float difference;
		// Move the ball in x , y 

		ballmesh->GlobalTransform.Position.x+= ball->velocity.x *0.5f;
	    ballmesh->GlobalTransform.Position.y += ball->velocity.y *0.5f;
		ball->Position.x += ball->velocity.x *0.5f;
		ball->Position.y += ball->velocity.y *0.5f;
		// Check if outside window bounds; if so, reverse velocity and restore at correct position
		if (ball->Position.x <= -1 * .5*screenwidth)
		{
			difference = (-1 * .5*screenwidth - ball->Position.x)*-1;

			ball->velocity.x = -ball->velocity.x;
			ballmesh->GlobalTransform.Position.x += difference;
			ball->Position.x += difference;
		}
	else if (ball->Position.x + ball->Size.x >= .5*screenwidth)
		{
		difference = (ball->Position.x + +ball->Size.x - .5*screenwidth)*-1;

			ball->velocity.x = -ball->velocity.x;
			ballmesh->GlobalTransform.Position.x += difference;
			ball->Position.x += difference;
	}
	if (ball->Position.y +  ball->Size.y >= .5*screenheight)
		{   
		difference = (ball->Position.y + ball->Size.y - .5*screenheight)*-1;

		ball->velocity.y= -ball->velocity.y;
        ballmesh->GlobalTransform.Position.y += difference;
        ball->Position.y += difference;

		}
	
	}


}

void checkballpos(GameObject * ball, Mesh *ballmesh, float screenwidth, float screenheight){
	float difference;


	// Check if outside window bounds; if so, reverse velocity and restore at correct position
	if (ball->Position.x <= -1 * .5*screenwidth)
	{
		difference = (-1 * .5*screenwidth - ball->Position.x)*-1;

		ball->velocity.x = -ball->velocity.x;
		ballmesh->GlobalTransform.Position.x += difference;
		ball->Position.x += difference;
	}
	else if (ball->Position.x + ball->Size.x >= .5*screenwidth)
	{
		difference = (ball->Position.x + +ball->Size.x - .5*screenwidth)*-1;

		ball->velocity.x = -ball->velocity.x;
		ballmesh->GlobalTransform.Position.x += difference;
		ball->Position.x += difference;
	}
	if (ball->Position.y + ball->Size.y >= .5*screenheight)
	{
		difference = (ball->Position.y + ball->Size.y - .5*screenheight)*-1;

		ball->velocity.y = -ball->velocity.y;
		ballmesh->GlobalTransform.Position.y += difference;
		ball->Position.y += difference;

	}






}

