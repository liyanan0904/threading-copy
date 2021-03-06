#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <time.h>

#define LEN (1024 * 1024 * 8)
#define LOOP 50

struct timespec start;
static int vec[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

static void loop(char *inst) {
	void *table[] = { &&L0, &&L1, &&L2, &&L3, &&L4, &&L5, &&L6, &&L7, &&L8, &&L9, &&L10, &&L11, &&L12, &&L13, &&L14, &&L15 };
	void *sp;
	void *machinecode = mmap(NULL, 12 * LEN, PROT_WRITE | PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (mprotect(machinecode, 12 * LEN, PROT_WRITE | PROT_READ | PROT_EXEC))
		perror("mprotect"); 
	unsigned char *p = machinecode;
	for (int i = 0; i < LEN; i++) {
		*(p++) = 0x48; *(p++) = 0xb8;
		*((long*) p) = (long) table[inst[i]];
		p += 8;
		*(p++) = 0xff; *(p++) = 0xd0;	
	}
	clock_gettime(CLOCK_REALTIME, &start); 	

#define ENTER                                   \
  asm volatile ("pop %0;" : "=r"(sp))

#define RET                                     \
  asm volatile ("push %0; ret;" : : "r"(sp) : "%rax")

		goto *machinecode;

L0:ENTER; return;
L1:ENTER; for (int i = 0; i < LOOP; i++) vec[1]++; RET;
L2:ENTER; for (int i = 0; i < LOOP; i++) vec[2]++; RET;
L3:ENTER; for (int i = 0; i < LOOP; i++) vec[3]++; RET;
L4:ENTER; for (int i = 0; i < LOOP; i++) vec[4]++; RET;
L5:ENTER; for (int i = 0; i < LOOP; i++) vec[5]++; RET;
L6:ENTER; for (int i = 0; i < LOOP; i++) vec[6]++; RET;
L7:ENTER; for (int i = 0; i < LOOP; i++) vec[7]++; RET;
L8:ENTER; for (int i = 0; i < LOOP; i++) vec[8]++; RET;
L9:ENTER; for (int i = 0; i < LOOP; i++) vec[9]++; RET;
L10:ENTER; for (int i = 0; i < LOOP; i++) vec[10]++; RET;
L11:ENTER; for (int i = 0; i < LOOP; i++) vec[11]++; RET;
L12:ENTER; for (int i = 0; i < LOOP; i++) vec[12]++; RET;
L13:ENTER; for (int i = 0; i < LOOP; i++) vec[13]++; RET;
L14:ENTER; for (int i = 0; i < LOOP; i++) vec[14]++; RET;
L15:ENTER; for (int i = 0; i < LOOP; i++) vec[15]++; RET;

}	

static void printvec()  {
	for (int i = 0; i < 16; i++) {
		printf("%d ", vec[i] ); 
	}	
	printf("\n") ;
}

static void printtime() {
	struct timespec end;
	clock_gettime(CLOCK_REALTIME, &end);
	long t1 = start.tv_sec * 1000 * 1000 * 1000 + start.tv_nsec;
	long t2 = end.tv_sec * 1000 * 1000 * 1000 + end.tv_nsec;
	printf("time: %f\n", (t2 - t1)/1e9); 	
}	

int main(int argc, char **argv) {
	char *inst = malloc(LEN);
	for (int i = 0; i < LEN - 1;) {
		int op = rand() & 15;
		if (op == 0) {
			continue;
		}	
		inst[i++] = op;
	}	
	inst[LEN -1] = 0;
	loop(inst);
	printvec(); 	
	printtime();
	return 0;	
}

