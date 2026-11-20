#ifndef DZ_CHORD_H
#define DZ_CHORD_H
/* Hold-to-toggle detector for the legacy-sticks switch.
 * Fires once when Touchpad + L1 have been held together for DZ_CHORD_HOLD_MS.
 * Release (or a missing/disconnected sample) re-arms it.
 * Button bits: L1 = 0x400, Touchpad = 0x100000 (verify against the SDK pad header). */
#include <stdint.h>
#define DZ_CHORD_MASK    (0x00000400u | 0x00100000u)
#define DZ_CHORD_HOLD_MS 3000
struct dz_chord { int64_t since; int fired; };
static inline void dz_chord_reset(struct dz_chord *c) { c->since = -1; c->fired = 0; }
/* Returns 1 on the poll where the hold reaches the threshold, else 0. */
static inline int dz_chord_update(struct dz_chord *c, uint32_t buttons, int connected, int64_t now) {
    if (!connected || now < 0 || (buttons & DZ_CHORD_MASK) != DZ_CHORD_MASK) { dz_chord_reset(c); return 0; }
    if (c->since < 0 || now < c->since) { c->since = now; c->fired = 0; return 0; }
    if (!c->fired && now - c->since >= DZ_CHORD_HOLD_MS) { c->fired = 1; return 1; }
    return 0;
}
#endif
