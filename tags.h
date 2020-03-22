#include <unistd.h>
#include <sys/syscall.h>
#include <linux/kernel.h>

int set_tag(int pid, int new_tag);

int get_tag(int pid);
