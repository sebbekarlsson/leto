#include "include/scene_utils.h"
#include <coelum/theatre.h>
#include <coelum/actor.h>


extern theatre_T* THEATRE;


actor_T* _init_actor(database_actor_instance_T* database_actor_instance)
{
    actor_T* a = actor_constructor(
        init_actor(),
        database_actor_instance->x,
        database_actor_instance->y,
        database_actor_instance->z,
        (void*) 0, // tick method
        (void*) 0, // draw method
        database_actor_instance->database_actor_definition->name
    );

    a->width = 16;
    a->height = 16;
    a->sprite = database_actor_instance->database_actor_definition->database_sprite->sprite;

    return a; 
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
