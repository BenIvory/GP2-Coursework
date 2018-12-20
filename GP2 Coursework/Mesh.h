#pragma once
#include <glm\glm.hpp>
#include <GL\glew.h>
#include <string>
#include "ObjectLoader.h"

struct Vertex
{
public:
	Vertex(const glm::vec3& pos, const glm::vec2& texCoord)
	{
		this->pos = pos;
		this->texCoord = texCoord;
		this->normal = normal;
	}

	glm::vec3* GetPos() { return &pos; }
	glm::vec2* GetTexCoord() { return &texCoord; }
	glm::vec3* GetNormal() { return &normal; }

private:
	glm::vec3 pos;
	glm::vec2 texCoord;
	glm::vec3 normal;
};


struct BoundingSphere
{
private:
	glm::vec3 pos;
	float radius;

public:
	BoundingSphere()
	{
		pos = glm::vec3();
		radius = 0.0f;
	}
	BoundingSphere(glm::vec3 pos, float radius)
	{
		this->pos = pos;
		this->radius = radius;
	}

	void SetPos(glm::vec3 pos)
	{
		this->pos = pos;
	}
	void SetRadius(float radius)
	{
		this->radius = radius;
	}
	glm::vec3 GetPos()
	{
		return pos;
	}
	float GetRadius()
	{
		return radius;
	}
};


class Mesh
{
public:
	Mesh();
	~Mesh();

	void draw();
	void loadModel(const std::string& filename);
	void initModel(const IndexedModel& model);
	
	void UpdateSphereData(glm::vec3 pos, float radius);
	glm::vec3 GetSpherePos();
	float GetSphereRadius();

private:
	enum
	{
		POSITION_VB,
		TEXCOORD_VB,
		NORMAL_VB,
		INDEX_VB,
		NUM_BUFFERS
	};

	GLuint vertexArrayObject;
	GLuint vertexArrayBuffers[NUM_BUFFERS]; // create our array of buffers
	unsigned int drawCount; //how much of the vertexArrayObject do we want to draw

	BoundingSphere boundingSphere = BoundingSphere();
};
