  # -std=c++20 \

# g++ -Wall -Wno-unused-function \
#   -g -pthread \
#   -D GAME=1 \
#   $(pkg-config --cflags glfw3) \
#   -o main.out src/main.cpp src/board.cpp src/gl.cpp src/games/horz_game.cpp src/temp.cpp \
#   $(pkg-config --static --libs glfw3)

g++ -Wall -Wno-unused-function \
  -g -pthread \
  -D GAME=2 \
  $(pkg-config --cflags glfw3) \
  -o main.out src/main.cpp src/board.cpp src/gl.cpp src/games/vert_game.cpp src/temp.cpp \
  $(pkg-config --static --libs glfw3)
