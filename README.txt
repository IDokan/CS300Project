/* Start Header -------------------------------------------------------
Copyright (C) FALL2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: README.txt
Purpose: describe overview of my application
Language: English none of computer language is used in here.
Platform: Compiler version: Compiler does not need for README, OS: Windows 10. GPU: NVIDIA GeForce840M. OpenGL Driver version: 10.18.15.4279, 8-24-2015
Project: sinil.kang_CS350_2
Author: Sinil Kang = sinil.kang = Colleague ID: 0052782
Creation date: 3/6/2021
End Header --------------------------------------------------------*/


1. How to use my interface.
1-a. 
WASD to move camera.
During move, press left shift to move faster.
Press right mouse button to look around.
Press E to move up
Press Q to move down

In MyGUI window,
use BVH Geometries tab for a couple of purposes.
1. Change BVH of center static, and dynamic objects to test basic intersection (Sphere VS Sphere, AABB VS Sphere, Sphere VS AABB, AABB VS AABB)
2. Stop all objects in the scene.


2. How to use the application
2-a. Do not run in DEBUG mode(Professor confirmed). Use it in Release mode only.
2-b. Probably it crashed because objects file are not provided (Syllabus submission restrictions). If it happens and don't know how to do, please let me know freely and I can give you my object files. I might be late for you email.

2-c.
Center spheres are for Basic Intersection
When you looked right, you can see Point-based intersections.
There are white sphere go through Sphere, AABB, Triangle, and Plane. When Point-based Intersection is detected, static objects(Sphere, Cube, Triangle, Plane) becomes red (Plane is really hard to see, but it detects).
Furthermore, there is a dynamic sphere which represents start position of a ray. When the sphere becomes sky color, it means Ray-based intersection has been detected.
Finally, there are dynamic cube, and dynamic triangle to test Plane-based intersection. When they detects plane intersection, intersected cube or triangle becomes red. Red means they detected Plane-based intersection.


3. Completed assignment parts
3-a. Everything perfect but I did not extra credit parts.
I did:  
Objects loaded correctly from the files and displayed in
proper position
Correct Rendering using CS 300 shaders

Sphere Vs Sphere 
AABB Vs Sphere 
Sphere Vs AABB 
AABB Vs AABB 
Point Vs Sphere 
Point Vs AABB 
Point Vs Triangle 
Point Vs Plane 
Ray Vs Plane 
Ray Vs AABB 
Ray Vs Sphere 
Ray Vs Triangle 
Plane Vs AABB 
Plane Vs Triangle

Camera Implementation

Missing information in README
Application does not compile 
Application cannot be executed 
Scene setup incorrect

4. Which part has NOT been completed
4-a. Extra credits part are not done, because I had no enough time to do them.

5. Where the relevant source codes.
5-a. Assignment1 folder contains the scene setup code of this assignment.
5-b. CS350_sinil.kang_1\Common\BVH\Intersections.cpp, and Intersections.h file contains intersection codes.
5-c. CS350_sinil.kang_1\Common\shaders folder contains all shader files.
5-d. CS350_sinil.kang_1\Common\Meshes\models\ folder should contain used object files.

6. Where my program is tested
6-a. OS: Windows 10. GPU: NVIDIA GeForce840M. OpenGL Driver version: 10.18.15.4279, 8-24-2015

7. How many I spend
I'm sorry, I should hurry. I did not record how many time I spend.
Approximately, I spent about 24 hours to do this.