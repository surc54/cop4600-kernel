#include <linux/sched.h>
#include <linux/syscalls.h>
#include <linux/printk.h>
#include <linux/capability.h>

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

SYSCALL_DEFINE2(set_tag, int, pid, unsigned int, tag)
{
	if (tag >> 31 != 0) {
		printk("set_tag called with invalid tag (not 0 in MSB)");
		return -1;
	}

	// has_capability was found in include/linux/capability.h
	bool is_root = has_capability(current, CAP_SYS_ADMIN);

	if (current->pid != pid) {
		// Modifying someone else
		if (!is_root) {
			printk("set_tag called without sudo and non-current pid");
			return -1;
		}

		struct task_struct *item = find_task_by_vpid(pid);

		if (!item) {
			printk("set_tag called on nonexistant pid %d", pid);
			return -1;
		}

		printk("set_tag on PID %d (%u -> %u)", pid, item->tag, tag);
		item->tag = tag;
	} else {
		// Modifying self
		unsigned int current_tag = current->tag;
		if (!is_root) {
			// check if new tag level is bigger than old tag level
			unsigned int cur_level = current_tag & 3;
			unsigned int new_level = tag & 3;
			if (new_level > cur_level) {
				printk("set_tag called without sudo and tag with higher level (%u -> %u)",
					cur_level, new_level);
				return -1;
			}

			// check if any bitmap item was set to 1 from 0
			unsigned int cur_bitmap = current_tag >> 2;
			unsigned int new_bitmap = tag >> 2;
			unsigned int diff = cur_bitmap ^ new_bitmap;
			unsigned int check = diff & cur_bitmap;

			if (diff != check) {
				printk("set_tag called without sudo and tag bitmap item set to 1");
				return -1;
			}
		}
		// everything is okay, set tag
		current->tag = tag;
		printk("set_tag on PID %d (%u -> %u)", pid, current_tag, tag);
	}
        return 0;
}
