#include <linux/sched.h>
#include <linux/syscalls.h>
#include <linux/printk.h>
#include <linux/capability.h>
#include "./sched/sched.h"

SYSCALL_DEFINE1(get_tag, int, givenPid)
{
        struct task_struct *item = find_task_by_vpid(givenPid);
        if (!item) {
                printk("get_tag called on nonexistant pid %d\n", givenPid);
                return -1;
        }
        printk("get_tag got tag of %d for pid = %d\n", item->tag, givenPid);
        return item->tag;
}

SYSCALL_DEFINE2(set_tag, int, pid, unsigned int, tag)
{
	bool is_root;
	struct task_struct *item;
	unsigned int
		current_tag,
		cur_level,
		new_level,
		cur_bitmap,
		new_bitmap,
		diff,
		check;

	if (tag >> 31 != 0) {
		printk("set_tag called with invalid tag (not 0 in MSB)\n");
		return -1;
	}

	// has_capability was found in include/linux/capability.h
	is_root = has_capability(current, CAP_SYS_ADMIN);

	if (current->pid != pid) {
		// Modifying someone else
		if (!is_root) {
			printk("set_tag called without sudo and non-current pid (cur=%d != %d)\n",
				current->pid, pid);
			return -1;
		}

		item = find_task_by_vpid(pid);

		if (!item) {
			printk("set_tag called on nonexistant pid %d\n", pid);
			return -1;
		}

		printk("set_tag on PID %d (%u -> %u)\n", pid, item->tag, tag);
		item->tag = tag;
	} else {
		// Modifying self
		current_tag = current->tag;
		if (!is_root) {
			// check if new tag level is bigger than old tag level
			cur_level = current_tag & 3;
			new_level = tag & 3;
			if (new_level > cur_level) {
				printk("set_tag called without sudo and tag with higher level (%u -> %u)\n",
					cur_level, new_level);
				return -1;
			}

			// check if any bitmap item was set to 1 from 0
			cur_bitmap = current_tag >> 2;
			new_bitmap = tag >> 2;
			diff = cur_bitmap ^ new_bitmap;
			check = diff & cur_bitmap;

			if (diff != check) {
				printk("set_tag called without sudo and tag bitmap item set to 1\n");
				return -1;
			}
		}
		// everything is okay, set tag
		current->tag = tag;
		printk("set_tag on PID %d (%u -> %u)\n", pid, current_tag, tag);
	}

        return tag;
}

SYSCALL_DEFINE1(get_level_alloc, unsigned int, level)
{
	unsigned int cur_lvl;
	const struct sched_class *scheduler;

	if (!current) {
		printk("[get_level_alloc] could not get current process.\n");
		return -1;
	}

	scheduler = current->sched_class;

	if (!scheduler) {
		printk("[get_level_alloc] could not get sched_class from current.\n");
		return -1;
	}

	cur_lvl = scheduler->current_level;

	printk("[get_level_alloc] got current_level of %u\n", cur_lvl);

	printk("get_level_alloc called!\n");
	return 0;
}

SYSCALL_DEFINE2(set_level_alloc, unsigned int, level, unsigned int, newAlloc)
{
	unsigned int cur_lvl;
	const struct sched_class *scheduler;

	if (!current) {
		printk("[set_level_alloc] could not get current process.\n");
		return -1;
	}

	scheduler = current->sched_class;

	if (!scheduler) {
		printk("[set_level_alloc] could not get sched_class from current.\n");
		return -1;
	}

	cur_lvl = scheduler->current_level;

	if (cur_lvl == 3) {
		scheduler->current_level = 0;
	} else {
		scheduler->current_level = cur_lvl + 1;
	}

	printk("set_level_alloc called!\n");
	return 0;
}
