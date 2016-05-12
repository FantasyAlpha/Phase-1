#include <Components.h>
#include <SDL2\SDL.h>
#include <SDL_mixer.h>

void SceneManager::RenderScene(bool debug)
{
	this->RendererManager.RenderAllActive();

	if (debug)
	{
		this->RendererManager.RenderDebugShapes();
	}
}

void SceneManager::InitScene(uint32 count)
{	
	this->ActorManager.Owner = this;
	this->RendererManager.Owner = this;
	this->CollisionManager.Owner = this;
    this->AnimationManager.Owner = this;

	this->ActorManager.InitActorSystem(count);
	this->RendererManager.InitRendererSystem(count);
	this->CollisionManager.InitCollisionSystem(count);
	this->AnimationManager.InitAnimationSystem(count);

	////Initialize SDL AUDIO
	//if (SDL_Init(SDL_INIT_AUDIO) < 0)
	//{
	//	std::cout << "SDL could not initialize! SDL Error: %s\n", SDL_GetError();
	//}


	////Initialize SDL_mixer
	///* The first argument sets the sound frequency, and 44100 is a standard frequency that works on most systems.
	//or you might use MIX_DEFAULT_FREQUENCY(22050) since that is a good value for most games.
	//Most games use 22050, because 44100 requires too much CPU power on older computers.

	//The second argument determines the sample format, which again here we're using the default.
	//MIX_DEFAULT_FORMAT is the same as AUDIO_S16SYS which means (Signed 16-bit samples, in system byte order).

	//The third argument is the number of hardware channels, and here we're using 2 channels for stereo.
	//and 1 channel for mono.

	//The last argument is the sample size, which determines the size of the chunks we use when playing sound.*/
	//if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	//	// this means that open 44.1KHz, signed 16bit, system byte order,
	//	//      stereo audio, using 2048 byte chunks
	//{
	//	std::cout << "SDL_mixer could not initialize! SDL_mixer Error: \n";
	//	//printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
	//}
}

void SceneManager::UpdateScene(float delta)
{
	ActivateShader(&RendererManager.MainShader);
	glUniform2f(GetUniformLocation(&RendererManager.MainShader, UNIFORMS::MOUSE_POS), this->MousePos.X, this->MousePos.Y);
	this->CollisionManager.UpdateCollisionSystem(delta);
	this->ActorManager.UpdateTransforms();
}