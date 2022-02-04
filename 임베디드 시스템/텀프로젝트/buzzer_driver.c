#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Johannes 4 GNU/Linux");
MODULE_DESCRIPTION("A simple gpio driver for setting a LED and reading a button");

static dev_t my_device_no;
static struct class *my_class;
static struct cdev my_device;

#define DRIVER_NAME "my_gpio2"
#define DRIVER_CLASS "MyModuleClass2"


static ssize_t driver_write(struct file *File, const char *user_buffer, size_t count, loff_t *offs) {
	int to_copy, not_copied, delta;
	char value;
	/* Get amount of data to copy */
	to_copy = min(count, sizeof(value));
	/* Copy data to user */
	not_copied = copy_from_user(&value, user_buffer, to_copy);
	/* Setting the LED */
	switch(value) {
		case '0':
			gpio_set_value(5, 0);
			break;
		case '1':
			gpio_set_value(5, 1);
			break;
		default:
			printk("Invalid Input!\n");
			break;
	}
	/* Calculate data */
	delta = to_copy - not_copied;
	return delta;
}

/**
 * @brief This function is called, when the device file is opened
 */
static int driver_open(struct inode *device_file, struct file *instance) {
	printk("led_button - open was called!\n");
	return 0;
}

/**
 * @brief This function is called, when the device file is opened
 */
static int driver_close(struct inode *device_file, struct file *instance) {
	printk("led_button - close was called!\n");
	return 0;
}

static struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = driver_open,
	.release = driver_close,
	.write = driver_write
};
/**
 * @brief This function is called, when the module is loaded into the kernel
 */
static int __init ModuleInit(void) {
	printk("Hello, Kernel!\n");

	/* Allocate a device nr */
	if( alloc_chrdev_region(&my_device_no, 0, 1, DRIVER_NAME) < 0) {
		printk("Device Nr. could not be allocated!\n");
		return -1;
	}
	printk("read_write - Device Nr. Major: %d, Minor: %d was registered!\n", my_device_no >> 20, my_device_no && 0xfffff);

	/* Create device class */
	if((my_class = class_create(THIS_MODULE, DRIVER_CLASS)) == NULL) {
		printk("Device class can not e created!\n");
		goto ClassError;
	}

	/* create device file */
	if(device_create(my_class, NULL, my_device_no, NULL, DRIVER_NAME) == NULL) {
		printk("Can not create device file!\n");
		goto FileError;
	}

	/* Initialize device file */
	cdev_init(&my_device, &fops);

	/* Regisering device to kernel */
	if(cdev_add(&my_device, my_device_no, 1) == -1) {
		printk("Registering of device to kernel failed!\n");
		goto AddError;
	}

	/* GPIO 5 init */
	if(gpio_request(5, "rpi-gpio-5")) {
		printk("Can not allocate GPIO 5\n");
		goto AddError;
	}

	/* Set GPIO 5 direction */
	if(gpio_direction_output(5,0)) {
		printk("Can not set GPIO 5 to output!\n");
		goto Gpio5Error;
	}
	gpio_set_value(5, 1);

	return 0;

Gpio5Error:
	gpio_free(5);
AddError:
	device_destroy(my_class, my_device_no);
FileError:
	class_destroy(my_class);
ClassError:
	unregister_chrdev_region(my_device_no, 1);
	return -1;
}

static void __exit ModuleExit(void) {
	
	gpio_free(5);
	cdev_del(&my_device);
	device_destroy(my_class, my_device_no);
	class_destroy(my_class);
	unregister_chrdev_region(my_device_no, 1);
	printk("Goodbye, Kernel\n");
}

module_init(ModuleInit);
module_exit(ModuleExit);