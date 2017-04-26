
#include "trace.h"

u_short
in_checknum(u_short *buffer, u_int size)
{
    unsigned long cksum=0;
    while (size>1)
    {
        cksum +=*buffer++;
        size -=sizeof(unsigned short);
    }
    if (size)
    {
        cksum +=*(unsigned char *) buffer;
    }
    while (cksum>>16)
        cksum = (cksum>>16) + (cksum & 0xffff);
    return (unsigned short) (~cksum);
}
