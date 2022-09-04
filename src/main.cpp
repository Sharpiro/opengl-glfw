// cspell:disable

#define GLAD_GL_IMPLEMENTATION
#define GLFW_INCLUDE_NONE

#define HORZ_GAME 1
#define VERT_GAME 2

#if GAME == HORZ_GAME
#include "games/horz_game.h"
#endif
#if GAME == VERT_GAME
#include "games/vert_game.h"
#endif
#if !GAME
#error no game specified
#endif
#include "board.h"
#include "gl.hpp"
#include "tools.hpp"
#include <GLFW/glfw3.h>
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <vector>

auto window_size = Point{600, 600};
auto cursor = Point{0.0, 0.0};
auto pressed_cursor = Point{0.0, 0.0};
auto board = board_new(3, 1);
auto game_state = GameSate{};
auto scaler = 1.f / (board.size * 2);
auto point_vertices = std::vector<Vertex>{
  {.color = {1, 0, 0}},
  {.color = {0, 0, 1}},
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

static void cursor_pos_callback(GLFWwindow *window, double xpos, double ypos) {
  cursor = {xpos, ypos};
}

static void mouse_click_callback(
  GLFWwindow *window,
  int button,
  int action,
  int mods
) {
  if (game_state.game_over) return;
  if (button == GLFW_MOUSE_BUTTON_1) {
    if (action == GLFW_PRESS) {
      pressed_cursor = cursor;
      board_on_press(&board, window_size, pressed_cursor);
    } else if (action == GLFW_RELEASE) {
      auto click = Click{.start = pressed_cursor, .end = cursor};
      auto board_state = board_on_release(&board, window_size, click);

      common();
#if GAME == VERT_GAME
      vert_on_mouse_release(&board, board_state);
#endif
#if GAME == HORZ_GAME
      horz_on_mouse_release(&board, board_state);
#endif
    }
  } else {
    puts("wrong mouse button");
  }
}

void mouse_drop_callback(
  GLFWwindow *window,
  int path_count,
  const char *paths[]
) {
  // paths is an n sized array of pointers to char
  for (auto i = 0; i < path_count; i++) {
    auto path = paths[i];
    puts(path);
  }
}

void window_resize_callback(GLFWwindow *window, int width, int height) {
  window_size = {(double)width, (double)height};
}

static void key_callback(
  GLFWwindow *window,
  int key,
  int scancode,
  int action,
  int mods
) {
  if (action != GLFW_PRESS && action != GLFW_REPEAT) {
    return;
  }
  bool is_ctrl_pressed = mods & GLFW_MOD_CONTROL;
  if (key == GLFW_KEY_ESCAPE) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  } else if (key == GLFW_KEY_LEFT) {
    // if (board.circle_index % board.size != 0) {
    //   board.circle_index--;
    // }
  } else if (key == GLFW_KEY_RIGHT) {
    // if (board.circle_index % board.size < board.size - 1) {
    //   board.circle_index++;
    // }
  } else if (key == GLFW_KEY_DOWN) {
    // if (board.circle_index + board.size <= (board.size * board.size) - 1) {
    //   board.circle_index += board.size;
    // }
  } else if (key == GLFW_KEY_UP) {
    // if (board.circle_index - board.size >= 0) {
    //   board.circle_index -= board.size;
    // }
  } else if (is_ctrl_pressed && key == GLFW_KEY_MINUS) {
    if (board.size > 1) {
      resize_board(&board, board.size - 1);
    }
  } else if (is_ctrl_pressed && key == GLFW_KEY_EQUAL) {
    resize_board(&board, board.size + 1);
  } else if (key == GLFW_KEY_ENTER || key == GLFW_KEY_KP_ENTER || key == GLFW_KEY_R) {
  }
}

void gl_draw(
  std::vector<Vertex> *vertices,
  DrawContext line_context,
  GLenum mode
) {
  glBindBuffer(GL_ARRAY_BUFFER, line_context.vertex_buffer);
  auto vertices_byte_size = sizeof(Vertex) * vertices->size();
  auto first_vertex = &(*vertices)[0];
  glBufferData(
    GL_ARRAY_BUFFER,
    vertices_byte_size,
    first_vertex,
    GL_STATIC_DRAW
  );

  glBindVertexArray(line_context.vertex_array);
  glEnableVertexAttribArray(line_context.vpos_location);
  glVertexAttribPointer(
    line_context.vpos_location,
    2,
    GL_FLOAT,
    GL_FALSE,
    sizeof(Vertex),
    (void *)offsetof(Vertex, position)
  );
  glEnableVertexAttribArray(line_context.vcol_location);
  glVertexAttribPointer(
    line_context.vcol_location,
    3,
    GL_FLOAT,
    GL_FALSE,
    sizeof(Vertex),
    (void *)offsetof(Vertex, color)
  );

  glDrawArrays(mode, 0, vertices->size());
}

std::vector<Vertex> get_line_vertices() {
  // todo: fmt fix
  auto all_lines =
    concat_vector(&board.horizontal_lines, &board.vertical_lines);
  auto all_vertices = std::vector<Vertex>();
  for (auto l : all_lines) {
    all_vertices.push_back(l.start);
    all_vertices.push_back(l.end);
  }
  return all_vertices;
}

std::vector<Vertex> get_circle_vertices(float scaler, vec3 color) {
  auto triangle_count = 32;
  float max_angle = (M_PI * 2) / triangle_count;
  float current_angle = 0;
  auto triangles = std::vector<Triangle>();
  for (auto i = 0; i < triangle_count; i++) {
    auto start = Vertex{
      .position = {cos(current_angle) * scaler, sin(current_angle) * scaler},
      .color = {color[0], color[1], color[2]}};
    auto end = Vertex{
      .position =
        {
          cos(current_angle + max_angle) * scaler,
          sin(current_angle + max_angle) * scaler,
        },
      .color = {color[0], color[1], color[2]},
    };
    auto triangle = Triangle{
      .a =
        {
          .position = {0, 0},
          .color = {color[0], color[1], color[2]},
        },
      start,
      end};
    current_angle += max_angle;
    triangles.push_back(triangle);
  }

  auto all_vertices = std::vector<Vertex>();
  for (auto &l : triangles) {
    all_vertices.push_back(l.a);
    all_vertices.push_back(l.b);
    all_vertices.push_back(l.c);
  }
  return all_vertices;
}

void update_points(double current_time, float scaler) {
  auto moving_point = &point_vertices[1];
  double x = cos(current_time) * scaler;
  double y = sin(current_time) * scaler;
  moving_point->position[0] = x;
  moving_point->position[1] = y;
}

void update() {
#if GAME == VERT_GAME
  vert_update(&board, &game_state);
#endif
#if GAME == HORZ_GAME
  horz_update(&board, &game_state);
#endif
}

int main(void) {
  srand(time(0));
  glfwSetErrorCallback([](int error, const char *description) -> void {
    fprintf(stderr, "Error: %s\n", description);
  });

  if (!glfwInit()) exit(EXIT_FAILURE);

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window =
    glfwCreateWindow(window_size.x, window_size.y, "glfw", NULL, NULL);
  if (!window) {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  glfwSetWindowPos(window, 450, 500);
  glfwSetKeyCallback(window, key_callback);
  glfwSetMouseButtonCallback(window, mouse_click_callback);
  glfwSetCursorPosCallback(window, cursor_pos_callback);
  glfwSetDropCallback(window, mouse_drop_callback);
  glfwSetWindowSizeCallback(window, window_resize_callback);

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

  auto line_context = get_draw_context(vpos_location, vcol_location);
  auto triangle_context = get_draw_context(vpos_location, vcol_location);
  auto point_context = get_draw_context(vpos_location, vcol_location);
  // auto temp_frames = 0;
  // auto temp_second = 0;
  while (!glfwWindowShouldClose(window)) {
    scaler = 1.f / (board.size * 2);
    timeval time;
    gettimeofday(&time, nullptr);
    auto current_time = time.tv_sec + (double)time.tv_usec / 1000 / 1000;
    update_points(current_time, scaler);

    update();

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    // const float ratio = width / (float)height;
    const float ratio = 1.0;
    // ratio = ratio + ratioTemp;

    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT);

    mat4x4 maybe_model, maybe_projection, model_view_proj;
    mat4x4_identity(maybe_model);
    // mat4x4_rotate_Z(m, m, glfwGetTime() * direction);
    mat4x4_ortho(maybe_projection, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
    mat4x4_mul(model_view_proj, maybe_projection, maybe_model);

    glUseProgram(program);
    glUniformMatrix4fv(
      mvp_location,
      1,
      GL_FALSE,
      (const GLfloat *)&model_view_proj
    );

    glClearColor(.5f, .5f, .5f, 1.f);

    auto line_vertices = get_line_vertices();
    gl_draw(&line_vertices, line_context, GL_LINES);

    for (auto &circle : board.circles) {
      // circle = Circle{};
      auto grid_x = circle.index % board.size;
      auto grid_y = (int)floor(circle.index / board.size);
      auto offset_x = -board.gl_draw_bounds + scaler + (scaler * 2 * grid_x);
      auto offset_y = board.gl_draw_bounds - scaler - (scaler * 2 * grid_y);
      mat4x4_translate(model_view_proj, offset_x, offset_y, 0);
      glUniformMatrix4fv(
        mvp_location,
        1,
        GL_FALSE,
        (const GLfloat *)&model_view_proj
      );

      auto circle_vertices = get_circle_vertices(scaler, circle.current_color);
      gl_draw(&circle_vertices, triangle_context, GL_TRIANGLES);

      if (!game_state.game_over) {
        glPointSize(10);
        gl_draw(&point_vertices, point_context, GL_POINTS);
      }
    }

    if (game_state.game_over) {
      if (!game_state.dumb_bool) {
        game_state.dumb_bool = true;
        puts("you win");
      }
    }

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // while (true) {
  //   sleep(1);
  // }

  glfwDestroyWindow(window);

  glfwTerminate();
  exit(EXIT_SUCCESS);
}
