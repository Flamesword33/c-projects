/*  stest -- simple server for queue
 * 
 *  Prints priority and name of object with highest priority
 *  on the queue.
 *
 *  Part of message queue facility example from
 *  Haviland, Gray, and Salama, "UNIX System Programming," 2e
 *  (Addison Wesley, 1998), pp. 195-199.
 */

#include "msgq.h"

extern int serve(void);


int main()
{
  pid_t pid;
  
  switch (pid = fork()) {
    case 0:        /* child */
      serve();
      break;       /* actually, serve never exits */
    case -1:
      perror("fork to start server failed");
      break;
    default:
      printf("server process pid is %d\n", pid);
  }
  
  exit(pid != -1 ? 0 : 1);
}
