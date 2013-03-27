#include <stdio.h>
#include <stdlib.h>
#include "race.h"

const uint32_t horse_count = 5;
const uint32_t final_rank[] = {
    5, 3, 2, 1, 5
};

int main(int argc, char *argv[])
{
    const uint32_t horse_count = 6;
    const uint32_t segment = 5;

    StateGrid state(horse_count, final_rank);

    for(int i=0; i<segment; i++){
        state.Step();
        state.Dump();
    }

    return 0;
}

