#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/efi.h>
#include <linux/platform_device.h>
#include <linux/fsl_devices.h>

#include <mach/mx6.h>

#define procfs_name "memdump"
#define PROCFS_MAX_SIZE 1024

#ifdef DEBUG
#define MEMDUMP_DBG(fmt, ...) printk(fmt, ##__VA_ARGS__)
#else
#define MEMDUMP_DBG(fmt, ...)
#endif

static char procfs_buffer[PROCFS_MAX_SIZE];
/*
 * the /proc file structure
 */
struct proc_dir_entry *memdump_proc_file;
static unsigned long procfs_buffer_size = 0;
static struct task_struct *dump_task = NULL;

int procfile_read(char *buffer,
		char **buffer_location,
		off_t offset, int buffer_length, int *eof, void *data)
{
	int ret;

	MEMDUMP_DBG(KERN_INFO "procfile_read (/proc/%s) called\n", procfs_name);

	if (offset > 0) {
		ret = 0;
	} else {
		ret = sprintf(buffer, "Hi alive!\n");
	}

	return ret;
}

/* function invoked when writing into this proc file */
int procfile_write(struct file *file, const char *buffer, unsigned long count
		,void *data)
{
	unsigned int base ,i,reg;
	unsigned int  reg_count;	
	void __iomem *addr ;

	/* get user input buffer size */
	procfs_buffer_size = count;
	if (procfs_buffer_size > PROCFS_MAX_SIZE ) {
		procfs_buffer_size = PROCFS_MAX_SIZE;
	}

	/* write data to the buffer */
	if ( copy_from_user(procfs_buffer, buffer, procfs_buffer_size) ) {
		MEMDUMP_DBG (KERN_INFO "Error in reading user buffer :Exiting\n");
		return -EFAULT;
	}

	/* get the pid and segment to dump from user */
	sscanf (procfs_buffer,"%x %x",&base,&reg_count);
	MEMDUMP_DBG (KERN_INFO "reg count %x %x\n",base,reg_count);
	
	for ( i =0;  i < reg_count ; i++ ) {
		
		addr = MX6_IO_ADDRESS(base + i *4);
		if ( (i % 4) == 0 )
        		printk("\n0x%08X: ",base + i *4 );
		reg = __raw_readl(addr);
        	printk(" %08X", reg );
	}
        printk("\n", reg );

	return procfs_buffer_size;
}

/* module init routine */
static int __init memdump_init(void)
{
	/* create a proc entry under /proc/ for user to input pid,segmnet name*/
	memdump_proc_file = create_proc_entry(procfs_name, 0644, NULL);

	if (memdump_proc_file == NULL) {
		remove_proc_entry(procfs_name, NULL);
		MEMDUMP_DBG(KERN_ALERT "Error: Could not initialize /proc/%s\n",
				procfs_name);
		return -ENOMEM;
	}

	memdump_proc_file->read_proc = procfile_read;
	memdump_proc_file->write_proc = procfile_write;
	memdump_proc_file->mode = S_IFREG | S_IRUGO | S_IWUGO;
	memdump_proc_file->uid = 1;
	memdump_proc_file->gid = 0;

	MEMDUMP_DBG(KERN_INFO "/proc/%s created\n", procfs_name);	
	return 0;
}

/* module clean up routine */
static void __exit cleanup_mod(void)
{
	remove_proc_entry(procfs_name, NULL);
	MEMDUMP_DBG(KERN_ERR "/proc/%s removed\n", procfs_name);
}
module_init(memdump_init);
module_exit(cleanup_mod);
