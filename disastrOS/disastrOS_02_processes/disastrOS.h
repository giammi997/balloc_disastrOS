#pragma once
#include "disastrOS_pcb.h"
#include "linked_list.h"

// *** GIANMARCO ***
#include <stdint.h>
#include "bit_map.h"
// *** GIANMARCO ***

#ifdef _DISASTROS_DEBUG_
#include <stdio.h>

#define disastrOS_debug(...) printf(__VA_ARGS__) 

#else //_DISASTROS_DEBUG_

#define disastrOS_debug(...) ;

#endif //_DISASTROS_DEBUG_

// initializes the structures and spawns a fake init process
void disastrOS_start(void (*f)(void*), void* args, char* logfile);

// generic syscall 
int disastrOS_syscall(int syscall_num, ...);

// classical process control
int disastrOS_getpid(); // this should be a syscall, but we have no memory separation, so we return just the running pid
int disastrOS_fork();
void disastrOS_exit(int exit_value);
int disastrOS_wait(int pid, int* retval);
void disastrOS_preempt();
void disastrOS_spawn(void (*f)(void*), void* args );
void disastrOS_shutdown();

// *** GIANMARCO ***

// Settings
#define MAX_BYTES_ALLOCATABLE 1U << 29 // 512 MB
#define ZERO_GENERATOR "/dev/zero"

#define DEFAULT_MEM_SIZE 1U << 20 // 1 MB
#define DEFAULT_LEVELS 14
#define DEFAULT_MIN_SIZE (DEFAULT_MEM_SIZE >> (DEFAULT_LEVELS - 1))
// #####

// Params
uint64_t MEM_SIZE;
uint8_t LEVELS;
uint32_t MIN_SIZE;

// Buddy references
uint8_t * bitmap_buffer;
BitMap bitmap;
char * memory;

void * disastrOS_balloc(size_t bytes);
void disastrOS_bfree(void * ptr);
// *** GIANMARCO ***

// debug function, prints the state of the internal system
void disastrOS_printStatus();
