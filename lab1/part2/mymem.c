#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/cdev.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Guojun Chen");
MODULE_DESCRIPTION("A simple memory module.");
MODULE_VERSION("0.01");

/**
 * 1. create a new device file in /dev/mymem
 * 2. allocate 512KB region of memory when init
 * 3. deallocate the memory when exit
 * 4. open/read/write/llseek/close
 */

#define BUFFER_LENGTH (512 * 1024)
#define DEVICE_NAME "mymem"
#define CLASS_NAME "mymem_class"

struct mymem_struct {
    struct cdev cdev_;
};

static void *mem_ptr = NULL;
static int is_open = 0;
struct mymem_struct m_s;
static int mymem_open(struct inode *, struct file *);
static ssize_t mymem_read(struct file *, char __user *, size_t, loff_t *);
static ssize_t mymem_write(struct file *, const char __user *, size_t, loff_t *);
static loff_t mymem_llseek(struct file *, loff_t, int);
static int mymem_close(struct inode *, struct file *);

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .read = mymem_read,
    .write = mymem_write,
    .open = mymem_open,
    .release = mymem_close,
    .llseek = mymem_llseek,
};

static int mymem_open(struct inode *_inode, struct file *filp) {
    if (is_open)
        return -EBUSY;
    is_open++;
    return 0;
}

static int mymem_close(struct inode *_inode, struct file *filp) {
    is_open--;
    return 0;
}

static ssize_t mymem_read(struct file *filp, char __user *buffer, size_t len, loff_t *offset) {
    if (*offset + len > BUFFER_LENGTH) {
        return -EINVAL;
    }

    if (copy_to_user(buffer, mem_ptr + *offset, len)) {
        printk(KERN_INFO "mymem_read: copy_to_user [FAILED]\n");
        return -EFAULT;
    }

    *offset += len;
    return len;
}

static ssize_t mymem_write(struct file *filp, const char __user *buffer, size_t len, loff_t *offset) {
    if (*offset + len > BUFFER_LENGTH) {
        return -EINVAL;
    }

    if (copy_from_user(mem_ptr + *offset, buffer, len)) {
        printk(KERN_INFO "mymem_write: copy_from_user [FAILED]\n");
        return -EFAULT;
    }

    *offset += len;
    return len;
}

static loff_t mymem_llseek(struct file *filp, loff_t offset, int whence) {
    int temp_pos;
    switch (whence) {
        case SEEK_SET:
            temp_pos = offset;
            break;
        case SEEK_CUR:
            temp_pos = filp->f_pos + offset;
            break;
        case SEEK_END:
            temp_pos = BUFFER_LENGTH + offset;
            break;
        default:
            return -EINVAL;
    }

    if (temp_pos >= 0 && temp_pos < BUFFER_LENGTH) {
        filp->f_pos = temp_pos;
    } else {
        return -EINVAL;
    }

    return temp_pos;
}

static dev_t dev_nums;
static struct class *mymem_class;
static struct device *mymem_dev;

static int __init mymem_init(void) {
    int ret;
    mem_ptr = kmalloc(BUFFER_LENGTH, GFP_KERNEL);
    if (mem_ptr == NULL) {
        printk(KERN_INFO "mymem_init: kmalloc [FAILED]\n");
        return ENOMEM;
    } else {
        ret = alloc_chrdev_region(&dev_nums, 0, 1, DEVICE_NAME); //allocating major and minor numbers for the device
        if (ret < 0) {
            printk(KERN_INFO "mymem_init: unable to allocate region");
            return ret;
        }

        cdev_init(&m_s.cdev_, &fops);
        if (cdev_add(&m_s.cdev_, dev_nums, 1) < 0) {
            printk(KERN_INFO "mymem_init: cdev add [FAILED]\n");
            return ret;
        }

        mymem_class = class_create(THIS_MODULE, CLASS_NAME);
        mymem_dev = device_create(mymem_class, NULL, dev_nums, NULL, DEVICE_NAME);

        if (mymem_dev == NULL) {
            return -1;
        }
    }
    printk(KERN_INFO "mymem_init [OK]\n");
    return 0;
}
static void __exit mymem_exit(void) {
    device_destroy(mymem_class, dev_nums);
    class_destroy(mymem_class);
    cdev_del(&m_s.cdev_);
    unregister_chrdev_region(dev_nums, 1);
    kfree(mem_ptr);
    printk(KERN_INFO "mymem_exit\n");
}

module_init(mymem_init);
module_exit(mymem_exit);
