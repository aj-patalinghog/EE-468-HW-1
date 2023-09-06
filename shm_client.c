/*  This program will create a shared memory block
 *  that can be accessed by client programs.  The
 *  The server and shared memory block will terminate
 *  terminate when the first byte of the shared memory
 *  is 't', i.e., shm[0] = 't'.  The server will check
 *  this byte every second.
 */

#include <sys/types.h>  // for shared memory
#include <sys/ipc.h>    // for shared memory
#include <sys/shm.h>    // for shared memory
#include <stdio.h>
#include <stdlib.h>     // for exit()
#include <unistd.h>     // for sleep()

#define SHMSZ 50        // Shared memory size 
#define SHMNAME 5678    // Name for the shared memory

void main(int argc, char *argv[]) {
   int id;  // shared memory id
   key_t key;
   char *shm;
   int k;

   // Create shared memory
   id = shmget(SHMNAME, SHMSZ, 0666);
   if(id < 0) {
      perror("shmget");
      printf("Server: Creation of shared memory failed\n");
      exit(1);
   }


   // Attached shared memory
   shm = shmat(id, NULL, 0);
   if(shm == (char *)-1) { 
      perror("shmat");
      printf("Client: Couldn't attach shared memory\n");
      exit(1);
   }

   printf("Client: shared memory %d is on\n", SHMNAME);

   while(shm[0] != 'c'); // Wait until server is ready

   // Write parameter into shm
   for(k = 0; (k < SHMSZ-1) && (argv[1][k] != '\0'); k++) {
      shm[k+1] = argv[1][k];
   }
   if(k < SHMSZ-1) {
      shm[k+1] = '\0';
   }
   shm[0] = 'd'; // Command server to display

   while(shm[0] != 'c'); // Wait until server is ready

   shm[0] = 't';

   shmdt(shm); // detach shared memory

   printf("Client: exit\n");
}

