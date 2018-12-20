#include "Mesh.h"



void Mesh::initModel(const IndexedModel& model)
{
	drawCount = model.indices.size();

	glGenVertexArrays(1, &vertexArrayObject); //generate a vertex array and store it in the VAO
	glBindVertexArray(vertexArrayObject); //bind the VAO (any operation that works on a VAO will work on our bound VAO - binding)

	glGenBuffers(NUM_BUFFERS, vertexArrayBuffers); //generate our buffers based of our array of data/buffers - GLuint vertexArrayBuffers[NUM_BUFFERS];
	
	glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBuffers[POSITION_VB]); //Informs opengl what type of data the buffer is.
	glBufferData(GL_ARRAY_BUFFER, model.positions.size() * sizeof(model.positions[0]), &model.positions[0], GL_STATIC_DRAW); //move the data to the GPU.
	glEnableVertexAttribArray(0);// Defining the location of the layout in the shader.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); //index of the above line ^, and how many pieces of data relevant (x,y,z).

	glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBuffers[TEXCOORD_VB]); //tell opengl what type of data the buffer is (GL_ARRAY_BUFFER), and pass the data
	glBufferData(GL_ARRAY_BUFFER, model.positions.size() * sizeof(model.texCoords[0]), &model.texCoords[0], GL_STATIC_DRAW); //move the data to the GPU - type of data, size of data, starting address (pointer) of data, where do we store the data on the GPU
	glEnableVertexAttribArray(1);// Defining the location of the layout in the shader.
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);//index of the above line ^, and how many pieces of data relevant (x,y).

	glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBuffers[NORMAL_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(model.normals[0]) * model.normals.size(), &model.normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);// Defining the location of the layout in the shader.
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);//index of the above line ^, and how many pieces of data relevant (x,y,z).

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexArrayBuffers[INDEX_VB]); //tell opengl what type of data the buffer is (GL_ARRAY_BUFFER), and pass the data
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, model.indices.size() * sizeof(model.indices[0]), &model.indices[0], GL_STATIC_DRAW); //move the data to the GPU - type of data, size of data, starting address (pointer) of data, where do we store the data on the GPU

	glBindVertexArray(0); // unbind our VAO
}

void Mesh::UpdateSphereData(glm::vec3 pos, float radius)
{
	boundingSphere.SetPos(pos); //Set position of the bounding sphere
	boundingSphere.SetRadius(radius); //set radius of the bouding sphere
}

glm::vec3 Mesh::GetSpherePos()
{
	return boundingSphere.GetPos();//returns poositoin of the bounding sphere
}

float Mesh::GetSphereRadius()
{
	return boundingSphere.GetRadius();//returns radius of the bounding sphere
}

Mesh::Mesh()
{
	drawCount = NULL; //sets draw count to null
}

void Mesh::loadModel(const std::string& filename)
{
	IndexedModel model = OBJModel(filename).ToIndexedModel(); //gets the model from the resource folder
	initModel(model); //initlises the loaded model
}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &vertexArrayObject); // delete arrays
}

void Mesh::draw()
{
	glBindVertexArray(vertexArrayObject); //bind the VAO
	
	glDrawElements(GL_TRIANGLES, drawCount, GL_UNSIGNED_INT, 0); //Speficiy the type (triangles) anbd the draw count of the model.
	
	glBindVertexArray(0); //Unbind
}

