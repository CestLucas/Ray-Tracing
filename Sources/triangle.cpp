#include "triangle.h"

//constructor given  center, radius, and material
triangle::triangle(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, float tx0, float tx1, float tx2, float ty0, float ty1, float ty2, int m, scene* s) : rtObject(s)
{
	point0 = p0;
	point1 = p1;
	point2 = p2;

	texX0 = tx0;
	texX1 = tx1;
	texX2 = tx2;
	texY0 = ty0;
	texY1 = ty1;
	texY2 = ty2;
	matIndex = m;
	myScene = s;
}

float triangle::testIntersection(glm::vec3 eye, glm::vec3 dir)
{
	//see the book/slides for a description of how to use Cramer's rule to solve
	//for the intersection(s) of a line and a plane, implement it here and
	//return the minimum distance (if barycentric coordinates indicate it hit
	//the triangle) otherwise 9999999

	//Using the method from the text
	float a = point0.x - point1.x;
	float b = point0.y - point1.y;
	float c = point0.z - point1.z;
	float d = point0.x - point2.x;
	float e = point0.y - point2.y;
	float f = point0.z - point2.z;
	float g = dir.x;
	float h = dir.y;
	float i = dir.z;
	float j = point0.x - eye.x;
	float k = point0.y - eye.y;
	float l = point0.z - eye.z;

	float ei_minus_hf = e * i - h * f;
	float gf_minus_di = g * f - d * i;
	float dh_minus_eg = d * h - e * g;
	float ak_minus_jb = a * k - j * b;
	float jc_minus_al = j * c - a * l;
	float bl_minus_kc = b * l - k * c;

	float m = a * ei_minus_hf + b * gf_minus_di + c * dh_minus_eg;

	float gamma = (i * ak_minus_jb + h * jc_minus_al + g * bl_minus_kc) / m;
	if (gamma < 0 || gamma > 1)
		return 9999999;

	float beta = (j * ei_minus_hf + k * gf_minus_di + l * dh_minus_eg) / m;
	if (beta < 0 || beta > 1 - gamma)
		return 9999999;

	float t = -(f * ak_minus_jb + e * jc_minus_al + d * bl_minus_kc) / m;

	return (t < 0) ? 9999999 : t;
}

glm::vec3 triangle::getNormal(glm::vec3 eye, glm::vec3 dir)
{
	//construct the barycentric coordinates for the plane
	glm::vec3 bary1 = point1 - point0;
	glm::vec3 bary2 = point2 - point0;

	//cross them to get the normal to the plane
	//note that the normal points in the direction given by right-hand rule
	//(this can be important for refraction to know whether you are entering or leaving a material)
	glm::vec3 normal = glm::normalize(glm::cross(bary1, bary2));
	return normal;
}

glm::vec2 triangle::getTextureCoords(glm::vec3 eye, glm::vec3 dir)
{
	//find alpha and beta (parametric distance along barycentric coordinates)
	//use these in combination with the known texture surface location of the vertices
	//to find the texture surface location of the point you are seeing

	// ref: https://en.wikipedia.org/wiki/Barycentric_coordinate_system
	glm::vec3 p = point0 - eye;
	glm::vec3 ab = point0 - point1;
	glm::vec3 ac = point0 - point2;

	float det = cross_helper(ab, ac, dir);
	float beta = cross_helper(p, ac, dir) / det;
	float gamma = cross_helper(ab, p, dir) / det;

	glm::vec2 texture_ori = glm::vec2(texX0, texY0);
	glm::vec2 texture_ab = glm::vec2(texX1 - texX0, texY1 - texY0);
	glm::vec2 texture_ac = glm::vec2(texX2 - texX0, texY2 - texY0);

	glm::vec2 coords = texture_ori + beta * texture_ab + gamma * texture_ac;

	return coords;
}

// cross_helper for calculating beta and gamma
float triangle::cross_helper(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3)
{
	float a = v1.x * (v2.y * v3.z - v2.z * v3.y);
	float b = v2.x * (v3.y * v1.z - v3.z * v1.y);
	float c = v3.x * (v1.y * v2.z - v1.z * v2.y);

	return a + b + c;
}
