
#pragma once


/*
  Copyright (C) 2005, William Donnelly (wdonnelly@uwaterloo.ca)
                  and Stefanus Du Toit (sjdutoit@cgl.uwaterloo.ca)

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
      claim that you wrote the original software. If you use this software
      in a product, an acknowledgment in the product documentation would be
      appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
      misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/

namespace gloperate
{

// A type to hold the distance map while it's being constructed
typedef unsigned short Distance;

/**
*  @brief
*    Used to compute a 3D distance texture from a provided 2D height texture.
*/
struct DistanceMap
{
public:
	static void init_distance_map(const float * heightmap, float * distancemap, int width, int alignedWidth, int height, int depth);

protected:
	DistanceMap(int width, int height, int depth);
	Distance& operator()(int x, int y, int z, int i);
	Distance operator()(int x, int y, int z, int i) const;
	bool inside(int x, int y, int z) const;
	void combine(int dx, int dy, int dz,
		int cx, int cy, int cz,
		int x, int y, int z);


protected:
	Distance* m_data;
	int m_width, m_height, m_depth;
};

} // namespace gloperate