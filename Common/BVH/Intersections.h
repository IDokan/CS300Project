/* Start Header -------------------------------------------------------
Copyright (C) FALL2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Intersections.h
Purpose: Header file of bvh geometries' intersections.
Language: C++
Platform: Windows SDK version: 10.0.19041.0, OS: Windows 10. GPU: NVIDIA GeForce840M. OpenGL Driver version: 10.18.15.4279, 8-24-2015
Project: sinil.kang_CS350_1
Author: Sinil Kang = sinil.kang = Colleague ID: 0052782
Creation date: 3/1/2022
End Header --------------------------------------------------------*/
#pragma once
#include <iostream>
#include <glm/glm.hpp>

namespace BVH
{
	enum class Type
	{
		Point3D,
		Plane,
		Triangle,
		Sphere,
		AABB,
		Ray,
	};
	struct BVHGeometry
	{
		BVHGeometry()
		{

		}
		virtual ~BVHGeometry() {}
		virtual void SetPosition(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);
		virtual void SetPosition(glm::vec3 p1);
		virtual void SetPosition(glm::vec3 v, float s);
		virtual void SetPosition(glm::vec3 p1, glm::vec3 p2);
		virtual void SetPosition(glm::mat4 matrix) = 0;
	};

	struct Point3D : public BVHGeometry
	{
		Point3D(glm::vec3 position)
			: BVHGeometry(), position(position), initPosition(position)
		{
		}

		//virtual void SetPosition(glm::vec3 p1)
		//{
		//	position = p1;
		//}

		virtual void SetPosition(glm::mat4 matrix)
		{
			glm::vec4 newPos = glm::vec4(initPosition, 1.f);
			newPos = matrix * newPos;
			position.x = newPos.x;
			position.y = newPos.y;
			position.z = newPos.z;
		}

		// (x, y, z)
		glm::vec3 position;
		const glm::vec3 initPosition;
	};

	struct Plane : public BVHGeometry
	{
		Plane(glm::vec3 n, glm::vec3 pointOnPlane)
			:BVHGeometry(), normal(glm::normalize(n)), p(pointOnPlane), initNormal(normal), initPointOnPlane(p)
		{
		}
		//virtual void SetPosition(glm::vec3 v, glm::vec3 pointOnPlane)
		//{
		//	normal = v;
		//	p = pointOnPlane;
		//}

		virtual void SetPosition(glm::mat4 matrix)
		{
			glm::vec4 transformedNormal = matrix * glm::vec4(initNormal, 0.f);
			normal = glm::normalize(glm::vec3(transformedNormal.x, transformedNormal.y, transformedNormal.z));

			glm::vec4 transformedPointOnPlane = matrix * glm::vec4(initPointOnPlane, 1.f);
			p = glm::vec3(transformedPointOnPlane.x, transformedPointOnPlane.y, transformedPointOnPlane.z);
		}
		glm::vec3 normal;
		glm::vec3 p;

		const glm::vec3 initNormal;
		const glm::vec3 initPointOnPlane;
	};

	struct Triangle : public BVHGeometry
	{
		Triangle(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2)
			:BVHGeometry(), v0(v0), v1(v1), v2(v2), c_v0(v0), c_v1(v1), c_v2(v2)
		{

		}
		//virtual void SetPosition(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3)
		//{
		//	v0 = p1;
		//	v1 = p2;
		//	v2 = p3;
		//}

		virtual void SetPosition(glm::mat4 matrix)
		{
			glm::vec4 transformedV0 = matrix * glm::vec4(c_v0, 1.f);
			v0 = glm::vec3(transformedV0.x, transformedV0.y, transformedV0.z);

			glm::vec4 transformedV1 = matrix * glm::vec4(c_v1, 1.f);
			v1 = glm::vec3(transformedV1.x, transformedV1.y, transformedV1.z);

			glm::vec4 transformedV2 = matrix * glm::vec4(c_v2, 1.f);
			v2 = glm::vec3(transformedV2.x, transformedV2.y, transformedV2.z);
		}
		glm::vec3 v0;
		glm::vec3 v1;
		glm::vec3 v2;

		const glm::vec3 c_v0;
		const glm::vec3 c_v1;
		const glm::vec3 c_v2;
	};

	struct Sphere : public BVHGeometry
	{
		Sphere(glm::vec3 p, float r)
			:BVHGeometry(), position(p), radius(r), initPosition(p), initRadius(r)
		{

		}
		//virtual void SetPosition(glm::vec3 v, float s)
		//{
		//	position = v;
		//	radius = s;
		//}

		virtual void SetPosition(glm::mat4 matrix)
		{
			glm::vec4 v4Pos = (matrix * glm::vec4(initPosition, 1.f));
			position = glm::vec3(v4Pos.x, v4Pos.y, v4Pos.z);
			glm::vec4 v4Radius = (matrix * glm::vec4(initRadius, initRadius, initRadius, 0.f));
			float finalRadius = 0.f;
			for (int i = 0; i < 3; i++)
			{
				if (finalRadius < v4Radius[i])
				{
					finalRadius = v4Radius[i];
				}
			}
			
			radius = finalRadius;
		}

		glm::vec3 position;
		float radius;

		const glm::vec3 initPosition;
		const float initRadius;
	};

	struct AABB : public BVHGeometry
	{
		AABB(glm::vec3 c, glm::vec3 h)
			:BVHGeometry(), center(c), halfExtents(h), initCenter(c), initHalfExtents(h)
		{

		}
		//virtual void SetPosition(glm::vec3 p1, glm::vec3 p2)
		//{
		//	center = p1;
		//	halfExtents = p2;
		//}

		virtual void SetPosition(glm::mat4 matrix)
		{
			glm::vec4 transformedCenter = matrix * glm::vec4(initCenter, 1.f);
			center = glm::vec3(transformedCenter.x, transformedCenter.y, transformedCenter.z);

			glm::vec4 transformedHalf = matrix * glm::vec4(initHalfExtents, 0.f);
			halfExtents = glm::vec3(transformedHalf.x, transformedHalf.y, transformedHalf.z);
		}
		glm::vec3 center;
		glm::vec3 halfExtents;

		const glm::vec3 initCenter;
		const glm::vec3 initHalfExtents;
	};

	struct Ray : public BVHGeometry
	{
		Ray(glm::vec3 s, glm::vec3 d)
			:BVHGeometry(), start(s), direction(glm::normalize(d)), initStart(start), initDirection(direction)
		{
		}
		//virtual void SetPosition(glm::vec3 p1, glm::vec3 p2)
		//{
		//	start = p1;
		//	direction = glm::normalize(p2);
		//}

		virtual void SetPosition(glm::mat4 matrix)
		{
			glm::vec4 transformedStart = matrix * glm::vec4(initStart, 1.f);
			start = glm::vec3(transformedStart.x, transformedStart.y, transformedStart.z);

			glm::vec4 transformedDirection = matrix * glm::vec4(initDirection, 0.f);
			direction = normalize(glm::vec3(transformedDirection.x, transformedDirection.y, transformedDirection.z));
		}
		glm::vec3 start;
		glm::vec3 direction;
		const glm::vec3 initStart;
		const glm::vec3 initDirection;
	};

	bool Intersection(BVHGeometry* b1, BVHGeometry* b2, Type b1Type, Type b2Type);

	bool Intersection(const Point3D& p, const Sphere& s);
	bool Intersection(const Sphere& s, Point3D& p);
	bool Intersection(Sphere* s, Point3D* p);
	bool Intersection(Point3D* s, Sphere* p);

	bool Intersection(const Sphere& lhs, const Sphere& rhs);
	bool Intersection(Sphere* lhs, Sphere* rhs);

	bool Intersection(const Point3D& p, const AABB& aabb);
	bool Intersection(const AABB& aabb, const Point3D& p);
	bool Intersection(Point3D* p, AABB* aabb);
	bool Intersection(AABB* aabb, Point3D* p);

	bool Intersection(const Point3D& p, const Triangle& triangle);
	bool Intersection(const Triangle& triangle, const Point3D& p);
	bool Intersection(Point3D* p, Triangle* triangle);
	bool Intersection(Triangle* triangle, Point3D* p);


	bool Intersection(const AABB& lhs, const AABB& rhs);
	bool Intersection(AABB* lhs, AABB* rhs);

	bool Intersection(const Ray& r, const Plane& p);
	bool Intersection(const Plane& p, const Ray& r);
	bool Intersection(Ray* r, Plane* p);
	bool Intersection(Plane* p, Ray* r);


	bool Intersection(const Ray& r, const Triangle& t);
	bool Intersection(const Triangle& t, const Ray& r);
	bool Intersection(Ray* r, Triangle* t);
	bool Intersection(Triangle* t, Ray* r);


	bool Intersection(const Ray& r, const Sphere& s);
	bool Intersection(const Sphere& s, const Ray& r);
	bool Intersection(Ray* r, Sphere* s);
	bool Intersection(Sphere* s, Ray* r);


	bool Intersection(const Ray& r, const AABB& aabb);
	bool Intersection(const AABB& aabb, const Ray& r);
	bool Intersection(Ray* r, AABB* aabb);
	bool Intersection(AABB* aabb, Ray* r);

	bool Intersection(const Plane& plane, const Point3D& point);
	bool Intersection(const Point3D& point, const Plane& plane);
	bool Intersection(Plane* plane, Point3D* point);
	bool Intersection(Point3D* point, Plane* plane);


	bool Intersection(const Plane& p, const Triangle& t);
	bool Intersection(const Triangle& t, const Plane& p);
	bool Intersection(Plane* p, Triangle* t);
	bool Intersection(Triangle* t, Plane* p);


	bool Intersection(const Plane& p, const Sphere& s);
	bool Intersection(const Sphere& s, const Plane& p);
	bool Intersection(Plane* p, Sphere* s);
	bool Intersection(Sphere* s, Plane* p);

	bool Intersection(const Plane& p, const AABB& a);
	bool Intersection(const AABB& a, const Plane& p);
	bool Intersection(Plane* p, AABB* a);
	bool Intersection(AABB* a, Plane* p);

	bool Intersection(const Sphere& s, const AABB& a);
	bool Intersection(const AABB& a, const Sphere& s);
	bool Intersection(Sphere* s, AABB* a);
	bool Intersection(AABB* a, Sphere* s);
}