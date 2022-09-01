#include "gl.hpp"

DrawContext get_draw_context(GLint vpos_location, GLint vcol_location)
{
  auto context = DrawContext{.vpos_location = vpos_location, .vcol_location = vcol_location};
  glGenBuffers(1, &context.vertex_buffer);
  glGenVertexArrays(1, &context.vertex_array);
  return context;
}
