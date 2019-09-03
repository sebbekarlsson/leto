#ifndef LETO_ACTOR_SCRIPTABLE_H
#define LETO_ACTOR_SCRIPTABLE_H
#include <coelum/actor.h>
#include <hermes/hermes_lexer.h>
#include <hermes/hermes_parser.h>
#include <hermes/hermes_runtime.h>


typedef struct ACTOR_SCRIPTABLE_STRUCT
{
    actor_T base;
    char* init_source;
    char* tick_source;
    char* draw_source;

    hermes_scope_T* init_source_scope;
    hermes_scope_T* tick_source_scope;

    hermes_lexer_T* init_source_hermes_lexer;
    hermes_parser_T* init_source_hermes_parser;

    hermes_lexer_T* tick_source_hermes_lexer;
    hermes_parser_T* tick_source_hermes_parser;

    AST_T* init_source_ast_tree;
    AST_T* tick_source_ast_tree;
    AST_T* x_var;
    AST_T* y_var;
    AST_T* dx_var;
    AST_T* dy_var;
    AST_T* friction_var;

    AST_T* ast_object;
    AST_T* ast_variable_this;
} actor_scriptable_T;

actor_scriptable_T* init_actor_scriptable(float x, float y, float z, char* init_source, char* tick_source, char* draw_source, char* type_name);

void actor_scriptable_tick(actor_T* self);

void actor_scriptable_draw(actor_T* self);
#endif
