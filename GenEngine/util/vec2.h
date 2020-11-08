#pragma once
#ifndef VEC2_H
#define VEC2_H

#include<math.h>

class vec2 {
public:
	// Initializations
	vec2() { e[0] = e[1] = 0.f; }
	vec2(float x, float y) { e[0] = x;	 e[1] = y; }

	inline float x() const { return e[0]; }
	inline float y() const { return e[1]; }

	// Value returns
	inline vec2 operator -() const { return vec2(-e[0], -e[1]); }		// Return negative value of each component of the vector in a vec2 object
	inline float operator[] (int i) const { return e[i]; }				// Return value of a component directly from the array
	inline float& operator[] (int i) { return e[i]; }					// As a recipient

	// Operators when vec2 is the recipient of the operations
	inline vec2& operator+=(const vec2& v2);
	inline vec2& operator-=(const vec2& v2);
	inline vec2& operator*=(const vec2& v2);
	inline vec2& operator/=(const vec2& v2);
	inline int   operator==(const vec2& v2)	{ return x() == v2.x() && y() == v2.y(); };
	inline vec2& operator*=(const float& t);
	inline vec2& operator/=(const float& t);

	// vector operations
	inline float length() const { return (float) sqrt(e[0] * e[0] + e[1] * e[1]); };			//return length of vector
	inline float squared_length() const { return e[0] * e[0] + e[1] * e[1]; };			//return squared length of vector
	inline void make_unit_vector() {													//normalize vector
		float k = 1.0f / (float)sqrt(e[0] * e[0] + e[1] * e[1]);
		e[0] *= k; e[1] *= k;
	};														

	float e[2]; //x,y components
};

// Implementation of operators when vec2 is the recipient of the operation

inline vec2& vec2::operator+=(const vec2& v2) {
	e[0] += v2.e[0];
	e[1] += v2.e[1];
	return *this;
}

inline vec2& vec2::operator-= (const vec2& v2) {
	e[0] -= v2.e[0];
	e[1] -= v2.e[1];
	return *this;
}

inline vec2& vec2::operator*= (const vec2& v2) {
	e[0] *= v2.e[0];
	e[1] *= v2.e[1];
	return *this;
}


inline vec2& vec2::operator/= (const vec2& v2) {
	e[0] /= v2.e[0];
	e[1] /= v2.e[1];
	return *this;
}

inline vec2& vec2::operator*= (const float& t) {
	e[0] *= t;
	e[1] *= t;
	return *this;
}

inline vec2& vec2::operator/= (const float& t) {
	e[0] /= t;
	e[1] /= t;
	return *this;
}


// Vec2 Components when vec2 is not the recipient of the operation

inline vec2 operator+(const vec2& v1, const vec2& v2) {
	return vec2(v1.e[0] + v2.e[0], v1.e[1] + v2.e[1]);
}

inline vec2 operator-(const vec2& v1, const vec2& v2) {
	return vec2(v1.e[0] - v2.e[0], v1.e[1] - v2.e[1]);
}

inline vec2 operator*(const vec2& v1, const float& t) {
	return vec2(v1.e[0] * t, v1.e[1] * t);
}

inline vec2 operator*(const float& t, const vec2& v1) {
	return vec2(v1.e[0] * t, v1.e[1] * t);
}

inline vec2 operator*(const vec2& v0, const vec2& v1) {
	return vec2(v1.e[0] * v0.e[0], v1.e[1] * v0.e[1]);
}

inline vec2 operator/(const vec2& v1, const float& t) {
	return vec2(v1.e[0] / t, v1.e[1] / t);
}

inline vec2 operator/(const vec2& v0, const vec2& v1) {
	return vec2(v1.e[0] / v0.e[0], v1.e[1] / v0.e[1]);
}


// Return the normalized version of a given vector in a separate object
inline vec2 unit_vector(const vec2& v) {
	return v / v.length();
}

#endif // !VEC2_H
