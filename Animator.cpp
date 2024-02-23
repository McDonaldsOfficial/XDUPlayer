#include <iostream>
#include <cmath>
#include "./Headers/Animator.h"
#define SCREEN_WIDTH 1500
#define SCREEN_HEIGHT 900

Animator::Animator(){
    
}

Animator::Animator(Camera2D *camera){
    this->camera = camera;
    this->prev_distance = {0};
    this->prev_zoom = 1;
}
Animator::~Animator(){

}

bool Animator::FadeInScreen(Color color){
    return false;
}
bool Animator::FadeOutScreen(Color color){
    return false;
}

bool Animator::is_the_same_place(Vector2 start, Vector2 end){
    return this->distance_between_two_points(start,end) == 0;
}

float Animator::distance_between_two_points(Vector2 start, Vector2 end){
    return std::sqrt(std::pow( end.x - start.x,2 ) + std::pow(end.y - start.y,2) );
}


void Animator::move_point(Vector2 start, Vector2 end, float zoom, float time){        
    const float distance = this->distance_between_two_points(start, end);
    if( std::round(distance) == 0 ) return;

    const float speed    = (distance / time) * GetFrameTime();
    const float zoom_rate = (zoom - this->camera->zoom) / time * GetFrameTime();

    const float dx = ( end.x - start.x ) / distance;
    const float dy = ( end.y - start.y ) / distance;

    this->camera->zoom += zoom_rate;
    this->camera->target.x += dx * speed;
    this->camera->target.y += dy * speed;
}

float convert_range(float old_value, float old_min, float old_max, float new_min, float new_max){
    return ((old_value - old_min)/(old_max - old_min)) * (new_max - new_min) + new_min;
}


void Animator::MoveCamera(Vector2 pos, float zoom, float seconds){    
    this->move_point(this->camera->target, pos, zoom, seconds/2);        
}