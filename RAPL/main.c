#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include "rapl.h"

#define RUNTIME 1

int main(int argc, char **argv)
{
  char command[500] = "", language[500] = "", test[500] = "", path[500] = "";
  int ntimes = 10;
  int core = 0;
  int i = 0;

#ifdef RUNTIME
  // clock_t begin, end;
  double time_spent;
  struct timespec tvb, tva;
#endif

  FILE *fp;

  // Run command
  //  strcpy(command, ".//" );
  strcat(command, argv[1]);
  // Language name
  strcpy(path, "../");

  strcpy(language, argv[2]);
  strcat(language, ".csv");
  strcat(path, language);
  // Test name
  strcpy(test, argv[3]);

  // ntimes = atoi (argv[2]);

  fp = fopen(path, "a");

  rapl_init(core);

  // fprintf(fp,"Package , CPU , GPU , DRAM? , Time (sec) \n");

  for (i = 0; i < ntimes; i++)
  {

    fprintf(fp, "%s ; ", test);

#ifdef RUNTIME
    // begin = clock();
    clock_gettime(CLOCK_MONOTONIC, &tvb);
#endif

    rapl_before(fp, core);

    system(command);

    rapl_after(fp, core);

#ifdef RUNTIME
    // end = clock();
    // time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    clock_gettime(CLOCK_MONOTONIC, &tva);
    time_spent = (tva.tv_sec - tvb.tv_sec) * 1000.0 + (double)(tva.tv_nsec - tvb.tv_nsec) / 1000000.0;
#endif

#ifdef RUNTIME
    fprintf(fp, " %G \n", time_spent);
#endif
  }

  fclose(fp);
  fflush(stdout);

  return 0;
}
