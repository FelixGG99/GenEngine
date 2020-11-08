#ifndef GEN_OBJ_CLASS
#define GEN_OBJ_CLASS

#include "../util/vec.h"
#include "../util/glad/glad.h"
#include "../renderer/Shader.h"
#include <vector>
#include <deque>
#include <bitset>
class GenObject {
	unsigned int VAO = 0, VBO = 0;
	std::bitset<1>flags;

public:
	std::vector<float>vbo_verts;

	inline void set_v_buffer();
	inline void set_e_buffer();
	inline void set_flags(const unsigned short int bitmask);
	inline void draw(const Shader& shader, const mat4x4 view, const mat4x4 projection);
};

inline void GenObject::set_v_buffer() {
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vbo_verts.size() * sizeof(float), &vbo_verts[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
}

inline void GenObject::draw(const Shader& shader, const mat4x4 view, const mat4x4 projection) {
	if (!VAO)
		set_v_buffer();

	shader.use();
	shader.setMat4f("view", view);
	shader.setMat4f("projection", projection);
	shader.setVec3f("color", 0.8f, 0.8f, 0.8f);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

class GenWall : public GenObject {
public:
	GenWall(vec3 p1, vec3 p2, float w, float h) : l_point(p1), r_point(p2), w_size(w), h_size(h) { set_verts_p(l_point, r_point, h_size); }
	GenWall(float x, float y, float z, float a, float b, float c, float w, float h) : l_point(vec3(x, y, z)), r_point(vec3(a, b, c)), w_size(w), h_size(h) { set_verts_p(l_point, r_point, h_size); }
	GenWall() {}

	vec3 l_point, r_point;
	float w_size, h_size;

	inline void set_verts_p(const vec3 l, const vec3 r, const float h);
	inline void append_left(const vec3 p);
	inline void append_right(const vec3 p);
	inline void set_norms();
};

inline void GenWall::set_verts_p(const vec3 l, const vec3 r, const float h) {

	vbo_verts.push_back(l.x());
	vbo_verts.push_back(l.y());
	vbo_verts.push_back(l.z());
	vbo_verts.push_back(l.x());
	vbo_verts.push_back(l.y() + h);
	vbo_verts.push_back(l.z());
	vbo_verts.push_back(r.x());
	vbo_verts.push_back(r.y());
	vbo_verts.push_back(r.z());
	vbo_verts.push_back(r.x());
	vbo_verts.push_back(r.y() + h);
	vbo_verts.push_back(r.z());
	set_v_buffer();
}

inline void GenWall::append_left(const vec3 p) {
	vbo_verts.push_back(p.x());
	vbo_verts.push_back(p.y());
	vbo_verts.push_back(p.z());
	set_v_buffer();
}

inline void GenWall::append_right(const vec3 p) {
	vbo_verts.push_back(p.x());
	vbo_verts.push_back(p.y());
	vbo_verts.push_back(p.z());
	set_v_buffer();
}


const float cubeVertex[] = {
	 0.5f, 0.5f, 0.5f,		// 1
	 0.5f,-0.5f, 0.5f,		// 2
	 0.5f,-0.5f,-0.5f,		// 3
	 0.5f, 0.5f,-0.5f,		// 4
	-0.5f,-0.5f, 0.5f,		// 5
	-0.5f, 0.5f, 0.5f,		// 6
	-0.5f, 0.5f,-0.5f,		// 7
	-0.5f,-0.5f,-0.5f		// 8
};



// const float line[] = {};


#endif // !GEN_OBJ_CLASS
