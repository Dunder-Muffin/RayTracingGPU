#pragma once
#include "component.h"
#include "GameObject/game_object.h"
#include "Postfx/postfx.h"
#include "Texture/textures.h"

class RayTracingScene : public PostFX, public ISaveable
{
private:
	CubeMap skyBox;
	Texture3D perlinNoise3D;
public:
	RayTracingScene(Shader shader);
	void postfx_render() override;
	void postfx_unbind() override;
	void save() override;

};
void init_scene(vector<GameObjectPtr>&gameObjects, DirectionLight& sun);


