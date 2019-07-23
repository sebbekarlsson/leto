#include <coelum/main.h>
#include <coelum/io.h>
#include <hermes/lexer.h>
#include <hermes/hermes_parser.h>
#include <hermes/hermes_runtime.h>
#include <coelum/scene.h>
#include <coelum/theatre.h>
//#include "include/scene_impl.h"
#include <stdio.h>
#include <string.h>


extern theatre_T* THEATRE;


scene_T* init_scene_main()
{
    // creating a scene
    scene_T* s = scene_constructor(init_scene(), (void*) 0, (void*) 0, 2);
    s->bg_r = 154;
    s->bg_g = 55;
    s->bg_g = 200;

    return s;
}

int main(int argc, char* argv[])
{ 
    coelum_init();

    scene_manager_register_scene(THEATRE->scene_manager, (scene_T*) init_scene_main());

    if (coelum_main(argc, argv))
    {
        printf("Coelum crashed.");
    }

    return 0;
}
