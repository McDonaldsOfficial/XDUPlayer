CFLAGS = -I C:\raylib\raylib\src -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
run: g++ Lexer.cpp Reader.cpp Renderer.cpp  main.cpp -o a.out $(CFLAGS)