#ifndef _MISC_H
#define _MISC_H

#define _dbg() printf("%d %s\n", __LINE__, __FILE__)
#define print_log()    printf("[%-10lu %-20s %3d]\n", micros(), __FILE__, __LINE__)

#endif
