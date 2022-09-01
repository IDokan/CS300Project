/* Start Header -------------------------------------------------------
Copyright (C) FALL2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Intersections.cpp
Purpose: Source file of bvh geometries' intersections.
Language: C++
Platform: Windows SDK version: 10.0.19041.0, OS: Windows 10. GPU: NVIDIA GeForce840M. OpenGL Driver version: 10.18.15.4279, 8-24-2015
Project: sinil.kang_CS350_1
Author: Sinil Kang = sinil.kang = Colleague ID: 0052782
Creation date: 3/1/2022
End Header --------------------------------------------------------*/

#include <glm/gtc/epsilon.hpp>
#include <../Common/BVH/Intersections.h>

bool FloatEqual(float val, float target)
{
	const float e = glm::epsilon<float>();

	return (val <= target + e) && (val >= target - e);
}

std::pair<float, float> MySlabIntersection(const BVH::Ray& r, glm::vec3 normal, float d0, float d1)
{
	const float e = glm::epsilon<float>();
	const float NdotStart = dot(normal, r.start);
	const float NdotDir = dot(normal, r.direction);


	// Ray intersects both slab planes
	if (!FloatEqual(NdotDir, 0.f))
	{
		float t0 = -(d0 + NdotStart) / NdotDir;
		float t1 = -(d1 + NdotStart) / NdotDir;

		if (t0 > t1)
		{
			std::swap(t0, t1);
		}

		return std::pair(t0, t1);
	}
	// Ray is parallel to slab planes
	else
	{
		float s0 = NdotStart + d0;
		float s1 = NdotStart + d1;

		if ((s0 < e) ? (s1 >= e) : (s1 < e))
		{
			// ray is between planes
			return std::pair(0.f, INFINITY);
		}
		else
		{
			// Ray is outside planes, return empty interval
			return std::pair(INFINITY, 0.f);
		}
	}
}

bool BVH::Intersection(BVHGeometry* b1, BVHGeometry* b2, Type b1Type, Type b2Type)
{


	Point3D* p1 = nullptr;
	Plane* plane1 = nullptr;
	Triangle* triangle1 = nullptr;
	Sphere* sphere1 = nullptr;
	AABB* aabb1 = nullptr;
	Ray* ray1 = nullptr;

	Point3D* p2 = nullptr;
	Plane* plane2 = nullptr;
	Triangle* triangle2 = nullptr;
	Sphere* sphere2 = nullptr;
	AABB* aabb2 = nullptr;
	Ray* ray2 = nullptr;

	switch (b1Type)
	{
	case	Type::Point3D:
		p1 = dynamic_cast<Point3D*>(b1);
		switch (b2Type)
		{
		case BVH::Type::Point3D:
			return false;
			break;
		case BVH::Type::Plane:
			plane2 = dynamic_cast<Plane*>(b2);
			return Intersection(p1, plane2);
			break;
		case BVH::Type::Triangle:
			triangle2 = dynamic_cast<Triangle*>(b2);
			return Intersection(p1, triangle2);
			break;
		case BVH::Type::Sphere:
			sphere2= dynamic_cast<Sphere*>(b2);
			return Intersection(p1, sphere2);
			break;
		case BVH::Type::AABB:
			aabb2 = dynamic_cast<AABB*>(b2);
			return Intersection(p1, aabb2);
			break;
		case BVH::Type::Ray:
			return false;
			break;
		default:
			break;
		}
		break;
	case Type::Plane:
		plane1 = dynamic_cast<Plane*>(b1);
		switch (b2Type)
		{
		case BVH::Type::Point3D:
			p2 = dynamic_cast<Point3D*>(b2);
			return Intersection(plane1, p2);
			break;
		case BVH::Type::Plane:
			return false;
			break;
		case BVH::Type::Triangle:
			triangle2 = dynamic_cast<Triangle*>(b2);
			return Intersection(plane1, triangle2);
			break;
		case BVH::Type::Sphere:
			sphere2 = dynamic_cast<Sphere*>(b2);
			return Intersection(plane1, sphere2);
			break;
		case BVH::Type::AABB:
			aabb2 = dynamic_cast<AABB*>(b2);
			return Intersection(plane1, aabb2);
			break;
		case BVH::Type::Ray:
			ray2 = dynamic_cast<Ray*>(b2);
			return Intersection(plane1, ray2);
			break;
		default:
			break;
		}
		break;
	case Type::Triangle:
		triangle1 = dynamic_cast<Triangle*>(b1);
		switch (b2Type)
		{
		case BVH::Type::Point3D:
			return false;
			break;
		case BVH::Type::Plane:
			plane2 = dynamic_cast<Plane*>(b2);
			return Intersection(triangle1, plane2);
			break;
		case BVH::Type::Triangle:
			return false;
			break;
		case BVH::Type::Sphere:
			return false;
			break;
		case BVH::Type::AABB:
			return false;
			break;
		case BVH::Type::Ray:
			ray2 = dynamic_cast<Ray*>(b2);
			return Intersection(triangle1, ray2);
			break;
		default:
			break;
		}
		break;
	case Type::Sphere:
		sphere1 = dynamic_cast<Sphere*>(b1);
		switch (b2Type)
		{
		case BVH::Type::Point3D:
			p2 = dynamic_cast<Point3D*>(b2);
			return Intersection(sphere1, p2);
			break;
		case BVH::Type::Plane:
			plane2 = dynamic_cast<Plane*>(b2);
			return Intersection(sphere1, plane2);
			break;
		case BVH::Type::Triangle:
			return false;
			break;
		case BVH::Type::Sphere:
			sphere2 = dynamic_cast<Sphere*>(b2);
			return Intersection(sphere1, sphere2);
			break;
		case BVH::Type::AABB:
			aabb2 = dynamic_cast<AABB*>(b2);
			return Intersection(sphere1, aabb2);
			break;
		case BVH::Type::Ray:
			ray2 = dynamic_cast<Ray*>(b2);
			return Intersection(sphere1, ray2);
			break;
		default:
			break;
		}
		break;
	case Type::AABB:
		aabb1 = dynamic_cast<AABB*>(b1);
		switch (b2Type)
		{
		case BVH::Type::Point3D:
			p2 = dynamic_cast<Point3D*>(b2);
			return Intersection(aabb1, p2);
			break;
		case BVH::Type::Plane:
			plane2 = dynamic_cast<Plane*>(b2);
			return Intersection(aabb1, plane2);
			break;
		case BVH::Type::Triangle:
			return false;
			break;
		case BVH::Type::Sphere:
			sphere2 = dynamic_cast<Sphere*>(b2);
			return Intersection(aabb1, sphere2);
			break;
		case BVH::Type::AABB:
			aabb2 = dynamic_cast<AABB*>(b2);
			return Intersection(aabb1, aabb2);
			break;
		case BVH::Type::Ray:
			ray2 = dynamic_cast<Ray*>(b2);
			return Intersection(aabb1, ray2);
			break;
		default:
			break;
		}
		break;
	case Type::Ray:
		ray1 = dynamic_cast<Ray*>(b1);
		switch (b2Type)
		{
		case BVH::Type::Point3D:
			return false;
			break;
		case BVH::Type::Plane:
			plane2 = dynamic_cast<Plane*>(b2);
			return Intersection(ray1, plane2);
			break;
		case BVH::Type::Triangle:
			triangle2 = dynamic_cast<Triangle*>(b2);
			return Intersection(ray1, triangle2);
			break;
		case BVH::Type::Sphere:
			sphere2 = dynamic_cast<Sphere*>(b2);
			return Intersection(ray1, sphere2);
			break;
		case BVH::Type::AABB:
			aabb2 = dynamic_cast<AABB*>(b2);
			return Intersection(ray1, aabb2);
			break;
		case BVH::Type::Ray:
			return false;
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}


	return false;
}

bool BVH::Intersection(const Point3D& p, const Sphere& s)
{
	float d =
		((p.position.x - s.position.x) * (p.position.x - s.position.x)) +
		((p.position.y - s.position.y) * (p.position.y - s.position.y)) +
		((p.position.z - s.position.z) * (p.position.x - s.position.z));
	float r = s.radius * s.radius;

	return (d <= r);
}

bool BVH::Intersection(const Sphere& s, Point3D& p)
{
	return Intersection(p, s);
}

bool BVH::Intersection(Sphere* s, Point3D* p)
{
	return Intersection(*p, *s);
}

bool BVH::Intersection(Point3D* s, Sphere* p)
{
	return Intersection(*p, *s);
}

bool BVH::Intersection(const Sphere& lhs, const Sphere& rhs)
{
	Point3D p = rhs.position;
	Sphere s(lhs.position, lhs.radius + rhs.radius);

	return Intersection(p, s);
}

bool BVH::Intersection(Sphere* lhs, Sphere* rhs)
{
	return Intersection(*lhs, *rhs);
}

bool BVH::Intersection(const Point3D& p, const AABB& aabb)
{
	// Compare each axis (x, y, z)
	for (int i = 0; i < 3; i++)
	{
		if (p.position[i] < aabb.center[i] - aabb.halfExtents[i] ||
			p.position[i] > aabb.center[i] + aabb.halfExtents[i])
		{
			return false;
		}
	}

	return true;
}

bool BVH::Intersection(const AABB& aabb, const Point3D& p)
{
	return Intersection(p, aabb);
}

bool BVH::Intersection(Point3D* p, AABB* aabb)
{
	return Intersection(*p, *aabb);
}

bool BVH::Intersection(AABB* aabb, Point3D* p)
{
	return Intersection(*p, *aabb);
}

bool BVH::Intersection(const Point3D& p, const Triangle& triangle)
{
	glm::vec3 v0 = p.position - triangle.v0;
	glm::vec3 v1 = triangle.v1 - triangle.v0;
	glm::vec3 v2 = triangle.v2 - triangle.v0;

	float a = glm::dot(v1, v1);
	float b = glm::dot(v2, v1);
	float e = glm::dot(v0, v1);
	float c = glm::dot(v1, v2);
	float d = glm::dot(v2, v2);
	float f = glm::dot(v0, v2);

	float u = (e * d - f * b) / (a * d - c * b);
	float v = (a * f - c * e) / (a * d - c * b);
	float w = 1 - u - v;

	return (u >= 0.f) && (u <= 1.f)&&
		(v >= 0.f) && (v <= 1.f)&&
		(w >= 0.f) && (w <= 1.f);
}

bool BVH::Intersection(const Triangle& triangle, const Point3D& p)
{
	return Intersection(p, triangle);
}

bool BVH::Intersection(Point3D* p, Triangle* triangle)
{
	return Intersection(*p, *triangle);
}

bool BVH::Intersection(Triangle* triangle, Point3D* p)
{
	return Intersection(*p, *triangle);
}

bool BVH::Intersection(const AABB& lhs, const AABB& rhs)
{
	// Compare each axis (x, y, z)
	for (int i = 0; i < 3; i++)
	{
		if (rhs.center[i] - rhs.halfExtents[i] > lhs.center[i] + lhs.halfExtents[i] ||
			lhs.center[i] - lhs.halfExtents[i] > rhs.center[i] + rhs.halfExtents[i])
		{
			return false;
		}
	}


	return false;
}

bool BVH::Intersection(AABB* lhs, AABB* rhs)
{
	return Intersection(*lhs, *rhs);
}

bool BVH::Intersection(const Ray& r, const Plane& p)
{
	float denominator = dot(p.normal, r.direction);
	float numerator = dot(-p.normal, r.start - p.p);

	if (denominator < glm::epsilon<float>())
	{
		return false;
	}

	float result = numerator / denominator;

	return (result > 0.f);
}

bool BVH::Intersection(const Plane& p, const Ray& r)
{
	return Intersection(r, p);
}

bool BVH::Intersection(Ray* r, Plane* p)
{
	return Intersection(*r, *p);
}

bool BVH::Intersection(Plane* p, Ray* r)
{
	return Intersection(*r, *p);
}

bool BVH::Intersection(const Ray& r, const Triangle& t)
{
	glm::vec3 v1v0 = t.v1 - t.v0;
	glm::vec3 v2v0 = t.v2 - t.v0;
	glm::vec3 r0v0 = r.start - t.v0;

	glm::vec3 n = glm::cross(v1v0, v2v0);
	glm::vec3 q = glm::cross(r0v0, r.direction);

	float d = 1.f / glm::dot(r.direction, n);
	float u = d * dot(-q, v2v0);
	float v = d * dot(q, v1v0);
	float w = 1 - u - v;
	return (u >= 0.f && u <= 1.f && 
		v >= 0.f && v <= 1.f &&
		w >= 0.f && w <= 1.f);
}

bool BVH::Intersection(const Triangle& t, const Ray& r)
{
	return Intersection(r, t);
}

bool BVH::Intersection(Ray* r, Triangle* t)
{
	return Intersection(*r, *t);
}

bool BVH::Intersection(Triangle* t, Ray* r)
{
	return Intersection(*r, *t);
}

bool BVH::Intersection(const Ray& r, const Sphere& s)
{
	const float epsilon = glm::epsilon<float>();

	glm::vec3 q = r.start - s.position;

	float dotQD = dot(q, r.direction);
	float discriminant = (dotQD * dotQD) - dot(q, q) + (s.radius * s.radius);

	if (discriminant < epsilon)
	{
		return false;
	}

	float sqrtDiscriminant = sqrt(discriminant);

	float tNegative = -dotQD - sqrtDiscriminant;
	float tPositive = -dotQD + sqrtDiscriminant;

	if (tNegative < 0 && tPositive < 0)
	{
		// If both are negative -> No intersections
		return false;
	}

	return true;
}

bool BVH::Intersection(const Sphere& s, const Ray& r)
{
	return Intersection(r, s);
}

bool BVH::Intersection(Ray* r, Sphere* s)
{
	return Intersection(*r, *s);
}

bool BVH::Intersection(Sphere* s, Ray* r)
{
	return Intersection(*r, *s);
}

bool BVH::Intersection(const Ray& r, const AABB& aabb)
{
	const float e = glm::epsilon<float>();


	// std::pair<float, float> == Interval
	const std::pair<float, float> xInterval = MySlabIntersection(r, glm::vec3(1, 0, 0), -(aabb.center.x - aabb.halfExtents.x), -(aabb.center.x + aabb.halfExtents.x));
	const std::pair<float, float> yInterval = MySlabIntersection(r, glm::vec3(0, 1, 0), -(aabb.center.y - aabb.halfExtents.y), -(aabb.center.y + aabb.halfExtents.y));
	const std::pair<float, float> zInterval = MySlabIntersection(r, glm::vec3(0, 0, 1), -(aabb.center.z - aabb.halfExtents.z), -(aabb.center.z + aabb.halfExtents.z));

	std::pair<float, float> interval(0.f, INFINITY);

	interval.first = std::max(std::max(std::max(interval.first, xInterval.first), yInterval.first), zInterval.first);
	interval.second = std::min(std::min(std::min(interval.second, xInterval.second), yInterval.second), zInterval.second);

	if (interval.first > interval.second)
	{
		return false;
	}

	if (interval.first < e)
	{
		return false;
	}

	return true;
}

bool BVH::Intersection(const AABB& aabb, const Ray& r)
{
	return Intersection(r, aabb);
}

bool BVH::Intersection(Ray* r, AABB* aabb)
{
	return Intersection(*r, *aabb);
}

bool BVH::Intersection(AABB* aabb, Ray* r)
{
	return Intersection(*r, *aabb);
}

bool BVH::Intersection(const Plane& plane, const Point3D& point)
{
	float dot = glm::dot(plane.normal, point.position);
	float result = dot - glm::dot(plane.normal, plane.p);

	return FloatEqual(result, 0.f);
}

bool BVH::Intersection(const Point3D& point, const Plane& plane)
{
	return Intersection(plane, point);
}

bool BVH::Intersection(Plane* plane, Point3D* point)
{
	return Intersection(*plane, *point);
}

bool BVH::Intersection(Point3D* point, Plane* plane)
{
	return Intersection(*plane, *point);
}

bool BVH::Intersection(const Plane& p, const Triangle& t)
{
	float resultV0 = glm::dot(p.normal, t.v0) - glm::dot(p.normal, p.p);
	float resultV1 = glm::dot(p.normal, t.v1) - glm::dot(p.normal, p.p);
	float resultV2 = glm::dot(p.normal, t.v2) - glm::dot(p.normal, p.p);

	if (resultV0 > 0 && resultV1 > 0 && resultV2 > 0)
	{
		return false;
	}
	if (resultV0 < 0 && resultV1 < 0 && resultV2 < 0)
	{
		return false;
	}

	return true;
}

bool BVH::Intersection(const Triangle& t, const Plane& p)
{
	return Intersection(p, t);
}

bool BVH::Intersection(Plane* p, Triangle* t)
{
	return Intersection(*p, *t);
}

bool BVH::Intersection(Triangle* t, Plane* p)
{
	return Intersection(*p, *t);
}

bool BVH::Intersection(const Plane& p, const Sphere& s)
{
	float dot = glm::dot(p.normal, s.position);
	float result = dot - glm::dot(p.normal, p.p);

	return result <= s.radius && result >= -s.radius;
}

bool BVH::Intersection(const Sphere& s, const Plane& p)
{
	return Intersection(p, s);
}

bool BVH::Intersection(Plane* p, Sphere* s)
{
	return Intersection(*p, *s);
}

bool BVH::Intersection(Sphere* s, Plane* p)
{
	return Intersection(*p, *s);
}

bool BVH::Intersection(const Plane& p, const AABB& a)
{
	const float epsilon = glm::epsilon<float>();

	glm::vec3 p1 = glm::vec3(a.center.x - a.halfExtents.x, a.center.y - a.halfExtents.y, a.center.z - a.halfExtents.z);
	glm::vec3 p2 = glm::vec3(a.center.x - a.halfExtents.x, a.center.y - a.halfExtents.y, a.center.z + a.halfExtents.z);
	glm::vec3 p3 = glm::vec3(a.center.x - a.halfExtents.x, a.center.y + a.halfExtents.y, a.center.z - a.halfExtents.z);
	glm::vec3 p4 = glm::vec3(a.center.x - a.halfExtents.x, a.center.y + a.halfExtents.y, a.center.z + a.halfExtents.z);
	glm::vec3 p5 = glm::vec3(a.center.x + a.halfExtents.x, a.center.y - a.halfExtents.y, a.center.z - a.halfExtents.z);
	glm::vec3 p6 = glm::vec3(a.center.x + a.halfExtents.x, a.center.y - a.halfExtents.y, a.center.z + a.halfExtents.z);
	glm::vec3 p7 = glm::vec3(a.center.x + a.halfExtents.x, a.center.y + a.halfExtents.y, a.center.z - a.halfExtents.z);
	glm::vec3 p8 = glm::vec3(a.center.x + a.halfExtents.x, a.center.y + a.halfExtents.y, a.center.z + a.halfExtents.z);

	float distance = glm::dot(p.normal, p.p);

	float results[8];
	results[0] = glm::dot(p.normal, p1) - distance;
	results[1] = glm::dot(p.normal, p2) - distance;
	results[2] = glm::dot(p.normal, p3) - distance;
	results[3] = glm::dot(p.normal, p4) - distance;
	results[4] = glm::dot(p.normal, p5) - distance;
	results[5] = glm::dot(p.normal, p6) - distance;
	results[6] = glm::dot(p.normal, p7) - distance;
	results[7] = glm::dot(p.normal, p8) - distance;

	if (results[0] < 0)
	{
		for (int i = 1; i < 8; i++)
		{
			if (results[i] > 0)
			{
				return true;
			}
		}
	}
	else
	{
		for (int i = 1; i < 8; i++)
		{
			if (results[i] < 0)
			{
				return true;
			}
		}
	}

	return false;
}

bool BVH::Intersection(const AABB& a, const Plane& p)
{
	return Intersection(p, a);
}

bool BVH::Intersection(Plane* p, AABB* a)
{
	return Intersection(*p, *a);
}

bool BVH::Intersection(AABB* a, Plane* p)
{
	return Intersection(*p, *a);
}

bool BVH::Intersection(const Sphere& s, const AABB& a)
{
	float sqDistance = 0.f;
	for (int i = 0; i < 3; i++)
	{
		float sphereCenter = s.position[i];

		float min = a.center[i] - a.halfExtents[i];
		if (sphereCenter < min)
		{
			sqDistance += (min - sphereCenter) * (min - sphereCenter);
			continue;
		}

		float max = a.center[i] + a.halfExtents[i];
		if (sphereCenter > max)
		{
			sqDistance += (sphereCenter - max) * (sphereCenter - max);
		}
	}

	return sqDistance <= s.radius * s.radius;
}

bool BVH::Intersection(const AABB& a, const Sphere& s)
{
	return Intersection(s, a);
}

bool BVH::Intersection(Sphere* s, AABB* a)
{
	return Intersection(*s, *a);
}

bool BVH::Intersection(AABB* a, Sphere* s)
{
	return Intersection(*s, *a);
}

void BVH::BVHGeometry::SetPosition(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3)
{
	assert("Error, call wrong BVH init function");
}

void BVH::BVHGeometry::SetPosition(glm::vec3 p1)
{
	assert("Error, call wrong BVH init function");
}

void BVH::BVHGeometry::SetPosition(glm::vec3 v, float s)
{
	assert("Error, call wrong BVH init function");
}

void BVH::BVHGeometry::SetPosition(glm::vec3 p1, glm::vec3 p2)
{
	assert("Error, call wrong BVH init function");
}
