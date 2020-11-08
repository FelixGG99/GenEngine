#ifndef GEN_ENG_VIEW_H
#define GEN_ENG_VIEW_H

#include "util/vec.h"

#ifndef pi
#define pi 3.141592f
#endif 

mat4x4	lookAt(const vec3& camPos, const vec3& target);						// Returns the view matrix that must be used in order for the camera in camPos to be pointing at target.
mat4x4	getProjMatrix(float w, float h, float n, float f, float fov);		// Returns the perspective matrix generated from the size of the screen, the near and far clipping planes and the fov.
float	getScaleFromFOV(float fov, float n);								// Returns the scale of objects according to the fov angle.

mat4x4 lookAt(const vec3& camPos, const vec3& target) {

	vec3 camFw = camPos - target;							// Vector f (forward)
	camFw.make_unit_vector();
	vec3 camRight = cross(vec3(0.0f, 1.0f, 0.0f), camFw);	// Vector l (left of the target, right of the camera)
	camRight.make_unit_vector();
	vec3 camUp = cross(camFw, camRight);					// Vector u (up)
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
	view(0, 0) = camRight.x();
	view(1, 0) = camRight.y();
	view(2, 0) = camRight.z();
	view(0, 1) = camUp.x();
	view(1, 1) = camUp.y();
	view(2, 1) = camUp.z();
	view(0, 2) = camFw.x();
	view(1, 2) = camFw.y();
	view(2, 2) = camFw.z();
	view(3, 3) = 1;

	// Translation of the scene, applied on a column major order matrix.
	view.translate_col_major((camPos.x() * camRight.x() + camRight.y() * camPos.y() + camRight.z() * camPos.z()) * (-1.0f),
		(camPos.x() * camUp.x() + camUp.y() * camPos.y() + camUp.z() * camPos.z()) * (-1.0f),
		(camPos.x() * camFw.x() + camFw.y() * camPos.y() + camFw.z() * camPos.z()) * (-1.0f));
	return view;
}

mat4x4 getProjMatrix(const float w, const float h, const float n, const float f, const float fov) {

	static mat4x4 mP;

	// Define vertical and horizontal limits of the image plane, given fov and dimensions of viewport.
	// Upper right corner of the image plane is (r,t). Both r and t are positive. 
	// Lower left corner of the image plane is (l,b). Both l and b are positive.
	// Image plane is symmetrical both vertical and horizontally, so l = -r and b = -t.

	// Calculate using a triangle formed by half the angle of view (fov)
	float t = (float)tan((fov / 2.0f) * (pi / 180.0f)) * n;		//upper limit of the image plane
	float b = -t;												//lower limit of the image plane
	float aspectRatio = w / h;									//aspect ratio of the screen

	//right limit of the image plane
	float r = t * aspectRatio;									
																//Proportion between w and h must be kept while defining coordinates of the image plane
																//aspectRatio = w / h = r / t;	---->	r = t * aspectRatio
	//left limit of the image plane
	float l = -r;												

	mP(0, 0) = (2.0f * n) / (r - l);
	mP(1, 0) = 0;
	mP(2, 0) = (r + l) / (r - l);;
	mP(3, 0) = 0;

	mP(0, 1) = 0;
	mP(1, 1) = (2.0f * n) / (t - b);
	mP(2, 1) = (t + b) / (t - b);
	mP(3, 1) = 0;

	mP(0, 2) = 0;
	mP(1, 2) = 0;
	mP(2, 2) = -(f + n) / (f - n);
	mP(3, 2) = -2.0f * f * n / (f - n);

	mP(0, 3) = 0;
	mP(1, 3) = 0;
	mP(2, 3) = -1;
	mP(3, 3) = 0;

	return mP;
}

float getScaleFromFOV(const float fov, const float n) {
	return (n * (float) tan((fov / 2.0f) * (pi / 180.0f)));
}

#endif // !GEN_ENG_VIEW_H
