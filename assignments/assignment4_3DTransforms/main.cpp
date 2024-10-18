#pragma once

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>

#include <stdio.h>
#include <math.h>
#include <iostream>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ccarreon/shader.h"
#include "ccarreon/texture2d.h"

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;


float boxVertices[] = {
	// X     Y     Z       R     G     B    A        U     V
	 1.0f,  1.0f, 0.0f,   1.0f, 0.0f, 0.0f, 1.0f,  1.0f, 1.0f,   // top right
	 1.0f, -1.0f, 0.0f,   0.0f, 1.0f, 0.0f, 1.0f,  1.0f, 0.0f,   // bottom right
	-1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f, 1.0f,  0.0f, 0.0f,   // bottom left
	-1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 0.0f, 1.0f,  0.0f, 1.0f    // top left 
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

	//MATRIX TRANSFORM
	glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
	glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));
	vec = trans * vec;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(boxVertices), boxVertices, GL_STATIC_DRAW);

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

	ccarreon::Shader boxShader("assets/boxVertexShader.vert", "assets/boxFragmentShader.frag");
	ccarreon::Texture2D boxTexture("assets/container.jpg", GL_LINEAR, GL_REPEAT);
	ccarreon::Texture2D faceTexture("assets/awesomeface.png", GL_LINEAR, GL_REPEAT);

	unsigned int transformLoc = glGetUniformLocation(boxShader.ID, "transform");

	// Activate the shader
	boxShader.use();
	boxShader.setInt("texture1", 0);
	boxShader.setInt("texture2", 1);

	//Render loop
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		float time = (float)glfwGetTime();

		// Clear framebuffer
		glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Set uniforms
		boxShader.setFloat("uTime", time);

		// Bind textures
		boxTexture.Bind(0);
		faceTexture.Bind(1);

		// Set transformation matrix
		glm::mat4 trans = glm::mat4(1.0f);
		trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
		trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

		// Update vertex buffer if necessary
		glBufferData(GL_ARRAY_BUFFER, sizeof(boxVertices), boxVertices, GL_STATIC_DRAW);

		// Draw elements
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// Swap buffers
		glfwSwapBuffers(window);
	}
	printf("Shutting down...");
}
