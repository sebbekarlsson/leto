#include "include/actor_scriptable.h"


extern runtime_T* HERMES_RUNTIME;


static AST_T* setup_ast_object(actor_scriptable_T* as)
{
    actor_T* a = (actor_T*) as;
    AST_T* object = init_ast(AST_OBJECT);
    object->variable_name = "this";
    object->object_children = init_dynamic_list(sizeof(struct AST_STRUCT*));
    dynamic_list_T* obj_children = object->object_children;

    // x
    as->x_var = init_ast(AST_VARIABLE_DEFINITION);
    as->x_var->variable_name = "x";
    as->x_var->variable_value = init_ast(AST_INTEGER);
    as->x_var->variable_type = init_ast(AST_TYPE);
    as->x_var->variable_type->type_value = "int";
    as->x_var->variable_value->int_value = a->x;
    dynamic_list_append(obj_children, as->x_var);

    // y
    as->y_var = init_ast(AST_VARIABLE_DEFINITION);
    as->y_var->variable_name = "y";
    as->y_var->variable_value = init_ast(AST_INTEGER);
    as->y_var->variable_type = init_ast(AST_TYPE);
    as->y_var->variable_type->type_value = "int";
    as->y_var->variable_value->int_value = a->y;
    dynamic_list_append(obj_children, as->y_var);

    as->ast_variable_this = init_ast(AST_VARIABLE_DEFINITION);
    as->ast_variable_this->variable_type = init_ast(AST_TYPE);
    as->ast_variable_this->variable_type->type_value = "object";
    as->ast_variable_this->variable_name = object->variable_name;
    as->ast_variable_this->variable_value = object;

    return object;
}

actor_scriptable_T* init_actor_scriptable(float x, float y, float z, char* init_source, char* tick_source, char* draw_source, char* type_name)
{
    actor_scriptable_T* as = calloc(1, sizeof(struct ACTOR_SCRIPTABLE_STRUCT));
    actor_T* a = (actor_T*) as;
    actor_constructor(a, x, y, z, actor_scriptable_tick, actor_scriptable_draw, type_name);

    as->init_source = init_source;
    as->tick_source = tick_source;
    as->draw_source = draw_source;

        if (as->init_source)
        {
            as->init_source_scope = init_hermes_scope();
        }
        else
        {
            as->init_source_scope = (void*) 0;
        }

        if (as->tick_source)
        {
            as->tick_source_scope = init_hermes_scope();
        }
        else
        {
            as->tick_source_scope = (void*) 0;
        }

    if (as->init_source)
    {
        as->init_source_lexer = init_lexer(as->init_source);
        as->init_source_hermes_parser = init_hermes_parser(as->init_source_lexer);
        as->init_source_ast_tree = hermes_parser_parse(as->init_source_hermes_parser, as->init_source_scope); 
    }

    if (as->tick_source)
    {
        as->tick_source_lexer = init_lexer(as->tick_source);
        as->tick_source_hermes_parser = init_hermes_parser(as->tick_source_lexer);
        as->tick_source_ast_tree = hermes_parser_parse(as->tick_source_hermes_parser, as->tick_source_scope); 
    }

    as->ast_object = setup_ast_object(as);

    if (as->init_source)
    {
        dynamic_list_append(
            as->init_source_scope->variable_definitions,
            as->ast_variable_this
        );

        runtime_visit(HERMES_RUNTIME, as->init_source_ast_tree);
    }

    return as;
}

void actor_scriptable_tick(actor_T* self)
{
    actor_scriptable_T* actor_scriptable = (actor_scriptable_T*) self;

    if (actor_scriptable->init_source_scope != (void*) 0)
    {
        for (int i = 0; i < actor_scriptable->init_source_scope->variable_definitions->size; i++)
        {
            AST_T* vardef = actor_scriptable->init_source_scope->variable_definitions->items[i];

            dynamic_list_append(actor_scriptable->tick_source_scope->variable_definitions, vardef);
        }
    }

    if (actor_scriptable->tick_source_scope != (void*) 0)
    {
        dynamic_list_append(
            actor_scriptable->tick_source_scope->variable_definitions,
            actor_scriptable->ast_variable_this
        );
    }

    if (actor_scriptable->tick_source)
    {
        runtime_visit(HERMES_RUNTIME, actor_scriptable->tick_source_ast_tree);
    }

    self->x = actor_scriptable->x_var->variable_value->int_value;
    self->y = actor_scriptable->y_var->variable_value->int_value;

    if (actor_scriptable->tick_source_scope != (void*) 0)
    {
        hermes_scope_clear_variable_definitions(actor_scriptable->tick_source_scope);
    }
}

void actor_scriptable_draw(actor_T* self)
{
}
