#include"gamelevel.h"

#include<fstream>
//  to handel string stuff   like :  read each char of string 
#include <string>

#include <sstream>


void loadLevel(Gamelevel *level, Mesh * levelmesh, const GLchar * fileName, float levelweidth, float levelHeight){

	level->Bricks.clear();
	//load from file 
	std::string line;
	std::ifstream fstream(fileName);
	// 0 ,1,2,3,,,,,,
	GLuint tilecode;

	// hild all the rows (the level data)
	std::vector<std::vector<GLuint>> tileData;

	if (fstream)
	{
		while (std::getline(fstream, line))   // while read each line 
		{
			// row of the level ex : 1 1 1 1 or 2 0 2 2 2 
			std::vector<GLuint> row;
			std::istringstream sstream(line);
			while (sstream >> tilecode)
			{      // while reac each word in the line 
				// form the rows of the level
				row.push_back(tilecode);
			}
			// after reading all the rpw push it in the level vector 
			tileData.push_back(row);

		}
		// check that we read at least one row of the level and give the level to the init function 
		if (tileData.size() > 0)
		{
			createLevel(level, levelmesh, tileData, levelweidth, levelHeight);
		}


	}

}


void  createLevel(Gamelevel * level, Mesh * levelmesh, std::vector<std::vector<GLuint>>tileData, float levelwidth, float levelHeight){
	//nooooooooooooote for this intilize to make things work
	// intilize levelmesh which tlater we will draw 
	level->levelMesh = levelmesh;
	// Calculate dimensions

	GLuint height = tileData.size();
	GLuint width = tileData[0].size(); // Note we can index vector at [0] since this function is only called if height > 0
	// each brick or unit of the row weidth and height 

	float unit_width = levelwidth / static_cast<float>(width), unit_height = levelHeight / height;

	// loop for each brick object  to set its init data : pos , size , texture , color 
	Texture brick_text;
	for (GLuint y = 0; y < height; ++y){

		for (GLuint x = 0; x < width; ++x){
			GameObject *object = new GameObject();

			// 1===>solid brick
			if (tileData[y][x] == 1)
			{
				//vec3 pos = vec3((-1*.5*levelwidth)+unit_width*x, levelHeight-(y*unit_height),0);
				//vec2 size = vec2(unit_width, unit_height);
				SetTexture(&brick_text, "resources\\textures\\block_solid.png");

				// then intilaize the game object 
				initGameObject(object, level->levelMesh, vec2(unit_width, unit_height), vec3((-1 * .5*levelwidth) + unit_width*x,
					levelHeight - (y*unit_height), 0), &Color(0.8f, 0.8f, 0.7f, 1),
					&brick_text);
				level->levelMesh = createobjectSprite(object);
				level->levelMeshes.push_back(*level->levelMesh);
				object->IsSolid = true;
				level->Bricks.push_back(*object);
			}
			else if (tileData[y][x] > 1)	// Non-solid; now determine its color based on level data
			{
				Color *color = &Color(1.0f, 1, 1, 1); // original: white
				SetTexture(&brick_text, "resources\\textures\\block.png");

				if (tileData[y][x] == 2)
					color = &Color(0.2f, 0.6f, 1.0f, 1);
				else if (tileData[y][x] == 3)
					color = &Color(0.0f, 0.7f, 0.0f, 1);
				else if (tileData[y][x] == 4)
					color = &Color(0.8f, 0.8f, 0.4f, 1);
				else if (tileData[y][x] == 5)
					color = &Color(1.0f, 0.5f, 0.0f, 1);

				//vec3 pos=vec3((-1 * 0.5*levelwidth) + unit_width*x, levelHeight - (y*unit_height), 0);
				//	vec2 size=vec2(unit_width, unit_height);
				initGameObject(object, level->levelMesh, vec2(unit_width, unit_height), vec3((-1 * 0.5*levelwidth) + unit_width*x, levelHeight - (y*unit_height), 0), color,
					&brick_text);
				level->levelMesh = createobjectSprite(object);
				level->levelMeshes.push_back(*level->levelMesh);
				level->Bricks.push_back(*object);

			}

		}
	}

}




GLboolean IsCompleted(Gamelevel * level)
{
	for (GameObject &tile : level->Bricks)
	if (!tile.IsSolid && !tile.Destroyed)
		return GL_FALSE;
	return GL_TRUE;
}

