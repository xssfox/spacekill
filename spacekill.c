#define _GNU_SOURCE
#include <dlfcn.h>

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include <limits.h>

static ssize_t (*real_write)(int fd, const void *buf, size_t count)=NULL;


// This loads the real "write"
static void real_write_init(void)
{
    real_write = dlsym(RTLD_NEXT, "write");
    if (NULL == real_write) {
        fprintf(stderr, "Error in `dlsym`: %s\n", dlerror());
    }
}



ssize_t write(int fd, const void *buf, size_t count){
    if(real_write==NULL) { // Load the "real" write function
        real_write_init();
    }

    ssize_t write_return = NULL;

    write_return = real_write(fd, buf, count); // Run the write normally

    if (write_return == -1 && (errno == ENOSPC || errno == EDQUOT)){ // If any error occurs check if it's a disk space or quota issue has occured
        
        //try to get filename - only works on linux
        char filename[PATH_MAX+1];
        char fd_name[PATH_MAX+1];

        snprintf(fd_name, sizeof fd_name, "/proc/self/fd/%d", fd); 
        
        
        if (!realpath(fd_name, filename)){
            // error getting filename (like on Mac?)
            printf("[SPACEKILL] write returned a file system space error. Terminating app\n");
        } else { 
            printf("[SPACEKILL] write returned a file system space error when writing %s.\nThis file might not have caused the space but should indicate which mount point the issue is with.\nTerminating app\n", filename);
        }    
        exit(91); // Because it seems like a cool number?
    }

    // return the original response back to the program if all goes well
    return write_return;
}