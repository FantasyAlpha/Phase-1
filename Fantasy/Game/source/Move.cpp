#include <Move.h>

void Move::Update(float delta, Game_Input *input)
{
	vec3f accel;
	vec3f newPos;
	bool right = false;
	bool left = false;
	bool up = false;
	bool down = false;
	bool attack = false;
	if (Owner->Name == "test3")
	{
		if (input->Buttons[Left].KeyDown)
		{
			accel.X = -Acceleration * Speed;
			Owner->ObjectTransform.Scale.X *= Owner->ObjectTransform.Scale.X > 0 ? -1.0f : 1.0f;
			left = true;
		}

		if (input->Buttons[Right].KeyDown)
		{
			accel.X = Acceleration * Speed;
			Owner->ObjectTransform.Scale.X *= Owner->ObjectTransform.Scale.X > 0 ? 1.0f : -1.0f;
			right = true;
		}

		if (input->Buttons[Up].KeyDown)
		{
			accel.Y = Acceleration * Speed;
			up = true;
		}

		if (input->Buttons[Down].KeyDown)
		{
			accel.Y = -Acceleration * Speed;
			down = true;
		}
	}	

	if (input->Z.KeyDown)
	{
		if (Owner->Name == "test8")
		{
			Owner->ObjectTransform.Rotation.Z += 0.5f;
		}
	}

	accel += -2.5f * Velocity;
	
	Owner->ObjectTransform.Position += (Velocity * delta + (0.5f * delta * delta * accel));
	Velocity += accel * delta;

	right = false;
	left = false;
	up = false;
	down = false;
	attack = false;
}
