#include <stdio.h>
#include <stdlib.h>
#include "chord.h"
#define CHECK(c) do{ if(!(c)){ printf("FAIL line %d: %s\n",__LINE__,#c); exit(1);} }while(0)
int main(void){
    struct dz_chord c; dz_chord_reset(&c);
    uint32_t both = DZ_CHORD_MASK, l1 = 0x400, tp = 0x100000;
    /* held 3 s at 100 ms polls: fires exactly once, at 3000 ms */
    int fires = 0; int64_t at = -1;
    for (int64_t t = 1000; t <= 6000; t += 100) if (dz_chord_update(&c, both, 1, t)) { fires++; at = t; }
    CHECK(fires == 1); CHECK(at == 4000);
    /* still held: no repeat. release re-arms, second hold fires again */
    CHECK(!dz_chord_update(&c, both, 1, 7000));
    CHECK(!dz_chord_update(&c, 0, 1, 7100));
    fires = 0;
    for (int64_t t = 8000; t <= 11100; t += 100) fires += dz_chord_update(&c, both, 1, t);
    CHECK(fires == 1);
    /* only one button of the two: never fires */
    dz_chord_reset(&c);
    for (int64_t t = 0; t <= 10000; t += 100) { CHECK(!dz_chord_update(&c, l1, 1, t)); CHECK(!dz_chord_update(&c, tp, 1, t)); }
    /* releasing at 2.9 s then re-pressing restarts the clock */
    dz_chord_reset(&c);
    for (int64_t t = 0; t < 2900; t += 100) CHECK(!dz_chord_update(&c, both, 1, t));
    CHECK(!dz_chord_update(&c, l1, 1, 2900));
    for (int64_t t = 3000; t < 5900; t += 100) CHECK(!dz_chord_update(&c, both, 1, t));
    CHECK(dz_chord_update(&c, both, 1, 6000));
    /* disconnected sample or clock error resets */
    dz_chord_reset(&c);
    dz_chord_update(&c, both, 1, 0);
    CHECK(!dz_chord_update(&c, both, 0, 3500));
    CHECK(!dz_chord_update(&c, both, 1, 3600));
    CHECK(!dz_chord_update(&c, both, 1, 3700));
    /* clock going backwards restarts rather than firing */
    dz_chord_reset(&c);
    dz_chord_update(&c, both, 1, 5000);
    CHECK(!dz_chord_update(&c, both, 1, 100));
    puts("chord tests passed");
    return 0;
}
