#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "game.h"
#include "shader.h"

unsigned int shaderProgram;
unsigned int VAO, VBO;

int init() {
	GLFWwindow* window;
	if(!glfwInit()) return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Pong", NULL, NULL);
	if (!window) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
		
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	Shader::load("../shaders/paddleVertexShader.glsl", "../shaders/paddleFragmentShader.glsl", shaderProgram);
	Shader::load("../shaders/ballVertexShader.glsl", "../shaders/ballFragmentShader.glsl", shaderProgram);
	setupVertexData();
	
	float lastFrame = 0.0f;
	
	GameState gameState;		
	initGameState(&gameState);

	while(!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		float currentFrame = glfwGetTime();
		float deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		
		processInput(window, gameState, deltaTime);
		//updateGameState(gameState, deltaTime);

		render(gameState);

		glfwSwapBuffers(window);

		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0,0, width, height);
}

void setupVertexData() {
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.5f, 0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f,
	};	

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

void initGameState(GameState* gameState) {
	gameState->player1Paddle.x = 10.0f;
	gameState->player1Paddle.y = WINDOW_HEIGHT / 2.0f - PADDLE_HEIGHT / 2.0f;
	gameState->player1Paddle.width = PADDLE_WIDTH;
	gameState->player1Paddle.height = PADDLE_HEIGHT;
	gameState->player1Paddle.speed = 500.0f;
	
	gameState->player2Paddle.x = WINDOW_WIDTH - 20.0f;
	gameState->player2Paddle.y = WINDOW_HEIGHT / 2.0f - PADDLE_HEIGHT / 2.0f;
	gameState->player2Paddle.width = PADDLE_WIDTH;
	gameState->player2Paddle.height = PADDLE_HEIGHT;
	gameState->player2Paddle.speed = 500.0f;

	gameState->gameBall.x = WINDOW_WIDTH / 2.0f;
	gameState->gameBall.y = WINDOW_HEIGHT / 2.0f;
	gameState->gameBall.radius = BALL_RADIUS;
	gameState->gameBall.speedX = 200.0f;
	gameState->gameBall.speedY = 200.0f;

	gameState->player1Score = 0;
	gameState->player2Score = 0;
}

void drawPaddle(const Paddle& paddle, const glm::mat4& view, const glm::mat4& projection) {
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(paddle.x, paddle.y, 0.0f));
    model = glm::scale(model, glm::vec3(paddle.width, paddle.height, 1.0f));

    unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

    unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

void drawBall(const Ball& ball, const glm::mat4& view, const glm::mat4& projection) {
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(ball.x, ball.y, 0.0f));
    model = glm::scale(model, glm::vec3(ball.radius * 2.0f)); 

    unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

    unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4); }

void drawScore(float x, float y, unsigned int score){}

void processInput(GLFWwindow* window, GameState& gameState, float deltaTime) {
	// Player 1 Input (WASD)
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		gameState.player1Paddle.y += gameState.player1Paddle.speed * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		gameState.player1Paddle.y -= gameState.player1Paddle.speed * deltaTime;

	// Player 2 Input (ARROWS)
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		gameState.player2Paddle.y += gameState.player2Paddle.speed * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		gameState.player2Paddle.y -= gameState.player2Paddle.speed * deltaTime;

	if (gameState.player1Paddle.y < 0)
		gameState.player1Paddle.y = 0;
	else if (gameState.player1Paddle.y > WINDOW_HEIGHT - gameState.player1Paddle.height)
		gameState.player1Paddle.y = WINDOW_HEIGHT - gameState.player1Paddle.height;

	if (gameState.player2Paddle.y < 0)
		gameState.player2Paddle.y = 0;
	else if (gameState.player2Paddle.y > WINDOW_HEIGHT - gameState.player2Paddle.height)
		gameState.player2Paddle.y = WINDOW_HEIGHT - gameState.player2Paddle.height;
}

void updateGameState(GameState& gameState, float deltaTime) {}

void render(const GameState& gameState) {
    // Set up view and projection matrices
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(WINDOW_WIDTH), 0.0f, static_cast<float>(WINDOW_HEIGHT), -1.0f, 1.0f);

	drawPaddle(gameState.player1Paddle, view, projection);
	drawPaddle(gameState.player2Paddle, view, projection);
	drawBall(gameState.gameBall, view, projection);
    // TODO: Draw other pong elements
}

