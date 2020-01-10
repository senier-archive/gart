#ifndef abort
#define abort() do { fprintf(stderr, "PROGRAM ABORTED:\n   %s\n%s +%d\n", __func__, __FILE__, __LINE__); exit (1); } while (0)
#endif
