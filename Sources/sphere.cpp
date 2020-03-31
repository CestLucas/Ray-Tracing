#include "sphere.h"

//constructor given  center, radius, and material
sphere::sphere(glm::vec3 c, float r, int m, scene* s) : rtObject(s)
{
	center = c;
	radius = r;
	matIndex = m;
	myScene = s;
}

float sphere::testIntersection(glm::vec3 eye, glm::vec3 dir)
{
	//see the book for a description of how to use the quadratic rule to solve
	//for the intersection(s) of a line and a sphere, implement it here and
	//return the minimum positive distance or 9999999 if none
	glm::vec3 e_minus_c = eye - center;
	float d_dot_d = glm::dot(dir, dir);
	float d_dot_e_minus_c = glm::dot(dir, e_minus_c);

	float det = d_dot_e_minus_c * d_dot_e_minus_c - d_dot_d * (glm::dot(e_minus_c, e_minus_c) - radius * radius);

	if (det < 0) return 9999999; //no intersection
	
	float t1 = (-d_dot_e_minus_c + sqrt(det)) / d_dot_d;
	float t2 = (-d_dot_e_minus_c - sqrt(det)) / d_dot_d;

	//decide the closer intersection
	float min_dist = (t1 < 0) ? 9999999 : t1;
	if (t2 < min_dist && t2 > 0) min_dist = t2;

	return min_dist; //return the closer intersection
}

glm::vec3 sphere::getNormal(glm::vec3 eye, glm::vec3 dir)
{
	//once you can test for intersection,
	//simply add (distance * view direction) to the eye location to get surface location,
	//then subtract the center location of the sphere to get the normal out from the sphere
	glm::vec3 surfaceLoc = eye + testIntersection(eye, dir) * dir;

	//don't forget to normalize
	glm::vec3 normal = glm::normalize(surfaceLoc - center);
	
	return normal;
}

glm::vec2 sphere::getTextureCoords(glm::vec3 eye, glm::vec3 dir)
{
	//find the normal as in getNormal
	glm::vec3 normal = getNormal(eye, dir);

	//Using the method from the text, section 11.2
	float theta = acos(normal.z);
	float phi = atan2(normal.y, normal.x);

	float pi = 3.1416;

	if (phi < 0) phi += 2 * pi;

	float u = phi / (2 * pi);
	float v = 1 - (pi - theta) / pi; //inverse v so the texture won't be upside down

	glm::vec2 coords = glm::vec2(u, v);

	return coords;
}