#pragma once

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>

#include <stdio.h>
#include <math.h>
#include <iostream>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "ccarreon/shader.h"
#include "ccarreon/texture2d.h"

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;


float bgVertices[] = {
	// X     Y     Z       R     G     B    A        U     V
	 1.0f,  1.0f, 0.0f,   1.0f, 0.0f, 0.0f, 1.0f,  2.0f, 1.0f,   // top right
	 1.0f, -1.0f, 0.0f,   0.0f, 1.0f, 0.0f, 1.0f,  2.0f, 0.0f,   // bottom right
	-1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f, 1.0f,  0.0f, 0.0f,   // bottom left
	-1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 0.0f, 1.0f,  0.0f, 1.0f    // top left 
};
float frogVertices[] = {
	// X     Y     Z       R     G     B    A        U     V
	 0.333f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f, 1.0f,  1.0f, 1.0f,   // top right
	 0.333f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f, 1.0f,  1.0f, 0.0f,   // bottom right
	-0.333f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f, 1.0f,  0.0f, 0.0f,   // bottom left
	-0.333f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f, 1.0f,  0.0f, 1.0f    // top left 
};
unsigned int indices[] = {
	0, 1, 3,
	1, 2, 3  
};
int main() {
	printf("Initializing...");
	if (!glfwInit()) {
		printf("GLFW failed to init!");
		return 1;
	}
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello Triangle", NULL, NULL);
	if (window == NULL) {
		printf("GLFW failed to create window");
		return 1;
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGL(glfwGetProcAddress)) {
		printf("GLAD Failed to load GL headers");
		return 1;
	}
	//Initialization goes here!
	unsigned int VAO, VBO, EBO;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(frogVertices), frogVertices, GL_STATIC_DRAW);

	//NEWER OPTION:  glNamedBufferData(VBO, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//POSITION (XYZ)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//COLOR (RGBA)
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(1);

	//TEXTURE (UV)
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(sizeof(float) * 7));
	glEnableVertexAttribArray(2);

	ccarreon::Shader bgShader("assets/bgVertexShader.vert", "assets/bgFragmentShader.frag");
	ccarreon::Shader frogShader("assets/frogVertexShader.vert", "assets/frogFragmentShader.frag");
	ccarreon::Texture2D bgTexture("assets/pond.jpg", GL_LINEAR, GL_REPEAT);
	ccarreon::Texture2D frogTexture("assets/frog.png", GL_NEAREST, GL_CLAMP_TO_BORDER);
	//Pond image: https://coloradopondpros.com/2022/09/26/12-benefits-of-a-natural-swim-pond-backyard/

	//Render loop
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		float time = (float)glfwGetTime();
		//Clear framebuffer
		glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		//Drawing happens here!
		
		glBufferData(GL_ARRAY_BUFFER, sizeof(bgVertices), bgVertices, GL_STATIC_DRAW);
		bgShader.use();
		bgShader.setFloat("uTime", time);
		bgTexture.Bind();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		
		glBufferData(GL_ARRAY_BUFFER, sizeof(frogVertices), frogVertices, GL_STATIC_DRAW);
		frogShader.use();
		frogShader.setFloat("uTime", time);
		frogTexture.Bind();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		
		glfwSwapBuffers(window);
	}
	printf("Shutting down...");
}
