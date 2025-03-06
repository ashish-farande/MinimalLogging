#ifndef BDB39F60_A5E7_4D2A_A2E0_B57303FB1BE6
#define BDB39F60_A5E7_4D2A_A2E0_B57303FB1BE6

#include <stdio.h>

enum class Level
{
    kError,
    kWarn,
    kInfo,
    kDebug,
};

int64_t gen_id()
{
    static int64_t id{-1};
    ++id;
    return id;
}

#endif /* BDB39F60_A5E7_4D2A_A2E0_B57303FB1BE6 */
