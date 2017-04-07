// Enable debug messages
#define DEBUG

// Enable CSV output
//#define CSV

#ifdef CSV
#define SILENT
#undef DEBUG
#endif

// Enable parallelism
//#define PARALLEL
