// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <array>
#include <stack>   
#include <sstream>
// Include GLEW
#include <GL/glew.h>
// Include GLFW
#include <glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
using namespace glm;
// Include AntTweakBar
#include <AntTweakBar.h>

#include <common/shader.hpp>
#include <common/controls.hpp>
#include <common/objloader.hpp>
#include <common/vboindexer.hpp>

#include<math.h>

GLfloat theta_for_camera = 60, phi_for_camera = 42;

//const int window_width = 800, window_height = 600;
const int window_width = 1600, window_height = 1200;
int click_c = 0;
int ObjectPicked = 0; //1=base,2=top,3=arm1,4=arm2,5=pen

int  b = 0, p = 0, t = 0, key_one = 0, key_two = 0;
float angle_top=0, angle_arm1=0, angle_arm2 = 0, angle_pen1=0, angle_pen2=0, angle_pen3=0;
int ob = 0, ot = 0, oa1 = 0, oa2 = 0, op = 0;
glm::vec4 base_color  (0.5, 0.3, 0.8, 1.0);
glm::vec4 top_color   (0.5, 0.4, 0.8, 1.0);
glm::vec4 arm1_color  (0.6, 0.5, 0.8, 1.0);
glm::vec4 arm2_color  (0.6, 0.4, 0.9, 1.0);
glm::vec4 joint_color (0.6, 0.3, 0.7, 1.0);
glm::vec4 button_color(1.0, 0.0, 0.8, 1.0);
glm::vec4 ball_color  (0.0, 1.0, 1.0, 1.0);
glm::vec4 pen_color   (0.5, 0.6, 0.8, 1.0);

glm::vec4 highlight_color(1.0, 1.0, 1.0, 1.0);


typedef struct Vertex {
	float Position[4];
	float Color[4];
	float Normal[3];
	void SetPosition(float *coords) {
		Position[0] = coords[0];
		Position[1] = coords[1];
		Position[2] = coords[2];
		Position[3] = 1.0;
	}
	void SetColor(float *color) {
		Color[0] = color[0];
		Color[1] = color[1];
		Color[2] = color[2];
		Color[3] = color[3];
	}
	void SetNormal(float *coords) {
		Normal[0] = coords[0];
		Normal[1] = coords[1];
		Normal[2] = coords[2];
	}
};

Vertex for_grid[] = {

	{ { 5.0, 0.0, 5.0, 1.0 },{ 1.0, 1.0, 1.0, 1.0 },{ 0.0, 0.0, 1.0 } },
	{ { 5.0, 0.0, -5.0, 1.0 },{ 1.0, 1.0, 1.0, 1.0 },{ 0.0, 0.0, 1.0 } },

	{ { 4.0, 0.0, 5.0, 1.0 },{ 1.0, 1.0, 1.0, 1.0 },{ 0.0, 0.0, 1.0 } },
	{ { 4.0, 0.0, -5.0, 1.0 },{ 1.0, 1.0, 1.0, 1.0 },{ 0.0, 0.0, 1.0 } },

	{ { 3.0, 0.0, 5.0, 1.0 },{ 1.0, 1.0, 1.0, 1.0 },{ 0.0, 0.0, 1.0 } },
	{ { 3.0, 0.0, -5.0, 1.0 },{ 1.0, 1.0, 1.0, 1.0 },{ 0.0, 0.0, 1.0 } },

	{ { 2.0, 0.0, 5.0, 1.0 },{ 1.0, 1.0, 1.0, 1.0 },{ 0.0, 0.0, 1.0 } },
	{ { 2.0, 0.0, -5.0, 1.0 },{ 1.0, 1.0, 1.0, 1.0 },{ 0.0, 0.0, 1.0 } },

	{ { 1.0, 0.0, 5.0, 1.0 },{ 1.0, 1.0, 1.0, 1.0 },{ 0.0, 0.0, 1.0 } },
	{ { 1.0, 0.0, -5.0, 1.0 },{ 1.0, 1.0, 1.0, 1.0 },{ 0.0, 0.0, 1.0 } },

	{ { 0.0, 0.0, 5.0, 1.0 },{ 1.0, 1.0, 1.0, 1.0 },{ 0.0, 0.0, 1.0 } },
	{ { 0.0, 0.0, -5.0, 1.0 },{ 1.0, 1.0, 1.0, 1.0 },{ 0.0, 0.0, 1.0 } },

	{ { -1.0, 0.0, 5.0, 1.0 },{ 1.0, 1.0, 1.0, 1.0 },{ 0.0, 0.0, 1.0 } },
	{ { -1.0, 0.0, -5.0, 1.0 },{ 1.0, 1.0, 1.0, 1.0 },{ 0.0, 0.0, 1.0 } },

	{ { -2.0, 0.0, 5.0, 1.0 },{ 1.0, 1.0, 1.0, 1.0 },{ 0.0, 0.0, 1.0 } },
	{ { -2.0, 0.0, -5.0, 1.0 },{ 1.0, 1.0, 1.0, 1.0 },{ 0.0, 0.0, 1.0 } },

	{ { -3.0, 0.0, 5.0, 1.0 },{ 1.0, 1.0, 1.0, 1.0 },{ 0.0, 0.0, 1.0 } },
	{ { -3.0, 0.0, -5.0, 1.0 },{ 1.0, 1.0, 1.0, 1.0 },{ 0.0, 0.0, 1.0 } },

	{ { -4.0, 0.0, 5.0, 1.0 },{ 1.0, 1.0, 1.0, 1.0 },{ 0.0, 0.0, 1.0 } },
	{ { -4.0, 0.0, -5.0, 1.0 },{ 1.0, 1.0, 1.0, 1.0 },{ 0.0, 0.0, 1.0 } },

	{ { -5.0, 0.0, 5.0, 1.0 },{ 1.0, 1.0, 1.0, 1.0 },{ 0.0, 0.0, 1.0 } },
	{ { -5.0, 0.0, -5.0, 1.0 },{ 1.0, 1.0, 1.0, 1.0 },{ 0.0, 0.0, 1.0 } },

	{ { 5.0, 0.0, 5.0, 1.0 },{ 1.0, 1.0, 1.0, 1.0 },{ 0.0, 0.0, 1.0 } },
	{ { -5.0, 0.0, 5.0, 1.0 },{ 1.0, 1.0, 1.0, 1.0 },{ 0.0, 0.0, 1.0 } },

	{ { 5.0, 0.0, 4.0, 1.0 },{ 1.0, 1.0, 1.0, 1.0 },{ 0.0, 0.0, 1.0 } },
	{ { -5.0, 0.0, 4.0, 1.0 },{ 1.0, 1.0, 1.0, 1.0 },{ 0.0, 0.0, 1.0 } },

	{ { 5.0, 0.0, 3.0, 1.0 },{ 1.0, 1.0, 1.0, 1.0 },{ 0.0, 0.0, 1.0 } },
	{ { -5.0, 0.0, 3.0, 1.0 },{ 1.0, 1.0, 1.0, 1.0 },{ 0.0, 0.0, 1.0 } },

	{ { 5.0, 0.0, 2.0, 1.0 },{ 1.0, 1.0, 1.0, 1.0 },{ 0.0, 0.0, 1.0 } },
	{ { -5.0, 0.0, 2.0, 1.0 },{ 1.0, 1.0, 1.0, 1.0 },{ 0.0, 0.0, 1.0 } },

	{ { 5.0, 0.0, 1.0, 1.0 },{ 1.0, 1.0, 1.0, 1.0 },{ 0.0, 0.0, 1.0 } },
	{ { -5.0, 0.0, 1.0, 1.0 },{ 1.0, 1.0, 1.0, 1.0 },{ 0.0, 0.0, 1.0 } },

	{ { 5.0, 0.0, 0.0, 1.0 },{ 1.0, 1.0, 1.0, 1.0 },{ 0.0, 0.0, 1.0 } },
	{ { -5.0, 0.0, 0.0, 1.0 },{ 1.0, 1.0, 1.0, 1.0 },{ 0.0, 0.0, 1.0 } },

	{ { 5.0, 0.0, -1.0, 1.0 },{ 1.0, 1.0, 1.0, 1.0 },{ 0.0, 0.0, 1.0 } },
	{ { -5.0, 0.0, -1.0, 1.0 },{ 1.0, 1.0, 1.0, 1.0 },{ 0.0, 0.0, 1.0 } },

	{ { 5.0, 0.0, -2.0, 1.0 },{ 1.0, 1.0, 1.0, 1.0 },{ 0.0, 0.0, 1.0 } },
	{ { -5.0, 0.0, -2.0, 1.0 },{ 1.0, 1.0, 1.0, 1.0 },{ 0.0, 0.0, 1.0 } },

	{ { 5.0, 0.0, -3.0, 1.0 },{ 1.0, 1.0, 1.0, 1.0 },{ 0.0, 0.0, 1.0 } },
	{ { -5.0, 0.0, -3.0, 1.0 },{ 1.0, 1.0, 1.0, 1.0 },{ 0.0, 0.0, 1.0 } },

	{ { 5.0, 0.0, -4.0, 1.0 },{ 1.0, 1.0, 1.0, 1.0 },{ 0.0, 0.0, 1.0 } },
	{ { -5.0, 0.0, -4.0, 1.0 },{ 1.0, 1.0, 1.0, 1.0 },{ 0.0, 0.0, 1.0 } },

	{ { 5.0, 0.0, -5.0, 1.0 },{ 1.0, 1.0, 1.0, 1.0 },{ 0.0, 0.0, 1.0 } },
	{ { -5.0, 0.0, -5.0, 1.0 },{ 1.0, 1.0, 1.0, 1.0 },{ 0.0, 0.0, 1.0 } }
};

unsigned short Indices_FOR_GRID[44];

// function prototypes
int initWindow(void);
void initOpenGL(void);
void loadObject(char*, glm::vec4, Vertex * &, GLushort* &, int);
void createVAOs(Vertex[], GLushort[], int);
void createObjects(void);
void pickObject(void);
void renderScene(void);
void cleanup(void);
static void keyCallback(GLFWwindow*, int, int, int, int);
static void mouseCallback(GLFWwindow*, int, int, int);

// GLOBAL VARIABLES
GLFWwindow* window;

glm::mat4 gProjectionMatrix;
glm::mat4 gViewMatrix;

GLuint gPickedIndex = -1;
std::string gMessage;

GLuint programID;
GLuint pickingProgramID;

const GLuint NumObjects = 10;	// ATTN: THIS NEEDS TO CHANGE AS YOU ADD NEW OBJECTS
GLuint VertexArrayId[NumObjects] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
GLuint VertexBufferId[NumObjects] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
GLuint IndexBufferId[NumObjects] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

size_t NumIndices[NumObjects] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
size_t VertexBufferSize[NumObjects] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
size_t IndexBufferSize[NumObjects] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

GLuint MatrixID;
GLuint ModelMatrixID;
GLuint ViewMatrixID;
GLuint ProjMatrixID;
GLuint PickingMatrixID;
GLuint pickingColorID;
GLuint LightID;
GLuint LightID_2;

GLint gX = 0.0;
GLint gZ = 0.0;

// animation control
bool animation = false;
GLfloat phi = 0.0;

void loadObject(char* file, glm::vec4 color, Vertex * &out_Vertices, GLushort* &out_Indices, int ObjectId)
{
	// Read our .obj file
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	bool res = loadOBJ(file, vertices, normals);

	std::vector<GLushort> indices;
	std::vector<glm::vec3> indexed_vertices;
	std::vector<glm::vec2> indexed_uvs;
	std::vector<glm::vec3> indexed_normals;
	indexVBO(vertices, normals, indices, indexed_vertices, indexed_normals);

	const size_t vertCount = indexed_vertices.size();
	const size_t idxCount = indices.size();

	// populate output arrays
	out_Vertices = new Vertex[vertCount];
	for (int i = 0; i < vertCount; i++) {
		out_Vertices[i].SetPosition(&indexed_vertices[i].x);
		out_Vertices[i].SetNormal(&indexed_normals[i].x);
		out_Vertices[i].SetColor(&color[0]);
	}
	out_Indices = new GLushort[idxCount];
	for (int i = 0; i < idxCount; i++) {
		out_Indices[i] = indices[i];
	}

	// set global variables!!
	NumIndices[ObjectId] = idxCount;
	VertexBufferSize[ObjectId] = sizeof(out_Vertices[0]) * vertCount;
	IndexBufferSize[ObjectId] = sizeof(GLushort) * idxCount;
}


void createObjects(void)
{
	//-- COORDINATE AXES --//
	Vertex CoordVerts[] =
	{
		{ { 0.0, 0.0, 0.0, 1.0 }, { 1.0, 0.0, 0.0, 1.0 }, { 0.0, 0.0, 1.0 } },
		{ { 5.0, 0.0, 0.0, 1.0 }, { 1.0, 0.0, 0.0, 1.0 }, { 0.0, 0.0, 1.0 } },
		{ { 0.0, 0.0, 0.0, 1.0 }, { 0.0, 1.0, 0.0, 1.0 }, { 0.0, 0.0, 1.0 } },
		{ { 0.0, 5.0, 0.0, 1.0 }, { 0.0, 1.0, 0.0, 1.0 }, { 0.0, 0.0, 1.0 } },
		{ { 0.0, 0.0, 0.0, 1.0 }, { 0.0, 0.0, 1.0, 1.0 }, { 0.0, 0.0, 1.0 } },
		{ { 0.0, 0.0, 5.0, 1.0 }, { 0.0, 0.0, 1.0, 1.0 }, { 0.0, 0.0, 1.0 } },
	};

	VertexBufferSize[0] = sizeof(CoordVerts);	// ATTN: this needs to be done for each hand-made object with the ObjectID (subscript)
	createVAOs(CoordVerts, NULL, 0);

	//-- GRID --//

	// ATTN: create your grid vertices here!
		
	VertexBufferSize[1] = sizeof(for_grid);
	createVAOs(for_grid, Indices_FOR_GRID, 1);

	//-- .OBJs --//

	// ATTN: load your models here
	Vertex* Verts_base;
	GLushort* Idcs_base;
	loadObject("base.obj", base_color, Verts_base, Idcs_base, 2);
	createVAOs(Verts_base, Idcs_base, 2);

	Vertex* Verts_top;
	GLushort* Idcs_top;
	loadObject("top.obj", top_color, Verts_top, Idcs_top, 3);
	createVAOs(Verts_top, Idcs_top, 3);

	Vertex* Verts_arm1;
	GLushort* Idcs_arm1;
	loadObject("arm1.obj", arm1_color, Verts_arm1, Idcs_arm1, 4);
	createVAOs(Verts_arm1, Idcs_arm1, 4);

	Vertex* Verts_joint;
	GLushort* Idcs_joint;
	loadObject("joint.obj",joint_color, Verts_joint, Idcs_joint, 5);
	createVAOs(Verts_joint, Idcs_joint, 5);

	Vertex* Verts_arm2;
	GLushort* Idcs_arm2;
	loadObject("arm2.obj",arm2_color, Verts_arm2, Idcs_arm2, 6);
	createVAOs(Verts_arm2, Idcs_arm2, 6);

	Vertex* Verts_pen;
	GLushort* Idcs_pen;
	loadObject("pen.obj", pen_color, Verts_pen, Idcs_pen, 7);
	createVAOs(Verts_pen, Idcs_pen, 7);

	Vertex* Verts_button;
	GLushort* Idcs_button;
	loadObject("button.obj", button_color, Verts_button, Idcs_button, 8);
	createVAOs(Verts_button, Idcs_button, 8);

	Vertex* Verts_ball;
	GLushort* Idcs_ball;
	loadObject("ball.obj", ball_color, Verts_ball, Idcs_ball, 9);
	createVAOs(Verts_ball, Idcs_ball, 9);

}

void renderScene(void)
{
	//ATTN: DRAW YOUR SCENE HERE. MODIFY/ADAPT WHERE NECESSARY!
	
	// Camera matrix
	if (click_c > 0)
	{
		gViewMatrix = glm::lookAt(glm::vec3(17.0f * (sin(radians(theta_for_camera)) * cos(radians(phi_for_camera))), 17.0f * (cos(radians(theta_for_camera))) , 17.0f * (sin(radians(theta_for_camera)) * sin(radians(phi_for_camera)))),	// eye
		glm::vec3(0.0, 0.0, 0.0),	// center
		glm::vec3(0.0, 1.0, 0.0));	// up
	}

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.2f, 0.0f);
	// Re-clear the screen for real rendering
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(programID);
	{
		glm::vec3 lightPos = glm::vec3(0, 4, 4);		//Light 1
		glm::vec3 lightPos_2 = glm::vec3(4, 4, -1);		//Light 2
		glm::mat4x4 ModelMatrix = glm::mat4(1.0);
		glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);
		glUniform3f(LightID_2, lightPos_2.x, lightPos_2.y, lightPos_2.z);
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &gViewMatrix[0][0]);
		glUniformMatrix4fv(ProjMatrixID, 1, GL_FALSE, &gProjectionMatrix[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

		glBindVertexArray(VertexArrayId[0]);	// draw CoordAxes
		glDrawArrays(GL_LINES, 0, 6);

		glBindVertexArray(0); 
		
		glBindVertexArray(VertexArrayId[1]);	// draw for_grid
		glDrawArrays(GL_LINES, 0, 44);

		glBindVertexArray(1);
	
		glm::mat4x4 TranslateMatrix;
		TranslateMatrix = glm::translate(TranslateMatrix, glm::vec3(gX, 0.0, gZ));
		ModelMatrix = ModelMatrix * TranslateMatrix;
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
		
		glm::mat4x4 RotateMatrix;
		RotateMatrix = glm::rotate(TranslateMatrix, radians(angle_top), glm::vec3(0.0f, 1.0f, 0.0f));
		ModelMatrix = ModelMatrix * RotateMatrix;
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

		glBindVertexArray(VertexArrayId[2]);		//Base
		glDrawElements(GL_TRIANGLES, NumIndices[2], GL_UNSIGNED_SHORT, (void *)0);

		glBindVertexArray(VertexArrayId[3]);		//Top
		glDrawElements(GL_TRIANGLES, NumIndices[3], GL_UNSIGNED_SHORT, (void *)0);

		glm::mat4x4 TranslateMatrix_R;
		glm::mat4x4 RotateMatrix_ARM1;
		TranslateMatrix_R = glm::translate(TranslateMatrix_R, glm::vec3(0.0f, 2.0f, 0.0f));
		RotateMatrix_ARM1 = glm::rotate(TranslateMatrix_R, radians(angle_arm1), glm::vec3(0.0f, 0.0f, 1.0f));
		TranslateMatrix_R = glm::translate(RotateMatrix_ARM1, glm::vec3(0.0f, -2.0f, 0.0f));
		ModelMatrix = ModelMatrix * TranslateMatrix_R;
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

		glBindVertexArray(VertexArrayId[4]);		//Arm1
		glDrawElements(GL_TRIANGLES, NumIndices[4], GL_UNSIGNED_SHORT, (void *)0);

		glBindVertexArray(VertexArrayId[5]);		//Joint
		glDrawElements(GL_TRIANGLES, NumIndices[5], GL_UNSIGNED_SHORT, (void *)0);

		glm::mat4x4 TranslateMatrix_R2;
		glm::mat4x4 RotateMatrix_ARM2;
		TranslateMatrix_R2 = glm::translate(TranslateMatrix_R2, glm::vec3(2.13f, 2.23f, 0.0f));
		RotateMatrix_ARM2 = glm::rotate(TranslateMatrix_R2, radians(angle_arm2), glm::vec3(0.0f, 0.0f, 1.0f));
		TranslateMatrix_R2 = glm::translate(RotateMatrix_ARM2, glm::vec3(-2.13f, -2.23f, 0.0f));
		ModelMatrix = ModelMatrix * TranslateMatrix_R2;
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

		glBindVertexArray(VertexArrayId[6]);		//Arm2
		glDrawElements(GL_TRIANGLES, NumIndices[6], GL_UNSIGNED_SHORT, (void *)0);

		glm::mat4x4 TranslateMatrix_Pen;
		glm::mat4x4 RotateMatrix_Pen1;
		glm::mat4x4 RotateMatrix_Pen2;
		glm::mat4x4 RotateMatrix_Pen3;
		glm::mat4x4 TranslateMatrix_Pen2;
		TranslateMatrix_Pen = glm::translate(TranslateMatrix_Pen, glm::vec3(2.35f, 1.19f, 0.0f));
		RotateMatrix_Pen1 = glm::rotate(TranslateMatrix_Pen, radians(angle_pen1), glm::vec3(-0.2f, 0.98f, 0.0f));
		RotateMatrix_Pen2 = glm::rotate(RotateMatrix_Pen1, radians(angle_pen2), glm::vec3(0.0f, 0.0f, 1.0f));
		RotateMatrix_Pen3 = glm::rotate(RotateMatrix_Pen2, radians(angle_pen3), glm::vec3(0.7f, 0.72f, 0.0f));
		TranslateMatrix_Pen2 = glm::translate(RotateMatrix_Pen3, glm::vec3(-2.35f, -1.19f, 0.0f));
		ModelMatrix = ModelMatrix * TranslateMatrix_Pen2;
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

		glBindVertexArray(VertexArrayId[7]);		//Pen
		glDrawElements(GL_TRIANGLES, NumIndices[7], GL_UNSIGNED_SHORT, (void *)0);

		glBindVertexArray(VertexArrayId[8]);		//Button
		glDrawElements(GL_TRIANGLES, NumIndices[8], GL_UNSIGNED_SHORT, (void *)0);

		glBindVertexArray(VertexArrayId[9]);		//Ball
		glDrawElements(GL_TRIANGLES, NumIndices[9], GL_UNSIGNED_SHORT, (void *)0);

	}
	glUseProgram(0);
	// Draw GUI
	TwDraw();

	// Swap buffers
	glfwSwapBuffers(window);
	glfwPollEvents();
}

void pickObject(void)
{
	// Clear the screen in white
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(pickingProgramID);
	{
		glm::mat4 ModelMatrix = glm::mat4(1.0); // TranslationMatrix * RotationMatrix;
		glm::mat4 MVP = gProjectionMatrix * gViewMatrix * ModelMatrix;

		// Send our transformation to the currently bound shader, in the "MVP" uniform
		glUniformMatrix4fv(PickingMatrixID, 1, GL_FALSE, &MVP[0][0]);
		
		// ATTN: DRAW YOUR PICKING SCENE HERE. REMEMBER TO SEND IN A DIFFERENT PICKING COLOR FOR EACH OBJECT BEFOREHAND
		
		int r;
		int g;
		int b;
		
		r = (2 & 0x000000FF) >> 0;
		g = (2 & 0x0000FF00) >> 8;
		b = (2 & 0x00FF0000) >> 16;

		// OpenGL expects colors to be in [0,1], so divide by 255.
		glUniform4f(pickingColorID, r / 255.0f, g / 255.0f, b / 255.0f, 1.0f);

		glm::mat4x4 TranslateMatrix;
		TranslateMatrix = glm::translate(TranslateMatrix, glm::vec3(gX, 0.0, gZ));
		ModelMatrix = ModelMatrix * TranslateMatrix;
		MVP = gProjectionMatrix * gViewMatrix * ModelMatrix;
		glUniformMatrix4fv(PickingMatrixID, 1, GL_FALSE, &MVP[0][0]);

		glm::mat4x4 RotateMatrix;
		RotateMatrix = glm::rotate(TranslateMatrix, 5.0f*angle_top, glm::vec3(0.0f, 1.0f, 0.0f));
		ModelMatrix = ModelMatrix * RotateMatrix;
		MVP = gProjectionMatrix * gViewMatrix * ModelMatrix;
		glUniformMatrix4fv(PickingMatrixID, 1, GL_FALSE, &MVP[0][0]);

		glBindVertexArray(VertexArrayId[2]);		//Base
		glDrawElements(GL_TRIANGLES, NumIndices[2], GL_UNSIGNED_SHORT, (void *)0);


		r = (3 & 0x000000FF) >> 0;
		g = (3 & 0x0000FF00) >> 8;
		b = (3 & 0x00FF0000) >> 16;

		// OpenGL expects colors to be in [0,1], so divide by 255.
		glUniform4f(pickingColorID, r / 255.0f, g / 255.0f, b / 255.0f, 1.0f);

		glBindVertexArray(VertexArrayId[3]);		//Top
		glDrawElements(GL_TRIANGLES, NumIndices[3], GL_UNSIGNED_SHORT, (void *)0);

		glm::mat4x4 TranslateMatrix_R;
		glm::mat4x4 RotateMatrix_ARM1;
		TranslateMatrix_R = glm::translate(TranslateMatrix_R, glm::vec3(0.0f, 2.0f, 0.0f));
		RotateMatrix_ARM1 = glm::rotate(TranslateMatrix_R, 18.0f*angle_arm1, glm::vec3(0.0f, 0.0f, 1.0f));
		TranslateMatrix_R = glm::translate(RotateMatrix_ARM1, glm::vec3(0.0f, -2.0f, 0.0f));
		ModelMatrix = ModelMatrix * TranslateMatrix_R;
		MVP = gProjectionMatrix * gViewMatrix * ModelMatrix;
		glUniformMatrix4fv(PickingMatrixID, 1, GL_FALSE, &MVP[0][0]);


		r = (4 & 0x000000FF) >> 0;
		g = (4 & 0x0000FF00) >> 8;
		b = (4 & 0x00FF0000) >> 16;

		// OpenGL expects colors to be in [0,1], so divide by 255.
		glUniform4f(pickingColorID, r / 255.0f, g / 255.0f, b / 255.0f, 1.0f);

		glBindVertexArray(VertexArrayId[4]);		//Arm1
		glDrawElements(GL_TRIANGLES, NumIndices[4], GL_UNSIGNED_SHORT, (void *)0);


		r = (5 & 0x000000FF) >> 0;
		g = (5 & 0x0000FF00) >> 8;
		b = (5 & 0x00FF0000) >> 16;

		// OpenGL expects colors to be in [0,1], so divide by 255.
		glUniform4f(pickingColorID, r / 255.0f, g / 255.0f, b / 255.0f, 1.0f);

		glBindVertexArray(VertexArrayId[5]);		//Joint
		glDrawElements(GL_TRIANGLES, NumIndices[5], GL_UNSIGNED_SHORT, (void *)0);

		glm::mat4x4 TranslateMatrix_R2;
		glm::mat4x4 RotateMatrix_ARM2;
		TranslateMatrix_R2 = glm::translate(TranslateMatrix_R2, glm::vec3(2.13f, 2.23f, 0.0f));
		RotateMatrix_ARM2 = glm::rotate(TranslateMatrix_R2, 18.0f*angle_arm2, glm::vec3(0.0f, 0.0f, 1.0f));
		TranslateMatrix_R2 = glm::translate(RotateMatrix_ARM2, glm::vec3(-2.13f, -2.23f, 0.0f));
		ModelMatrix = ModelMatrix * TranslateMatrix_R2;
		MVP = gProjectionMatrix * gViewMatrix * ModelMatrix;
		glUniformMatrix4fv(PickingMatrixID, 1, GL_FALSE, &MVP[0][0]);


		r = (6 & 0x000000FF) >> 0;
		g = (6 & 0x0000FF00) >> 8;
		b = (6 & 0x00FF0000) >> 16;

		// OpenGL expects colors to be in [0,1], so divide by 255.
		glUniform4f(pickingColorID, r / 255.0f, g / 255.0f, b / 255.0f, 1.0f);

		glBindVertexArray(VertexArrayId[6]);		//Arm2
		glDrawElements(GL_TRIANGLES, NumIndices[6], GL_UNSIGNED_SHORT, (void *)0);

		glm::mat4x4 TranslateMatrix_Pen;
		glm::mat4x4 RotateMatrix_Pen1;
		glm::mat4x4 RotateMatrix_Pen2;
		glm::mat4x4 RotateMatrix_Pen3;
		glm::mat4x4 TranslateMatrix_Pen2;
		TranslateMatrix_Pen = glm::translate(TranslateMatrix_Pen, glm::vec3(2.35f, 1.19f, 0.0f));
		RotateMatrix_Pen1 = glm::rotate(TranslateMatrix_Pen, 18.0f*angle_pen1, glm::vec3(-0.2f, 0.98f, 0.0f));
		RotateMatrix_Pen2 = glm::rotate(RotateMatrix_Pen1, 18.0f*angle_pen2, glm::vec3(0.0f, 0.0f, 1.0f));
		RotateMatrix_Pen3 = glm::rotate(RotateMatrix_Pen2, 18.0f*angle_pen3, glm::vec3(0.7f, 0.72f, 0.0f));
		TranslateMatrix_Pen2 = glm::translate(RotateMatrix_Pen3, glm::vec3(-2.35f, -1.19f, 0.0f));
		ModelMatrix = ModelMatrix * TranslateMatrix_Pen2;
		MVP = gProjectionMatrix * gViewMatrix * ModelMatrix;
		glUniformMatrix4fv(PickingMatrixID, 1, GL_FALSE, &MVP[0][0]);

		r = (7 & 0x000000FF) >> 0;
		g = (7 & 0x0000FF00) >> 8;
		b = (7 & 0x00FF0000) >> 16;

		// OpenGL expects colors to be in [0,1], so divide by 255.
		glUniform4f(pickingColorID, r / 255.0f, g / 255.0f, b / 255.0f, 1.0f);

		glBindVertexArray(VertexArrayId[7]);		//Pen
		glDrawElements(GL_TRIANGLES, NumIndices[7], GL_UNSIGNED_SHORT, (void *)0);


		r = (8 & 0x000000FF) >> 0;
		g = (8 & 0x0000FF00) >> 8;
		b = (8 & 0x00FF0000) >> 16;

		// OpenGL expects colors to be in [0,1], so divide by 255.
		glUniform4f(pickingColorID, r / 255.0f, g / 255.0f, b / 255.0f, 1.0f);

		glBindVertexArray(VertexArrayId[8]);		//Button
		glDrawElements(GL_TRIANGLES, NumIndices[8], GL_UNSIGNED_SHORT, (void *)0);

		glBindVertexArray(0);

	}
	glUseProgram(0);
	// Wait until all the pending drawing commands are really done.
	// Ultra-mega-over slow ! 
	// There are usually a long time between glDrawElements() and
	// all the fragments completely rasterized.
	glFlush();
	glFinish();

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Read the pixel at the center of the screen.
	// You can also use glfwGetMousePos().
	// Ultra-mega-over slow too, even for 1 pixel, 
	// because the framebuffer is on the GPU.
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	unsigned char data[4];
	glReadPixels(xpos, window_height - ypos, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, data); // OpenGL renders with (0,0) on bottom, mouse reports with (0,0) on top
	
	// Convert the color back to an integer ID
	gPickedIndex = int(data[0]);

	if (gPickedIndex == 255) { // Full white, must be the background !
		gMessage = "Background";
		ObjectPicked = 0;
	}
	else {
		printf("%d\n", gPickedIndex);
		std::ostringstream oss;
		if (gPickedIndex == 2 )
		{
			b++;
			ObjectPicked = 1;
			oss << "Object " << gPickedIndex << ": Base";
			ob = 1;
		}
		if (gPickedIndex == 3)
		{
			t++;
			ObjectPicked = 2;
			oss << "Object " << gPickedIndex << ": Top";
			ot = 1;
		}
		if (gPickedIndex == 4)
		{
			key_one++;
			ObjectPicked = 3;
			oss << "Object " << gPickedIndex << ": Arm 1";
			oa1 = 1;
		}
		if (gPickedIndex == 6)
		{
			key_two++;
			ObjectPicked = 4;
			oss << "Object " << gPickedIndex << ": Arm 2";
			oa2 = 1;
		}
		if (gPickedIndex == 7)
		{
			p++;
			ObjectPicked = 5;
			oss << "Object " << gPickedIndex << ": Pen";
			op = 1;
		}
	}

	// Uncomment these lines to see the picking shader in effect
	//glfwSwapBuffers(window);
	//continue; // skips the normal rendering
}

int initWindow(void)
{
	// Initialise GLFW
	if (!glfwInit()) {
		fprintf(stderr, "Failed to initialize GLFW\n");
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(window_width, window_height, "Singh,Aishwarya(13143268)", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	// Initialize the GUI
	TwInit(TW_OPENGL_CORE, NULL);
	TwWindowSize(window_width, window_height);
	TwBar * GUI = TwNewBar("Picking");
	TwSetParam(GUI, NULL, "refresh", TW_PARAM_CSTRING, 1, "0.1");
	TwAddVarRW(GUI, "Last picked object", TW_TYPE_STDSTRING, &gMessage, NULL);

	// Set up inputs
	glfwSetCursorPos(window, window_width / 2, window_height / 2);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetMouseButtonCallback(window, mouseCallback);

	return 0;
}

void initOpenGL(void)
{
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);
	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	gProjectionMatrix = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
	// Or, for an ortho camera :
	//gProjectionMatrix = glm::ortho(-4.0f, 4.0f, -3.0f, 3.0f, 0.0f, 100.0f); // In world coordinates

	// Camera matrix

	gViewMatrix = glm::lookAt(glm::vec3(17.0f * (sin(radians(theta_for_camera)) * cos(radians(phi_for_camera))), 17.0f * (cos(radians(theta_for_camera))), 17.0f * (sin(radians(theta_for_camera)) * sin(radians(phi_for_camera)))),	// eye
		glm::vec3(0.0, 0.0, 0.0),	// center
		glm::vec3(0.0, 1.0, 0.0));	// up

	//gViewMatrix = glm::lookAt(glm::vec3(10.0, 10.0, 10.0f),	// eye
	//	glm::vec3(0.0, 0.0, 0.0),	// center
	//	glm::vec3(0.0, 1.0, 0.0));	// up

	// Create and compile our GLSL program from the shaders
	programID = LoadShaders("StandardShading.vertexshader", "StandardShading.fragmentshader");
	pickingProgramID = LoadShaders("Picking.vertexshader", "Picking.fragmentshader");

	// Get a handle for our "MVP" uniform
	MatrixID = glGetUniformLocation(programID, "MVP");
	ModelMatrixID = glGetUniformLocation(programID, "M");
	ViewMatrixID = glGetUniformLocation(programID, "V");
	ProjMatrixID = glGetUniformLocation(programID, "P");
	
	PickingMatrixID = glGetUniformLocation(pickingProgramID, "MVP");
	// Get a handle for our "pickingColorID" uniform
	pickingColorID = glGetUniformLocation(pickingProgramID, "PickingColor");
	// Get a handle for our "LightPosition" uniform
	LightID = glGetUniformLocation(programID, "LightPosition_worldspace");
	LightID_2 = glGetUniformLocation(programID, "LightPosition_2_worldspace");

	createObjects();
}

void createVAOs(Vertex Vertices[], unsigned short Indices[], int ObjectId) {

	GLenum ErrorCheckValue = glGetError();
	const size_t VertexSize = sizeof(Vertices[0]);
	const size_t RgbOffset = sizeof(Vertices[0].Position);
	const size_t Normaloffset = sizeof(Vertices[0].Color) + RgbOffset;

	// Create Vertex Array Object
	glGenVertexArrays(1, &VertexArrayId[ObjectId]);	//
	glBindVertexArray(VertexArrayId[ObjectId]);		//

	// Create Buffer for vertex data
	glGenBuffers(1, &VertexBufferId[ObjectId]);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBufferId[ObjectId]);
	glBufferData(GL_ARRAY_BUFFER, VertexBufferSize[ObjectId], Vertices, GL_STATIC_DRAW);

	// Create Buffer for indices
	if (Indices != NULL) {
		glGenBuffers(1, &IndexBufferId[ObjectId]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferId[ObjectId]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndexBufferSize[ObjectId], Indices, GL_STATIC_DRAW);
	}

	// Assign vertex attributes
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, VertexSize, 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, VertexSize, (GLvoid*)RgbOffset); 
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, VertexSize, (GLvoid*)Normaloffset);

	glEnableVertexAttribArray(0);	// position
	glEnableVertexAttribArray(1);	// color
	glEnableVertexAttribArray(2);	// normal

	// Disable our Vertex Buffer Object 
	glBindVertexArray(0);

	ErrorCheckValue = glGetError();
	if (ErrorCheckValue != GL_NO_ERROR)
	{
		fprintf(
			stderr,
			"ERROR: Could not create a VBO: %s \n",
			gluErrorString(ErrorCheckValue)
			);
	}
}

void cleanup(void)
{
	// Cleanup VBO and shader
	for (int i = 0; i < NumObjects; i++) {
		glDeleteBuffers(1, &VertexBufferId[i]);
		glDeleteBuffers(1, &IndexBufferId[i]);
		glDeleteVertexArrays(1, &VertexArrayId[i]);
	}
	glDeleteProgram(programID);
	glDeleteProgram(pickingProgramID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();
}


static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// ATTN: MODIFY AS APPROPRIATE
	if (action == GLFW_PRESS) {
		
		switch (key)
		{
		case GLFW_KEY_P:
			p++;
			if (p % 2 != 0 )
			{
				Vertex* Verts_pen;
				GLushort* Idcs_pen;
				loadObject("pen.obj",highlight_color, Verts_pen, Idcs_pen, 7);
				createVAOs(Verts_pen, Idcs_pen, 7);

			}
			else
			{
				Vertex* Verts_pen;
				GLushort* Idcs_pen;
				loadObject("pen.obj", pen_color, Verts_pen, Idcs_pen, 7);
				createVAOs(Verts_pen, Idcs_pen, 7);

			}
			break;
		case GLFW_KEY_B:
			b++;
			if (b % 2 != 0)
			{
				Vertex* Verts_base;
				GLushort* Idcs_base;
				loadObject("base.obj", highlight_color, Verts_base, Idcs_base, 2);
				createVAOs(Verts_base, Idcs_base, 2);

			}
			else
			{
				Vertex* Verts_base;
				GLushort* Idcs_base;
				loadObject("base.obj", base_color, Verts_base, Idcs_base, 2);
				createVAOs(Verts_base, Idcs_base, 2);
			}
			break;
		case GLFW_KEY_T:
			t++;
			if (t % 2 != 0)
			{
				Vertex* Verts_top;
				GLushort* Idcs_top;
				loadObject("top.obj", highlight_color, Verts_top, Idcs_top, 3);
				createVAOs(Verts_top, Idcs_top, 3);
			}
			else
			{
				Vertex* Verts_top;
				GLushort* Idcs_top;
				loadObject("top.obj", top_color, Verts_top, Idcs_top, 3);
				createVAOs(Verts_top, Idcs_top, 3);
			}
			break;
		case GLFW_KEY_1:
			key_one++;
			if (key_one % 2 != 0)
			{
				Vertex* Verts_arm1;
				GLushort* Idcs_arm1;
				loadObject("arm1.obj", highlight_color, Verts_arm1, Idcs_arm1, 4);
				createVAOs(Verts_arm1, Idcs_arm1, 4);
			}
			else
			{
				Vertex* Verts_arm1;
				GLushort* Idcs_arm1;
				loadObject("arm1.obj", arm1_color, Verts_arm1, Idcs_arm1, 4);
				createVAOs(Verts_arm1, Idcs_arm1, 4);
			}
			break;
		case GLFW_KEY_2:
			key_two++;
			if(key_two % 2 != 0)
			{	
				Vertex* Verts_arm2;
				GLushort* Idcs_arm2;
				loadObject("arm2.obj", highlight_color, Verts_arm2, Idcs_arm2, 6);
				createVAOs(Verts_arm2, Idcs_arm2, 6);
			}
			else
			{
				Vertex* Verts_arm2;
				GLushort* Idcs_arm2;
				loadObject("arm2.obj", arm2_color, Verts_arm2, Idcs_arm2, 6);
				createVAOs(Verts_arm2, Idcs_arm2, 6);
			}
			break;
		case GLFW_KEY_C:
			click_c++;
			break;
		case GLFW_KEY_LEFT:
			if (click_c % 2 != 0)
			{
				phi_for_camera += 5;
			}
			if(b % 2 != 0)
			{
				if (gZ > 5)
					gZ = -5;
				else if (gZ < -5)
					gZ = 5;
				else
					gZ += 1;
			}
			if (t % 2 != 0)
				angle_top += 5;
			
			if (p % 2 != 0)
			{
				if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
				{
					angle_pen3 -= 5;
				}
				else
				{
					angle_pen1 -= 5;
				}
			}
			break;

		case GLFW_KEY_RIGHT:
			if (click_c % 2 != 0)
			{
				phi_for_camera -= 5;
				
			}
			if (b % 2 != 0)
			{
				if (gZ > 5)
					gZ = -5;
				else if (gZ < -5)
					gZ = 5;
				else
					gZ -= 1;
			}

			if (t % 2 != 0)
				angle_top -= 5;

			if (p % 2 != 0)
			{
				if (glfwGetKey(window,GLFW_KEY_LEFT_SHIFT)== GLFW_PRESS)
				{
					angle_pen3 += 5;
				}
				else 
				{
					angle_pen1 += 5;
				}
			}
			break;
		case GLFW_KEY_UP:
			if (click_c % 2 != 0)
			{
				theta_for_camera -= 5;
			}
			if (b % 2 != 0)
			{
				if (gX > 5)
					gX = -5;
				else if (gX < -5)
					gX = 5;
				else
					gX -= 1;
			}

			if (key_one % 2 != 0)
			{
				if (angle_arm1<120)
					angle_arm1 += 5;
			}
			if (key_two % 2 != 0)
			{
				if (angle_arm2 < 200)
					angle_arm2 += 5;
			}
			if (p % 2 != 0)
				angle_pen2 += 5;
			break;
		case GLFW_KEY_DOWN:			
			if (click_c % 2 != 0)
			{
				theta_for_camera += 5;
			
			}
			if (b % 2 != 0)
			{
				if (gX > 5)
					gX = -5;
				else if (gX < -5)
					gX = 5;
				else
					gX += 1;
			}

			if (key_one % 2 != 0)
			{
				if (angle_arm1>=-10)
					angle_arm1 -= 5;
			}

			if (key_two % 2 != 0)
			{
				if (angle_arm2>=-60)
					angle_arm2 -= 5;
			}
			if (p % 2 != 0)
				angle_pen2 -= 5;
			break;
		default:
			break;
		}

	}
}

static void mouseCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		pickObject();

		if (ObjectPicked == 1 && b % 2 !=0)
		{
			Vertex* Verts_base;
			GLushort* Idcs_base;
			loadObject("base.obj", highlight_color, Verts_base, Idcs_base, 2);
			createVAOs(Verts_base, Idcs_base, 2);

		}
		if ( (ob == 1 && ObjectPicked != 1) || (ob == 1 && b % 2 == 0))
		{
			Vertex* Verts_base;
			GLushort* Idcs_base;
			loadObject("base.obj", base_color, Verts_base, Idcs_base, 2);
			createVAOs(Verts_base, Idcs_base, 2);
			ob = 0;
		}
		if (ObjectPicked == 2 && t % 2 != 0)
		{
			Vertex* Verts_top;
			GLushort* Idcs_top;
			loadObject("top.obj", highlight_color, Verts_top, Idcs_top, 3);
			createVAOs(Verts_top, Idcs_top, 3);
		}
		if ((ot == 1 && ObjectPicked != 2) || (ot == 1 && t % 2 == 0))
		{
			Vertex* Verts_top;
			GLushort* Idcs_top;
			loadObject("top.obj", top_color, Verts_top, Idcs_top, 3);
			createVAOs(Verts_top, Idcs_top, 3);
			ot = 0;
		}
		if (ObjectPicked == 3 && key_one %2 !=0)
		{
			Vertex* Verts_arm1;
			GLushort* Idcs_arm1;
			loadObject("arm1.obj", highlight_color, Verts_arm1, Idcs_arm1, 4);
			createVAOs(Verts_arm1, Idcs_arm1, 4);
		}
		if( (oa1 ==1 && ObjectPicked != 3) || (oa1 == 1 && key_one % 2 == 0))
		{
			Vertex* Verts_arm1;
			GLushort* Idcs_arm1;
			loadObject("arm1.obj", arm1_color, Verts_arm1, Idcs_arm1, 4);
			createVAOs(Verts_arm1, Idcs_arm1, 4);
			oa1 = 0;
		}
		if (ObjectPicked == 4 && key_two %2 !=0)
		{
			Vertex* Verts_arm2;
			GLushort* Idcs_arm2;
			loadObject("arm2.obj", highlight_color, Verts_arm2, Idcs_arm2, 6);
			createVAOs(Verts_arm2, Idcs_arm2, 6);
		}
		if((oa2 == 1 && ObjectPicked != 4) || (oa2 == 1 && key_two % 2 == 0))
		{
			Vertex* Verts_arm2;
			GLushort* Idcs_arm2;
			loadObject("arm2.obj", arm2_color, Verts_arm2, Idcs_arm2, 6);
			createVAOs(Verts_arm2, Idcs_arm2, 6);
			oa2 = 0;
		}
		if (ObjectPicked == 5 && p % 2 !=0)
		{
			Vertex* Verts_pen;
			GLushort* Idcs_pen;
			loadObject("pen.obj", highlight_color, Verts_pen, Idcs_pen, 7);
			createVAOs(Verts_pen, Idcs_pen, 7);

		}
		if((op == 1 && ObjectPicked != 5) || (op == 1 && p % 2 == 0))
		{
			Vertex* Verts_pen;
			GLushort* Idcs_pen;
			loadObject("pen.obj", pen_color, Verts_pen, Idcs_pen, 7);
			createVAOs(Verts_pen, Idcs_pen, 7);
			op = 0;
		}
	}
}

int main(void)
{

	int i = 0;
	for (i = 0; i < 44; i++)
		Indices_FOR_GRID[i] = i;

	// initialize window
	int errorCode = initWindow();
	if (errorCode != 0)
		return errorCode;

	// initialize OpenGL pipeline
	initOpenGL();

	// For speed computation
	double lastTime = glfwGetTime();
	int nbFrames = 0;
	do {
		//// Measure speed
		//double currentTime = glfwGetTime();
		//nbFrames++;
		//if (currentTime - lastTime >= 1.0){ // If last prinf() was more than 1sec ago
		//	// printf and reset
		//	printf("%f ms/frame\n", 1000.0 / double(nbFrames));
		//	nbFrames = 0;
		//	lastTime += 1.0;
		//}
		
		if (animation){
			phi += 0.01;
			if (phi > 360)
				phi -= 360;
		}

		// DRAWING POINTS
		renderScene();


	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
	glfwWindowShouldClose(window) == 0);

	cleanup();

	return 0;
}