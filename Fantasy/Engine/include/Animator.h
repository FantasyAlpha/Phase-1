#pragma once 

#include <RenderingEngine.h>
#include <SpriteRenderer.h>
#include <map>

struct AnimationClip
{
public:
	AnimationClip();

	AnimationClip(Texture texture, uint32 maxH, uint32 maxV
		, float frameTime = 1.0f / 60.0f, bool loop = true);

	AnimationClip(Texture texture, uint32 maxH, uint32 maxV
		, std::vector<std::pair<uint32, float>> *frames, bool loop = true);

public:
	uint32 MaxCountHorizontal, MaxCountVertical;

	float FrameWidth, FrameHeight;
	float FrameTime;
	float Counter;

	Texture Sheet;

	uint32 CurrentFrame;
	
	std::vector<std::pair<uint32, float>> Frames;

	char *Name;
	bool Loop;
	bool IsRunning;
};

struct Animator	: public GameComponent
{
public:
	Animator(char *name, SpriteRenderer *renderer);

public:
	virtual void Init()
	{
	
	}

	virtual void Render()
	{
		
	}

	virtual void Destroy();

	virtual void Update(float delta, Game_Input *input);

	void SetStartClip(char *name);

	void SwitchClip(char *name);

	void AddClip(char *name, AnimationClip *clip);

	AnimationClip* GetClip(char *name);

	void PauseClip();

	void ResumeClip();

	void StopClip();


	void StopWhenDone();

	char* GetCurrentClipName();
	char* GetLastClipName();
	
	bool IsRunning(char *name);

private:
	bool IsFinished();
public:
	AnimationClip *CurrentClip;
	bool StopFlag;
private:
	SpriteRenderer *Renderer;
	std::map<char *, AnimationClip *> *Clips;
	AnimationClip *LastRunningClip;
};
