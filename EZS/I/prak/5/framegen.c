#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAXTASKS 100
#define MAXLINE   80

struct periodic_task {
  double t_phi;  /* phase of task */
  unsigned long t_p;    /* period */
  double t_e;    /* execution time */
  double t_d;    /* deadline */
};

struct periodic_task tasks[MAXTASKS];

unsigned int read_taskset(void)
{
  char linebuf[MAXLINE], *aktpos;
  long index;
  unsigned int numTasks = 0;
  
  while(!feof(stdin)) {
    if (!fgets(linebuf, MAXLINE-1, stdin)) {
      continue;
    }
    aktpos = strchr(linebuf, 'T');
    if (aktpos==NULL) {
      continue;        /* skip lines without 'T' */
    }
    aktpos = strtok(aktpos+1, "(");        /* find start of index */
    index = strtol(aktpos, NULL, 10) - 1;  
    aktpos = strtok(NULL, ",");        /* find t_phi */
    tasks[index].t_phi = strtod(aktpos, NULL);
    aktpos = strtok(NULL, ",");        /* find t_p ... */
    tasks[index].t_p = strtoul(aktpos, NULL, 10);
    aktpos = strtok(NULL, ","); /* find t_e ... */
    tasks[index].t_e = strtod(aktpos, NULL);
    aktpos = strtok(NULL, ","); /* find t_d ... */
    tasks[index].t_d = strtod(aktpos, NULL);
    numTasks++;
  }
  return numTasks;
}

unsigned long gcd_euclid(unsigned long a, unsigned long b)
{
    if (b==0) {
        return a;
    }
    else {
        return gcd_euclid(b, (a % b));
    }
}

unsigned long kgv(unsigned long a, unsigned long b) {
    return (a * b) / gcd_euclid(a, b);
}

unsigned long hyperperiod(unsigned long numTasks) {
    unsigned long hp = tasks[0].t_p;
    for (int i = 1; i < numTasks; i++) {
        hp = kgv(hp, tasks[i].t_p);
    }
    return hp;
}

double maxTe(unsigned int numTasks) {
    double max_te = 0.0;
    for (int i = 0; i < numTasks; i++) {
        if (tasks[i].t_e > max_te) {
            max_te = tasks[i].t_e;
        }
    }
    return max_te;
}

// Ensure whole frame fits between t_phi and t_d
unsigned long maxFramesize(unsigned int numTasks) {
    double maxFs = 0.0;
    for (int i = 0; i < numTasks; i++) {
        double fs = tasks[i].t_d - tasks[i].t_phi;
        if (fs < 0) fs = -fs;
        if (fs > maxFs) {
            maxFs = fs;
        }
    }
    unsigned long fs = (unsigned long) maxFs;
    if ((double)fs < maxFs) fs++;
    return fs;
}

void framesize(unsigned int numTasks) {
    unsigned long hp = hyperperiod(numTasks);
    unsigned long minFs = maxTe(numTasks);
    unsigned long maxFs = maxFramesize(numTasks);

    printf("Hyperperiod: %lu\n", hp);
    printf("Min FS: %lu\n", minFs);
    printf("Max FS: %lu\n", maxFs);

    int found = 0;
    for (unsigned long fs = minFs; fs <= maxFs; fs++) {
        if (((double)hp / (double)fs) == (double)(int)(hp / fs)) {
            printf("Found FS: %lu\n", fs);
            found = 1;
        }
    }
    if (!found) {
        printf("No framesize found\n");
    }
}

int main(int argc, char *argv[])
{
    unsigned int numTasks = read_taskset();
    printf("Number of tasks: %d\n", numTasks);
    printf("Tasks:\n");
    for (int i = 0; i < numTasks; i++) {
        printf("Task %d: t_phi = %f, t_p = %lu, t_e = %f, t_d = %f\n", i, tasks[i].t_phi, tasks[i].t_p, tasks[i].t_e, tasks[i].t_d);
    }

    framesize(numTasks);

    return 0;
}
