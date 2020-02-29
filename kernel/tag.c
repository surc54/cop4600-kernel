#include <linux/sched.h>
#include <linux/syscalls.h>
#include <linux/printk.h>

SYSCALL_DEFINE1(get_tag, int, givenPid)
{
        struct task_struct *item = find_task_by_vpid(givenPid);
        if (!item) {
                printk("get_tag called on nonexistant pid %d", givenPid);
                return -1;
        }
        printk("get_tag got tag of %d for pid = %d", item->tag, givenPid);
        return item->tag;
}

SYSCALL_DEFINE2(set_tag, int, givenPid, unsigned int, tag)
{
	// Found has_capability in include/linux/capability.h
	if (!has_capability(current, CAP_SYS_ADMIN) && current->pid != givenPid) {
		// no sudo
		printk("set_tag called without required capability");
		return -1;
	}

	if (tag >> 31 != 0) {
		printk("set_tag called with invalid tag (not 0 in MSB)");
		return -1;
	}

	struct task_struct *item = find_task_by_vpid(givenPid);

	if (!item) {
		printk("set_tag called on nonexistant pid %d", givenPid);
		return -1;
	}

        printk("set_tag syscall called...");
        return 0;
}
