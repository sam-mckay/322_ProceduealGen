////////////////////////////////////////////////////////          
// squareShaderized.cpp
//
// Forward-compatible core GL 4.3 version of square.cpp.
// 
// Sumanta Guha
////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>

#include"glm.hpp"
#include"gtc/matrix_transform.hpp"
#include"gtc/type_ptr.hpp"
#include"glew.h"
#include<freeglut.h>
#include<glext.h>

#pragma comment(lib, "glew32.lib") 

using namespace std;
using namespace glm;

static mat4 projMat = mat4(1.0);

struct Vertex
{
   float coords[4];
   float colors[4];
};

struct Matrix4x4
{
   float entries[16];
};

static const Matrix4x4 IDENTITY_MATRIX4x4 = 
{
   {
      1.0, 0.0, 0.0, 0.0,
      0.0, 1.0, 0.0, 0.0,
      0.0, 0.0, 1.0, 0.0,
      0.0, 0.0, 0.0, 1.0
   }
};

static enum buffer {SQUARE_VERTICES}; 
static enum object {SQUARE}; 

int indexBuffers[30] = {0,1,1, 2, 1, 3, 2, 4, 2, 5, 3, 6, 3, 7, 4, 8, 4, 9, 5, 10, 5, 11, 6, 12, 6, 13, 7, 14, 7, 15 };
int indexData[7] = { 2, 6, 10, 14, 18, 22, 26 };
int levelBuffer[7] = { 1, 2, 2, 3, 3, 3, 3 };
//int indexBuffers[28] = {1, 2, 1, 3, 2, 4, 2, 5, 3, 6, 3, 7, 4, 8, 4, 9, 5, 10, 5, 11, 6, 12, 6, 13, 7, 14, 7, 15 };

//int indexBuffers[16] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };

const int MAX_LEVEL = 2;

// Globals
Vertex squareVertices[16];

static unsigned int
   programId,
   vertexShaderId,
   fragmentShaderId,
   modelViewMatLoc,
   projMatLoc,
   buffer[1],
   vao[1];
   
// Function to read text file.
char* readTextFile(char* aTextFile)
{
   FILE* filePointer = fopen(aTextFile, "rb");	
   char* content = NULL;
   long numVal = 0;

   fseek(filePointer, 0L, SEEK_END);
   numVal = ftell(filePointer);
   fseek(filePointer, 0L, SEEK_SET);
   content = (char*) malloc((numVal+1) * sizeof(char)); 
   fread(content, 1, numVal, filePointer);
   content[numVal] = '\0';
   fclose(filePointer);
   return content;
}

float convertToRad(float angle)
{
	return (3.14/ 180) * angle;
}

//
//
//
//
//
//
//

void drawTrunk(Vertex treeTrunk1)
{
	squareVertices[0].coords[0] = treeTrunk1.coords[0];
	squareVertices[0].coords[1] = treeTrunk1.coords[1];
	squareVertices[0].coords[2] = 0.0;
	squareVertices[0].coords[3] = 1.0;
	
	squareVertices[1].coords[0] = treeTrunk1.coords[2];
	squareVertices[1].coords[1] = treeTrunk1.coords[3];
	squareVertices[1].coords[2] = 0.0;
	squareVertices[1].coords[3] = 1.0;

	squareVertices[0].colors[0] = treeTrunk1.colors[0];
	squareVertices[0].colors[1] = treeTrunk1.colors[1];
	squareVertices[0].colors[2] = treeTrunk1.colors[2];
	squareVertices[0].colors[3] = treeTrunk1.colors[3];

	squareVertices[1].colors[0] = treeTrunk1.colors[0];
	squareVertices[1].colors[1] = treeTrunk1.colors[1];
	squareVertices[1].colors[2] = treeTrunk1.colors[2];
	squareVertices[1].colors[3] = treeTrunk1.colors[3];

}

int drawLeftBranch(Vertex Branch, int previousIndex, float angle)
{
	int index = indexBuffers[indexData[previousIndex]+1];
	previousIndex = indexBuffers[indexData[previousIndex]];

	int indexBuffer = (previousIndex * 2);
	Branch.coords[0] = squareVertices[indexBuffer].coords[0];
	Branch.coords[1] = squareVertices[indexBuffer + 1].coords[1];

	float newAngle = convertToRad(angle);
	mat2x2 rotatationMatrix = mat2x2{ cos(newAngle / 2), sin(newAngle / 2), -sin(newAngle / 2), cos(newAngle / 2) };

	glm::vec2 start(Branch.coords[0], Branch.coords[1]);
	glm::vec2 end(Branch.coords[2], Branch.coords[3]);

	end = rotatationMatrix*end;

	squareVertices[previousIndex].coords[0] = start.x;
	squareVertices[previousIndex].coords[1] = start.y;
	squareVertices[previousIndex].coords[2] = 0.0;
	squareVertices[previousIndex].coords[3] = 1.0;

	squareVertices[index].coords[0] = end.x;
	squareVertices[index].coords[1] = end.y;
	squareVertices[index].coords[2] = 0.0;
	squareVertices[index].coords[3] = 1.0;

	squareVertices[previousIndex].colors[0] = Branch.colors[0];
	squareVertices[previousIndex].colors[1] = Branch.colors[1];
	squareVertices[previousIndex].colors[2] = Branch.colors[2];
	squareVertices[previousIndex].colors[3] = Branch.colors[3];

	squareVertices[index].colors[0] = Branch.colors[0];
	squareVertices[index].colors[1] = Branch.colors[1];
	squareVertices[index].colors[2] = Branch.colors[2];
	squareVertices[index].colors[3] = Branch.colors[3];


	cout << "DRAWN LEFT BRANCH  : PREVIOUS INDEX: " << previousIndex << " INDEX: " << index << endl;


	return index;
}

int drawRightBranch(Vertex Branch, int previousIndex, float angle)
{
	int index = indexBuffers[indexData[previousIndex] + 3];
	previousIndex = indexBuffers[indexData[previousIndex]];

	int indexBuffer = (previousIndex * 2) + 2;
	Branch.coords[0] = squareVertices[indexBuffer].coords[0];
	Branch.coords[1] = squareVertices[indexBuffer + 1].coords[1];



	float newAngle = convertToRad(angle);
	newAngle = -newAngle/2;
	mat2x2 rotatationMatrix = mat2x2{ cos(newAngle), sin(newAngle), -sin(newAngle), cos(newAngle)};

	glm::vec2 start(Branch.coords[0], Branch.coords[1]);
	glm::vec2 end(Branch.coords[2], Branch.coords[3]);

	end = rotatationMatrix*end;

	squareVertices[previousIndex].coords[0] = start.x;
	squareVertices[previousIndex].coords[1] = start.y;
	squareVertices[previousIndex].coords[2] = 0.0;
	squareVertices[previousIndex].coords[3] = 1.0;

	squareVertices[index].coords[0] = end.x;
	squareVertices[index].coords[1] = end.y;
	squareVertices[index].coords[2] = 0.0;
	squareVertices[index].coords[3] = 1.0;

	squareVertices[previousIndex].colors[0] = Branch.colors[0];
	squareVertices[previousIndex].colors[1] = Branch.colors[1];
	squareVertices[previousIndex].colors[2] = Branch.colors[2];
	squareVertices[previousIndex].colors[3] = Branch.colors[3];

	squareVertices[index].colors[0] = Branch.colors[0];
	squareVertices[index].colors[1] = Branch.colors[1];
	squareVertices[index].colors[2] = Branch.colors[2];
	squareVertices[index].colors[3] = Branch.colors[3];
	
	cout << "DRAWN RIGHT BRANCH : PREVIOUS INDEX: " << previousIndex << " INDEX: " << index << endl;

	return index;
}

void drawBranch(int level, float prevPosX, float prevPosY, float height, float angle, int prevIndex)
{
	Vertex Branch = Vertex{ { 0, 0, 0, prevPosY+height }, { 0.55f, 0.27f, 0.075f, 1.0f } };
	
	drawLeftBranch(Branch, prevIndex, angle);
	drawRightBranch(Branch, prevIndex, angle);

	cout << "LEVEL: " << level << " MAX LEVEL: " << MAX_LEVEL << endl;
	
	if (prevIndex < MAX_LEVEL)
	{
		level++;
		prevIndex++;
		drawBranch(level, prevPosX, prevPosY+height, height, angle, prevIndex);
	}
	//cout << "DONE" << endl;
	
}

//generates a tree
void drawTree(int *level)
{	
	Vertex Trunk = Vertex{ { 0, -30, 0, -15 }, {0.55f, 0.27f, 0.075f, 1.0f} };
	drawTrunk(Trunk);


	drawBranch(0, -0, -15, 15, 40, 0);

	//test
	for (int i = 0; i < 15; i++)
	{
		cout << squareVertices[i].coords[0] << endl;
		cout << squareVertices[i].coords[1] << endl;
	}
	
	cout << "DONE TREE" << endl;
}

// Initialization routine.
void setup(void)
{
   glClearColor(1.0, 1.0, 1.0, 0.0);
   //
   //
   int levelVal = 0;
   int *level = &levelVal;
   
   drawTree(level);
   //

   // Create shader program executable.
   char* vertexShader = readTextFile("vertexShader.glsl");
   vertexShaderId = glCreateShader(GL_VERTEX_SHADER); 
   glShaderSource(vertexShaderId, 1, (const char**) &vertexShader, NULL); 
   glCompileShader(vertexShaderId); 

   char* fragmentShader = readTextFile("fragmentShader.glsl");
   fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
   glShaderSource(fragmentShaderId, 1, (const char**) &fragmentShader, NULL); 
   glCompileShader(fragmentShaderId); 

   programId = glCreateProgram(); 
   glAttachShader(programId, vertexShaderId); 
   glAttachShader(programId, fragmentShaderId); 
   glLinkProgram(programId); 
   glUseProgram(programId); 
   ///////////////////////////////////////

   // Create VAO and VBO and associate data with vertex shader.
   glGenVertexArrays(1, vao);
   glGenBuffers(1, buffer);
   glBindVertexArray(vao[SQUARE]);
   glBindBuffer(GL_ARRAY_BUFFER, buffer[SQUARE_VERTICES]);
   glBufferData(GL_ARRAY_BUFFER, sizeof(squareVertices), squareVertices, GL_STATIC_DRAW);

   glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(squareVertices[0]), 0);
   glEnableVertexAttribArray(0);
   glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(squareVertices[0]), (GLvoid*)sizeof(squareVertices[0].coords));
   glEnableVertexAttribArray(1);
   ///////////////////////////////////////

	//
   projMatLoc = glGetUniformLocation(programId, "projMat");
   projMat = ortho(-50.0, 50.0, -50.0, 50.0, -1.0, 1.0);
   glUniformMatrix4fv(projMatLoc, 1, GL_FALSE, value_ptr(projMat));
   // Obtain modelview matrix uniform location and set value.
   Matrix4x4 modelViewMat = IDENTITY_MATRIX4x4;
   modelViewMatLoc = glGetUniformLocation(programId, "modelViewMat"); 
   glUniformMatrix4fv(modelViewMatLoc, 1, GL_TRUE, modelViewMat.entries);
   ///////////////////////////////////////
}

// Drawing routine.
void drawScene(void)
{
   glClear(GL_COLOR_BUFFER_BIT);
   glLineWidth(10);
  
   glDrawArrays(GL_LINES, 0, 8);
   


   glFlush();
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
   glViewport(0, 0, w, h);
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
   switch(key) 
   {
      case 27:
         exit(0);
         break;
      default:
         break;
   }
}

// Main routine.
int main(int argc, char* argv[])
{	
   glutInit(&argc, argv);

   glutInitContextVersion(4, 2);
   glutInitContextProfile(GLUT_CORE_PROFILE);
   //glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
   
   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(100, 100); 
   glutCreateWindow("squareShaderized.cpp");
   glutDisplayFunc(drawScene);
   glutReshapeFunc(resize);
   glutKeyboardFunc(keyInput);
   
   glewExperimental = GL_TRUE;
   glewInit();

   setup();

   glutMainLoop();
}
