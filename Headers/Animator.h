#pragma once
#include <raylib.h>
#include <string>


class Animator{
    public:
        Animator();
        Animator(Camera2D *camera);
        ~Animator();        
        void MoveCamera(Vector2 pos, float zoom, float seconds);
        bool FadeInScreen(Color color);
        bool FadeOutScreen(Color color);
    private:
        Camera2D *camera;
        float prev_distance;
        float prev_zoom;
        bool set_new_values = true;

        void move_point(Vector2 start, Vector2 end, float zoom, float time);
        bool DiferenceVector2(Vector2 start, Vector2 end);        
        bool is_the_same_place(Vector2 start, Vector2 end);
        float distance_between_two_points(Vector2 start, Vector2 end);             
};