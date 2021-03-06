/************************************************************************/
/*                                                                      */
/* This file is part of VDrift.                                         */
/*                                                                      */
/* VDrift is free software: you can redistribute it and/or modify       */
/* it under the terms of the GNU General Public License as published by */
/* the Free Software Foundation, either version 3 of the License, or    */
/* (at your option) any later version.                                  */
/*                                                                      */
/* VDrift is distributed in the hope that it will be useful,            */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of       */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        */
/* GNU General Public License for more details.                         */
/*                                                                      */
/* You should have received a copy of the GNU General Public License    */
/* along with VDrift.  If not, see <http://www.gnu.org/licenses/>.      */
/*                                                                      */
/************************************************************************/

#ifndef _RENDER_INPUT_SCENE_H
#define _RENDER_INPUT_SCENE_H

#include "render_input.h"
#include "mathvector.h"
#include "quaternion.h"
#include "matrix4.h"
#include "frustum.h"
#include "reseatable_reference.h"
#include <vector>

struct GraphicsCamera;
class Drawable;
class VertexArray;
class TextureInterface;
class Shader;

class RenderInputScene : public RenderInput
{
public:
	RenderInputScene();

	~RenderInputScene();

	void SetShader(Shader * newshader);

	void SetFSAA(unsigned value);

	void ClearOutput(GraphicsState & glstate, bool clearcolor, bool cleardepth);

	void SetColorMask(GraphicsState & glstate, bool write_color, bool write_alpha);

	void SetDepthMode(GraphicsState & glstate, int mode, bool write_depth);

	/// blend mode setup depends on current shader and fsaa value, make sure to set them first
	void SetBlendMode(GraphicsState & glstate, BlendMode::BLENDMODE mode);

	void SetTextures(
		GraphicsState & glstate,
		const std::vector <TextureInterface*> & textures,
		std::ostream & error_output);

	void SetCamera(const GraphicsCamera & cam);

	void SetSunDirection(const Vec3 & newsun);

	void SetContrast(float value);

	void SetCarPaintHack(bool hack);

	void SetDrawLists(
		const std::vector <Drawable*> & dl_dynamic,
		const std::vector <Drawable*> & dl_static);

	virtual void Render(GraphicsState & glstate, std::ostream & error_output);

private:
	reseatable_reference <const std::vector <Drawable*> > dynamic_drawlist_ptr;
	reseatable_reference <const std::vector <Drawable*> > static_drawlist_ptr;
	bool last_transform_valid;
	Mat4 last_transform;
	Quat cam_rotation; // used for the skybox effect
	Vec3 cam_position;
	Vec3 lightposition;
	Mat4 projMatrix;
	Mat4 viewMatrix;
	Frustum frustum; // used for frustum culling
	float lod_far; // used for distance culling
	Shader * shader;
	unsigned fsaa;
	float contrast;
	bool carpainthack;
	bool vlighting;

	void EnableCarPaint(GraphicsState & glstate);

	void DisableCarPaint(GraphicsState & glstate);

	void Draw(GraphicsState & glstate, const std::vector <Drawable*> & drawlist, bool preculled);

	void DrawVertexArray(const VertexArray & va, float linesize) const;

	/// returns true if the object was culled and should not be drawn
	bool FrustumCull(const Drawable & d) const;

	void SetFlags(const Drawable & d, GraphicsState & glstate);

	void SetTextures(const Drawable & d, GraphicsState & glstate);

	void SetTransform(const Drawable & d, GraphicsState & glstate);
};

#endif // _RENDER_INPUT_SCENE_H
