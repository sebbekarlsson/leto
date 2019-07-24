#ifndef LETO_HERMES_EXTENSIONS_H
#define LETO_HERMES_EXTENSIONS_H
#include <hermes/AST.h>
#include <hermes/dynamic_list.h>


AST_T* keyboard_press_left(dynamic_list_T* args);

AST_T* keyboard_press_right(dynamic_list_T* args);

AST_T* keyboard_press_up(dynamic_list_T* args);

AST_T* keyboard_press_down(dynamic_list_T* args);
#endif
