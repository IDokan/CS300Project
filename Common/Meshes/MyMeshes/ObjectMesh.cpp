/* Start Header -------------------------------------------------------
Copyright (C) FALL2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: ObjectMesh.cpp
Purpose: Class of ObjectMesh. All object in the scene is drawn by this class
Language: C++
Platform: Windows SDK version: 10.0.19041.0, OS: Windows 10. GPU: NVIDIA GeForce840M. OpenGL Driver version: 10.18.15.4279, 8-24-2015
Project: sinil.kang_CS300_1
Author: Sinil Kang = sinil.kang = Colleague ID: 0052782
Creation date: 9/29/2021
End Header --------------------------------------------------------*/

#include "ObjectMesh.h"
#include <iostream>

ObjectMesh::ObjectMesh()
	:MyMesh(), normalBuffer(0), uvBuffer(0), indexBuffer(0), uniformBlockBuffer(0), bvh(nullptr)
{
}

ObjectMesh::~ObjectMesh()
{
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &normalBuffer);
	glDeleteBuffers(1, &uvBuffer);
	glDeleteBuffers(1, &indexBuffer);
	glDeleteBuffers(1, &uniformBlockBuffer);

	glDeleteVertexArrays(1, &VAO);

	if (bvh != nullptr)
	{
		delete bvh;
	}
}

bool ObjectMesh::Init(int vertexCount, GLfloat* vertices, GLfloat* normals, GLfloat* uvs, int indexCount, GLuint* indices)
{
	if (VAO == 0)
	{
		glGenVertexArrays(1, &VAO);
	}
	//else
	//{
	//	glDeleteVertexArrays(1, &VAO);
	//	glGenVertexArrays(1, &VAO);
	//}
	glBindVertexArray(VAO);

	if (VBO == 0)
	{
		glGenBuffers(1, &VBO);
	}
	//else
	//{
	//	glDeleteBuffers(1, &VBO);
	//	glGenBuffers(1, &VBO);
	//}
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * vertexCount, vertices, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, nullptr);

	if (normalBuffer == 0)
	{
		glGenBuffers(1, &normalBuffer);
	}
	//else
	//{
	//	glDeleteBuffers(1, &normalBuffer);
	//	glGenBuffers(1, &normalBuffer);
	//}
	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * vertexCount, normals, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, nullptr);

	if (uvBuffer == 0)
	{
		glGenBuffers(1, &uvBuffer);
	}
	//else
	//{
	//	glDeleteBuffers(1, &uvBuffer);
	//	glGenBuffers(1, &uvBuffer);
	//}
	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * vertexCount, uvs, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, nullptr);

	if (indexBuffer == 0)
	{
		glGenBuffers(1, &indexBuffer);
	}
	//else
	//{
	//	glDeleteBuffers(1, &indexBuffer);
	//	glGenBuffers(1, &indexBuffer);
	//}
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(unsigned int), indices, GL_DYNAMIC_DRAW);

	return true;
}

bool ObjectMesh::SendUniformBlockVector3s(const GLchar* blockName, const GLsizei blockPropertyCount, const GLchar* const* blockPropertyNames, const float ** blockPropertyData)
{	// Step 1.	Design layout of the uniform Blocks

	glUseProgram(programIDReference);
	glBindVertexArray(VAO);
	// Step 2.	First get the block index
	uniformBlockIndex = glGetUniformBlockIndex(programIDReference, blockName);
	if (uniformBlockIndex == GL_INVALID_INDEX)
	{
		std::cout << blockName << " is not applied, uniformBlockIndex == GL_INVALID_INDEX" << std::endl;
		return false;
	}

	// Step 3.	Allocate the block, get block indices and offsets
	GLint uniformBlockSize = 0;
	glGetActiveUniformBlockiv(programIDReference, uniformBlockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &uniformBlockSize);

	GLuint* blockPropertyIndices = new GLuint[blockPropertyCount];
	glGetUniformIndices(programIDReference, blockPropertyCount, blockPropertyNames, blockPropertyIndices);

	if (blockPropertyIndices[0] == GL_INVALID_INDEX)
	{
		std::cout << blockName << " is not applied, blockPropertyIndices[0] == GL_INVALID_INDEX" << std::endl;
		return false;
	}

	GLint* offsets = new GLint[blockPropertyCount];
	glGetActiveUniformsiv(programIDReference, blockPropertyCount, blockPropertyIndices, GL_UNIFORM_OFFSET, offsets);

	// Step 4.	Copy data into the buffer from CPU memory
	char* buffer = new char[uniformBlockSize];
	for (int i = 0; i < blockPropertyCount; ++i)
	{
		if (buffer + offsets[i] < &buffer[uniformBlockSize])
		{
			memcpy_s(buffer + offsets[i], sizeof(float) * 3, blockPropertyData[i], sizeof(float) * 3);
		}
	}

	// Step 5.	Create OpenGL buffer to manage this uniform block
	if (uniformBlockBuffer == 0)
	{
		glGenBuffers(1, &uniformBlockBuffer);
		glBindBuffer(GL_UNIFORM_BUFFER, uniformBlockBuffer);
	}
	glBufferData(GL_UNIFORM_BUFFER, uniformBlockSize, buffer, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, uniformBlockIndex, uniformBlockBuffer);
	
	delete[] buffer;

	return true;
}

bool ObjectMesh::SendUniformBlockVector3s(const GLchar* blockName, const GLsizei blockPropertyCount, const GLchar* const* blockPropertyNames, const glm::vec3* blockPropertyData)
{	// Step 1.	Design layout of the uniform Blocks

	glUseProgram(programIDReference);
	glBindVertexArray(VAO);
	// Step 2.	First get the block index
	uniformBlockIndex = glGetUniformBlockIndex(programIDReference, blockName);
	if (uniformBlockIndex == GL_INVALID_INDEX)
	{
		std::cout << blockName << " is not applied, uniformBlockIndex == GL_INVALID_INDEX" << std::endl;
		return false;
	}

	// Step 3.	Allocate the block, get block indices and offsets
	GLint uniformBlockSize = 0;
	glGetActiveUniformBlockiv(programIDReference, uniformBlockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &uniformBlockSize);

	GLuint* blockPropertyIndices = new GLuint[blockPropertyCount];
	glGetUniformIndices(programIDReference, blockPropertyCount, blockPropertyNames, blockPropertyIndices);

	if (blockPropertyIndices[0] == GL_INVALID_INDEX)
	{
		std::cout << blockName << " is not applied, blockPropertyIndices[0] == GL_INVALID_INDEX" << std::endl;
		return false;
	}

	GLint* offsets = new GLint[blockPropertyCount];
	glGetActiveUniformsiv(programIDReference, blockPropertyCount, blockPropertyIndices, GL_UNIFORM_OFFSET, offsets);

	// Step 4.	Copy data into the buffer from CPU memory
	char* buffer = new char[uniformBlockSize];
	for (int i = 0; i < blockPropertyCount; ++i)
	{
		if (buffer + offsets[i] < &buffer[uniformBlockSize])
		{
			memcpy_s(buffer + offsets[i], sizeof(float) * 3, &blockPropertyData[i], sizeof(float) * 3);
		}
	}

	// Step 5.	Create OpenGL buffer to manage this uniform block
	if (uniformBlockBuffer == 0)
	{
		glGenBuffers(1, &uniformBlockBuffer);
		glBindBuffer(GL_UNIFORM_BUFFER, uniformBlockBuffer);
	}
	glBufferData(GL_UNIFORM_BUFFER, uniformBlockSize, buffer, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, uniformBlockIndex, uniformBlockBuffer);

	delete[] buffer;

	return true;
}

void ObjectMesh::PrepareDrawing()
{
	glUseProgram(programIDReference);

	glBindVertexArray(VAO);
	if (uniformBlockBuffer != 0)
	{
		glBindBuffer(GL_UNIFORM_BUFFER, uniformBlockBuffer);
	}

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
}

void ObjectMesh::Draw(int indexCount)
{
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

void ObjectMesh::SetBVH(BVH::BVHGeometry* newBVH)
{
	bvh = newBVH;
}
