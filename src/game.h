#ifndef GAME_H
#define GAME_H
#include "patch.h"
struct game_status {
    int pid, supported, enabled, imports, swap;
    struct dz_config config;
    struct dz_state input;
    const char *error;
};
void game_refresh(void);
void game_tick(const struct dz_config *config, int enabled);
void game_read(struct game_status *out);
int game_sync(const struct dz_config *config, int enabled);
/* Legacy sticks: on=1 swaps left/right stick X in the running game. Needs the game filter applied. */
int game_set_swap(int on);
/* Call every ~100 ms: watches Touchpad+L1 and toggles the swap after a 3 s hold. */
void game_poll(void);
#endif
