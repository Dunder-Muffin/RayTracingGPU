#pragma once
#include <iostream>
#include <string>
#include "glad/glad.h"
#include <memory>
using namespace std;

void debug_error(const char *format, ...);
void debug_log(const char *format, ...);
void debug_show();

#define F4FMT "[%f, %f, %f, %f]\n"
#define F4ARG(v) v[0], v[1], v[2], v[3]

#define log_mat4x4(mat) \
{ mat4x4 _m_ = (mat); fprintf(stdout, "%s\n" F4FMT F4FMT F4FMT F4FMT, #mat, F4ARG(_m_[0]), F4ARG(_m_[1]), F4ARG(_m_[2]), F4ARG(_m_[3])); }

#define debug_vec2(v) \
{vec2 _v_ = (v); fprintf(stdout, "%s\n[%f, %f]\n", #v, _v_.x,_v_.y); }
#define debug_vec3(v) \
{vec3 _v_ = (v); fprintf(stdout, "%s\n[%f, %f, %f]\n", #v, _v_.x,_v_.y,_v_.z); }
#define debug_vec4(v) \
{vec4 _v_ = (v); fprintf(stdout, "%s\n[%f, %f, %f, %f]\n", #v, _v_.x,_v_.y,_v_.z,_v_.w); }



