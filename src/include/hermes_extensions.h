#ifndef LETO_HERMES_EXTENSIONS_H
#define LETO_HERMES_EXTENSIONS_H
#include <hermes/AST.h>
#include <hermes/dynamic_list.h>
#include <hermes/hermes_scope.h>


void hermes_register_constants(hermes_scope_T* hermes_scope);

AST_T* keyboard_press(AST_T* self, dynamic_list_T* args);

AST_T* keyboard_press_left(AST_T* self, dynamic_list_T* args);

AST_T* keyboard_press_right(AST_T* self, dynamic_list_T* args);

AST_T* keyboard_press_up(AST_T* self, dynamic_list_T* args);

AST_T* keyboard_press_down(AST_T* self, dynamic_list_T* args);

AST_T* get_intersecting(AST_T* self, dynamic_list_T* args);

AST_T* actor_instantiate(AST_T* self, dynamic_list_T* args);

AST_T* scene_goto(AST_T* self, dynamic_list_T* args);

AST_T* math_cos(AST_T* self, dynamic_list_T* args);

AST_T* math_sin(AST_T* self, dynamic_list_T* args);

AST_T* time_now(AST_T* self, dynamic_list_T* args);
#endif
