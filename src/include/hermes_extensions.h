#ifndef LETO_HERMES_EXTENSIONS_H
#define LETO_HERMES_EXTENSIONS_H
#include <hermes/AST.h>
#include <hermes/dynamic_list.h>
#include <hermes/hermes_scope.h>


void hermes_register_constants(hermes_scope_T* hermes_scope);

AST_T* keyboard_press(dynamic_list_T* args);

AST_T* keyboard_press_left(dynamic_list_T* args);

AST_T* keyboard_press_right(dynamic_list_T* args);

AST_T* keyboard_press_up(dynamic_list_T* args);

AST_T* keyboard_press_down(dynamic_list_T* args);

AST_T* get_intersecting(dynamic_list_T* args);

AST_T* actor_instantiate(dynamic_list_T* args);

AST_T* scene_goto(dynamic_list_T* args);

AST_T* math_cos(dynamic_list_T* args);

AST_T* math_sin(dynamic_list_T* args);

AST_T* time_now(dynamic_list_T* args);
#endif
