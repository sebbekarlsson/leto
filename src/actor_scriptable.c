#include "include/actor_scriptable.h"
#include <coelum/physics.h>


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
    as->x_var->variable_value = init_ast(AST_FLOAT);
    as->x_var->variable_type = init_ast(AST_TYPE);
    as->x_var->variable_type->type_value = "float";
    as->x_var->variable_value->float_value = a->x;
    dynamic_list_append(obj_children, as->x_var);

    // y
    as->y_var = init_ast(AST_VARIABLE_DEFINITION);
    as->y_var->variable_name = "y";
    as->y_var->variable_value = init_ast(AST_FLOAT);
    as->y_var->variable_type = init_ast(AST_TYPE);
    as->y_var->variable_type->type_value = "float";
    as->y_var->variable_value->float_value = a->y;
    dynamic_list_append(obj_children, as->y_var);

    // dx
    as->dx_var = init_ast(AST_VARIABLE_DEFINITION);
    as->dx_var->variable_name = "dx";
    as->dx_var->variable_value = init_ast(AST_FLOAT);
    as->dx_var->variable_type = init_ast(AST_TYPE);
    as->dx_var->variable_type->type_value = "float";
    as->dx_var->variable_value->float_value = a->dx;
    dynamic_list_append(obj_children, as->dx_var);

    // dy
    as->dy_var = init_ast(AST_VARIABLE_DEFINITION);
    as->dy_var->variable_name = "dy";
    as->dy_var->variable_value = init_ast(AST_FLOAT);
    as->dy_var->variable_type = init_ast(AST_TYPE);
    as->dy_var->variable_type->type_value = "float";
    as->dy_var->variable_value->float_value = a->dy;
    dynamic_list_append(obj_children, as->dy_var);

    // friction
    as->friction_var = init_ast(AST_VARIABLE_DEFINITION);
    as->friction_var->variable_name = "friction";
    as->friction_var->variable_value = init_ast(AST_FLOAT);
    as->friction_var->variable_type = init_ast(AST_TYPE);
    as->friction_var->variable_type->type_value = "float";
    as->friction_var->variable_value->float_value = a->friction;
    dynamic_list_append(obj_children, as->friction_var);

    // rx
    as->rx_var = init_ast(AST_VARIABLE_DEFINITION);
    as->rx_var->variable_name = "rx";
    as->rx_var->variable_value = init_ast(AST_FLOAT);
    as->rx_var->variable_type = init_ast(AST_TYPE);
    as->rx_var->variable_type->type_value = "float";
    as->rx_var->variable_value->float_value = a->rx;
    dynamic_list_append(obj_children, as->rx_var);

    // ry
    as->ry_var = init_ast(AST_VARIABLE_DEFINITION);
    as->ry_var->variable_name = "ry";
    as->ry_var->variable_value = init_ast(AST_FLOAT);
    as->ry_var->variable_type = init_ast(AST_TYPE);
    as->ry_var->variable_type->type_value = "float";
    as->ry_var->variable_value->float_value = a->ry;
    dynamic_list_append(obj_children, as->ry_var);

    // rz
    as->rz_var = init_ast(AST_VARIABLE_DEFINITION);
    as->rz_var->variable_name = "rz";
    as->rz_var->variable_value = init_ast(AST_FLOAT);
    as->rz_var->variable_type = init_ast(AST_TYPE);
    as->rz_var->variable_type->type_value = "float";
    as->rz_var->variable_value->float_value = a->rz;
    dynamic_list_append(obj_children, as->rz_var);

    as->ast_variable_this = init_ast(AST_VARIABLE_DEFINITION);
    as->ast_variable_this->variable_type = init_ast(AST_TYPE);
    as->ast_variable_this->variable_type->type_value = "object";
    as->ast_variable_this->variable_name = object->variable_name;
    as->ast_variable_this->variable_value = object;

    printf("Created actor object\n");

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
    a->friction = 0.1f;

    if (as->init_source)
    {
        as->init_source_scope = init_hermes_scope(0);
    }
    else
    {
        as->init_source_scope = (void*) 0;
    }

    if (as->tick_source)
    {
        as->tick_source_scope = init_hermes_scope(0);
    }
    else
    {
        as->tick_source_scope = (void*) 0;
    }

    if (as->init_source)
    {
        as->init_source_hermes_lexer = init_hermes_lexer(as->init_source);
        as->init_source_hermes_parser = init_hermes_parser(as->init_source_hermes_lexer);
        as->init_source_ast_tree = hermes_parser_parse(as->init_source_hermes_parser, as->init_source_scope); 
    }

    if (as->tick_source)
    {
        as->tick_source_hermes_lexer = init_hermes_lexer(as->tick_source);
        as->tick_source_hermes_parser = init_hermes_parser(as->tick_source_hermes_lexer);
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

    printf("Done init actor\n");

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

    self->x = actor_scriptable->x_var->variable_value->float_value;
    self->y = actor_scriptable->y_var->variable_value->float_value;
    self->x = actor_scriptable->x_var->variable_value->float_value;
    
    self->rx = actor_scriptable->rx_var->variable_value->float_value;
    self->ry = actor_scriptable->ry_var->variable_value->float_value;
    self->rz = actor_scriptable->rz_var->variable_value->float_value;

    self->dx = actor_scriptable->dx_var->variable_value->float_value;
    self->dy = actor_scriptable->dy_var->variable_value->float_value;
    self->friction = actor_scriptable->friction_var->variable_value->float_value;

    physics_tick_actor(self);

    actor_scriptable->dx_var->variable_value->float_value = self->dx;
    actor_scriptable->dy_var->variable_value->float_value = self->dy;
    actor_scriptable->dx_var->variable_value->int_value = self->dx;
    actor_scriptable->dy_var->variable_value->int_value = self->dy;

    actor_scriptable->x_var->variable_value->float_value = self->x;
    actor_scriptable->y_var->variable_value->float_value = self->y;
    actor_scriptable->x_var->variable_value->int_value = self->x;
    actor_scriptable->y_var->variable_value->int_value = self->y;

    actor_scriptable->rx_var->variable_value->float_value = self->rx;
    actor_scriptable->ry_var->variable_value->float_value = self->ry;
    actor_scriptable->rz_var->variable_value->float_value = self->rz;
    actor_scriptable->rx_var->variable_value->int_value = self->rx;
    actor_scriptable->ry_var->variable_value->int_value = self->ry;
    actor_scriptable->rz_var->variable_value->int_value = self->rz;

    if (actor_scriptable->tick_source_scope != (void*) 0)
    {
        hermes_scope_clear_variable_definitions(actor_scriptable->tick_source_scope);
        hermes_scope_clear_function_definitions(actor_scriptable->tick_source_scope);
    }
}

void actor_scriptable_draw(actor_T* self)
{
}
