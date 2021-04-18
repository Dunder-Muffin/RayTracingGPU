#version 440
in vec2 fragUV;
out vec4 FragColor;

uniform mat4 ProjViewInv;
uniform vec3 CameraPos;
uniform float Time;
uniform samplerCube skybox;
uniform sampler3D perlin3D; 

#define FAR_INF 1e9
#define EPS 1e-4
#define RAY_DEPTH 7
#define STACK_SIZE 130 //2^RAY_DEPTH
//math phi
#define PHI (1.618033988749895)//(1.+sqrt(5.))*.5


//#define ROTATE
struct Ray
{
    vec3 pos, dir;
    float transparent;
    int depth;
    bool isInsideMaterial;
};
struct Sphere
{
    vec3 pos;
    float radius;
};
struct Cylinder
{
    vec3 pos;
    float radius;
    float height;
};

struct RegularDodec
{
    vec3 pos;
    float radius;
};
struct Collision
{
    bool hit;
    vec3 pos, normal;
    float dist;
};

struct Material
{
  float transparent;
  float reflectionFactor;
  float refractionFactor;
  vec3 color;
};
struct Light
{
  vec3 pos;
  vec3 color;
  float intensity;
};
struct RayTracingStack
{
  Ray rays[STACK_SIZE];
  int top;
};

RayTracingStack rayTracingStack;
const int SphereN = 3;
Sphere spheres[SphereN] = Sphere[SphereN](
  //Sphere(vec3(0,0,0), 1.5),
  Sphere(vec3(0,.2,0), 0.6),
  Sphere(vec3(1.4, 1.2,-0.3), 0.25),
  Sphere(vec3(1.2,-0.1,-0.9), 0.2));
Material spheresMat[SphereN] = Material[SphereN](
//Material(0.000001, 0.1, 0.005, vec3(0.4,0.9,0.9)),
Material(1.1, 0, 0.4, vec3(1,0.2,0.1)),
Material(1.1, 0, 0, vec3(0.9,0.9,0.6)),
Material(1.1, 0, 0, vec3(0.4,0.9,0.9)));

RegularDodec dodec = RegularDodec(vec3(0, 0, .0), 0.8);
Material dodecMat = Material(0.1, 0, 1.1, vec3(1,1.,1.));

Cylinder base = Cylinder(vec3(0, -1.03, 0), 0.9, 0.1);
Material baseMat = Material(1, 0, 0, vec3(0.10, 0.49, 0.39));

#define AMBIENT_INTENSIVITY 0.1
const int LightN = 3;
Light Lights[LightN] = Light[LightN](
Light(spheres[0].pos, spheresMat[0].color, 4.f),
Light(spheres[1].pos, spheresMat[1].color, 1.0f),
Light(spheres[2].pos, spheresMat[2].color, 1.0f));


vec3 computeLight(vec3 pos, vec3 color, vec3 normal) 
{
  vec3 light = textureLod(skybox, normal, 0).rgb * AMBIENT_INTENSIVITY;
  for (int i = 0; i < LightN; i++) 
  {
    vec3 toLight = Lights[i].pos - pos;
    float distSq = dot(toLight,toLight);
    float att = Lights[i].intensity/distSq;
    light += max(.0, dot(normal, normalize(toLight))) * att * Lights[i].color;
  }

  return color*light;
}

void clear_stack()
{
  rayTracingStack.top = 0;
}
bool stack_empty()
{
  return rayTracingStack.top == 0;
}
Ray get_ray_from_stack()
{
  Ray ray = rayTracingStack.rays[rayTracingStack.top-1];
  rayTracingStack.top--;
  return ray;
}
Ray get_ray(vec2 uv)
{
    Ray ray;
    vec4 pos = ProjViewInv * vec4(fragUV, 0, 1.0);
    ray.dir = normalize(pos.xyz / pos.w - CameraPos);
    ray.pos = CameraPos;
    ray.transparent = 0;
    ray.depth = 0;
    ray.isInsideMaterial = false;
    return ray;
}
void pop_ray_to_stack(Ray ray)
{
  if (rayTracingStack.top < STACK_SIZE && ray.depth < RAY_DEPTH)
  {
    ray.depth++;
    rayTracingStack.rays[rayTracingStack.top] = ray;
    rayTracingStack.top++;
  }
}

/*distance function realisation: https://www.shadertoy.com/user/yx
shadertoy has many implementations of platonic solids 
referring to Generalized distance functions by Akleman and Chen paper*/
float dScene(vec3 p)
{
    const vec3 n = normalize(vec3(PHI,1,0));

    p = abs(p);
    float a = dot(p,n.xyz);
    float b = dot(p,n.zxy);
    float c = dot(p,n.yzx);
    return max(max(a,b),c)-dodec.radius;
}

void raycast_dodec(inout Collision collision, in RegularDodec dodec, in Ray ray)
{

  vec3 cam = ray.pos;
  vec3 dir = ray.dir;
  float t;
  float k;

  for(int i=0;i<100;++i)
  {
    k = dScene(cam+dir*t- dodec.pos);
    t += k;
    if (k < .001 || k > 10.)
    {
      break;
    }
  }

    vec3 h = dir*t+cam - dodec.pos;
    vec2 o = vec2(.001, .0);
    vec3 n = normalize(vec3(
    dScene(h+o.xyy)-dScene(h-o.xyy),
    dScene(h+o.yxy)-dScene(h-o.yxy),
    dScene(h+o.yyx)-dScene(h-o.yyx)
  ));
  if (k > 10&& !ray.isInsideMaterial)// k>10 condition does not gives info about inner sides of dodec
  {
    collision.hit = false;
    return;
  }
  ray.isInsideMaterial =false;
  Material material= dodecMat;
  float transparent = material.transparent;

    collision.hit = true;
    collision.dist = t;
    collision.pos = ray.pos + ray.dir * t;
    collision.normal = n;
}
void raycast_sphere(inout Collision collision, in Sphere sphere, in Ray ray)
{
  vec3 center = ray.pos - sphere.pos;
  float c = dot(center, center) - sphere.radius*sphere.radius;
  float b = dot(center, ray.dir);
  float a = dot(ray.dir, ray.dir);
  float D = b * b - a * c;
  if (D < 0)
  {
    collision.hit = false;
    return;
  }
  D = sqrt(D);
  float t = 0;
  float t1 = (-D - b) / a;
  if (t1 >= 0)
    t = t1;
  else
  {
    float t2 = (D - b) / a;
    if (t2 >= 0)
      t = t2;
    else
    {
      collision.hit = false;
      return;
    }
  }
  collision.hit = true;
  collision.dist = t;
  collision.pos = ray.pos + ray.dir * t;
  collision.normal = normalize(collision.pos - sphere.pos);
}
void raycast_cylinder(inout Collision collision, in Cylinder cyl, in Ray ray)
{
/*   2 planes*/
  float t = FAR_INF;
  float tp1 = (cyl.pos.y + cyl.height - ray.pos.y)/ray.dir.y;
  float tp2 = (cyl.pos.y - ray.pos.y)/ray.dir.y;  
  if (tp2>0) 
  {
    if (tp1<tp2)
      t=tp1;
    else 
      t=tp2;

    vec3 worldPos = t * ray.dir + ray.pos -cyl.pos;
    if (dot(worldPos.xz, worldPos.xz) <= cyl.radius*cyl.radius )
    {
      collision.hit = true;
      collision.dist = t;
      collision.pos = t * ray.dir + ray.pos;
      collision.normal = vec3(0, 1, 0);
      return;
    }
  }

/*   Sphere without y coord between planes */
  vec2 center = ray.pos.xz - cyl.pos.xz;
  float c = dot(center, center) - cyl.radius*cyl.radius;
  float b = dot(center, ray.dir.xz);
  float a = dot(ray.dir.xz, ray.dir.xz);
  float D = b * b - a * c;
  if (D < 0)
  {
    collision.hit = false;
    return;
  }
  D = sqrt(D);
  float t1 = (-D - b) / a;
  if (t1 >= 0)
    t = t1;
  else
  {
    float t2 = (D - b) / a;
    if (t2 >= 0)
      t = t2;
    else
    {
      collision.hit = false;
      return;
    }
  }
  collision.pos = ray.pos + ray.dir * t;
  if (collision.pos.y>cyl.pos.y +cyl.height || collision.pos.y<cyl.pos.y) 
  {
    collision.hit = false;
    return;
  }
  collision.hit = true;
  collision.dist = t;

  collision.normal = normalize(collision.pos - cyl.pos);

}

Material find_collision(in Ray ray, out Collision bestCollision)
{
  bestCollision.hit = false;
  bestCollision.dist = FAR_INF;

  Material collisionMat;

/*---------------------------       TRACE SPHERE       ---------------------------*/
  for (int i = 0; i < SphereN; i++)
  {
    Collision collision;
    raycast_sphere(collision, spheres[i], ray);
    if (collision.hit && collision.dist < bestCollision.dist)
    {
      bestCollision = collision;
      collisionMat = spheresMat[i];
    }
  }
  /*---------------------------      TRACE DODEC       ---------------------------*/
  for (int i = 0; i < 1; i++)
  {
    Collision collision;
    raycast_dodec(collision, dodec, ray); //spheres[i], ray);
    if (collision.hit && collision.dist < bestCollision.dist)
    {
      bestCollision = collision;
      collisionMat = dodecMat;
    }
  }
/*---------------------------       TRACE CYLINDER       ---------------------------*/
for (int i = 0; i < 1; i++)
  {
    Collision collision;
    raycast_cylinder(collision, base, ray); //spheres[i], ray);
    if (collision.hit && collision.dist < bestCollision.dist)
    {
      bestCollision = collision;
      collisionMat = baseMat;
    }
  }

  return collisionMat;
}

void create_rays(in Ray ray, in Collision collision, Material material, inout vec4 color)
{
  vec3 normal = collision.normal;

  bool outedRay = dot(ray.dir, normal) > 0;
  float refractionFactor = outedRay ? 1 / material.refractionFactor : material.refractionFactor;

  float noise = textureLod(perlin3D, ray.pos.xzy * 3, 0).x;

  normal.xyz += (vec3(noise) - vec3(0.5))*0.1;//may vary noise parameters to get better image
  normal = normalize(normal);
  if (material.color ==vec3(1,0.2,0.1))///added special case when we want noisy pattern
  {
    color = vec4(noise,0.2,0.1,1);
    return;
  }
  if (material.transparent>1)/// emission material simulation
  {
    color = vec4(material.color,1);
    return;
  }
  vec3 reflected = reflect(ray.dir, normal);

  bool isInsideMaterial = false;
  if (outedRay)
  {
    normal = -normal;
  } else
      isInsideMaterial =true;

  vec3 refracted = refract(ray.dir, normal, refractionFactor);
  bool foolInsideReflaction = dot(refracted, refracted) < 0.1;
  float transparent = material.transparent;

  float t,r,d;
  t = transparent == 1 ? 0 : transparent;
  r = (1 - t) * (1 - material.reflectionFactor); 
  d = (1 - t) * material.reflectionFactor;

  if (transparent * material.reflectionFactor > EPS)
  {

    Ray reflectedRay = ray;
    reflectedRay.dir = reflected;
    reflectedRay.pos += reflected * EPS;
    reflectedRay.transparent += r * (1 - ray.transparent);
    reflectedRay.isInsideMaterial  = false;
    pop_ray_to_stack(reflectedRay);
  }

  if (((1 - transparent) > EPS)) 
  {
    if (foolInsideReflaction)
    {
      color += vec4(material.color, 1) * (1 - transparent) * (1 - ray.transparent);
    }
    else
    {
      Ray refractedRay = ray;
      refractedRay.dir = refracted;
      refractedRay.pos += refracted;//*EPS;
      refractedRay.transparent += t * (1 - ray.transparent);
      refractedRay.isInsideMaterial = isInsideMaterial;
     pop_ray_to_stack(refractedRay);
    }
  }
  color += vec4(computeLight(ray.pos, material.color * transparent * (1 - material.reflectionFactor) * (1 - ray.transparent), normal) ,1);
}

vec2 rotate(vec2 a, float b)
{
    float c = cos(b);
    float s = sin(b);
    return vec2(
        a.x * c - a.y * s,
        a.x * s + a.y * c
    );
}
void main()
{
  clear_stack();
  pop_ray_to_stack(get_ray(fragUV));
  vec4 color = vec4(0);
  Material collisionMat;
  while (!stack_empty())
  {
    Ray ray = get_ray_from_stack();
    Collision bestCollision;

    #ifdef ROTATE
      ray.pos.yz = rotate(ray.pos.yz, .0);
      ray.dir.yz = rotate(ray.dir.yz, .0);

      ray.pos.xz = rotate(ray.pos.xz, Time*.2);
      ray.dir.xz = rotate(ray.dir.xz, Time*.2);
    #endif

    collisionMat = find_collision(ray, bestCollision);
    if (bestCollision.hit)
    {
      ray.pos = bestCollision.pos;
      create_rays(ray, bestCollision, collisionMat, color);
    } else
    {
      color += vec4(textureLod(skybox, ray.dir, 0).rgb, 1) * (1 - ray.transparent);
    }
  }
  FragColor = vec4(color.rgb , 1.0);
}