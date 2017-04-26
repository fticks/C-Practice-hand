
#include "trace.h"

void
sig_alrm(int arg)
{
    status.gotalarm = 1;
    //alarm(3);
}