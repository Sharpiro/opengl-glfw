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
float y_value = 0;

auto board = Board{
    .horizontal_lines = {
        Line{
            .start = {{point_temp, y_value}, {0.f, 1.f, 0.f}},
            .end = {{1.0f, y_value}, {1.f, 0.f, 0.f}}}
        //  Line{
        //      .start = {{point_temp, 0.5f}, {1.f, 0.f, 0.f}},
        //      .end = {{1.0f, 0.5f}, {0.f, 1.f, 0.f}}},
        //  Line{
        //      .start = {{point_temp, -0.5f}, {1.f, 0.f, 0.f}},
        //      .end = {{1.0f, -0.5f}, {0.f, 1.f, 0.f}}}}
    }};

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

static void error_callback(int error, const char *description)
{
  fprintf(stderr, "Error: %s\n", description);
}

void resize_board(Board *board, int bump_amount)
{
  auto temp = board->horizontal_lines[0];
  // temp.start.position = {-1.f, 0.5f};
  // temp.start.position = {};
  int randVal = rand();
  int rand_sign = rand() & 0x1;
  rand_sign = rand_sign == 0 ? -1 : 1;
  int temp1 = 0x007fffff;
  // int temp1 = 0x807fffff;
  float temp2 = ((randVal & temp1) / (float)temp1) * rand_sign;

  temp.start = {{-1.f, temp2}, {0.f, 1.f, 0.f}};
  board->horizontal_lines.push_back(temp);
  // board->horizontal_lines = {
  // Line{
  //     .start = {{point_temp, y_value}, {0.f, 1.f, 0.f}},
  //     .end = {{1.0f, y_value}, {1.f, 0.f, 0.f}}},
  // Line{
  //     .start = {{point_temp, 0.5f}, {1.f, 0.f, 0.f}},
  //     .end = {{1.0f, 0.5f}, {0.f, 1.f, 0.f}}}};
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
    y_value += .01f;
  }
  else if (key == GLFW_KEY_DOWN)
  {
    y_value -= .01f;
  }
  else if (key == GLFW_KEY_ENTER || key == GLFW_KEY_KP_ENTER || key == GLFW_KEY_R)
  {
    resize_board(&board, 1);
    puts("enter...");
    // direction *= -1;
    // ratioTemp -= 1;
  }
}

time_t t;

int main(void)
{
  // srand(time(&t));
  srand(time(0));
  glfwSetErrorCallback(error_callback);

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

    GLuint vertex_buffer;
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    auto vertices_byte_size = sizeof(Line) * board.horizontal_lines.size();
    auto vertices = &board.horizontal_lines[0];
    // Vertex vertices[] =
    //     {
    //         // {{-1.0f, 0.f}, {0.f, 1.f, 0.f}},
    //         {{point_temp, y_value}, {0.f, 1.f, 0.f}},
    //         {{1.0f, y_value}, {1.f, 0.f, 0.f}}};
    // do_nothing(&vertices);
    // auto vert_pointer = &vertices;
    // const Vertex *vert_pointer2 = vertices;
    // {{0.f, 0.6f}, {0.f, 1.f, 0.f}},
    // {{0.f, -0.4f}, {0.f, 1.f, 1.f}}};
    // const int temp = sizeof(vertices);
    // const int temp2 = sizeof(*vert_pointer);
    // const int temp3 = sizeof(*vert_pointer2);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(lines[0]), &lines[0], GL_STATIC_DRAW);
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
