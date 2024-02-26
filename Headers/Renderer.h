#pragma once
#include <raylib.h>
#include <string>
#include <map>
#include "./Lexer.h"
#include "./Animator.h"

typedef enum {
    NO_ANIMATION = 0,
    FADE_IN,
    FADE_OUT
} Animation_Type;

typedef struct{
    //Textures
    Texture2D textbar;
    Texture2D hide_button;
    Texture2D skip_button;
    Texture2D next_icon;
    Texture2D background;

    //File
    std::string file;
    //Dialogue
    std::string current_name;
    std::string current_dialog;
    std::string full_dialog;
    int dialog_chara_counter = 0;

    //Sound
    Music background_music;
    Sound character_noise;
} Render;

struct Character {
    std::string name;
    std::string path;
};

struct ScreenAnimations {
    Animation_Type type;
    Color dest_color;
    Color origin_color;
    float animation_time;
};

struct CharaScreen {
    Texture2D texture;
    Vector2 pos;
    Color dest_color;
    Color from_color;
    Animation_Type animation;    
};

struct CameraParams {
    Vector2 camera_target;
    float zoom;
    float seconds;
};

struct Timer{
    float life_time;
};

class Renderer{
    public:        
        void render_update();
        Renderer();
        ~Renderer();             
    private:        
        //Variables
        Render render;
        Lexer lexer;
        CameraParams cam_param;
        Camera2D camera = {0};
        ScreenAnimations screen_animations;
        Timer timer = {.life_time = 0};
        int ofset;
        int outline_size = 1;
        float next_icon_pos_y;
        Animator animator;        
        Font font =  LoadFont("resources/fonts/alpha_beta.png");
        std::map<std::string, Character> variable_map;
        std::map<std::string, CharaScreen> character_on_screen;        
        Color visible_chara = {.r = 255, .g = 255, .b = 255, .a = 255};
        Color background_chara_color;
        Color invisible_chara = {.r = 255, .g = 255, .b = 255, .a = 0};
        Vector2 hide_button_pos;
        Vector2 skip_button_pos;
        Vector2 name_text_pos;
        Vector2 dialoge_text_pos;
        Vector2 next_icon_pos;
        Vector2 dialogue_text_pos;
        Vector2 screen_center;
        bool is_paused = false;
        std::string prev_character_name_dialogue;   
        //Functions
        Texture2D load_texture(std::string path);
        void add_dialogue();
        void draw_screen();
        void wait();
        void parse_instruction();
        void add_variable();
        void add_background();
        void add_character();
        void move_camera();
        void remove_character();
        void fade_screen(bool fade_out);
        void write_to_screen();
        void chara_dialogue();
        void fade_in(std::string key);
        bool fade_out(std::string key);        
        void play_music();
        void play_sound(); //Just in case
        bool are_the_same_color(Color color1, Color color2);
        bool timer_done();
        void update_timer();
        void start_timer(float life_time);
        void color_to_color(std::string key);
        void draw_background();
        void FadeScreen();       
        void write_outline(); 
        Color string_to_color(std::string color);
        Token validate_token(Token_Kind kind, std::string message);
        std::string get_sprite_number(std::string name);     
};