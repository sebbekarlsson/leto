#include "include/scene_utils.h"
#include "include/actor_scriptable.h"
#include <coelum/theatre.h>
#include <coelum/actor.h>
#include <hermes/io.h>


extern theatre_T* THEATRE;
extern database_T* DATABASE;


actor_scriptable_T* _init_actor(database_actor_instance_T* database_actor_instance)
{
    char* init_source = (void*) 0;
    char* tick_source = (void*) 0;
    char* draw_source = (void*) 0;

    if (database_actor_instance->database_actor_definition->init_script_id)
    {
        database_script_T* script = database_get_script_by_id(DATABASE, database_actor_instance->database_actor_definition->init_script_id);

        if (script)
            init_source = script->contents;
    }

    if (database_actor_instance->database_actor_definition->tick_script_id)
    {
        database_script_T* script = database_get_script_by_id(DATABASE, database_actor_instance->database_actor_definition->tick_script_id);

        if (script)
            tick_source = script->contents;
    }
    
    if (database_actor_instance->database_actor_definition->draw_script_id)
    {
        database_script_T* script = database_get_script_by_id(DATABASE, database_actor_instance->database_actor_definition->draw_script_id);

        if (script)
            draw_source = script->contents;
    }

    actor_scriptable_T* actor_scriptable = init_actor_scriptable(
        database_actor_instance->x,
        database_actor_instance->y,
        database_actor_instance->z,
        init_source,
        tick_source,
        draw_source,
        database_actor_instance->database_actor_definition->name
    );

    actor_T* a = (actor_T*) actor_scriptable;

    a->width = 16;
    a->height = 16;

    if (database_actor_instance->database_actor_definition->database_sprite != (void*) 0)
    {
        if (database_actor_instance->database_actor_definition->database_sprite->sprite != (void*) 0)
        {
            a->sprite = database_actor_instance->database_actor_definition->database_sprite->sprite;
        }
    }

    return actor_scriptable; 
}

scene_T* _init_scene(database_T* database, database_scene_T* database_scene)
{
    // creating a scene
    scene_T* s = scene_constructor(init_scene(), (void*) 0, (void*) 0, 2);
    state_T* state = (state_T*) s;
    s->type_name = database_scene->name;
    s->bg_r = 255;
    s->bg_g = 255;
    s->bg_g = 255;

    dynamic_list_T* database_actor_instances = database_get_all_actor_instances_by_scene_id(database, database_scene->id);
    
    for (int i = 0; i < database_actor_instances->size; i++)
    {
        database_actor_instance_T* database_actor_instance = (database_actor_instance_T*) database_actor_instances->items[i];
        
        dynamic_list_append(state->actors, _init_actor(database_actor_instance));
    }

    // TODO: free database_actor_instances

    return s;
}

void load_scenes(database_T* database)
{
    dynamic_list_T* database_scenes = database_get_all_scenes(database);

    for (int i = 0; i < database_scenes->size; i++)
    {
        database_scene_T* database_scene = (database_scene_T*) database_scenes->items[i];

        scene_T* scene = _init_scene(database, database_scene);

        scene_manager_register_scene(THEATRE->scene_manager, scene);
    }

    // TODO: free database_scenes
}
