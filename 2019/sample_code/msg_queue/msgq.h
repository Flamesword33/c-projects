/*  msgq.h -- header for message queue facility example
 *
 *  From Haviland, Gray, and Salama, "UNIX System Programming," 2e
 *  (Addison Wesley, 1998), p. 195.
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <errno.h>

// old-style declaration of constants, using preprocessor
#define QKEY       (key_t)0105 /* identifying key for queue */
#define QPERM      0660        /* permissions for queue */
#define MAXOBN     50          /* maximum length of obj. name */
#define MAXPRIOR   10          /* maximum priority level */

struct q_entry {
  long mtype;
  char mtext[MAXOBN + 1];
};
