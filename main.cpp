// Credit to youtube.com/@VictorGordan for the OpenGL tutorial that provided the simple foundation for this project - Texture.cpp, Texture.h, shaderClass.cpp, shaderClass.h, default.frag (modified), default.vert

#include <iostream>
#include <vector>
#include <array>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <chrono>
#include <cmath>

#include "Texture.h"
#include "shaderClass.h"

#define PI 3.14159265358979323846

struct Vector2
{
	float x, y;
};

struct Vector3
{
	float x, y, z;
};

struct Vertex 
{
	Vector3 position;
	Vector3 color;
};

struct TVertex {
	Vector3 position;
	Vector3 color;
	Vector2 textureCoordinates;
};

class Object
{
public:
	Vector2 position;
	float rotation;
	Vector2 scale;
	bool isKinetic;

	std::vector<Vector3> initialVertexPositions; // that define the object's shape

	Vector2 velocity;
	Vector2 acceleration;
	GLuint VAO, VBO, EBO;
	
	std::vector<unsigned int> indices;

	virtual void init() = 0;

	virtual void setupBuffers() = 0;

	virtual void Delete() = 0;

	Object(bool kinetic = false) : position({ 0, 0 }), rotation(0), scale({ 1, 1 }), isKinetic(kinetic), velocity({ 0, 0 }), acceleration({ 0, 0 }) {}

	void update(float deltaTime) {
		if (isKinetic) {
			velocity.x += acceleration.x * deltaTime;
			velocity.y += acceleration.y * deltaTime;

			position.x += velocity.x * deltaTime;
			position.y += velocity.y * deltaTime;
			
			updateVertices();
		}
	}

	virtual void updateBuffers() = 0;

	virtual void updateVertices() = 0;

	virtual void render() = 0;
};

class Object2D : public Object {
public:
    std::vector<Vertex> vertices;

	Object2D(bool kinetic = false) : Object(kinetic) {}

    void setupBuffers() override {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Vector3)));
		glEnableVertexAttribArray(1);

        glBindVertexArray(0);
    }

	void render() override {
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	void updateBuffers() override
	{
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void Delete() override {
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
	}

	void updateVertices() override {
		// Calculate the center of the object (origin for rotation)
		Vector2 origin = { 0.0f, 0.0f };
		for (const auto& v : initialVertexPositions) {
			origin.x += v.x;
			origin.y += v.y;
		}
		origin.x /= initialVertexPositions.size();
		origin.y /= initialVertexPositions.size();

		// Precompute sine and cosine for rotation
		float rad = rotation * (PI / 180.0f);
		float cosTheta = std::cos(rad);
		float sinTheta = std::sin(rad);

		for (size_t i = 0; i < initialVertexPositions.size(); ++i) {
			// Local position relative to the origin
			float localX = initialVertexPositions[i].x - origin.x;
			float localY = initialVertexPositions[i].y - origin.y;

			// Apply scaling
			localX *= scale.x;
			localY *= scale.y;

			// Apply rotation
			float rotatedX = localX * cosTheta - localY * sinTheta;
			float rotatedY = localX * sinTheta + localY * cosTheta;

			// Apply world position (translation)
			vertices[i].position.x = rotatedX + origin.x + position.x;
			vertices[i].position.y = rotatedY + origin.y + position.y;
			vertices[i].position.z = 0.0f; // Only supporting 2D for now, so Z stays at 0.0f
		}
		updateBuffers();
	}
};

class TObject2D : public Object {
public:
	std::vector<TVertex> vertices;
	Texture* texture;

	TObject2D(bool kinetic = false) : Object(kinetic) {}

	void setupBuffers() override {
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(TVertex), vertices.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TVertex), (void*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(TVertex), (void*)(sizeof(Vector3)));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(TVertex), (void*)(2 * sizeof(Vector3)));
		glEnableVertexAttribArray(2);
		
		glBindVertexArray(0);
	}

	void render() override {
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		texture->Bind();
	}

	void updateBuffers() override 
	{
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void Delete() override {
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
		texture->Delete();
	}

	void updateVertices() override {
		// Calculate the center of the object (origin for rotation)
		Vector2 origin = { 0.0f, 0.0f };
		for (const auto& v : initialVertexPositions) {
			origin.x += v.x;
			origin.y += v.y;
		}
		origin.x /= initialVertexPositions.size();
		origin.y /= initialVertexPositions.size();

		// Precompute sine and cosine for rotation
		float rad = rotation * (PI / 180.0f);
		float cosTheta = std::cos(rad);
		float sinTheta = std::sin(rad);

		for (size_t i = 0; i < initialVertexPositions.size(); ++i) {
			// Local position relative to the origin
			float localX = initialVertexPositions[i].x - origin.x;
			float localY = initialVertexPositions[i].y - origin.y;

			// Apply scaling
			localX *= scale.x;
			localY *= scale.y;

			// Apply rotation
			float rotatedX = localX * cosTheta - localY * sinTheta;
			float rotatedY = localX * sinTheta + localY * cosTheta;

			// Apply world position (translation)
			vertices[i].position.x = rotatedX + origin.x + position.x;
			vertices[i].position.y = rotatedY + origin.y + position.y;
			vertices[i].position.z = 0.0f; // Only supporting 2D for now, so Z stays at 0.0f
		}

		updateBuffers();
	}
};

class Triangle : public Object2D {
public:
    Triangle() : Object2D(true) {}

    void init() override {
		initialVertexPositions = {
			{-0.1f, -0.1f, 0.0f },
			{ 0.1f, -0.1f, 0.0f },
			{ 0.0f, 0.1f, 0.0f },
		};
		
		vertices = {
			{{-0.1f, -0.1f, 0.0f}, {1.0f, 1.0f, 1.0f}},
			{{0.1f, -0.1f, 0.0f}, {1.0f, 1.0f, 1.0f}},
			{{0.0f, 0.1f, 0.0f}, {1.0f, 1.0f, 1.0f}},
		};
        indices = {0, 1, 2};

        setupBuffers();
    }
};

class TextureSquare : public TObject2D
{
public:
	const char* texturePath;
	Shader* shader;
	TextureSquare(bool kinetic = false, const char* texturePath = nullptr, Shader* shader = nullptr) : TObject2D(kinetic), texturePath(texturePath), shader(shader) {}

	float offset = 0.5f;

	void init() override {
		initialVertexPositions = {
			{-0.1f + offset, -0.1f + offset, 0.0f},
			{-0.1f + offset, 0.1f + offset, 0.0f},
			{0.1f + offset, 0.1f + offset, 0.0f},
			{0.1f + offset, -0.1f + offset, 0.0f}
		};

		vertices = {
			{{-0.1f + offset, -0.1f + offset, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
			{{-0.1f + offset, 0.1f + offset, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}},
			{{0.1f + offset, 0.1f + offset, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
			{{0.1f + offset, -0.1f + offset, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
		};

		indices = {
			0, 2, 1, // Upper Triangle
			0, 3, 2 // Lower Triangle
		};

		setupBuffers();

		texture = new Texture(texturePath, GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
		texture->texUnit(*shader, "tex0", 0);
	}
};

class Scene {
public:
    std::vector<Object*> objects;

    void addObject(Object* obj) {
        objects.push_back(obj);
    }

    void update(float deltaTime) {
        for (auto& obj : objects) {
            obj->update(deltaTime);
        }
    }

	void render(Shader& shader) {
		for (auto& obj : objects) {
			GLuint uniID = glGetUniformLocation(shader.ID, "useTexture");

			// Check if the object is a textured object
			TObject2D* texturedObj = dynamic_cast<TObject2D*>(obj);
			if (texturedObj && texturedObj->texture) {
				glUniform1i(uniID, 1);
			}
			else {
				glUniform1i(uniID, 0);
			}

			obj->render();
		}
	}

	void Delete() {
		for (auto& obj : objects) {
			obj->Delete();
		}
	}
};

int main() {
    if (!glfwInit()) return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 800, "Claymore 2D Framework", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    gladLoadGL();

	glViewport(0, 0, 800, 800);

	Shader shader("default.vert", "default.frag");

    Scene scene;
    Triangle player;
    player.init();
    scene.addObject(&player);

	TextureSquare turtle(false, "turtle.png", &shader);
	turtle.init();
	scene.addObject(&turtle);

    auto lastTime = std::chrono::high_resolution_clock::now();

    while (!glfwWindowShouldClose(window)) {
        auto currentTime = std::chrono::high_resolution_clock::now();
        float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
        lastTime = currentTime;

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

		shader.Activate();

        scene.update(deltaTime);
        scene.render(shader);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

	scene.Delete();
	shader.Delete();

    glfwTerminate();
    return 0;
}
