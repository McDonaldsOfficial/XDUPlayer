#include <raylib.h>
#include <iostream>
#define LEXER_TEST 0

#include "./Headers/Renderer.h"
#define SCREEN_WIDTH 1500
#define SCREEN_HEIGHT 900
#if LEXER_TEST
#include "./Headers/Animator.h"
#include "./Headers/Reader.h"
#include "./Headers/Lexer.h"
#endif
int main(){
#if LEXER_TEST
    std::string file = read_file();        
    Lexer lexer(file);
    Token t = lexer.next();
    while( t.kind != TOKEN_END ){
        std::cout << t.text << "  " << lexer.token_kind_name(t.kind) << '\n';
        t = lexer.next();
    }
    std::cout << "End\n";
    return 0;
#endif
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Player");    
    InitAudioDevice();
    Renderer *renderer = new Renderer();
    SetTargetFPS(24);
    while(!WindowShouldClose()){        
        renderer->render_update();
    }
    delete renderer;
    CloseAudioDevice();
    CloseWindow();
    return 0;
}

int main2(){
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Player");        
    SetTargetFPS(24);
    Font font = LoadFont("resources/fonts/mecha.png");
    const int posX = 0;
    const int posY = 0;
    const int outlineSize = 1;
    const int fontSize = 50;

    while(!WindowShouldClose()){        
        BeginDrawing();
        ClearBackground(GRAY);
            
        EndDrawing();    
    }
    
    CloseWindow();
    return 0;
}