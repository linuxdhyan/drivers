#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/mm.h>
#include <linux/mman.h>
#include <linux/errno.h>
#include <linux/signal.h>
#include <linux/binfmts.h>
#include <linux/string.h>
#include <linux/file.h>
#include <asm/uaccess.h>
struct file *dump_file;	

static void __write_to_file(struct file *file, void *buf, size_t len)
{
        file->f_op->write(file, buf, len, &file->f_pos);
        if (file->f_op->flush) file->f_op->flush(file, 0);
}
int write_to_file(ulong *buf ,unsigned long size)
{
	mm_segment_t fs;
	fs = get_fs();
        set_fs(KERNEL_DS);
	__write_to_file(dump_file,buf,size);
	set_fs(fs);	
	return 0;
}
EXPORT_SYMBOL( write_to_file );

int open_file(char *fname)
{
	
	dump_file = filp_open(fname, O_CREAT | O_TRUNC, S_IRUGO | S_IWUSR);
	
	if (IS_ERR(dump_file)) {
		printk("Can't create '%s'\n", fname);
		return 0;
	}
	if (!dump_file->f_op->write) {
		filp_close(dump_file, NULL);
		printk("No write op\n");
		return 0;
	}
	return 1;

}
EXPORT_SYMBOL( open_file );

int close_file(void)
{
	if (dump_file){
		filp_close(dump_file, NULL);
		return 0;
	}
	return 1;
}
EXPORT_SYMBOL( close_file );


