#include "ray_tracing_scene.h"
#include "Camera/cameras.h"
#include "Application/application.h"
#include "Screenshoot/screenshoot.h"
#include "Camera/camera.h"//#include "Components/MeshRender/mesh_render.h"
#include "skybox.h"
RayTracingScene::RayTracingScene(Shader shader):PostFX(shader)
{
  SkyBox cubeMap(common_resources_path("Textures/CubeMaps/nvlobby"));
  skyBox = cubeMap.skybox;
  perlinNoise3D = Texture3D(256, 256, 256, "perlinNoise3D");
  Shader perlinNoise3dGeneration = get_shader("perlin3d_generation");
  perlinNoise3dGeneration.use();
  perlinNoise3D.execute_dispatch();
}

void RayTracingScene::postfx_render() 
{
  CameraPtr camera = main_camera();
  if (!camera)
    return;
  const Camera& mainCam = *camera;
  mat4 camTransform = mainCam.get_transform_matrix();
  mat4 ProjViewInv =  camTransform * inverse(mainCam.get_projection());
  shader.set_mat4x4("ProjViewInv", ProjViewInv);
  vec3 camPos = camTransform[3];
  shader.set_vec3("CameraPos", camPos); /*set shader uniform "CameraPos"*/
  shader.set_float("Time", Time::time());
  skyBox.bind(shader, "skybox");
  perlinNoise3D.bind(shader, "perlin3D");
}
void RayTracingScene::postfx_unbind()
{ //unbind all resources here
  skyBox.unbind();
  perlinNoise3D.unbind();
}
void RayTracingScene::save()
{
  string ScreenshotDir = "Screenshots";
  string timestamp = to_string(int(Time::time()*1000));

  take_screenshoot(ScreenshotDir + "/" + "shot_" + timestamp + ".png");
}

void init_scene(vector<GameObjectPtr>&gameObjects, DirectionLight&)
{

  Input &input = Application::get_input();
  {
    shared_ptr<ArcballCamera> arcballCam;
  
    GameObjectPtr camera = make_game_object();
    arcballCam = camera->add_component<ArcballCamera>(vec3(0), 10, vec2(0.f, 30.f*DegToRad));
    arcballCam->set_perspective(90.f * DegToRad, 0.01f, 5000.f);//90.f -field of view
    add_camera(arcballCam);
    input.mouse_move_event() += createMethodEventHandler(*arcballCam, &ArcballCamera::mouse_move_handler);
    input.mouse_click_event() += createMethodEventHandler(*arcballCam, &ArcballCamera::mouse_click_handler);
    input.mouse_wheel_event() += createMethodEventHandler(*arcballCam, &ArcballCamera::mouse_wheel_handler);
    gameObjects.push_back(camera);
  }
  {
    GameObjectPtr postfx = make_game_object();
    postfx->add_component<RayTracingScene>(get_shader("ray_tracing_scene"));
    gameObjects.push_back(postfx);
  }


}