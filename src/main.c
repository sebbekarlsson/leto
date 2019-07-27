#include <coelum/main.h>
#include <coelum/io.h>
#include <hermes/lexer.h>
#include <hermes/hermes_parser.h>
#include <hermes/hermes_runtime.h>
#include <coelum/scene.h>
#include <coelum/theatre.h>
#include <athena/database.h>
#include "include/scene_utils.h"
#include "include/hermes_extensions.h"
#include <stdio.h>
#include <string.h>


extern theatre_T* THEATRE;
database_T* DATABASE;
runtime_T* HERMES_RUNTIME;


void setup_hermes_env()
{
    HERMES_RUNTIME = init_runtime();
    hermes_register_constants(HERMES_RUNTIME->scope);

    AST_T* fdef_keyboard_press = init_ast(AST_FUNCTION_DEFINITION);
    fdef_keyboard_press->function_name = "keyboard_press";
    fdef_keyboard_press->fptr = keyboard_press;
    fdef_keyboard_press->scope = (struct hermes_scope_T*) HERMES_RUNTIME->scope;
    dynamic_list_append(HERMES_RUNTIME->scope->function_definitions, fdef_keyboard_press); 

    AST_T* fdef_get_intersecting = init_ast(AST_FUNCTION_DEFINITION);
    fdef_get_intersecting->function_name = "get_intersecting";
    fdef_get_intersecting->fptr = get_intersecting;
    fdef_get_intersecting->scope = (struct hermes_scope_T*) HERMES_RUNTIME->scope;
    dynamic_list_append(HERMES_RUNTIME->scope->function_definitions, fdef_get_intersecting);

    AST_T* fdef_actor_instantiate = init_ast(AST_FUNCTION_DEFINITION);
    fdef_actor_instantiate->function_name = "actor_instantiate";
    fdef_actor_instantiate->fptr = actor_instantiate;
    fdef_actor_instantiate->scope = (struct hermes_scope_T*) HERMES_RUNTIME->scope;
    dynamic_list_append(HERMES_RUNTIME->scope->function_definitions, fdef_actor_instantiate);

    AST_T* fdef_scene_goto = init_ast(AST_FUNCTION_DEFINITION);
    fdef_scene_goto->function_name = "scene_goto";
    fdef_scene_goto->fptr = scene_goto;
    fdef_scene_goto->scope = (struct hermes_scope_T*) HERMES_RUNTIME->scope;
    dynamic_list_append(HERMES_RUNTIME->scope->function_definitions, fdef_scene_goto);

    AST_T* fdef_math_cos = init_ast(AST_FUNCTION_DEFINITION);
    fdef_math_cos->function_name = "math_cos";
    fdef_math_cos->fptr = math_cos;
    fdef_math_cos->scope = (struct hermes_scope_T*) HERMES_RUNTIME->scope;
    dynamic_list_append(HERMES_RUNTIME->scope->function_definitions, fdef_math_cos);

    AST_T* fdef_time_now = init_ast(AST_FUNCTION_DEFINITION);
    fdef_time_now->function_name = "time_now";
    fdef_time_now->fptr = time_now;
    fdef_time_now->scope = (struct hermes_scope_T*) HERMES_RUNTIME->scope;
    dynamic_list_append(HERMES_RUNTIME->scope->function_definitions, fdef_time_now);
}

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

    DATABASE = init_database();

    setup_hermes_env();

    load_scenes(DATABASE);

    scene_manager_register_scene(THEATRE->scene_manager, (scene_T*) init_scene_main());

    if (coelum_main(argc, argv))
    {
        printf("Coelum crashed.");
    }

    return 0;
}
