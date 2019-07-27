#include "include/hermes_extensions.h"
#include "include/actor_scriptable.h"
#include <coelum/input.h>
#include <coelum/current.h>
#include <coelum/actor.h>
#include <coelum/theatre.h>
#include <athena/database.h>
#include <hermes/io.h>
#include <string.h>
#include <time.h>


extern theatre_T* THEATRE;
extern keyboard_state_T* KEYBOARD_STATE;
extern database_T* DATABASE;


int INTEGER_CONSTANTS_VALUES[] = {
    GLFW_KEY_LEFT,
    GLFW_KEY_RIGHT,
    GLFW_KEY_UP,
    GLFW_KEY_DOWN,
    GLFW_KEY_ENTER,
    GLFW_KEY_SPACE
};

char* INTEGER_CONSTANTS_NAMES[] = {
    "KEY_LEFT",
    "KEY_RIGHT",
    "KEY_UP",
    "KEY_DOWN",
    "KEY_ENTER",
    "KEY_SPACE"
};

void hermes_register_constants(hermes_scope_T* hermes_scope)
{
    for (int i = 0; i < 6; i++)
    {
        AST_T* ast = init_ast(AST_VARIABLE_DEFINITION);
        ast->variable_name = INTEGER_CONSTANTS_NAMES[i];
        ast->variable_type = init_ast(AST_TYPE);
        ast->variable_type->type_value = "int";
        ast->variable_value = init_ast(AST_INTEGER);
        ast->variable_value->int_value = INTEGER_CONSTANTS_VALUES[i];

        dynamic_list_append(hermes_scope->variable_definitions, ast);
    } 
}

static AST_T* expect_arg(const char* function_name, dynamic_list_T* args, unsigned int index, int type)
{
    if (args->size < index)
    {
        printf("%s:\n", function_name);
        printf("Not enough arguments\n");
        exit(1);
    }

    AST_T* ast = args->items[index];

    if (ast->type != type)
    {
        printf("%s:\n", function_name);
        printf("Wrong type for argument %d\n", index);
    }

    return ast;
}

AST_T* keyboard_press(dynamic_list_T* args)
{
    AST_T* ast_int = (AST_T*) args->items[0];

    AST_T* ast = init_ast(AST_BOOLEAN);
    ast->boolean_value = KEYBOARD_STATE->keys[ast_int->int_value];

    return ast;
}

AST_T* get_intersecting(dynamic_list_T* args)
{
    AST_T* ast_object = (AST_T*) args->items[0];
    AST_T* ast_string = (AST_T*) args->items[1];
    char* type_name = ast_string->string_value;

    scene_T* scene = get_current_scene();
    state_T* state = (state_T*) scene;

    int this_x = 0;
    int this_y = 0;

    hermes_scope_T* runtime_scope = (hermes_scope_T*) ast_object->scope;

    for (int j = 0; j < ast_object->object_children->size; j++)
    {
        AST_T* ast_obj_var = (AST_T*) ast_object->object_children->items[j];
        
        if (strcmp(ast_obj_var->variable_name, "x") == 0)
            this_x = ast_obj_var->variable_value->int_value;

        if (strcmp(ast_obj_var->variable_name, "y") == 0)
            this_y = ast_obj_var->variable_value->int_value;
    }

    for (int i = 0; i < state->actors->size; i++)
    {
        actor_T* actor = (actor_T*) state->actors->items[i];

        if (strcmp(actor->type_name, type_name) == 0)
        {
            if (this_x + 16 > actor->x && this_x < actor->x + 16)
            {
                if (this_y + 16 > actor->y && this_y < actor->y + 16)
                {
                    actor_scriptable_T* actor_scriptable = (actor_scriptable_T*) actor;

                    return actor_scriptable->ast_object;
                }
            }
        }
    }

    return init_ast(AST_NULL);
}

AST_T* actor_instantiate(dynamic_list_T* args)
{
    const char* fname = "actor_instantiate";

    AST_T* ast_string_name = expect_arg(fname, args, 0, AST_STRING);
    AST_T* ast_int_x = expect_arg(fname, args, 1, AST_INTEGER);
    AST_T* ast_int_y = expect_arg(fname, args, 2, AST_INTEGER);

    scene_T* scene = get_current_scene();
    state_T* state = (state_T*) scene;

    database_actor_definition_T* database_actor_definition = database_get_actor_definition_by_name(
        DATABASE,
        (const char*) ast_string_name->string_value
    );

    char* init_source = (void*) 0;
    char* tick_source = (void*) 0;
    char* draw_source = (void*) 0;

    if (database_actor_definition->init_script)
        init_source = read_file(database_actor_definition->init_script);
    
    if (database_actor_definition->tick_script)
        tick_source = read_file(database_actor_definition->tick_script);
    
    if (database_actor_definition->draw_script)
        draw_source = read_file(database_actor_definition->draw_script);

    actor_scriptable_T* actor_scriptable = init_actor_scriptable(
        (float) ast_int_x->int_value,
        (float) ast_int_y->int_value,
        0.0f,
        init_source,
        tick_source,
        draw_source,
        database_actor_definition->name
    );

    actor_T* a = (actor_T*) actor_scriptable;

    a->width = 16;
    a->height = 16;
    a->sprite = database_actor_definition->database_sprite->sprite;

    dynamic_list_append(state->actors, actor_scriptable);

    return actor_scriptable->ast_object;
}

AST_T* scene_goto(dynamic_list_T* args)
{
    const char* fname = "scene_goto";

    AST_T* ast_string_name = expect_arg(fname, args, 0, AST_STRING);

    scene_manager_goto(THEATRE->scene_manager, ast_string_name->string_value);
    
    return init_ast(AST_NULL);
}

AST_T* math_cos(dynamic_list_T* args)
{
    const char* fname = "math_cos";

    AST_T* ast_float = expect_arg(fname, args, 0, AST_FLOAT);

    AST_T* ast_result = init_ast(AST_FLOAT);
    ast_result->float_value = cos(ast_float->float_value);

    return ast_result;
}

AST_T* math_sin(dynamic_list_T* args)
{
    const char* fname = "math_sin";

    AST_T* ast_float = expect_arg(fname, args, 0, AST_FLOAT);

    AST_T* ast_result = init_ast(AST_FLOAT);
    ast_result->float_value = sin(ast_float->float_value);

    return ast_result;
}

AST_T* time_now(dynamic_list_T* args)
{
    AST_T* ast_result = init_ast(AST_INTEGER);
    ast_result->int_value = (int) time(NULL);

    return ast_result;
}
