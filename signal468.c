#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>  

int proc_state = 0;

void sig_handler(int signo) {
   if(signo == SIGUSR1) {
      proc_state++;
   }
}

int main() {
   pid_t pid = fork();
   if(pid == 0) { // Child
      printf("Child begins:  address of proc_state = %x\n", &proc_state);
      pid_t child_id = getpid();
      // Register sig_handler with the signal SIGUSR1
      signal(SIGUSR1, sig_handler);
      for(int i = 0; i < 6; i++) {
         printf("Child: proc_state=%d, address=%#x\n", proc_state, &proc_state);
         sleep(1);
      }
      printf("Child: bye bye\n");
      exit(0);
   } else { // Parent
      printf("Parent begins:  address of proc_state = %#x\n", &proc_state);
      for(int i = 0; i < 3; i++) {
         sleep(1);
         printf("Parent: proc_state=%d, address=%#x\n", proc_state, &proc_state);
         // Send a SIGUSR1 signal here
         kill(pid, SIGUSR1);
      }
      sleep(2);
      wait(NULL);
      printf("Parent: proc_state=%d, address=%#x\n", proc_state, &proc_state);
      printf("Parent: bye bye\n");
   }
}

