/*  
 *  This server program will create a shared memory block shm[]
 *  that can be accessed by client programs.  
 *  Clients can send commands in shm[0]:
 *     shm[0] = 't' means terminate the server
 *     shm[1] = 'd' means display the character string in shm[]
 *                  starting from shm[1].  The maximum string length
 *                  is 49 bytes or terminated by '0\'.
 *  When the server completes a command, it sets shm[0] to 'c'
 */

#include <sys/types.h>  // for shared memory
#include <sys/ipc.h>    // for shared memory
#include <sys/shm.h>    // for shared memory
#include <stdio.h>
#include <stdlib.h>     // for exit()
#include <unistd.h>     // for sleep()

#define SHMSZ 50        // Shared memory size
#define SHMNAME 5678    // Name for the shared memory

void main() {
   int id; // shared memory id
   key_t key;
   char *shm;
   int k;
   char buffer[SHMSZ]; // Local buffer for displaying char string

   // Create shared memory
   id = shmget(SHMNAME, SHMSZ, IPC_CREAT | 0666);
   if(id < 0) {
      perror("shmget");
      printf("Server: Creation of shared memory failed\n");
      exit(1);
   }

   // Attached shared memory
   shm = shmat(id, NULL, 0);
   if (shm == (char *)-1) { 
      perror("shmat");
      printf("Server: Couldn't attach shared memory\n");
      exit(1);
   }

   printf("Server: shared memory %d is on\n", SHMNAME);
   for(shm[0] = 'c'; shm[0] != 't'; sleep(1)) {
      if(shm[0] == 'd') { // Display. First copy to buffer
         printf("Server: display\n");	
         for(k = 0; (k < SHMSZ-1) && (shm[k+1] != '\0'); k++) {
            buffer[k] = shm[k+1];
         }
         shm[k+1] = '\0';
         printf("Server: shm = %s\n", buffer);
         shm[0] = 'c';
      }
   }

   shmdt(shm); // detach shared memory
   shmctl(id, IPC_RMID, NULL); // remove shared memory

   printf("Server: exit\n");
}

