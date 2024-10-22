#include "camera.h"

using namespace ccarreon;

Camera::Camera(float width, float height) {
	SCREEN_WIDTH = width;
	SCREEN_HEIGHT = height;
}

void Camera::processInput(GLFWwindow* window)
{
	float cameraSpeed = 2.5f * deltaTime; // adjust accordingly
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		cameraSpeed *= 2.0f; // Double if sprinting
	}
	//Movement forward, strafe, and up
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		cameraPos += cameraSpeed * cameraFront;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		cameraPos -= cameraSpeed * cameraFront;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		cameraPos += cameraSpeed * cameraUp;
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		cameraPos -= cameraSpeed * cameraUp;
	}
	//https://stackoverflow.com/questions/52492426/glfw-switching-boolean-toggle
	if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS && !tabHeld) {
		perspective = !perspective; // Toggle perspective or orthographic
	}
	tabHeld = glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS;
}
void Camera::mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	//Check if first mouse call
	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	// Get change in mouse position
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;
	// Multiply offset by sensitivity
	float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;
	// Change rotation angles
	yaw += xoffset;
	pitch += yoffset;
	// Make sure that you can't look up too far up or down
	if (pitch > 89.0f) pitch = 89.0f;
	if (pitch < -89.0f) pitch = -89.0f;
	// Change camera look direction
	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(direction);
}
void Camera::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {\
	// Get scroll
	fov -= (float)yoffset;
	if (fov < 1.0f) fov = 1.0f;
	if (fov > 120.0f) fov = 120.0f;
}
void Camera::use(GLFWwindow* window) {
	processInput(window);

	glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xpos, double ypos) {
		Camera* camera = static_cast<Camera*>(glfwGetWindowUserPointer(window));
		camera->mouse_callback(window, xpos, ypos);
	});

	glfwSetScrollCallback(window, [](GLFWwindow* window, double xoffset, double yoffset) {
		Camera* camera = static_cast<Camera*>(glfwGetWindowUserPointer(window));
		camera->scroll_callback(window, xoffset, yoffset);
	});

	// Set the user pointer to this camera instance for the window
	glfwSetWindowUserPointer(window, this);
}
void Camera::projectionSwap(glm::mat4& projection) {
	//Switch between projection and orthographic
	if (perspective) projection = glm::perspective(glm::radians(fov), ((float)SCREEN_WIDTH / SCREEN_HEIGHT), 0.1f, 1000.0f);
	else projection = glm::ortho(-2.0f, 2.0f, -2.0f * ((float)SCREEN_HEIGHT / SCREEN_WIDTH), 2.0f * ((float)SCREEN_HEIGHT / SCREEN_WIDTH), 0.1f, 1000.0f);
}
void Camera::viewLookAt(glm::mat4& view) {
	// Set view position
	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}
void Camera::timeChange(float currentFrame) {
	// Change the time
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
}