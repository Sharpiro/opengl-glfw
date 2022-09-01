// cspell:disable

#define GLAD_GL_IMPLEMENTATION
#define GLFW_INCLUDE_NONE

#include "gl.hpp"
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <math.h>
#include <vector>
#include <time.h>
#include <sys/time.h>
#include "board.h"
#include "tools.hpp"

// int direction = 1;
// int ratio_temp = 0;
// float point_temp = -1;

auto board = Board{.size = 8};
auto point_vertices = std::vector<Vertex>{
    {.position = {0, 0}, .color = {1, 0, 0}},
    {.position = {0, .5}, .color = {0, 0, 1}},
};

static const char *vertex_shader_text =
    "#version 330\n"
    "uniform mat4 MVP;\n"
    "in vec3 vCol;\n"
    "in vec2 vPos;\n"
    "out vec3 color;\n"
    "void main()\n"
    "{\n"
    "    gl_Position = MVP * vec4(vPos, 0.0, 1.0);\n"
    "    color = vCol;\n"
    "}\n";

static const char *fragment_shader_text =
    "#version 330\n"
    "in vec3 color;\n"
    "out vec4 fragment;\n"
    "void main()\n"
    "{\n"
    "    fragment = vec4(color, 1.0);\n"
    "}\n";

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
  // if (action != GLFW_PRESS)
  // {
  //   return;
  // }
  if (key == GLFW_KEY_ESCAPE)
  {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }
  else if (key == GLFW_KEY_LEFT)
  {
    // point_temp -= .01f;
  }
  else if (key == GLFW_KEY_RIGHT)
  {
    // point_temp += .01f;
  }
  else if (key == GLFW_KEY_UP)
  {
    board.size++;
    resize_board(&board, board.size);
    resize_board_vert(&board, board.size);
  }
  else if (key == GLFW_KEY_DOWN)
  {
    board.size--;
    resize_board(&board, board.size);
    resize_board_vert(&board, board.size);
  }
  else if (key == GLFW_KEY_ENTER || key == GLFW_KEY_KP_ENTER || key == GLFW_KEY_R)
  {
    puts("enter...");
  }
}

void gl_draw(
    std::vector<Vertex> *vertices,
    DrawContext line_context,
    GLenum mode)
{
  glBindBuffer(GL_ARRAY_BUFFER, line_context.vertex_buffer);
  auto vertices_byte_size = sizeof(Vertex) * vertices->size();
  auto first_vertex = &(*vertices)[0];
  glBufferData(GL_ARRAY_BUFFER, vertices_byte_size, first_vertex, GL_STATIC_DRAW);

  glBindVertexArray(line_context.vertex_array);
  glEnableVertexAttribArray(line_context.vpos_location);
  glVertexAttribPointer(line_context.vpos_location, 2, GL_FLOAT, GL_FALSE,
                        sizeof(Vertex), (void *)offsetof(Vertex, position));
  glEnableVertexAttribArray(line_context.vcol_location);
  glVertexAttribPointer(line_context.vcol_location, 3, GL_FLOAT, GL_FALSE,
                        sizeof(Vertex), (void *)offsetof(Vertex, color));

  glDrawArrays(mode, 0, vertices->size());
}

std::vector<Vertex> get_line_vertices()
{
  auto all_lines = concat_vector(&board.horizontal_lines, &board.vertical_lines);
  auto all_vertices = std::vector<Vertex>();
  for (auto l : all_lines)
  {
    all_vertices.push_back(l.start);
    all_vertices.push_back(l.end);
  }
  return all_vertices;
}

std::vector<Vertex> get_triangle_vertices(float scaler)
{
  auto triangle_count = 32;
  float max_angle = (M_PI * 2) / triangle_count;
  float current_angle = 0;
  auto triangles = std::vector<Triangle>();
  for (auto i = 0; i < triangle_count; i++)
  {
    auto start = Vertex{
        .position = {cos(current_angle) * scaler, sin(current_angle) * scaler},
        .color = {0, 1, 0}};
    auto end = Vertex{
        .position = {cos(current_angle + max_angle) * scaler, sin(current_angle + max_angle) * scaler},
        .color = {0, 1, 0}};
    auto triangle = Triangle{
        .a = {.position = {0, 0}, .color = {0, 1, 0}},
        start,
        end};
    current_angle += max_angle;
    triangles.push_back(triangle);
  }

  auto circle = ChunkyCircle{
      .triangles = triangles};
  auto all_vertices = std::vector<Vertex>();
  for (auto &l : circle.triangles)
  {
    all_vertices.push_back(l.a);
    all_vertices.push_back(l.b);
    all_vertices.push_back(l.c);
  }
  return all_vertices;
}

void update_points(double current_time, float scaler)
{
  auto moving_point = &point_vertices[1];
  double x = cos(current_time) * scaler;
  double y = sin(current_time) * scaler;
  moving_point->position[0] = x;
  moving_point->position[1] = y;
}

int main(void)
{
  srand(time(0));
  glfwSetErrorCallback([](int error, const char *description) -> void
                       { fprintf(stderr, "Error: %s\n", description); });

  if (!glfwInit())
    exit(EXIT_FAILURE);

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window = glfwCreateWindow(800, 800, "glfw", NULL, NULL);
  if (!window)
  {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  glfwSetWindowPos(window, 500, 500);
  glfwSetKeyCallback(window, key_callback);

  glfwMakeContextCurrent(window);
  gladLoadGL(glfwGetProcAddress);
  glfwSwapInterval(1);

  const GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
  glCompileShader(vertex_shader);

  const GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
  glCompileShader(fragment_shader);

  const GLuint program = glCreateProgram();
  glAttachShader(program, vertex_shader);
  glAttachShader(program, fragment_shader);
  glLinkProgram(program);

  const GLint mvp_location = glGetUniformLocation(program, "MVP");
  const GLint vpos_location = glGetAttribLocation(program, "vPos");
  const GLint vcol_location = glGetAttribLocation(program, "vCol");

  resize_board(&board, board.size);
  resize_board_vert(&board, board.size);

  auto line_context = get_draw_context(vpos_location, vcol_location);
  auto triangle_context = get_draw_context(vpos_location, vcol_location);
  auto point_context = get_draw_context(vpos_location, vcol_location);
  while (!glfwWindowShouldClose(window))
  {
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    // const float ratio = width / (float)height;
    const float ratio = 1.0;
    // ratio = ratio + ratioTemp;

    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT);

    mat4x4 m, p, mvp;
    mat4x4_identity(m);
    // mat4x4_rotate_Z(m, m, glfwGetTime() * direction);
    mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
    mat4x4_mul(mvp, p, m);

    glUseProgram(program);
    glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat *)&mvp);

    auto line_vertices = get_line_vertices();
    gl_draw(&line_vertices, line_context, GL_LINES);

    float scaler = .25;
    auto triangle_vertices = get_triangle_vertices(scaler);
    gl_draw(&triangle_vertices, triangle_context, GL_TRIANGLES);

    glPointSize(10);
    gl_draw(&point_vertices, point_context, GL_POINTS);

    glfwSwapBuffers(window);
    glfwPollEvents();

    timeval time;
    gettimeofday(&time, nullptr);
    auto current_time = time.tv_sec + (double)time.tv_usec / 1000 / 1000;
    update_points(current_time, scaler);
  }

  glfwDestroyWindow(window);

  glfwTerminate();
  exit(EXIT_SUCCESS);
}
