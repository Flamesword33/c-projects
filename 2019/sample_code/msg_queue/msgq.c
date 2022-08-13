/*  msgq.c -- functions for message queue facility example
 *
 *  From Haviland, Gray, and Salama, "UNIX System Programming," 2e
 *  (Addison Wesley, 1998), p. 195.
 */

#include <stdio.h>
#include "msgq.h"

int init_queue(void);
int proc_obj(struct q_entry *msg);
int warn(char *s);

/* 
 * enter -- place an object into queue 
 * 
 * The 'q_entry' object consists of a priority and a
 * null-terminated name.
 */

int enter(char *objname, int priority)
{
  int len, s_qid;
  struct q_entry s_entry;  // structure to hold message
  
  /* validate name length, priority level */
  
  if ( (len = strlen(objname)) > MAXOBN )
  {
    warn("name too long");
    return (-1);
  }
  
  if (priority > MAXPRIOR || priority < 0)
  {
    warn("invalid priority level");
    return (-1);
  }

  /* initialize message queue as necessary */
  
  if ( (s_qid = init_queue()) == -1)
    return (-1);
  
  /*initialize s_entry */
  s_entry.mtype = (long) priority;
  strncpy(s_entry.mtext, objname, MAXOBN);
  
  /* send message, waiting if necessary */
  
  /* int msgsnd(int mquid, const void *message,
   *            size_t size, int flags);
   */
  if (msgsnd(s_qid, &s_entry, len, 0) == -1)
  {
    perror("msgsnd failed");
    return (-1);
  }
  else
    return (0);
}


/*
 * serve -- serve object with highest priority in the queue 
 */

int serve(void)
{
  int mlen, r_qid;
  struct q_entry r_entry;
  
  /* initialize message queue as necessary
  
  if ( (r_qid = init_queue()) == -1)
    return (-1);
  
  /* get and process next message, waiting if necessary */
  
  for (;;)
  {
    /* int msgrcv(int mqid, void *message, size_t size,
     *            long msg_type, int flags)
     * If 'msg_type' is 0, the first message on the queue is read.
     * If 'msg_type' is positive, the first message with that value is read.
     * If 'msg_type' is negative, the first message with the lowest 
     *   type value <= abs(msg_type) is read.
     */
    if ( (mlen = msgrcv(r_qid, &r_entry, MAXOBN, 
      (-1 * MAXPRIOR), MSG_NOERROR)) == -1)
    {
      perror("msgrcv failed");
      return (-1);
    }
    else
    {
      /* make sure we have a null-terminated string */
      r_entry.mtext[mlen] = '\0';
      
      /* process object name */
      proc_obj(&r_entry);
    }
  }
}


/*
 * init_queue -- get queue identifier
 */

int init_queue(void)
{
  int queue_id;
  
  /* attempt to create or open message queue */
  if ( (queue_id = msgget(QKEY, IPC_CREAT | QPERM )) == -1)
    perror("msgget failed");
  
  return (queue_id);
}


/*
 *  proc_obj -- print object's priority and name 
 */

int proc_obj(struct q_entry *msg)
{
  printf("\npriority: %ld   name: %s\n", msg->mtype, msg->mtext);
}


/*
 * warn -- print error message
 */

int warn(char *s)
{
  fprintf(stderr, "warning: %s\n", s);
}
