  # -std=c++20 \
g++ -Wall -g -pthread \
  $(pkg-config --cflags glfw3) \
  -o main.out src/main.cpp src/board.cpp \
  $(pkg-config --static --libs glfw3)
