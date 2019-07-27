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
        as->scope = init_hermes_scope();
        as->hermes_parser = init_hermes_parser(as->lexer);
        as->ast_tree = hermes_parser_parse(as->hermes_parser, as->scope); 
    }
    else
    {
        as->scope = (void*) 0;
    }

    as->ast_object = setup_ast_object(as);

    as->runtime_reference = init_runtime_reference();
    as->runtime_reference->object->variable_name = "actor";

    hermes_scope_T* scope = init_hermes_scope();
    scope->owner = as->runtime_reference->object; 

    as->runtime_reference->object->scope = (struct hermes_scope_T*) scope;
    runtime_register_reference(HERMES_RUNTIME, as->runtime_reference);

    return as;
}

void actor_scriptable_tick(actor_T* self)
{
    actor_scriptable_T* actor_scriptable = (actor_scriptable_T*) self;

    if (actor_scriptable->scope != (void*) 0)
    {
        dynamic_list_append(
            actor_scriptable->scope->variable_definitions,
            actor_scriptable->ast_variable_this
        );
    }

    if (actor_scriptable->tick_source)
    {
        runtime_visit(HERMES_RUNTIME, actor_scriptable->ast_tree);
    }

    self->x = actor_scriptable->x_var->variable_value->int_value;
    self->y = actor_scriptable->y_var->variable_value->int_value;

    if (actor_scriptable->scope != (void*) 0)
    {
        hermes_scope_clear_variable_definitions(actor_scriptable->scope);
    }
}

void actor_scriptable_draw(actor_T* self)
{
}
