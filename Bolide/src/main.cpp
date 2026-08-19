#include <gl/glew.h>
#include <glfw/glfw3.h>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "Camera.h"

#include "Shader.h"
#include "Shadows.h"

#include "Model.h"
#include "Cuboid.h"

using namespace std;

enum CameraMode {
	FREE,
	LOCK_TO_DRIVER_HEAD,
	LOCK_BEHIND_BOLIDE,
	FRONT_VIEW
};

GLuint SCR_WIDTH = 1920;
GLuint SCR_HEIGHT = 1080;

// shaders configuration
void applyViewToShader(shared_ptr<Shader> shader, const glm::mat4& projection, const glm::mat4& view);
void applyLightToShader(shared_ptr<Shader> shader);
void applyShadowToShader(shared_ptr<Shader> shader, const glm::mat4& lightSpaceMatrix);

// keys and mouse movement callbacks
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// elements movement
void moveCamera(GLFWwindow* window, double mouseX, double mouseY, float deltaTime);
void updateLockedCamera(Model& bolide);
void moveBolide(GLFWwindow* window, float deltaTime, Model& bolide);

// Camera setup
Camera camera(glm::vec3(-191.369f, 207.608f, 351.143f), glm::vec3(0.0f, 1.0f, 0.0f), -100.0f, -3.0f);
CameraMode cameraMode = FREE;

double lastX, lastY;
bool firstMouse = true;

// Bolide setup
bool stopBolide = false;
float bolideSpeed = 0.0f;	

int main()
{
	if (!glfwInit()) {
		cout << "GLFW initialization failed" << endl;
		return -1;
	}
	
	// Window creation
	GLFWwindow*  window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Bolide", glfwGetPrimaryMonitor(), NULL);
	//GLFWwindow*  window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Bolide", NULL, NULL);
	if (!window) {
		throw exception("GLFW window not created");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glewExperimental = GL_TRUE;

	// GLEW library initiation
	if (glewInit() != GLEW_OK)
		throw exception("GLEW Initialization failed");
	glfwMakeContextCurrent(window);

	// Depth Buffer ON
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	// Blending ON
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// ----- SHADERS ----------------------------------------------------------------------------
	// SkyBox
	auto skyboxShader = ShaderManager::instance().getShader("shaders_config/skybox.vert", "shaders_config/skybox.frag");
	skyboxShader->use();
	skyboxShader->setInt("Texture0", 0);
	// Shadow shader;
	auto shShadows = GenerateShadows();
	// Main objects shader
	auto shader = ShaderManager::instance().getShader("shaders_config/shader.vert", "shaders_config/shader.frag");
	shader->use();
	shader->setInt("shadowMap", 1);

	// ----- OBJECTS ----------------------------------------------------------------------------
	auto SkyBox = Cuboid(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(2000.0f, 2000.0f, 2000.0f), glm::vec3(0.0f, 0.0f, 0.0f), "textures/sky.jpg");
	auto Monza = Model("Monza.obj");
	auto Bolide = Model("F1_Bolide.obj");
	Bolide.move(glm::vec3(-171.29f, 197.73f, 117.0f));
	Bolide.rotate(glm::vec3(0.0f, 175.2f, 0.0f));
	Bolide.scale(glm::vec3(1.07f));
	
	//tryb wyswietlajacy same krawedzie
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	// ----- TEXTURES ---------------------------------------------------------------------------
	TextureProvider::instance().flushTextures();

	// ----- LIGHT SPACE MATRIX -----------------------------------------------------------------
	float near_plane = -1.0f, far_plane = 1000.0f;
	glm::mat4 lightProjection = glm::ortho(-150.0f, 150.0f, -150.0f, 150.0f, near_plane, far_plane);
	glm::mat4 lightView = glm::lookAt({ 200.0f, 500.0f, 1000.0f }, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0, 1.0, 0.0));
	glm::mat4 lightSpaceMatrix = lightProjection * lightView;
	
	// ----- DRIVER -----------------------------------------------------------------------------	
	glm::vec3 driverHeadPos = glm::vec3(-2.5, 4.0, 0.0);
	glm::vec3 driverHeadVec = glm::vec3(-1.0, 0.0, 0.0);

	// ---- LOCAL VARIABLES ---------------------------------------------------------------------
	float deltaTime = 0.0f;
	float currentFrame = 0.0;
	float lastFrame = glfwGetTime();

	double mouseX, mouseY;
		
	// ---- MAIN LOOP ---------------------------------------------------------------------------
	while (!glfwWindowShouldClose(window)) {
		
		currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		
		glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
		glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// ---- OBJECTS MOVEMENT ----------------------------------------------------------------
	    //driverHeadPos += glm::vec3(deltaTime * speed, 0.0f, 0.0f);
		glfwGetCursorPos(window, &mouseX, &mouseY);
		moveBolide(window, deltaTime, Bolide);
		if (cameraMode == FREE) {
			moveCamera(window, mouseX, mouseY, deltaTime);
		}
		else {
			updateLockedCamera(Bolide);
		}
		
		// ---- CAMERA METRICS ------------------------------------------------------------------
		auto view = glm::lookAt(camera.Position,camera.Position + camera.Front,camera.Up);
		auto projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.01f, 10000.0f);
		
		// ---- SHADOW PASS ---------------------------------------------------------------------
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		
		glClear(GL_DEPTH_BUFFER_BIT);

		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LESS);

		shShadows->use();
		shShadows->setTransformMatrix("lightSpaceMatrix", lightSpaceMatrix);
		Monza.draw(shShadows);
		Bolide.draw(shShadows);

		// ---- NORMAL PASS ---------------------------------------------------------------------
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

		glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// ---- SKYBOX --------------------------------------------------------------------------
		glDepthFunc(GL_LEQUAL);
		glDepthMask(GL_FALSE);
		
		glDisable(GL_BLEND);

		applyViewToShader(skyboxShader, projection, view);
		SkyBox.draw(skyboxShader);

		// ---- MODELS --------------------------------------------------------------------------
		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LESS);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			
		applyViewToShader(shader, projection, view);
		applyLightToShader(shader);
		applyShadowToShader(shader, lightSpaceMatrix);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, depthMap);

		Monza.draw(shader);
		Bolide.draw(shader);

		cout << "\rCamera position: X = " << camera.Position.x << " Y = " << camera.Position.y << " Z = " << camera.Position.z << flush;
	
		// ---- SWAP BUFFERS --------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	glfwTerminate();
	return 0;
}

void applyViewToShader(shared_ptr<Shader> shader, const glm::mat4& projection, const glm::mat4& view)
{
	shader->use();
	shader->setTransformMatrix("projection", projection);
	shader->setTransformMatrix("view", view);
}

void applyLightToShader(shared_ptr<Shader> shader)
{
	shader->use();
	shader->setVec3f("light.direction", glm::vec3(-0.3f, -1.0f, -0.4f));
	shader->setFloat("light.ambient", 0.25f);
	shader->setFloat("light.specular", 0.6f);
	shader->setVec3f("light.color", glm::vec3(1.0f, 1.0f, 1.0f));
}

void applyShadowToShader(std::shared_ptr<Shader> shader, const glm::mat4& lightSpaceMatrix)
{
	shader->use();
	shader->setTransformMatrix("lightSpaceMatrix", lightSpaceMatrix);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	
	if (key == GLFW_KEY_P == GLFW_PRESS)
		stopBolide = true;

	if (key == GLFW_KEY_0 == GLFW_PRESS)
	    cameraMode = FREE;

	if (key == GLFW_KEY_1 == GLFW_PRESS)
		cameraMode = LOCK_TO_DRIVER_HEAD;

	if (key == GLFW_KEY_2 == GLFW_PRESS)
		cameraMode = LOCK_BEHIND_BOLIDE;

	if (key == GLFW_KEY_3 == GLFW_PRESS)
		cameraMode = FRONT_VIEW;

}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (cameraMode != FREE)
		return;

	float zoomSpeed = 2.0f;

	camera.Position += camera.Front * zoomSpeed * static_cast<float>(yoffset);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (cameraMode != FREE)
		return;

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) != GLFW_PRESS)
	{
		firstMouse = true;
		return;
	}

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
		return;
	}

	float xoffset = static_cast<float>(xpos - lastX);
	float yoffset = static_cast<float>(lastY - ypos);

	lastX = xpos;
	lastY = ypos;

	xoffset *= camera.MouseSensitivity;
	yoffset *= camera.MouseSensitivity;

	camera.Yaw += xoffset;
	camera.Pitch += yoffset;

	camera.Pitch = glm::clamp(camera.Pitch, -89.0f, 89.0f);

	camera.updateCameraVectors();
}

void moveCamera(GLFWwindow* window, double mouseX, double mouseY, float deltaTime)
{
	if (cameraMode != FREE)
		return;

	const float maxCamSpeed = 1.0f;
	const int border = 100;

	float cameraSpeed = 0.0f;

	// move left
	if (mouseX < border)
	{
		cameraSpeed = (border - mouseX) / border * maxCamSpeed;

		camera.Position -= camera.Right * cameraSpeed;
	}
	// move right
	if (mouseX > SCR_WIDTH - border)
	{
		cameraSpeed =
			(mouseX - (SCR_WIDTH - border)) / border * maxCamSpeed;

		camera.Position += camera.Right * cameraSpeed;
	}
	// move up
	if (mouseY < border)
	{
		cameraSpeed =
			(border - mouseY) / border * maxCamSpeed;

		camera.Position += camera.WorldUp * cameraSpeed;
	}
	// move down
	if (mouseY > SCR_HEIGHT - border)
	{
		cameraSpeed =
			(mouseY - (SCR_HEIGHT - border)) / border * maxCamSpeed;

		camera.Position -= camera.WorldUp * cameraSpeed;
	}
}

void updateLockedCamera(Model& bolide)
{
	glm::vec3 bolidePosition = bolide.getPosition();
	float rotationY = glm::radians(bolide.getRotation().y);

	glm::vec3 forward(sin(rotationY), 0.0f, cos(rotationY));

	cout << "\Forward: X = " << forward.x << " Y = " << forward.y << " Z = " << forward.z << flush;


	float cosY = cos(rotationY);
	float sinY = sin(rotationY);

	if (cameraMode == LOCK_TO_DRIVER_HEAD)
	{
		glm::vec3 localHeadOffset = glm::vec3(-0.012f, 0.64f, -0.04f);
		glm::vec3 rotatedHeadOffset(localHeadOffset.x * cosY + localHeadOffset.z * sinY, localHeadOffset.y, -localHeadOffset.x * sinY + localHeadOffset.z * cosY);

		camera.Position = bolidePosition + rotatedHeadOffset;
		camera.Front = glm::normalize(forward);
		camera.Right = glm::normalize(glm::cross(camera.Front, camera.WorldUp));
		camera.Up = glm::normalize(glm::cross(camera.Right, camera.Front));
		return;
	}
	if (cameraMode == LOCK_BEHIND_BOLIDE)
	{
		const float distanceBehind = 6.0f;
		const float height = 1.5f;

		camera.Position = bolidePosition - forward * distanceBehind + glm::vec3(0.0f, height, 0.0f);
		glm::vec3 target = bolidePosition + glm::vec3(0.0f, 1.0f, 0.0f);
		camera.Front = glm::normalize(target - camera.Position);
		camera.Right = glm::normalize(glm::cross(camera.Front, camera.WorldUp));
		camera.Up = glm::normalize(glm::cross(camera.Right, camera.Front));
		return;
	}
	if (cameraMode == FRONT_VIEW)
	{
		const float distanceFromFront = 8.0f;
		const float height = 2.0f;

		camera.Position = bolidePosition + forward * distanceFromFront + glm::vec3(0.0f, height, 0.0f);
		glm::vec3 target = bolidePosition + glm::vec3(0.0f, 1.0f, 0.0f);
		camera.Front = glm::normalize(target - camera.Position);
		camera.Right = glm::normalize(glm::cross(camera.Front, camera.WorldUp));
		camera.Up = glm::normalize(glm::cross(camera.Right, camera.Front));
		return;
	}
}

void moveBolide(GLFWwindow* window, float deltaTime, Model& bolide)
{
	// Parameters
	const float acceleration = 15.0f;   
	const float maxSpeed = 100.0f;      
	const float turnSpeed = 30.0f;      // degrees/second
	const float friction = 15.0f;      

	if (stopBolide) {
		bolideSpeed -= bolideSpeed < 0 ? -0.5f : 0.1f;
		if (std::fabsf(bolideSpeed) - 0.5f < 0) {
			bolideSpeed = 0;
			stopBolide = false;
		}
	}

	// Acceleretion (UP) / Breaking (DOWN)
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		bolideSpeed += acceleration * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		bolideSpeed -= acceleration * deltaTime;

	// Friction
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_RELEASE)
	{
		if (bolideSpeed > 0.0f) {
			bolideSpeed -= friction * deltaTime;
			if (bolideSpeed < 0.0f) bolideSpeed = 0.0f;
		}
		else if (bolideSpeed < 0.0f) {
			bolideSpeed += friction * deltaTime;
			if (bolideSpeed > 0.0f) bolideSpeed = 0.0f;
		}
	}

	// Speed limit
	if (bolideSpeed > maxSpeed)
		bolideSpeed = maxSpeed;
	if (bolideSpeed < -maxSpeed / 2.0f)
		bolideSpeed = -maxSpeed / 2.0f;

	// Steering (LEFT/RIGHT)
	// turn only if Bolide is moving
	if (std::abs(bolideSpeed) > 0.1f)
	{
		float turnDirection = (bolideSpeed > 0.0f) ? 1.0f : -1.0f;
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
			bolide.rotate(glm::vec3(0.0f, turnSpeed * turnDirection * deltaTime, 0.0f));
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
			bolide.rotate(glm::vec3(0.0f, -turnSpeed * turnDirection * deltaTime, 0.0f));
	}

	// Movement relative to bolide rotation
	float rotationY = bolide.getRotation().y;
	float rad = glm::radians(rotationY);
	glm::vec3 direction(sin(rad), 0.0f, cos(rad));

	// Update bolide osition
	bolide.move(direction * bolideSpeed * deltaTime);
}
