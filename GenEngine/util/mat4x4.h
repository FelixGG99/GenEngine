#ifndef MAT4x4_H
#define MAT4x4_H

class mat4x4 {
public:
	float e[16];

	mat4x4() : e{ 1,0,0,0,	0,1,0,0,	0,0,1,0,	0,0,0,1 } {}
	mat4x4(const float(&m)[4][4]) : e{ m[0][0],m[0][1],m[0][2],m[0][3],	m[1][0],m[1][1],m[1][2],m[1][3],	m[2][0],m[2][1],m[2][2],m[2][3],	m[3][0],m[3][1],m[3][2],m[3][3] } {}
	mat4x4(const float m[]) : e{ m[0],m[1],m[2],m[3],	m[4],m[5],m[6],m[7],	m[8],m[9],m[10],m[11],	m[12],m[13],m[14],m[15] } {}

	// Data-transformation functions
	inline void copy_from_matrix(const float(&m)[4][4]);
	inline void make_identity();

	// Operations when using row-major order
	inline void translate_row_major(const float& x, const float& y, const float& z);
	inline void translate_row_major(const float v[]);
	inline void translate_row_major(const vec3& v);
	inline void scale_u_row_major(const float& x);
	inline void scale_row_major(const float& x, const float& y, const float& z);
	inline void scale_row_major(const float v[]);
	inline void scale_row_major(const vec3& v);

	// Operations when using column-major order
	inline void translate_col_major(const float& x, const float& y, const float& z);
	inline void translate_col_major(const float v[]);
	inline void translate_col_major(const vec3& v);
	inline void scale_u_col_major(const float& x);
	inline void scale_col_major(const float& x, const float& y, const float& z);
	inline void scale_col_major(const float v[]);
	inline void scale_col_major(const vec3& v);

	// Matrix as a data structure operations
	inline mat4x4 transpose();
	inline mat4x4 inverse();

	// Operators
	inline float operator()(const int& i, const int& j) const { /*if (i >= 4 || i < 0 || j >= 4 || j < 0) throw BadIndex("Index out of bounds");*/ return e[i * 4 + j]; }
	inline float& operator()(const int& i, const int& j) { /*if (i >= 4 || i < 0 || j >= 4 || j < 0) throw BadIndex("Index out of bounds");*/ return e[i * 4 + j]; }
	inline float& operator()(const int& i) { /*if (i >= 4 || i < 0 || j >= 4 || j < 0) throw BadIndex("Index out of bounds");*/ return e[i]; }
	inline float operator()(const int& i) const { /*if (i >= 4 || i < 0 || j >= 4 || j < 0) throw BadIndex("Index out of bounds");*/ return e[i]; }
	//inline mat4x4& operator *= (const mat4x4 &m2) const;
};

inline void mat4x4::copy_from_matrix(const float(&m)[4][4]) {
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			e[i * 4 + j] = m[i][j];
}

inline void mat4x4::make_identity() {
	e[0] = 1; e[1] = 0; e[2] = 0; e[3] = 0; e[4] = 0; e[5] = 1; e[6] = 0; e[7] = 0; e[8] = 0;
	e[9] = 0; e[10] = 1; e[11] = 0; e[12] = 0; e[13] = 0; e[14] = 0; e[15] = 1;
}

inline mat4x4 mat4x4::transpose() {
	mat4x4 mat;
	mat.e[0] = e[0];
	mat.e[1] = e[4];
	mat.e[2] = e[8];
	mat.e[3] = e[12];
	mat.e[4] = e[1];
	mat.e[5] = e[5];
	mat.e[6] = e[9];
	mat.e[7] = e[13];
	mat.e[8] = e[2];
	mat.e[9] = e[6];
	mat.e[10] = e[10];
	mat.e[11] = e[14];
	mat.e[12] = e[3];
	mat.e[13] = e[7];
	mat.e[14] = e[11];
	mat.e[15] = e[15];
	return mat;
}

inline mat4x4 mat4x4::inverse() {

}

/*inline mat4x4& mat4x4::operator *= (const mat4x4 &m2) const {
	float mat[] = {
		e[0] * m2(0, 0) + e[1] * m2(1, 0) + e[2] * m2(2, 0) + e[3] * m2(3, 0),
		e[0] * m2(0, 1) + e[1] * m2(1, 1) + e[2] * m2(2, 1) + e[3] * m2(3, 1),
		e[0] * m2(0, 2) + e[1] * m2(1, 2) + e[2] * m2(2, 2) + e[3] * m2(3, 2),
		e[0] * m2(0, 3) + e[1] * m2(1, 3) + e[2] * m2(2, 3) + e[3] * m2(3, 3),

		e[4] * m2(0, 0) + e[5] * m2(1, 0) + e[6] * m2(2, 0) + e[7] * m2(3, 0),
		e[4] * m2(0, 1) + e[5] * m2(1, 1) + e[6] * m2(2, 1) + e[7] * m2(3, 1),
		e[4] * m2(0, 2) + e[5] * m2(1, 2) + e[6] * m2(2, 2) + e[7] * m2(3, 2),
		e[4] * m2(0, 3) + e[5] * m2(1, 3) + e[6] * m2(2, 3) + e[7] * m2(3, 3),

		e[8] * m2(0, 0) + e[9] * m2(1, 0) + e[10] * m2(2, 0) + e[11] * m2(3, 0),
		e[8] * m2(0, 1) + e[9] * m2(1, 1) + e[10] * m2(2, 1) + e[11] * m2(3, 1),
		e[8] * m2(0, 2) + e[9] * m2(1, 2) + e[10] * m2(2, 2) + e[11] * m2(3, 2),
		e[8] * m2(0, 3) + e[9] * m2(1, 3) + e[10] * m2(2, 3) + e[11] * m2(3, 3),

		e[12] * m2(0, 0) + e[13] * m2(1, 0) + e[14] * m2(2, 0) + e[15] * m2(3, 0),
		e[12] * m2(0, 1) + e[13] * m2(1, 1) + e[14] * m2(2, 1) + e[15] * m2(3, 1),
		e[12] * m2(0, 2) + e[13] * m2(1, 2) + e[14] * m2(2, 2) + e[15] * m2(3, 2),
		e[12] * m2(0, 3) + e[13] * m2(1, 3) + e[14] * m2(2, 3) + e[15] * m2(3, 3)
	};
	return mat4x4();
}*/

inline mat4x4 operator*(const mat4x4& m1, const mat4x4& m2) {
	float mat[] = {
		m1(0, 0) * m2(0, 0) + m1(0, 1) * m2(1, 0) + m1(0, 2) * m2(2, 0) + m1(0, 3) * m2(3, 0),
		m1(0, 0) * m2(0, 1) + m1(0, 1) * m2(1, 1) + m1(0, 2) * m2(2, 1) + m1(0, 3) * m2(3, 1),
		m1(0, 0) * m2(0, 2) + m1(0, 1) * m2(1, 2) + m1(0, 2) * m2(2, 2) + m1(0, 3) * m2(3, 2),
		m1(0, 0) * m2(0, 3) + m1(0, 1) * m2(1, 3) + m1(0, 2) * m2(2, 3) + m1(0, 3) * m2(3, 3),

		m1(1, 0) * m2(0, 0) + m1(1, 1) * m2(1, 0) + m1(1, 2) * m2(2, 0) + m1(1, 3) * m2(3, 0),
		m1(1, 0) * m2(0, 1) + m1(1, 1) * m2(1, 1) + m1(1, 2) * m2(2, 1) + m1(1, 3) * m2(3, 1),
		m1(1, 0) * m2(0, 2) + m1(1, 1) * m2(1, 2) + m1(1, 2) * m2(2, 2) + m1(1, 3) * m2(3, 2),
		m1(1, 0) * m2(0, 3) + m1(1, 1) * m2(1, 3) + m1(1, 2) * m2(2, 3) + m1(1, 3) * m2(3, 3),

		m1(2, 0) * m2(0, 0) + m1(2, 1) * m2(1, 0) + m1(2, 2) * m2(2, 0) + m1(2, 3) * m2(3, 0),
		m1(2, 0) * m2(0, 1) + m1(2, 1) * m2(1, 1) + m1(2, 2) * m2(2, 1) + m1(2, 3) * m2(3, 1),
		m1(2, 0) * m2(0, 2) + m1(2, 1) * m2(1, 2) + m1(2, 2) * m2(2, 2) + m1(2, 3) * m2(3, 2),
		m1(2, 0) * m2(0, 3) + m1(2, 1) * m2(1, 3) + m1(2, 2) * m2(2, 3) + m1(2, 3) * m2(3, 3),

		m1(3, 0) * m2(0, 0) + m1(3, 1) * m2(1, 0) + m1(3, 2) * m2(2, 0) + m1(3, 3) * m2(3, 0),
		m1(3, 0) * m2(0, 1) + m1(3, 1) * m2(1, 1) + m1(3, 2) * m2(2, 1) + m1(3, 3) * m2(3, 1),
		m1(3, 0) * m2(0, 2) + m1(3, 1) * m2(1, 2) + m1(3, 2) * m2(2, 2) + m1(3, 3) * m2(3, 2),
		m1(3, 0) * m2(0, 3) + m1(3, 1) * m2(1, 3) + m1(3, 2) * m2(2, 3) + m1(3, 3) * m2(3, 3)
	};
	return mat4x4(mat);
}

inline void mat4x4::translate_col_major(const float &x, const float &y, const float &z) {
	e[12] = x;
	e[13] = y;
	e[14] = z;
}
inline void mat4x4::translate_col_major(const float v[]) {
	e[12] = v[0];
	e[13] = v[1];
	e[14] = v[2];
}
inline void mat4x4::translate_col_major(const vec3& v) {
	e[12] = v.x();
	e[13] = v.y();
	e[14] = v.z();
}

inline void mat4x4::scale_u_col_major(const float& x) {
	e[0] *= x;
	e[5] *= x;
	e[10] *= x;
}
inline void mat4x4::scale_col_major(const float& x, const float& y, const float& z) {
	e[0] *= x;
	e[5] *= y;
	e[10] *= z;
}
inline void mat4x4::scale_col_major(const float v[]) {
	e[0] *= v[0];
	e[5] *= v[1];
	e[10] *= v[2];
}
inline void mat4x4::scale_col_major(const vec3& v) {
	e[0] *= v.x();
	e[5] *= v.y();
	e[10] *= v.z();
}

#endif