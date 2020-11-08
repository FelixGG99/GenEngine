#pragma once
#ifndef GEN_ENG_LEVEL_DATA_H
#define GEN_ENG_LEVEL_DATA_H

#include "../util/vec.h"

namespace GenEngine {

	// Data structure for each vertex of the sections' walls. Contains its x,z coords and the next wall vertex, which is the one it is connected to and makes a wall with.
	//-------------------------------------------------------------------------------------------------------------------------------------------
	struct wall_vert {
		vec2 coords;	// x,z coords
		int neighbor;	// Stores the ID of the 
		wall_vert* next;
	};

	// Class which represents each section.
	//-------------------------------------------------------------------------------------------------------------------------------------------
	class section {

		unsigned int ID_sect;
		float height;				// height of the walls in the sector.
		float y_level;				// y-coordinate of the section, or "how high is the platform". 
		unsigned char mask;			/* Bitmask used for certain flags.
										- mask & 1 (first bit):		hide ceiling if on
									    - mask & 2 (second bit):	hide floor if on
									*/

		wall_vert *verts;			// list of the vertices that conform the section's walls and the neighboring sections "portals".

	public:
		
		// Constructor and destructor
		//-------------------------------------------------------------------------------------------------------------------------------------------
		section() {};
		~section() {};

		// Section data retrieval functions
		//-------------------------------------------------------------------------------------------------------------------------------------------
		wall_vert*			get_verts()		{ return verts; }		// Returns first vertex pointer
		inline const float	get_y_level()	{ return y_level; }		// Returns y-coordinate of the section
		inline const float	get_height()	{ return height; }		// Returns height of the section's walls

		// Data manipulation functions
		//-------------------------------------------------------------------------------------------------------------------------------------------
		void				gen_vao()						{}		// Creates a vertex array object for this section.
		void				store_data(const char *file)	{}		// Stores the data from this specific section on the map data file indicated by the user.
	};

	void section::store_data(const char *file) {
		 
	}
}

#endif // !GEN_ENG_LEVEL_DATA_H
