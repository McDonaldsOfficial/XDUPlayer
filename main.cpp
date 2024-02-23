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
    Renderer *renderer = new Renderer();
    SetTargetFPS(24);
    while(!WindowShouldClose()){        
        renderer->render_update();
    }
    delete renderer;
    CloseWindow();
    return 0;
}

int main2(){
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Player");        
    SetTargetFPS(24);
    Image image = LoadImage("Images/bg3001_03_02.png");    
    Texture2D texture = LoadTextureFromImage(image);
    UnloadImage(image);
    Camera2D camera = {0};
    camera.zoom = 1.2;
    camera.offset = {SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2};
    camera.target = {SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2};
    Animator animator(&camera);
    while(!WindowShouldClose()){        
        BeginDrawing();
        ClearBackground(BLACK);
        BeginMode2D(camera);
        DrawTexturePro(
            texture,
            (Rectangle){.x = 0.0F, .y = 0.0F, .width = (float)texture.width, .height = (float)texture.height},
            (Rectangle){.x = 0.0F, .y = 0.0F, .width = (float)SCREEN_WIDTH, .height = (float)SCREEN_HEIGHT * 2},
            (Vector2){.x = 0.0F, .y = 500},
            0.0F,
            WHITE);  

        animator.MoveCamera({SCREEN_WIDTH /2 - 230, SCREEN_HEIGHT / 2 + 230 } ,2,2);   
             
        EndMode2D();
        EndDrawing();    
    }
    
    CloseWindow();
    return 0;
}