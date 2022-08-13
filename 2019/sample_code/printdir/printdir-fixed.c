/*  printdir2.c
    Neil Matthew and Richard Stones
    Beginning Linux Programming, 4e (Wrox/Wiley, 2008)

    Revised by J. Mohr, adding more error checking on system calls.

    We start with the appropriate headers and then a function, printdir,
    which prints out the current directory.
    It will recurse for subdirectories, using the depth parameter is used for indentation.  */

#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stddef.h>
#include <errno.h>

/* Print an error message with an embedded filename, then call perror.
 * This function assumes that the message string ('msg') contains
 * a "%s" conversion specifier that indicates where the filename
 * is to be inserted in the message string.
 */
void syserr(char * msg, char * name)
{
    fprintf(stderr, msg, name);
    fprintf(stderr, ": ");
    /* In ISO C, calling 'perror' with a null pointer causes it
     * to print only the system-specific error message, not a
     * prefix string, colon, or space.
     */
    perror(NULL);
}

void printdir(char *dir, int depth)
{
    DIR *dp;
    int status;
    struct dirent *entry;
    struct stat statbuf;

    if((dp = opendir(dir)) == NULL) {
        syserr("Cannot open directory '%s'", dir);
        return;
    }
    status = chdir(dir);
    if (status != 0)
    {
	syserr("Cannot change directory to '%s'", dir);
	/* Since we could open this directory, but not change to it, we can at least
	   list its contents. */
	printf("  Contents of %s (non-recursive):\n", dir);
	errno = 0;		/* clear errno so we can distinguish error from EOF */
	while((entry = readdir(dp)) != NULL) {
	    /* Found a directory, but ignore . and .. */
	    if(strcmp(".",entry->d_name) == 0 || 
	       strcmp("..",entry->d_name) == 0)
		continue;
	    printf("%*s%s\n",depth,"",entry->d_name);
	}
	/* Check if 'readdir' returned NULL due to error or end of directory stream. */
	if (errno != 0) {
	    syserr("Failure while reading directory stream '%s'", dir);
	}	
    }
    else {
	errno = 0;		/* clear errno so we can distinguish error from EOF */
	while((entry = readdir(dp)) != NULL) {
	    status = lstat(entry->d_name,&statbuf);
	    if (status != 0)
		syserr("lstat failed on '%s'", entry->d_name);
	    else {
		if(S_ISDIR(statbuf.st_mode)) {
		    /* Found a directory, but ignore . and .. */
		    if(strcmp(".",entry->d_name) == 0 || 
		       strcmp("..",entry->d_name) == 0)
			continue;
		    printf("%*s%s/\n",depth,"",entry->d_name);
		    /* Recurse at a new indent level */
		    printdir(entry->d_name,depth + 4);
		}
		else printf("%*s%s\n",depth,"",entry->d_name);
	    }
	    errno = 0;		/* clear errno so we can distinguish error from EOF */
	}
	/* Check if 'readdir' returned NULL due to error or end of directory stream. */
	if (errno != 0) {
	    syserr("Failure while reading directory stream '%s'", dir);
	}
	status = chdir("..");
	if (status != 0)
	{
	    char dirname[NAME_MAX + 1];	/* NAME_MAX is defined in dirent.h */
	    if(getcwd(dirname, NAME_MAX) == NULL)
		perror("getcwd failed");
	    else
		syserr("Cannot change to parent directory of '%s'", dirname);
	}
    }
    status = closedir(dp);
    if (status != 0)
	syserr("Cannot close directory stream '%s'", dir);
}

/*  Now we move onto the main function.  */

int main(int argc, char* argv[])
{
    char *topdir, pwd[2]=".";
    if (argc != 2)
        topdir=pwd;
    else
        topdir=argv[1];

    printf("Directory scan of %s:\n",topdir);
    printdir(topdir,0);
    //printf("done.\n");

    exit(EXIT_SUCCESS);
}


