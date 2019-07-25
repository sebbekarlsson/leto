#ifndef LETO_ACTOR_SCRIPTABLE_H
#define LETO_ACTOR_SCRIPTABLE_H
#include <coelum/actor.h>
#include <hermes/lexer.h>
#include <hermes/hermes_parser.h>
#include <hermes/hermes_runtime.h>


typedef struct ACTOR_SCRIPTABLE_STRUCT
{
    actor_T base;
    char* tick_source;
    char* draw_source;

    lexer_T* lexer;
    hermes_parser_T* hermes_parser;
    runtime_reference_T* runtime_reference;

    AST_T* ast_tree;
    AST_T* x_var;
    AST_T* y_var;
} actor_scriptable_T;

actor_scriptable_T* init_actor_scriptable(float x, float y, float z, char* tick_source, char* draw_source, char* type_name);

void actor_scriptable_tick(actor_T* self);

void actor_scriptable_draw(actor_T* self);
#endif
