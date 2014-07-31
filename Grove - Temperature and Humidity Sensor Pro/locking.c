/*
 * locking.c
 * Prevents rapid use of application by implementing a file lock
 *
 * Copyright (c) 2012 seeed technology inc.
 * Website    : www.seeed.cc
 * Author     : technion@lolware.net
 * Create Time:
 * Change Log :
 *
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <sys/file.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

int open_lockfile(const char *filename)
{
   int fd;
   fd = open(filename, O_CREAT | O_RDONLY, 0600);

   if (fd < 0)
   {
      printf("Failed to access lock file: %s\nerror: %s\n",
		filename, strerror(errno));
      exit(EXIT_FAILURE);
   }

   while(flock(fd, LOCK_EX | LOCK_NB) == -1)
   {
      if(errno == EWOULDBLOCK)
      {
         printf("Lock file is in use, exiting...\n");
         /* If the lockfile is in use, we COULD sleep and try again.
          * However, a lockfile would more likely indicate an already runaway
	  * process.
         */
	 exit(EXIT_FAILURE);
      }
      perror("Flock failed");
      exit(EXIT_FAILURE);
   }
   return fd;
}

void close_lockfile(int fd)
{
   if(flock(fd, LOCK_UN) == -1)
   {
      perror("Failed to unlock file");
      exit(EXIT_FAILURE);
   }
   if(close(fd) == -1)
   {
      perror("Closing descriptor on lock file failed");
      exit(EXIT_FAILURE);
   }
}
