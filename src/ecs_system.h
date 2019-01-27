#ifndef ECS_SYSTEM_H
#define ECS_SYSTEM_H

/* Stuff needed for the systems to work */
struct S_TurnSys {
};

/* Extern Functions */
void s_camera_move();
void s_movement();
void s_render();
void lock_s_tick();
void unlock_s_tick();
void s_tick();

#endif
