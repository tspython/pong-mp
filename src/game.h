#ifndef GAME_H
#define GAME_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

// Game Constants
const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;
const float PADDLE_WIDTH = 10.0f;
const float PADDLE_HEIGHT = 100.0f;
const float BALL_RADIUS = 10.0f;

struct Paddle {
	float x, y;
	float width, height;
	float speed;
};

struct Ball {
	float x, y;
	float radius;
	float speedX, speedY;
};

struct GameState {
    Paddle player1Paddle;
    Paddle player2Paddle;
    Ball gameBall;
    unsigned int player1Score;
    unsigned int player2Score;
};

// Function Declarations
void drawPaddle(const Paddle& paddle, const glm::mat4& view, const glm::mat4& projection);
void drawBall(const Ball& ball, const glm::mat4& view, const glm::mat4& projection);
void drawScore(float x, float y, unsigned int score);
int init();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window, GameState& gameState, float deltaTime);
void updateGameState(GameState& gameState, float deltaTime);
void render(const GameState& gameState);
void initGameState(GameState* gameState);
void setupVertexData();

#endif // GAME_H
