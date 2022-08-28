cc -Wall -g -pthread \
  $(pkg-config --cflags glfw3) -o main.out main.c $(pkg-config --static --libs glfw3)
