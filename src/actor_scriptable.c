#include "include/actor_scriptable.h"


extern runtime_T* HERMES_RUNTIME;

actor_scriptable_T* init_actor_scriptable(float x, float y, float z, char* tick_source, char* draw_source, char* type_name)
{
    actor_scriptable_T* as = calloc(1, sizeof(struct ACTOR_SCRIPTABLE_STRUCT));
    actor_T* a = (actor_T*) as;
    actor_constructor(a, x, y, z, actor_scriptable_tick, actor_scriptable_draw, type_name);

    as->tick_source = tick_source;
    as->draw_source = draw_source;

    if (as->tick_source)
    {
        as->lexer = init_lexer(as->tick_source);
        as->hermes_parser = init_hermes_parser(as->lexer);
        as->ast_tree = hermes_parser_parse(as->hermes_parser, (void*) 0); 
    }

    as->runtime_reference = init_runtime_reference();
    as->runtime_reference->object->variable_name = "actor";

    hermes_scope_T* scope = init_hermes_scope();
    scope->owner = as->runtime_reference->object;

    // x
    as->x_var = init_ast(AST_VARIABLE_DEFINITION);
    as->x_var->variable_name = "x";
    as->x_var->variable_value = init_ast(AST_INTEGER);
    as->x_var->variable_type = init_ast(AST_TYPE);
    as->x_var->variable_type->type_value = "int";
    as->x_var->variable_value->int_value = a->x;
    dynamic_list_append(scope->variable_definitions, as->x_var);
    dynamic_list_append(as->runtime_reference->object->object_children, as->x_var);

    // y
    as->y_var = init_ast(AST_VARIABLE_DEFINITION);
    as->y_var->variable_name = "y";
    as->y_var->variable_value = init_ast(AST_INTEGER);
    as->y_var->variable_type = init_ast(AST_TYPE);
    as->y_var->variable_type->type_value = "int";
    as->y_var->variable_value->int_value = a->y;
    dynamic_list_append(scope->variable_definitions, as->y_var);
    dynamic_list_append(as->runtime_reference->object->object_children, as->y_var); 

    as->runtime_reference->object->scope = (struct hermes_scope_T*) scope;
    runtime_register_reference(HERMES_RUNTIME, as->runtime_reference);

    return as;
}

void actor_scriptable_tick(actor_T* self)
{
    actor_scriptable_T* actor_scriptable = (actor_scriptable_T*) self; 

   if (actor_scriptable->tick_source)
   {
       runtime_visit(HERMES_RUNTIME, actor_scriptable->ast_tree);

       self->x = actor_scriptable->x_var->variable_value->int_value;
       self->y = actor_scriptable->y_var->variable_value->int_value;
   }
}

void actor_scriptable_draw(actor_T* self)
{
}
