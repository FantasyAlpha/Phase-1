#define GLEW_STATIC 
#include <gl\glew.h>
#include <GLFW\glfw3.h>

#include <iostream>
#include <sstream>
#include <fstream>


#define CLOSE return
#define LOG(x)  std::cout << x << std::endl;

/*-------------------B-----------------------------------------------------------------------------------*/


GLFWwindow *window;

GLuint shaderProgram;

GLuint VBO, VAO, EBO;

std::string vertexCode;
std::string fragmentCode;


GLfloat vertices[] = {
	                     0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, // Top 
						 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // Bottom Right
						-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // Bottom Left
                     };


GLuint indices[] = {                        // Note that we start from 0!
                       	0, 1, 2,  // First Triangle
                   };


void readShader(const GLchar* vertexPath, const GLchar* fragmentPath)
{
		// 1. Retrieve the vertex/fragment source code from filePath
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;
		// ensures ifstream objects can throw exceptions:
		vShaderFile.exceptions(std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::badbit);
		try
		{
			// Open files
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			std::stringstream vShaderStream, fShaderStream;
			// Read file's buffer contents into streams
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			// close file handlers
			vShaderFile.close();
			fShaderFile.close();
			// Convert stream into GLchar array
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure e)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}

}


void initDisplay(int width, int height, char* title)
{
	           /*init window hints using glfw*/
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	                   /*create window*/
	window = glfwCreateWindow(width, height, title, nullptr, nullptr);
	if (window == nullptr)
	{
		LOG("faild to create window");
		glfwTerminate();
		CLOSE;
	}
	glfwMakeContextCurrent(window);   //link context to the currunt window pointer


	                     /*init glew*/

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		LOG("faild to init opengl");
		CLOSE;
	}
	glViewport(0, 0, width, height);

}

void inputHandler(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE)
		glfwSetWindowShouldClose(window, true);
}


void initShaderProgram()
{

	readShader("vertex.vs", "fragment.fs");
	const GLchar* vertexShaderSource = vertexCode.c_str();
	const GLchar* fragmentShaderSource = fragmentCode.c_str();


	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	/*check vaild compiling*/
	GLint success;
	GLchar infolog[512];
	
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infolog);
		std::cout << "vertex shader faild :" << infolog << std::endl;
	}
	
	GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragShader);

	/*check vaild compiling*/
	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragShader, 512, NULL, infolog);
		std::cout << "fragment shader faild :" << infolog << std::endl;
	}

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragShader);
	glLinkProgram(shaderProgram);

	/*check linking correctly*/
	glGetProgramiv(shaderProgram, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infolog);
		std::cout << "vertex shader faild :" << infolog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragShader);

}


void initVAO()
{
	glGenVertexArrays(1, &VAO);
	
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//setting vertex location(coordinates)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//setting color location
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}  


void render()
{

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(shaderProgram);

	GLfloat timeValue = glfwGetTime();
	GLfloat Value = (sin(timeValue) / 2) + 0.5;

	GLuint uniformLocation = glGetUniformLocation(shaderProgram, "trans");
	glUniform3f(uniformLocation, Value, Value, Value);

	GLuint uniformLocation1 = glGetUniformLocation(shaderProgram, "m_color");
	glUniform3f(uniformLocation1, Value, Value, Value);


	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

}


void destroyVAO()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

}


void run()
{

	glfwSetKeyCallback(window, inputHandler); 

	initShaderProgram();

	initVAO();

	           /*main loop*/
	while (!glfwWindowShouldClose(window))
	{
	//	LOG("update");
		glfwPollEvents();    //input listener

		render();

		glfwSwapBuffers(window); //rendering shapes 
	}

	  /*terminate glfw --> close the window*/
	LOG("terminate");
	destroyVAO();
	glfwTerminate();
}



int main()
{
	initDisplay(800, 600, "testing");
	run();
	system("PAUSE");
	return 0;
}
