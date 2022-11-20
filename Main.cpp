#include <glad/glad.h> //An abstraction above open GL
#include <GLFW/glfw3.h> // GLU to abstract tedious staff above Glad
#include <iostream>

const int initHeight{ 600 };
const int initWidth{ 800 };

void frame_buffer_calback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
};

void processInput(GLFWwindow *window) //what the asterisk does? // Well, I know it is a pointer, but I am not sure why a pointer is required here, Is it because when an object is passed, a copy of it is made, and we need the real deal to modify it?
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

//Vertex data
float verticies[] = {
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f
};



const char * vertexShader_GLSL_SourceCode = R"multilineString(
#version 330 core
layout (location = 0) in vec3 aPos;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
)multilineString";

const char* fragmentShader_GLSL_SourceCode = R"multilineString(
#version 330 core
out vec4 FragColor;

void main()
{
    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
} 
)multilineString";



int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);//this one is for apple

	//window creation
	GLFWwindow* window = glfwCreateWindow(initWidth, initHeight, "First real Window", NULL, NULL);
	if (window == NULL) {
		std::cout << "Window is NULL - failed to initiate a window";
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Glad failed to connect via glfw";
		glfwTerminate();
		return -1;
	}

	glViewport(0, 0, initWidth, initHeight);
	glfwSetFramebufferSizeCallback(window, frame_buffer_calback);


	//Vertex Shader
	unsigned int vertexShader; //the ID of the vertexShader by which we will refer to it in the next few lines
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShader_GLSL_SourceCode, NULL);
	glCompileShader(vertexShader);
	int compiledSuccessfully;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compiledSuccessfully);
	char infoLog[512];
	if (!compiledSuccessfully) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	else
		std::cout << "INFO::SHADER::VERTEX::COMPILATION_SUCCEEDED\n" << std::endl;

	//The same thing for fragment Shader
	unsigned int fragmentShader; //the ID of the vertexShader by which we will refer to it in the next few lines
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShader_GLSL_SourceCode, NULL);
	glCompileShader(fragmentShader);
	// int compiledSuccessfully; was already decleared, we will reuse it
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compiledSuccessfully);
	//char infoLog[512]; the same
	if (!compiledSuccessfully) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	else
		std::cout << "INFO::SHADER::FRAGMENT::COMPILATION_SUCCEEDED\n" << std::endl;

	//linking the shaders into a shader program
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	int successfullyLinked;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &successfullyLinked);
	if (!successfullyLinked) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);//reusing infolog, can bite me later
		std::cout << "ERROR::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	else
		std::cout << "INFO::PROGRAM::LINKING_SUCCEEDED\n" << std::endl;
	//cleanup (the shaders are already inside the program)
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);



	// Loading Vertex data into GPU memory via a Vertex Buffer Object (VBO) is a cumbersome for multiple objects.
	unsigned int VBO; //the ID of the VBO by which we will refer to it in the next few lines
	glGenBuffers(1, &VBO);
	unsigned int VAO; //the ID of the VAO by which we will refer to it in the next few lines
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); //a highly data-specific configuration
	glEnableVertexAttribArray(0);



	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
