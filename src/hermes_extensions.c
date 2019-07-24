#include "include/hermes_extensions.h"
#include <coelum/input.h>


extern keyboard_state_T* KEYBOARD_STATE;


AST_T* keyboard_press_left(dynamic_list_T* args)
{
    AST_T* ast = init_ast(AST_BOOLEAN);
    ast->boolean_value = KEYBOARD_STATE->keys[GLFW_KEY_LEFT];

    return ast;
}

AST_T* keyboard_press_right(dynamic_list_T* args)
{
    AST_T* ast = init_ast(AST_BOOLEAN);
    ast->boolean_value = KEYBOARD_STATE->keys[GLFW_KEY_RIGHT];

    return ast;
}

AST_T* keyboard_press_up(dynamic_list_T* args)
{
    AST_T* ast = init_ast(AST_BOOLEAN);
    ast->boolean_value = KEYBOARD_STATE->keys[GLFW_KEY_UP];

    return ast;
}

AST_T* keyboard_press_down(dynamic_list_T* args)
{
    AST_T* ast = init_ast(AST_BOOLEAN);
    ast->boolean_value = KEYBOARD_STATE->keys[GLFW_KEY_DOWN];

    return ast;
}
