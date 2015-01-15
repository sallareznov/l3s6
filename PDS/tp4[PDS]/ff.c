#include "forkfork.c"

int main(int argc, char *argv[]) {
	func_t f;
	void *arg;

	f = (func_t) &function;
	arg = (void *) ' ';

	forkfork(f,arg);
	return 0;
}