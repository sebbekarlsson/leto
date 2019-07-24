#include "include/hermes_extensions.h"
#include <coelum/input.h>


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
