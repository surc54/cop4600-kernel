#include <linux/sched.h>
#include <linux/syscalls.h>
#include <linux/printk.h>

SYSCALL_DEFINE1(get_tag, int, givenPid)
{
        struct task_struct *item = find_task_by_vpid(givenPid);
        if (!item) {
                printk("get_tag called on nonexistant task_struct\n");
                return -1;
        }
        printk("get_tag got tag of %d for pid = %d", item->tag, givenPid);
        return 0;
}

SYSCALL_DEFINE2(set_tag, int, givenPid, unsigned int, tag)
{
        printk("set_tag syscall called...");
        return 0;
}
