/******************************************************************************/
/*                                                                            */
/* Copyright and Disclaimer                                                   */
/*                                                                            */
/* All code listed here is copyright 2005 by David P. Riesz.  You are allowed */
/* to use or redistribute this code under any license you choose.  By using   */
/* this code, you are agreeing to the warranty terms stated below.            */
/*                                                                            */
/* The author makes absolutely no warranty guaranteeing the suitability of    */
/* this code for any purpose whatsoever.  Use it your own risk.               */
/*                                                                            */
/******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <time.h>

#define s2l(x) (*((strrchr(x, '\0'))-1))

void rest(long nsec);
void usage();
void timer(char *arg);
void getsec(char *arg, long *seconds);
double ts2d(struct timespec *ts);
void printtime(double seconds);
void ring();

int main(int argc, char **argv)
{
  if(argc < 2) usage();
  else timer(argv[1]);

  return 0;
}

/* The rest function sleeps the program for the */
/* number of nanoseconds specified by nsec.     */
/* nanosleep() is why we have to use -lrt.      */

void rest(long nsec)
{
  static struct timespec ts;

  ts.tv_sec = 0;
  ts.tv_nsec = nsec;
  nanosleep(&ts, NULL);
}

/* usage() prints a very dumb, almost meaningless usage message. */

void usage()
{
  printf("gimme some time!\n");
}

/* The actual timer code follows. */

void timer(char *arg)
{
  long tsec;
  struct timespec ts, ts_begin;
  double esec;

  /* convert the argument to seconds */
  getsec(arg, &tsec);

  printf("timer:     "); printtime((double)tsec); printf("\n");

  /* get the start time */
  clock_gettime(CLOCK_REALTIME, &ts_begin);
  for(;;)
  {
    /* get the current time */
    clock_gettime(CLOCK_REALTIME, &ts);

    /* compute elapsed time */
    esec = ts2d(&ts) - ts2d(&ts_begin);

    /* break if the elapsed time has reached or */
    /* exceeded the total time                  */
    if(esec >= (double)tsec) break;

    /* print the current time statistics */
    printf("\rremaining: "); printtime((double)tsec - esec);
    printf(" / elapsed: "); printtime(esec);
    printf("  ");
    
    /* flush the buffer since we're not printing newlines */
    fflush(stdout);

    /* pause for a milisecond to ease CPU usage */
    rest(1000000);
  }

  /* print zero time, since the elapsed time sometimes (even */
  /* most times) exceeds the requested total time by several */
  /* nanoseconds.                                            */
  printf("\rremaining: "); printtime(0);
  printf(" / elapsed: "); printtime((double)tsec);

  fflush(stdout);

  /* ring the bell and spit out a newline when we're done */
  ring();
  printf("\n");
}

/* The getsec() function very clumsily gets the command-line argument */
/* containing the requested time.  It is very possible for the user   */
/* to bork this, but hopefully not enough to crash it. */

void getsec(char *arg, long *seconds)
{
  long min=0, sec=0;

  if     (arg[0]   == ':') sscanf(arg, ":%d"        , &sec);
  else if(s2l(arg) == ':') sscanf(arg, "%d:"  , &min      );
  else                     sscanf(arg, "%d:%d", &min, &sec);

  *seconds = (min * 60) + sec;
}

/* ts2d() returns total number of seconds */
/* contained in the timespec structure. */

double ts2d(struct timespec *ts)
{
  return (double)ts->tv_sec + (((double)ts->tv_nsec)/1000000000.0);
}

/* printtime() prints the specified number of */
/* seconds in the format "mm:ss.sss". */

void printtime(double seconds)
{
  long min = (long)(seconds/60.0);
  double sec = seconds - ((double)(min * 60));

  printf("%5d:%06.3f", min, sec);
}

/* ring() is called when the timer finishes.  It can */
/* probably be easily modified to make a variety of  */
/* sounds or invoke other sound-playing programs.    */

void ring()
{
  int i;
  int m=4;
  for(i=0;i<m;i++)
  {
    /* print the BEL character */
    printf("%c", 0x07); fflush(stdout);
    if(i<(m-1))
    {
      /* wait one-half second (500,000,000 nanoseconds) */
      rest(500000000);
    }
  }
}
