#include<iostream>
#include"vgl.h"
#include"LoadShaders.h"
using namespace std;

enum VAO_IDS { Triangles, NumVAOs };;
enum Buffer_IDs { ArrayBuffer, NumBuffers };
enum Attrib_IDs { vPosition = 0 };

GLuint VAOs[NumVAOs];
GLuint Buffers[NumBuffers];

const GLuint NumVertices = 6;

void init()
{
	static const GLfloat vertices[NumVertices][2] =
	{
		{-0.90f,-0.90f}, {0.85f,-0.90f}, {-0.90f,0.85f},
		{0.90f,-0.85f}, {0.90f,0.90f}, {-0.85f,0.90f}
	};

	// 1. create vertex-array object and bind to current opengl context
	glCreateVertexArrays(NumVAOs, VAOs);												// create vertex-array objects
																						// opengl allocate the names of vertex-array objects and save them in VAOs
																						// name is like a c-pointer, and reference a memory
	glBindVertexArray(VAOs[Triangles]);													// once we have an opengl object, we need to bind the object to current opengl context
																						// and the same time vertex-array object is actived
																						// if param is zero, then stop using the previously bound vertex-array

	// 2. create buffer object and fill the buffer with the vertices data 
	// and bind to current opengl context
	glCreateBuffers(NumBuffers, Buffers);												// create buffer objects to save datas
																						// opengl allocate the names of buffer objects and save them in Buffers
																						// those data saved in buffer are managed by the currently bound vertex-array object
	glNamedBufferStorage(Buffers[ArrayBuffer], sizeof(vertices), vertices, 0);			// fill the buffer with the vertices data, the buffer doesnt't need to be bound
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[ArrayBuffer]);								// we bind the buffer to the current opengl context and sametime active the buffer
																						// because there many places where buffer objects can be in OpenGL, we need specify
																						// which we'd like to use
																						// when name is zero, we stop using any buffer object bound to "GL_ARRAY_BUFFER" target

	// 3. configure the shaders
	ShaderInfo shaders[] =
	{
		{GL_VERTEX_SHADER,"res/shaders/triangles.vert"},
		{GL_FRAGMENT_SHADER,"res/shaders/triangles.frag"},
		{GL_NONE,NULL}
	};
	GLuint program = LoadShaders(shaders);
	glUseProgram(program);

	// 4. connect the data in application to the variables in shader
	// we need to connect our shader 'in' variables to a 'vertex-attribute array'
	// below two function modify the state of currently bound vertex-array object
	glVertexAttribPointer(
		vPosition,								// index: shader attribute 'location'
		2,										// size: represents the number of components to be updated per vertex and can be 1,2,3,4
		GL_FLOAT,								// type: specifies the data type of each element in the array
		GL_FALSE,								// normalized: indicates that vertex data should be normalized before being stored
		0,										// stride: the byte offset between consecutive elements in the array
		BUFFER_OFFSET(0));						// pointer: this is offset from the start of the buffer object(unit: based-machine,ie.:bytes)
	glEnableVertexAttribArray(vPosition);		// enable vertex-attribute
}

void display()
{
	static const float black[] = { 0.0f,0.0f,0.0f,0.0f };

	// 1. clear color buffer in the current framebuffer to the specified clear values
	glClearBufferfv(GL_COLOR, 0, black);

	// 2. select the collection of vertices that we want to draw
	glBindVertexArray(VAOs[Triangles]);

	// 3. request draw, which actually sends vertex data to the OpenGL pipeline
	glDrawArrays(								
		GL_TRIANGLES,							// mode: geometric primitives
		0,										// first: the starting position of the currently bound vertex-array object
		NumVertices);							// count: the end is 'first + count - 1'
}

int main()
{
	// init glfw lib
	glfwInit();

	// create a window and an opengl context binded to the window,
	// make the context current and then opengl command are directed to the context
	GLFWwindow* window = glfwCreateWindow(640, 480, "Triangles", NULL, NULL);
	glfwMakeContextCurrent(window);

	// init gl3w lib
	gl3wInit();

	// init some datas for render
	init();

	// render loop
	while (!glfwWindowShouldClose(window))
	{
		display();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// destroy window & shut down glfw lib
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}