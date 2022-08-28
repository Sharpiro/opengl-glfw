g++ -Wall -g -pthread \
  $(pkg-config --cflags glfw3) -o main.out main.cpp $(pkg-config --static --libs glfw3)
