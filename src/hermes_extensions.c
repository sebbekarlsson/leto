#include "include/hermes_extensions.h"
#include "include/actor_scriptable.h"
#include <coelum/input.h>
#include <coelum/current.h>
#include <coelum/actor.h>
#include <string.h>


extern keyboard_state_T* KEYBOARD_STATE;


int INTEGER_CONSTANTS_VALUES[] = {
    GLFW_KEY_LEFT,
    GLFW_KEY_RIGHT,
    GLFW_KEY_UP,
    GLFW_KEY_DOWN
};

char* INTEGER_CONSTANTS_NAMES[] = {
    "KEY_LEFT",
    "KEY_RIGHT",
    "KEY_UP",
    "KEY_DOWN"
};

void hermes_register_constants(hermes_scope_T* hermes_scope)
{
    for (int i = 0; i < 4; i++)
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
            if (this_x + 16 >= actor->x && this_x <= actor->x + 16)
            {
                if (this_y + 16 >= actor->y && this_y <= actor->y + 16)
                {
                    actor_scriptable_T* actor_scriptable = (actor_scriptable_T*) actor;

                    return actor_scriptable->runtime_reference->object;
                }
            }
        }
    }

    return init_ast(AST_NULL);
}
