#pragma once
#ifndef GEN_CAM_H
#define GEN_CAM_H

#include "vec.h"
#include "../renderer/window.h"
#include <math.h>
#include <iostream>
#include <algorithm>
#include <GLFW/glfw3.h>

/*	Rotate camera axis using the roll, yaw and pitch angles stored within the class.
		Rotation order: Roll -> Yaw -> Pitch (Rz*Ry*Rx), where:
		Rz: Rotating about Z-Axis, using roll angle
		Ry: Rotating about Y-Axis, using yaw angle
		Rx: Rotating about X-Axis, using pitch angle
			  Rx				   Ry					Rz
		| 1	  0	  0  |	   |  Cy   0   Sy |	    | Cz   -Sz   0 |   | CyCz         -CySz          Sy   |
		| 0   Cx -Sx |  *  |  0    1   0  |  *  | Sz    Cz   0 | = | CxSz+SxSyCz  -SxSySz+CxCz  -SxCy |
		| 0   Sx  Cx |     | -Sy   0   Cy |     | 0     0    1 |   | SxSz-SyCzCx   SxCz+CxSySz   CxCy |

		In the resulting matrix, each column represents the left, up and front resulting axis respectively, since multiplying each original, unit axis with said matrix gives a vector
		identical to the column with the same index as the row on the unit axis with a 1 as a result.
*/

namespace GenEngine {
	class Camera {

		// Class members
		vec3	pos;						// camera pos
		vec3	front, left, up;			// camera axis. when front is (0,0,-1), the camera is pointing towards -z and the yaw angle is 180°. 
		vec3	angle;						// pitch, yaw and roll angles respectively
		float	fov;						// FOV angle

		// Flags
		unsigned short int updated;			// Needs to be updated? Can be used to skip calculations, generating proj matrices, etc.

	public:
		Camera() : pos(vec3(0.f,0.f,0.f)), front(vec3(0.f,0.f,-1.f)), left(vec3(1.f,0.f,0.f)), up(vec3(0.f,1.f,0.f)), angle(vec3(0.f,180.f,0.f)), fov(75.f), updated(0) {};
		Camera(const vec3& p, const float &y, const float &pit, const float &r) : pos(p), angle(r,y,pit), fov(75.f), updated(0) {};
		Camera(const vec3& p, const vec3 &angl) : pos(p), angle(angl), fov(75.f), updated(0) {};
		~Camera() {}

		inline const	vec3		get_pos()				const	{ return pos; }
		inline const	vec3		get_front()				const	{ return front; }
		inline const	vec3		get_left()				const	{ return left; }
		inline const	vec3		get_up()				const	{ return up; }
		inline const	vec3		get_angle()				const	{ return angle; }
		inline			short int	to_be_updated()					{ return updated; }
		inline 			void		set_pos(const vec3 &p)			{ pos = p; }
		inline			void		move_pos(const vec3& p)			{ pos += p; }
		inline 			void		set_front(const vec3& p)		{ front = p; }
		inline			void		move_front(const vec3& p)		{ front += p; }
		inline			void		set_angle(const vec3& ang)		{ angle = ang; }
						void		cursor_offset_to_angle(GLFWwindow *w);
						void		move_camera(GLFWwindow* w);
						void		angles_to_axis();
						mat4x4		look_at(const vec3& target);
						mat4x4		look_at();
		
	};


	void Camera::cursor_offset_to_angle(GLFWwindow *w) {
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Calculates pitch and yaw angles based on the cursor offset of the window the camera is "recording" in since last frame. */
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		static	int			lastX, lastY;									// Last cursor position
				double		xpos, ypos;										// Current cursor position
				float		sensitivity		= 0.05f;						// Camera Sensitivity
				static int	first_time		= 1;							// Flag to know if camera is newly created
		
		if (first_time) {
			// if it's a new camera, manually set last cursor pos to the center of the viewport.
			glfwGetFramebufferSize(w, &lastX, &lastY);
			lastX /= 2;
			lastY /= 2;
			first_time = 0;
		}

		// Get current cursor pos
		glfwGetCursorPos(w, &xpos, &ypos);
		
		// Calculate cursor offset
		float dx = xpos - lastX;		// Modifies yaw angle (moves around y-axis)
		float dy = ypos - lastY;		// Modifies pitch angle (moves around x-axis)

		// Set last pos to current pos for next frame.
		lastX = xpos;
		lastY = ypos;

		// Add to angle (p, y, r) based on offset and sensitivity
		angle += vec3(dy * sensitivity, -dx * sensitivity, 0.f);

		// Constratints to prevent pitch angle (vertical camera) to go beyond +-89 degrees to prevent math issues.
		if (angle.x() > 89.f) angle.e[0] = 89.f;
		else if (angle.x() < -89.f) angle.e[0] = -89.f;
	}

	void Camera::move_camera(GLFWwindow* w) {

		const	float max_speed		= 0.001f;
		const	float acceleration	= 0.0001f;
		static	float velocity		= 0.0f;
		static	float last_time		= 0.0f;
				float current_time;
				float delta_time;
		
		current_time = (float)glfwGetTime();
		delta_time = current_time - last_time;

		if (glfwGetKey(w, GLFW_KEY_W) == GLFW_PRESS) {
			velocity += acceleration * delta_time;
			velocity = std::min(max_speed, velocity);
			pos += velocity * delta_time * front;
		}
		else if(glfwGetKey(w, GLFW_KEY_S) == GLFW_PRESS) {
			velocity -= acceleration * delta_time;
			velocity = std::max(-max_speed, velocity);
			pos += velocity * delta_time * front;
		}
		else if (glfwGetKey(w, GLFW_KEY_D) == GLFW_PRESS) {
			velocity += acceleration * delta_time;
			velocity = std::min(max_speed, velocity);
			pos += velocity * delta_time * left;
		}
		else if (glfwGetKey(w, GLFW_KEY_A) == GLFW_PRESS) {
			velocity -= acceleration * delta_time;
			velocity = std::max(-max_speed, velocity);
			pos += velocity * delta_time * left;
		}
		else if(velocity){
			velocity -= acceleration * delta_time;
			velocity = std::max(0.f, velocity);
		}
	}

	void Camera::angles_to_axis() {
		
		float cx, sx, cy, sy, cz, sz;

		// X-Axis rotation (pitch angle)
		cx = cosf(get_angle().x() * DEG_IN_RAD);
		sx = sinf(get_angle().x() * DEG_IN_RAD);

		// Y-Axis rotation (yaw angle)
		cy = cosf(get_angle().y() * DEG_IN_RAD);
		sy = sinf(get_angle().y() * DEG_IN_RAD);

		// Z-Axis rotation (roll angle)
		cz = cosf(get_angle().z() * DEG_IN_RAD);
		sz = sinf(get_angle().z() * DEG_IN_RAD);

		// Rotate left axis
		left = vec3(cy * cz, cx * sz + sx * sy * cz, sx * sz - sy * cz * cx);

		// Rotate up axis
		up = vec3(-cy * sz, -sx * sy * sz + cx * cz, sx * cz + cx * sy * sz);

		// Rotate front axis
		front = vec3(sy, -sx * cy, cx * cy);
		
	}

	// Generate a view matrix with the camera position and any given target point. Doesn't really take into accaount pitch and roll angles?
	// NEED TO RECALCULATE ANGLES WHEN SETTING TARGET
	mat4x4 Camera::look_at(const vec3& target) {

		vec3 camPos = get_pos();

		// Generate the front vector. Direction comes from target since camPos since the scene and not the camera is what is being rotated.
		vec3 camFw = camPos - target;							
		camFw.make_unit_vector();

		// Left vector, cross product of front vector and a temporary up vector (0, 1, 0), since these two produce a plane perpendicular to the desired left vector. 
		vec3 camLeft = cross(vec3(0.0f, 1.0f, 0.0f), camFw);	//(left of the target, right of the camera)
		camLeft.make_unit_vector();

		// Actual up vector, built with the front and left vectors.
		vec3 camUp = cross(camFw, camLeft);						
		camUp.make_unit_vector();

		// VIEW MATRIX: transformation matrix used to indicate the rotation and translation of the vertices in the scene based on the rotation and translation of the camera.
		// For more information, visit http://www.songho.ca/opengl/gl_projectionmatrix.html.
		static mat4x4 view;

		/*
		ROTATION AND TRANSLATION IN OPENGL: In OpenGL, the camera is fixed while the world itself is rotated and translated in the opposite direction of the viewer's rotation/translation.
		If the rotation of the camera is described by the matrix A built using the vectors camFw, camRight and camUp, then A^-1, the inverse matrix, describes how the scene is rotated.
		For translation, just negate the movement of the camera in the x, y and z coordinates.


			CAMERA ROTATION (A)    	SCENE ROTATION (INVERSE CAMERA ROTATION, OR A^-1) 			SCENE TRANSLATION			RESULTING MATRIX AFTER COMBINING ROTATION AND TRANSLATION
			| lx ux fx 0 |			| lx ly lz 0 |												| 1 0 0 -cx |				| lx ly lz -lx*cx - ly*cy - lz*cz |
			| ly uy fy 0 |			| ux uy uz 0 |												| 0 1 0 -cy |				| ux uy uz -ux*cx - uy*cy - uz*cz |
			| lz uz fz 0 |			| fx fy fz 0 |												| 0 0 1 -cz |				| fx fy fz -fx*cx - fy*cy - fz*cz |
			| 0  0  0  1 |			| 0  0  0  1 |												| 0 0 0  1  |				| 0  0  0   1					  |
									**inverse is just the transpose								**where c is camPos
									because f,l and u are orthonormal


		The previous matrices have a row-major order layout (the one C++ uses), but GLSL uses column-major order when reading arrays as matrices
		---i dimension
		| 0,0  0,1  0,2  0,3 |
		| 1,0  1,1  1,2  1,3 |
		| 2,0  2,1  2,2  2,3 |
		| 3,0  3,1  3,2  3,3 |

		is the same as

		| 0 4 8  12 |
		| 1 5 9  13 |   ==	{0,4,8,12,1,5,9,13,2,6,10,14,3,7,11,15}, where the numbers are the sequence in which GLSL will read each value from the array given to follow a column-major order.
		| 2 6 10 14 |		This applies only when the GL_TRANSPOSE parameter of glUniformMatrix4fv is set to false, telling the function not to assume the matrix is supplied in a row-major order.
		| 3 7 11 15 |		Otherwise, the matrix must be provided without converting its layout from a row-major order to a column-major order.

		Therefore, the resulting matrix, after converted to a column-major format, is:

		| lx						ux						fx						0 |
		| ly						uy						fy						0 |
		| lz						uz						fz						0 |
		| -lx*cx - ly*cy - lz*cz	-ux*cx - uy*cy - uz*cz	-fx*cx - fy*cy - fz*cz  1 |

		*/


		// In the next statements, the values in the resulting matrix will be stored in column-major order.translated from the row-major order layout sketched before (matrix order c++ uses for its data structure) to column-major order (matrix order used in glsl).
		// Rows are "turned 90 degrees" so that they're now columns, like in the matrix above. 
		view(0, 0) = camLeft.x();
		view(1, 0) = camLeft.y();
		view(2, 0) = camLeft.z();
		view(0, 1) = camUp.x();
		view(1, 1) = camUp.y();
		view(2, 1) = camUp.z();
		view(0, 2) = camFw.x();
		view(1, 2) = camFw.y();
		view(2, 2) = camFw.z();
		view(3, 3) = 1;

		// Translation of the scene, applied on a column major order matrix.
		view.translate_col_major((camPos.x() * camLeft.x() + camLeft.y() * camPos.y() + camLeft.z() * camPos.z()) * (-1.0f),
			(camPos.x() * camUp.x() + camUp.y() * camPos.y() + camUp.z() * camPos.z()) * (-1.0f),
			(camPos.x() * camFw.x() + camFw.y() * camPos.y() + camFw.z() * camPos.z()) * (-1.0f));
		return view;
	}

	// Generate a view matrix with the camera position and the stored front, up and left vectors.
	mat4x4 Camera::look_at() {

		vec3 camPos = get_pos();

		// Get front vector from class member. Since get_front() returns the front vector from the camera to a certain target, the return value is negated to get the vector from the target to the camera instead.
		vec3 camFw = -get_front();
		camFw.make_unit_vector();

		// Gets Up vector from class member. 
		vec3 camUp = get_up();
		camUp.make_unit_vector();

		// Gets left vector from class member.
		vec3 camLeft = get_left();
		camLeft.make_unit_vector();

		mat4x4 view;

		// In the next statements, the values in the resulting matrix will be stored in column-major order.translated from the row-major order layout sketched before (matrix order c++ uses for its data structure) to column-major order (matrix order used in glsl).
		// Rows are "turned 90 degrees" so that they're now columns, like in the matrix above. 
		view(0, 0) = camLeft.x();
		view(1, 0) = camLeft.y();
		view(2, 0) = camLeft.z();
		view(0, 1) = camUp.x();
		view(1, 1) = camUp.y();
		view(2, 1) = camUp.z();
		view(0, 2) = camFw.x();
		view(1, 2) = camFw.y();
		view(2, 2) = camFw.z();
		view(3, 3) = 1;

		// Translation of the scene, applied on a column major order matrix.
		view.translate_col_major((camPos.x() * camLeft.x() + camLeft.y() * camPos.y() + camLeft.z() * camPos.z()) * (-1.0f),
			(camPos.x() * camUp.x() + camUp.y() * camPos.y() + camUp.z() * camPos.z()) * (-1.0f),
			(camPos.x() * camFw.x() + camFw.y() * camPos.y() + camFw.z() * camPos.z()) * (-1.0f));

		return view;
	}
}
#endif // !GEN_CAM_H

