#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <linmath.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <math.h>
#include <vector>
#include <time.h>

struct Vertex
{
  vec2 position;
  vec3 color;
};

struct Line
{
  Vertex start;
  Vertex end;
};

struct Board
{
  std::vector<Line> horizontal_lines;
};

int direction = 1;
int ratio_temp = 0;
float point_temp = -1;
int board_size = 1;

auto board = Board{};

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

void resize_board(Board *board, int new_size)
{
  board->horizontal_lines = std::vector<Line>{};
  auto section_size = 2.f / new_size;
  auto top = -1.0f;
  auto current_y = top;
  for (int i = 0; i < new_size; i++)
  {
    current_y += section_size;
    auto line = Line{
        .start = {.position = {-1, current_y}, .color = {0, 1, 0}},
        .end = {.position = {1, current_y}, .color = {1, 0, 0}}};
    board->horizontal_lines.push_back(line);
  }
}

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
  if (action != GLFW_PRESS)
  {
    return;
  }
  if (key == GLFW_KEY_ESCAPE)
  {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }
  else if (key == GLFW_KEY_LEFT)
  {
    point_temp -= .01f;
  }
  else if (key == GLFW_KEY_RIGHT)
  {
    point_temp += .01f;
  }
  else if (key == GLFW_KEY_UP)
  {
    board_size++;
    resize_board(&board, board_size);
  }
  else if (key == GLFW_KEY_DOWN)
  {
    board_size--;
    resize_board(&board, board_size);
  }
  else if (key == GLFW_KEY_ENTER || key == GLFW_KEY_KP_ENTER || key == GLFW_KEY_R)
  {
    puts("enter...");
  }
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

  GLFWwindow *window = glfwCreateWindow(640, 480, "glfw", NULL, NULL);
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

  resize_board(&board, board_size);
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
    mat4x4_rotate_Z(m, m, glfwGetTime() * direction);
    mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
    mat4x4_mul(mvp, p, m);

    glUseProgram(program);
    glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat *)&mvp);

    GLuint vertex_buffer;
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    auto vertices_byte_size = sizeof(Line) * board.horizontal_lines.size();
    auto vertices = &board.horizontal_lines[0];
    glBufferData(GL_ARRAY_BUFFER, vertices_byte_size, vertices, GL_STATIC_DRAW);

    GLuint vertex_array;
    glGenVertexArrays(1, &vertex_array);
    glBindVertexArray(vertex_array);
    glEnableVertexAttribArray(vpos_location);
    glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE,
                          sizeof(Vertex), (void *)offsetof(Vertex, position));
    glEnableVertexAttribArray(vcol_location);
    glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE,
                          sizeof(Vertex), (void *)offsetof(Vertex, color));

    glBindVertexArray(vertex_array);
    // glDrawArrays(GL_POINTS, 0, 3);
    // glDrawArrays(GL_TRIANGLES, 0, 3);
    glDrawArrays(GL_LINES, 0, board.horizontal_lines.size() * 2);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwDestroyWindow(window);

  glfwTerminate();
  exit(EXIT_SUCCESS);
}

//! [code]
