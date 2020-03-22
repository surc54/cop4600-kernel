#include "tags.h"

int set_tag(int pid, int new_tag) {
        int ret = syscall(336, pid, new_tag);

        if (ret < 0) {
                return -1;
        }

        return (ret);
}

int get_tag(int pid) {
        int ret = syscall(335, pid);

        if (ret < 0) {
                return -1;
        }

        return (ret);
}
