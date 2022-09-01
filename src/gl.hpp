#pragma once

#include <glad/gl.h>

struct DrawContext
{
  GLint vpos_location;
  GLint vcol_location;
  GLuint vertex_buffer;
  GLuint vertex_array;
};

DrawContext get_draw_context(GLint vpos_location, GLint vcol_location);
