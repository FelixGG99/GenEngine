#pragma once
#ifndef VEC3_H
#define VEC3_H

#include <math.h>

class vec3 {
public:
	// Initializations
	vec3() { e[0] = e[1] = e[2] = 0.f; }									// Blank: components equal to zeero
	vec3(float x, float y, float z) { e[0] = x;	 e[1] = y; e[2] = z; }		// Valued Initialization: components equal to parameters

	inline float x() const { return e[0]; }
	inline float y() const { return e[1]; }
	inline float z() const { return e[2]; }

	// Value returns
	inline vec3 operator -() const { return vec3(-e[0], -e[1], -e[2]); }		// Return negative value of each component of the vector in a vec3 object
	inline float operator[] (int i) const { return e[i]; }						// Return value of a component directly from the array
	inline float& operator[] (int i) { return e[i]; }							// As a recipient

	// Operators when vec3 is the recipient of the operations
	inline vec3& operator+=(const vec3& v2);
	inline vec3& operator-=(const vec3& v2);
	inline vec3& operator*=(const vec3& v2);
	inline vec3& operator/=(const vec3& v2);
	inline vec3& operator*=(const float& t);
	inline vec3& operator/=(const float& t);

	// vector operations
	inline float length() const { return (float) sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]); };			//return length of vector
	inline float squared_length() const { return e[0] * e[0] + e[1] * e[1] + e[2] * e[2]; };		//return squared length of vector
	inline void make_unit_vector() {																//normalize vector
		float k = 1.0f / (float) sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]);
		e[0] *= k; e[1] *= k; e[2] *= k;
	};

	float e[3]; //x,y components
};

// Implementation of operators when vec3 is the recipient of the operation

inline vec3& vec3::operator+=(const vec3& v2) {
	e[0] += v2.e[0];
	e[1] += v2.e[1];
	e[2] += v2.e[2];
	return *this;
}

inline vec3& vec3::operator-= (const vec3& v2) {
	e[0] -= v2.e[0];
	e[1] -= v2.e[1];
	e[2] -= v2.e[2];
	return *this;
}

inline vec3& vec3::operator*= (const vec3& v2) {
	e[0] *= v2.e[0];
	e[1] *= v2.e[1];
	e[2] *= v2.e[2];
	return *this;
}

inline vec3& vec3::operator/= (const vec3& v2) {
	e[0] /= v2.e[0];
	e[1] /= v2.e[1];
	return *this;
}

inline vec3& vec3::operator*= (const float& t) {
	e[0] *= t;
	e[1] *= t;
	e[2] *= t;
	return *this;
}

inline vec3& vec3::operator/= (const float& t) {
	e[0] /= t;
	e[1] /= t;
	e[2] /= t;
	return *this;
}

// Vec3 operators when vec3 is not the recipient of the operation

inline vec3 operator+(const vec3& v1, const vec3& v2) {
	return vec3(v1.e[0] + v2.e[0], v1.e[1] + v2.e[1], v1.e[2] + v2.e[2]);
}

inline vec3 operator-(const vec3& v1, const vec3& v2) {
	return vec3(v1.e[0] - v2.e[0], v1.e[1] - v2.e[1], v1.e[2] - v2.e[2]);
}

inline vec3 operator*(const vec3& v1, const float& t) {
	return vec3(v1.e[0] * t, v1.e[1] * t, v1.e[2] * t);
}

inline vec3 operator*(const float& t, const vec3& v1) {
	return vec3(v1.e[0] * t, v1.e[1] * t, v1.e[2] * t);
}

inline vec3 operator*(const vec3& v0, const vec3& v1) {
	return vec3(v1.e[0] * v0.e[0], v1.e[1] * v0.e[1], v1.e[2] * v1.e[2]);
}

inline vec3 operator/(const vec3& v1, const float& t) {
	return vec3(v1.e[0] / t, v1.e[1] / t, v1.e[2] / t);
}

inline vec3 operator/(const vec3& v0, const vec3& v1) {
	return vec3(v0.e[0] / v1.e[0], v0.e[1] / v1.e[1], v0.e[2] / v1.e[2]);
}


// Return a the normal vector of a given vec3
inline vec3 unit_vector(const vec3& v) {
	return v / v.length();
}

// Perform cross product given two vectors
inline vec3 cross(const vec3& v1, const vec3& v2) {
	return vec3((v1.y() * v2.z() - v1.z() * v2.y()), -(v1.x() * v2.z() - v1.z() * v2.x()), (v1.x() * v2.y() - v1.y() * v2.x()));
}

// Perform dot product given two vectors
inline float dot(const vec3& v1, const vec3& v2) {
	return v1.x() * v2.x() + v1.y() * v2.y() + v1.z() * v2.z();
}

#endif // !VEC3_H
