#include "./Headers/Renderer.h"
#include "./Headers/Reader.h"
#include <cstdlib>
#define SCREEN_WIDTH 1500
#define SCREEN_HEIGHT 900

Renderer::Renderer(){
    render.textbar = this->load_texture("Images/newui_main.png");
    render.hide_button = this->load_texture("Images/hide_button.png");
    render.skip_button = this->load_texture("Images/skip_button.png");
    render.next_icon = this->load_texture("Images/next_icon.png");
    render.current_dialog = "";          
    render.current_name = "";
    this->screen_center = {.x = (float)(SCREEN_WIDTH / 2), .y = (float)(SCREEN_HEIGHT / 2)};
    this->hide_button_pos = {.x = 1650, .y = 800};
    this->skip_button_pos = {.x = 1550, .y = 800};
    this->name_text_pos  = {.x = 100, .y = 675};
    this->dialoge_text_pos  = {.x = 100, .y = 740};
    this->dialogue_text_pos = {.x = 100, .y = 740};
    this->ofset = -1;
    this->next_icon_pos_y = 950;
    this->next_icon_pos = {.x = 1550, .y = next_icon_pos_y};    
    this->lexer = Lexer(read_file());
    this->background_chara_color = (Color){.r = 125, .g = 125, .b = 125, .a = 255};
    this->camera.zoom = 1;
    this->camera.offset = screen_center;
    this->camera.target = screen_center;
    this->animator = Animator(&this->camera);
    this->cam_param.camera_target = screen_center;
    this->cam_param.zoom = 1;
    this->screen_animations.type = NO_ANIMATION;
    this->parse_instruction();
    // for(auto const &[key, val] : this->variable_map){
    //     std::cout << key
    //               << ':'
    //               << val.name
    //               << '\t'
    //               << val.path
    //               << '\n';
    // }
}
Renderer::~Renderer(){
    UnloadTexture(render.textbar);
    UnloadTexture(render.hide_button);
    UnloadTexture(render.skip_button);
    UnloadTexture(render.next_icon);   
    if(render.background.id > 0) UnloadTexture(render.background); 
    for(auto const &[key, val] : this->character_on_screen){
        if(val.texture.id > 0)
            UnloadTexture(val.texture);
    }
}


Texture2D Renderer::load_texture(std::string path){
    Image image = LoadImage(path.c_str());    
    Texture2D texture = LoadTextureFromImage(image);
    
    UnloadImage(image);
    return texture;
}

void Renderer::write_dialoge_chara(){
    if(this->render.current_name.empty()){
        return;        
    }  
    DrawText(this->render.current_name.c_str(), name_text_pos.x, name_text_pos.y, 30, LIGHTGRAY);    
    if(this->render.current_dialog.length() < this->render.full_dialog.length()){
        this->render.current_dialog.push_back(this->render.full_dialog[this->render.dialog_chara_counter]);
        this->render.dialog_chara_counter++;
    }
    DrawText(this->render.current_dialog.c_str(), dialogue_text_pos.x, dialogue_text_pos.y, 27, LIGHTGRAY);  
}

void Renderer::fade_in(std::string key){
    int animation_rate = 15;
    if(this->character_on_screen[key].from_color.a + animation_rate >= 255){
        this->character_on_screen[key].from_color = this->character_on_screen[key].dest_color;
        this->character_on_screen[key].animation = NO_ANIMATION;
        return;
    }
    this->character_on_screen[key].from_color.a += animation_rate; //TODO: Cambiar el valor para que se ajuste a los fps                
    
}
bool Renderer::fade_out(std::string key){
    const int animation_rate = 50;
    if(this->character_on_screen[key].from_color.a - animation_rate <= 0){
        this->character_on_screen[key].from_color = this->character_on_screen[key].dest_color;
        UnloadTexture(this->character_on_screen[key].texture);
        this->character_on_screen.erase(key);
        return true;
    }
    this->character_on_screen[key].from_color.a -= animation_rate; //TODO: Cambiar el valor para que se ajuste a los fps                                
    return false;
}

//TODO:Maybe find possible bugswith FPS
void Renderer::color_to_color(std::string key){
    CharaScreen c = this->character_on_screen[key];
    const int animation_rate = 10;
    float r = c.from_color.r + (c.dest_color.r - c.from_color.r) * GetFrameTime() * GetFPS() / animation_rate ;
    float g = c.from_color.g + (c.dest_color.g - c.from_color.g) * GetFrameTime() * GetFPS() / animation_rate ;
    float b = c.from_color.b + (c.dest_color.b - c.from_color.b) * GetFrameTime() * GetFPS() / animation_rate ;
    float a = c.from_color.a + (c.dest_color.a - c.from_color.a) * GetFrameTime() * GetFPS() / animation_rate ;
    this->character_on_screen[key].from_color = (Color){r,g,b,a};
    // if( c.from_color.r < c.dest_color.r ){
    //     if(c.from_color.r + animation_rate > c.dest_color.r) this->character_on_screen[key].from_color.r = c.dest_color.r;
    //     else this->character_on_screen[key].from_color.r += animation_rate;    
    // }
    // if( c.from_color.g < c.dest_color.g ){
    //     if(c.from_color.g + animation_rate > c.dest_color.g) this->character_on_screen[key].from_color.g = c.dest_color.g;
    //     else this->character_on_screen[key].from_color.g += animation_rate;    
    // }
    // if( c.from_color.b < c.dest_color.b ){
    //     if(c.from_color.b + animation_rate > c.dest_color.b) this->character_on_screen[key].from_color.b = c.dest_color.b;
    //     else this->character_on_screen[key].from_color.b += animation_rate;    
    // }
    // if( c.from_color.a < c.dest_color.a ){
    //     if(c.from_color.a + animation_rate > c.dest_color.a) this->character_on_screen[key].from_color.a = c.dest_color.a;
    //     else this->character_on_screen[key].from_color.a += animation_rate;    
    // }
    // if( c.from_color.r > c.dest_color.r ){
    //     if(c.from_color.r - animation_rate < c.dest_color.r) this->character_on_screen[key].from_color.r = c.dest_color.r;
    //     else this->character_on_screen[key].from_color.r -= animation_rate;    
    // }
    // if( c.from_color.g > c.dest_color.g ){
    //     if(c.from_color.g - animation_rate < c.dest_color.g) this->character_on_screen[key].from_color.g = c.dest_color.g;
    //     else this->character_on_screen[key].from_color.g -= animation_rate;    
    // }
    // if( c.from_color.b > c.dest_color.b ){
    //     if(c.from_color.b - animation_rate < c.dest_color.b) this->character_on_screen[key].from_color.b = c.dest_color.b;
    //     else this->character_on_screen[key].from_color.b -= animation_rate;    
    // }
    // if( c.from_color.a > c.dest_color.a ){
    //     if(c.from_color.a - animation_rate < c.dest_color.a) this->character_on_screen[key].from_color.a = c.dest_color.a;
    //     else this->character_on_screen[key].from_color.a -= animation_rate;    
    // }
}

bool Renderer::are_the_same_color(Color color1, Color color2){
    return color1.r == color2.r
        && color1.g == color2.g
        && color1.b == color2.b
        && color1.a == color2.a;
}

void Renderer::FadeScreen(){
    //const float animation_rate = 5;
    Color start_color = this->screen_animations.origin_color;
    Color end_color = this->screen_animations.dest_color;
    float rate = GetFrameTime() / this->screen_animations.animation_time;
    float r = start_color.r + ( end_color.r - start_color.r ) * rate;
    float g = start_color.g + ( end_color.g - start_color.g ) * rate;
    float b = start_color.b + ( end_color.b - start_color.b ) * rate;
    float a = start_color.a + ( end_color.a - start_color.a ) * rate;

    this->screen_animations.origin_color = (Color){r,g,b,a};

    // if( this->screen_animations.origin_color.r < this->screen_animations.dest_color.r ){
    //     if(this->screen_animations.origin_color.r + animation_rate > this->screen_animations.dest_color.r) this->screen_animations.origin_color.r = this->screen_animations.dest_color.r;
    //     else this->screen_animations.origin_color.r += animation_rate;    
    // }
    // if( this->screen_animations.origin_color.g < this->screen_animations.dest_color.g ){
    //     if(this->screen_animations.origin_color.g + animation_rate > this->screen_animations.dest_color.g) this->screen_animations.origin_color.g = this->screen_animations.dest_color.g;
    //     else this->screen_animations.origin_color.g += animation_rate;    
    // }
    // if( this->screen_animations.origin_color.b < this->screen_animations.dest_color.b ){
    //     if(this->screen_animations.origin_color.b + animation_rate > this->screen_animations.dest_color.b) this->screen_animations.origin_color.b = this->screen_animations.dest_color.b;
    //     else this->screen_animations.origin_color.b += animation_rate;    
    // }
    // if( this->screen_animations.origin_color.a < this->screen_animations.dest_color.a ){
    //     if(this->screen_animations.origin_color.a + animation_rate > this->screen_animations.dest_color.a) this->screen_animations.origin_color.a = this->screen_animations.dest_color.a;
    //     else this->screen_animations.origin_color.a += animation_rate;    
    // }
    // if( this->screen_animations.origin_color.r > this->screen_animations.dest_color.r ){
    //     if(this->screen_animations.origin_color.r - animation_rate < this->screen_animations.dest_color.r) this->screen_animations.origin_color.r = this->screen_animations.dest_color.r;
    //     else this->screen_animations.origin_color.r -= animation_rate;    
    // }
    // if( this->screen_animations.origin_color.g > this->screen_animations.dest_color.g ){
    //     if(this->screen_animations.origin_color.g - animation_rate < this->screen_animations.dest_color.g) this->screen_animations.origin_color.g = this->screen_animations.dest_color.g;
    //     else this->screen_animations.origin_color.g -= animation_rate;    
    // }
    // if( this->screen_animations.origin_color.b > this->screen_animations.dest_color.b ){
    //     if(this->screen_animations.origin_color.b - animation_rate < this->screen_animations.dest_color.b) this->screen_animations.origin_color.b = this->screen_animations.dest_color.b;
    //     else this->screen_animations.origin_color.b -= animation_rate;    
    // }
    // if( this->screen_animations.origin_color.a > this->screen_animations.dest_color.a ){
    //     if(this->screen_animations.origin_color.a - animation_rate < this->screen_animations.dest_color.a) this->screen_animations.origin_color.a = this->screen_animations.dest_color.a;
    //     else this->screen_animations.origin_color.a -= animation_rate;    
    // }
}

void Renderer::draw_screen(){
    //DrawTexturePro(render.textbar, src_textbar, dst_textbar, (Vector2){0, 0}, 0, WHITE);
    
    for(auto const &[key, val] : this->character_on_screen){
        if(val.texture.id <= 0)
            continue;
        if(val.animation == FADE_IN){            
            this->fade_in(key);
        }        
        if(val.animation == FADE_OUT){
            if(this->fade_out(key)) continue;
        }        
        if(!this->are_the_same_color(val.from_color, val.dest_color)){
            this->color_to_color(key);
        }
        DrawTexture(val.texture, val.pos.x, val.pos.y ,val.from_color);
    }

    DrawTexturePro(
        render.textbar,
        (Rectangle){0, 0, (float)render.textbar.width, (float)render.textbar.height},
        (Rectangle){0, 0, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT},
        (Vector2){0,0},
        0,
        WHITE
    );

    DrawTexturePro(
        render.hide_button,
        (Rectangle){0, 0,(float) render.hide_button.width, (float)render.hide_button.height},
        (Rectangle){this->hide_button_pos.x , this->hide_button_pos.y, 100,100},
        (Vector2){.x = (float)render.hide_button.height / 2, .y = (float)render.hide_button.width / 2},
        0,
        WHITE
    );

    DrawTexturePro(
        render.skip_button,
        (Rectangle){0, 0, (float)render.hide_button.width, (float)render.hide_button.height},
        (Rectangle){.x = skip_button_pos.x, .y = skip_button_pos.y, .width = 100, .height = 100},
        (Vector2){.x = (float)render.hide_button.height / 2, .y =(float) render.hide_button.width / 2},
        0,
        WHITE
    );
    DrawTexturePro(
        render.next_icon,
        (Rectangle){.x = 0, .y = 0, .width = (float)render.next_icon.width, .height = (float)render.next_icon.height },
        (Rectangle){.x = next_icon_pos.x, .y = next_icon_pos.y, .width = 58, .height = 140},
        (Vector2){.x = (float)render.next_icon.height / 2, .y = (float)render.hide_button.width / 2},
        0.0f,
        WHITE
    );
    next_icon_pos.y += ofset;
    if (next_icon_pos.y < next_icon_pos_y - 5)
    {
        ofset = 1;
    }
    else if (next_icon_pos.y > next_icon_pos_y + 5)
    {
        ofset = -1;
    }
    this->write_dialoge_chara();
    if(this->screen_animations.type != NO_ANIMATION){
        if(!this->are_the_same_color( this->screen_animations.origin_color, this->screen_animations.dest_color )){
            this->FadeScreen();
        }else{
            this->screen_animations.type == NO_ANIMATION;
        }
        DrawRectangle(0,0, SCREEN_WIDTH, SCREEN_HEIGHT, this->screen_animations.origin_color);
    }
}

Token Renderer::validate_token(Token_Kind kind, std::string message){
    Token t = this->lexer.next();
    if(t.kind != kind){
        throw std::invalid_argument(message);
    }
    return t;
}   

void Renderer::add_variable(){
    Token t = this->validate_token(TOKEN_SYMBOL, "La variable CHARACTER debe de ser seguida por un identificador");    
    std::string var_name = t.text;    
    t = this->validate_token(TOKEN_EQUALS,"El identificador debe de ser seguido por el nombre" );
    t = this->validate_token(TOKEN_STRING_LITERAL,"Luego de igual se debe de poner un valor" );        
    std::string var_value = t.text;
    t = this->validate_token(TOKEN_STRING_LITERAL, "Luego del nombre del personaje tiene que venir su path al sprite");
    std::string var_path = t.text;    
  
    Character chara = {
        var_value,
        var_path      
    };
    this->variable_map[var_name] = chara;    
}

void Renderer::add_background(){
    Token t = this->validate_token(TOKEN_STRING_LITERAL, "ADD_BACKGROUND debe de ser seguido por un path");
    this->render.background = load_texture(t.text);
}

void Renderer::add_character(){
    Token t = this->validate_token(TOKEN_SYMBOL, "Debe de especificar el nombre el personaje");    
    if(this->variable_map.find(t.text) == this->variable_map.end()){
        throw std::invalid_argument("No existe la variable");
    }
    std::string name_var = t.text;
    Character c = this->variable_map[name_var];
    Texture2D texture = load_texture(c.path);
    t = this->validate_token(TOKEN_SYMBOL, "Debe de especificar la dirección del personaje");
    Vector2 pos;
    if(t.text == "LEFT"){
        pos = (Vector2) {.x = (float)(SCREEN_WIDTH / 3) - (float)(texture.width / 2), .y = 100} ;
    }else if(t.text == "CENTER"){
        pos = (Vector2) {.x= (float)(SCREEN_WIDTH / 2) - (float)(texture.width / 2), .y = 100} ;
    }else if(t.text == "RIGHT"){
        pos = (Vector2) {.x = (float)(texture.width / 2), .y = 100};
    }
    
    //this->character_on_screen[name_var] = (CharaScreen){texture, pos, background_chara_color, FADE_IN};
    this->character_on_screen[name_var] = (CharaScreen){texture, pos, background_chara_color, invisible_chara, FADE_IN};
}

void Renderer::remove_character(){
    Token t = this->validate_token(TOKEN_SYMBOL, "Remove recibe como argumento el nombre del personaej");
    if(this->character_on_screen.find(t.text) == this->character_on_screen.end()){
        throw std::invalid_argument("No existe la variable");        
    }
    this->character_on_screen[t.text].animation = FADE_OUT;
    

    // UnloadTexture(c.texture);
    // this->character_on_screen.erase(t.text);
}

void Renderer::move_camera(){
    Token t = this->lexer.next();
    if(t.kind != TOKEN_SYMBOL && t.kind != TOKEN_NUMBER){
        throw std::invalid_argument("Debe de especificar un número o una palabra válida");
    }
    if(t.text == "RESTORE"){
        this->cam_param.camera_target = this->screen_center;
        this->cam_param.zoom = 1;
        return;
    }
    float x,y, zoom;
    if(t.text == "CENTER"){
        x = this->screen_center.x;        
    }else{
        x = std::stof(t.text);
    }
    t = this->lexer.next();
    if(t.kind != TOKEN_SYMBOL && t.kind != TOKEN_NUMBER){
        throw std::invalid_argument("Debe de especificar una dirección o una palabra válida");
    }
    if(t.text == "CENTER"){
        y = this->screen_center.y;
    }else y = std::stof(t.text);
    t = this->validate_token(TOKEN_NUMBER, "El zoom debe de ser un valor númerico");    
    zoom = std::stof(t.text);
    this->cam_param.zoom = zoom;
    this->cam_param.camera_target = {.x = x, .y = y};
}

Color Renderer::string_to_color(std::string color){
    if(color == "BLACK"){
        return BLACK;
    }
    if(color == "WHITE"){
        return WHITE;
    }
    return (Color){0};
}

void Renderer::fade_screen(bool fade_out){
    Token t = this->validate_token(TOKEN_SYMBOL, "La animación debe de ser seguira por un color");
    Color c = this->string_to_color(t.text);
    t = this->validate_token(TOKEN_NUMBER, "La animación debe ser seguida por su duración");
    float time = std::stof(t.text);
    this->screen_animations.animation_time = time;
    if(fade_out){
        //Fade from image into color
        this->screen_animations.type = FADE_OUT;
        this->screen_animations.origin_color = c;
        this->screen_animations.dest_color = {c.r,c.g,c.b,0 };    
        return;
    }
    this->screen_animations.type = FADE_OUT;
    this->screen_animations.origin_color = {c.r,c.g,c.b,0 };    
    this->screen_animations.dest_color = c;
    
}

bool Renderer::timer_done(){
    return timer.life_time <= 0;
}
void Renderer::update_timer(){
    if(this->timer.life_time > 0)
        this->timer.life_time -= GetFrameTime();
}
void Renderer::start_timer(float life_time){
    timer.life_time = life_time;
}

void Renderer::wait(){
    this->is_paused = true;
    Token t = this->validate_token(TOKEN_NUMBER, "Debe de especificar el tiempo para la pausa");
    float pause_time = std::stof(t.text);
    this->start_timer(pause_time);
}

void Renderer::parse_instruction(){
    if(!this->timer_done()){
        return;
    }else{
        this->is_paused = false;
    }
    Token t = this->lexer.next();    
    if(t.kind == TOKEN_END) return;    
    while(t.kind == TOKEN_COMMENT) t = this->lexer.next();        

    while(t.kind != TOKEN_NAME_CHARA){
        if(t.text.compare("WAIT") == 0){
            this->wait();
            return;
        }else if(t.text.compare("FADE_OUT") == 0){
            this->fade_screen(true);
        }else if(t.text.compare("FADE_IN") == 0){
            this->fade_screen(false);
        }else if(t.text.compare("MOVE_CAMERA") == 0){
            this->move_camera();    
        }else if( t.text.compare("CHARACTER") == 0 ){
            this->add_variable();
        }else if( t.text.compare("ADD_BACKGROUND") == 0 ){
            this->add_background();
        }else if( t.text.compare("ADD_CHARACTER") == 0 ){
            this->add_character();
        }else if( t.text.compare("REMOVE_CHRACTER") == 0 ){
            this->remove_character();
        }
        
        t = this->lexer.next();
        if(t.kind == TOKEN_END) return; 
    }    
    std::string name = t.text;
    
    if(this->variable_map.find(name) == this->variable_map.end()){
        // throw std::invalid_argument("No existe la variable");        
    }else{        
        if(!this->prev_character_name_dialogue.empty()){
            // this->character_on_screen[this->prev_character_name_dialogue].from_color = this->background_chara_color;    
            this->character_on_screen[this->prev_character_name_dialogue].dest_color = this->background_chara_color;    
        }
        this->prev_character_name_dialogue = t.text;
        this->character_on_screen[name].dest_color = WHITE;
        name = this->variable_map[name].name;
    }
    t = this->validate_token(TOKEN_STRING_LITERAL, "El nombre del personaje debe de tener su dialogo");
    std::string dialogue = t.text;
    this->render.full_dialog = dialogue;
    this->render.current_dialog = "";
    this->render.current_name = name;
    this->render.dialog_chara_counter = 0;
}

void Renderer::draw_background(){
    DrawTexturePro(
        this->render.background,
        (Rectangle){.x = 0.0F, .y = 0.0F, .width = (float)this->render.background.width, .height = (float)this->render.background.height},
        (Rectangle){.x = 0.0F, .y = 0.0F, .width = (float)SCREEN_WIDTH, .height = (float)SCREEN_HEIGHT * 2},
        (Vector2){.x = 0.0F, .y = 500},
        0.0F,
        WHITE
    );
}

void Renderer::render_update(){
    BeginDrawing();
        ClearBackground(WHITE);
        if(this->is_paused){
            this->parse_instruction();
        }
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            this->parse_instruction();
        }
        if(this->render.background.id > 0){
            BeginMode2D(this->camera);
            this->draw_background();
            this->animator.MoveCamera(this->cam_param.camera_target, this->cam_param.zoom, 0);
            EndMode2D();
        }
        this->update_timer();
        this->draw_screen();        
    EndDrawing();
}
