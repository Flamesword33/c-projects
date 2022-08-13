/*  etest -- enter object names on queue
 *
 *  Part of message queue facility example from
 *  Haviland, Gray, and Salama, "UNIX System Programming," 2e
 *  (Addison Wesley, 1998), pp. 195-199.
 */

#include "msgq.h"

extern int enter(char *objname, int priority);


int main(int argc, char **argv)
{
  int priority;
  
  if (argc != 3 )
  {
    fprintf(stderr, "usage: %s objname priority\n", argv[0]);
    exit(1);
  }
  
  if ( (priority =  atoi(argv[2])) <= 0 || priority > MAXPRIOR)
  {
    warn("invalid priority");
    exit(2);
  }
  
  if (enter(argv[1], priority) < 0)
  {
    warn("failed to place object on queue");
    exit(3);
  }
  
  exit(0);
}
