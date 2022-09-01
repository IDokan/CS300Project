/* Start Header -------------------------------------------------------
Copyright (C) FALL2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: as1Scene.h
Purpose: main scene header code of this assignment
Language: C++
Platform: Windows SDK version: 10.0.19041.0, OS: Windows 10. GPU: NVIDIA GeForce840M. OpenGL Driver version: 10.18.15.4279, 8-24-2015
Project: sinil.kang_CS300_1
Author: Sinil Kang = sinil.kang = Colleague ID: 0052782
Creation date: 9/18/2021
End Header --------------------------------------------------------*/

#ifndef AS1_SCENE_H
#define AS1_SCENE_H

#include <../Common/Scene.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/detail/type_mat4x4.hpp>
#include <glm/vec3.hpp>
#include <vector>
#include <../Common/Cameras/Camera.h>
#include <../Common/Lights/LightManager.h>
#include <../Common/Textures/TextureManager.h>
#include <../Common/Meshes/Mesh.h>

#include <../Common/FBO/FBO.h>

#include <../Common/BVH/Intersections.h>


class Mesh;
class LineMesh;
class ObjectMesh;
class Model;
class AssimpShader;

class AS1Scene : public Scene
{
public:
	AS1Scene() = default;
	AS1Scene(int width, int height);
	virtual ~AS1Scene();

public:
	int Init();
	void LoadAllShaders();
	int preRender();
	int Render();
	int postRender();
	void CleanUp();
	void SetupNanoGUI(GLFWwindow* window);
	void UpdateGUI();

private:
	void InitGraphics();

	void AddMembersToGUI();

	void DrawMesh();
	void DrawVertexNormals();
	void DrawFaceNormals();
	void DrawSpheresAndOrbit(glm::vec3 position, glm::vec3 rotateAxis, float rotateScale = 1.f, float InitRotate = 0.f, glm::vec3 diffuseColor = glm::vec3(0.f, 0.f, 0.f), glm::mat4 matrix = glm::mat4());
	void DrawToFBO();

	void SetupCamera();
	void MoveViewOfCamera(int x, int y);
	void MovePositionOfCamera(float speed, short moveDirection);
	void UpdateCamera();

	void UpdateGUIShader();

	void InitLights();
	void UpdateLights();

	void ReloadShader();
	void ReloadMesh();

	void HybridRendering();
	void RenderDeferredObjects();
	void CopyDepthInfo();
	void RenderDebugObjects();

	void DrawGBufferRenderTargets();
	void ChangeBVH();
	void UpdateBVHTransform();

	glm::vec3 Interpolate(float t, glm::vec3 p1, glm::vec3 p2);
private:
	const GLuint vertexAttribute;
	const GLuint normalAttribute;
	const int numOfFloatVertex;
private:
	Mesh* sphereMesh;
	Mesh* centralMesh;
	Mesh* orbitMesh;
	Mesh* floorMesh;

	Model* staticModel;
	Model* dynamicModel;

	Model* dynamicPoint;
	Model* staticSphereP;
	Model* staticAABBP;
	Model* staticTriangleP;
	Model* staticPlaneP;

	Model* dynamicRay;
	Model* dynamicAABBP;
	Model* dynamicTriangleP;

	// Shaders
	GLuint programID;
	GLuint phongShading;
	ObjectMesh* mainObjMesh;
	ObjectMesh* spheres;
	ObjectMesh* floorObjMesh;
	// Normal Drawing
	GLuint normalDisplayProgramID;
	LineMesh* normalMesh;
	// Face Normals
	LineMesh* faceNormalMesh;
	LineMesh* sphereOrbit;

	// Hybrid rendering
	GLuint fboCheckShader;
	GLuint hybridFirstPass;
	AssimpShader* assimpHybridFirstPass;
	AssimpShader* assimpIntersectionDiffuse;
	GLuint hybridPhong;


	float angleOfRotate;
	glm::mat4 centralMatrix;
	glm::mat4 modelMatrix;
	glm::mat4 modelSphereMatrix;
	glm::mat4 sphereMatrix;
	glm::mat4 floorMatrix;
	glm::mat4 gbufferRenderTargetsMatrix[4];

	glm::mat4 dynamicPointMatrix;
	glm::mat4 staticSpherePMatrix;
	glm::mat4 staticAABBPMatrix;
	glm::mat4 staticTrianglePMatrix;
	glm::mat4 staticPlanePMatrix;

	glm::mat4 dynamicRayMatrix;
	glm::mat4 dynamicAABBPMatrix;
	glm::mat4 dynamicTrianglePMatrix;

	// GUI members
	GLFWwindow* displayWindow;
	GLFWwindow* guiWindow;

	bool vertexNormalFlag;
	bool faceNormalFlag;
	bool gbufferRenderTargetFlag;
	bool depthWriteFlag;

	glm::vec2 handlerDisplacement;

	Camera camera;
	glm::mat4 worldToNDC;
	Point sphericalViewPoint;
	float oldX;
	float oldY;
	Vector cartesianViewVector;
	float cameraMovementOffset;
	Vector cartesianRightVector;
	Vector cartesianUpVector;
	Point eyePoint;
	Point targetPoint;
	float fov;
	int width;
	int height;
	float aspect;
	float nearDistance;
	float farDistance;
	Vector relativeUp;
	enum
	{
		FORWARD = 0b1,
		BACKWARD = 0b10,
		RIGHTWARD = 0b100,
		LEFTWARD = 0b1000,
		UPWARD = 0b10000,
		DOWNWARD = 0b100000,
	};

	// material variables
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float ns;
	glm::vec3 intensityEmissive;
	// light variables
	LightManager lightManager;
	// environment variables
	glm::vec3 intensityFog;
	glm::vec3 attenuationConstants;

	TextureManager textureManager;


	// 0 - Phong Lighting
	// 1 - Phong Shading
	// 2 - Blinn Shading
	int currentShader;

	bool shouldReload;
	char buf[64];
	bool flip;
	Mesh::UVType uvImportType;
	bool calculateUVonCPU;

	bool reloadShader;

	MyObjReader* myReader;

	FBO frameBuffer;

	std::vector<Model*> models;
	BVH::Type staticType = BVH::Type::Sphere;
	BVH::Type dynamicType = BVH::Type::Sphere;
	bool isBVHChanged = false;
	bool stopBVH = false;

	BVH::Sphere* staticSphere = nullptr;
	BVH::Sphere* dynamicSphere = nullptr;
	BVH::AABB* staticAABB = nullptr;
	BVH::AABB* dynamicAABB = nullptr;
};

#endif // AS1_SCENE_H