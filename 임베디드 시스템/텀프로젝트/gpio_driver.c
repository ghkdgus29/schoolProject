#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/timer.h>

/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Johannes 4 GNU/Linux");
MODULE_DESCRIPTION("A simple gpio driver for setting a LED and reading a button");

/* Variables for device and device class */
static dev_t my_device_nr;
static struct class *my_class;
static struct cdev my_device;

#define DRIVER_NAME "my_gpio"
#define DRIVER_CLASS "MyModuleClass"

/**
 * @brief Read data out of the buffer
 */

static ssize_t driver_write(struct file *File, const char *user_buffer, size_t count, loff_t *offs) {
	int to_copy, not_copied, delta,i=0, j = 0;
	char value;
	int pin[4] = { 6, 13, 19, 26 };
	int seq[4][4] = { {1,0,0,1}, {1,1,0,0}, {0,1,1,0}, {0,0,1,1}};
	to_copy = min(count, sizeof(value));
	not_copied = copy_from_user(&value, user_buffer, to_copy);
	/* Setting the LED */
	switch (value) {
	case '1':
		i = 700;
		while (i > -1) {
			j = 3;
			while (j > -1) {
				gpio_set_value(pin[j], seq[i%4][j]);
				j--;
				mdelay(0.5);
			}
			i--;
		}
		break;
	case '2':
		while(i < 700) {
			j = 0;
			while (j < 4) {
				gpio_set_value(pin[j], seq[i%4][j]);
				j++;
				mdelay(0.5);
			}
			i++;
		}
		break;
	default:
		printk("Invalid Input!\n");
		break;
	}
	
	/* Calculate data */
	delta = to_copy - not_copied;
	return delta;
}

static ssize_t driver_read(struct file *File, char *user_buffer, size_t count, loff_t *offs) {
	int to_copy, not_copied, delta;
	char tmp;

	/* Get amount of data to copy */
	to_copy = min(count, sizeof(tmp));

	/* Read value of button */
	if (gpio_get_value(27) == 1)
	{
		tmp = '1';
	}
	else if (gpio_get_value(22) == 1)
	{
		tmp = '2';
	}
	else
		tmp = '0';
	/* Copy data to user */
	not_copied = copy_to_user(user_buffer, &tmp, to_copy);

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
	.read = driver_read,
	.write = driver_write
};

/**
 * @brief This function is called, when the module is loaded into the kernel
 */
static int __init ModuleInit(void) {
	printk("Hello, Kernel!\n");

	/* Allocate a device nr */
	if( alloc_chrdev_region(&my_device_nr, 0, 1, DRIVER_NAME) < 0) {
		printk("Device Nr. could not be allocated!\n");
		return -1;
	}
	printk("read_write - Device Nr. Major: %d, Minor: %d was registered!\n", my_device_nr >> 20, my_device_nr && 0xfffff);

	/* Create device class */
	if((my_class = class_create(THIS_MODULE, DRIVER_CLASS)) == NULL) {
		printk("Device class can not e created!\n");
		goto ClassError;
	}

	/* create device file */
	if(device_create(my_class, NULL, my_device_nr, NULL, DRIVER_NAME) == NULL) {
		printk("Can not create device file!\n");
		goto FileError;
	}

	/* Initialize device file */
	cdev_init(&my_device, &fops);

	/* Regisering device to kernel */
	if(cdev_add(&my_device, my_device_nr, 1) == -1) {
		printk("Registering of device to kernel failed!\n");
		goto AddError;
	}


	if(gpio_request(27, "rpi-gpio-27")) {				//27
		printk("Can not allocate GPIO 27\n");
		goto AddError;
	}
	if(gpio_direction_input(27)) {
		printk("Can not set GPIO 17 to input!\n");
		goto Gpio27Error;
	}

	if (gpio_request(22, "rpi-gpio-22")) {				//22
		printk("Can not allocate GPIO 22\n");
		goto AddError;
	}
	if(gpio_direction_input(22)) {
		printk("Can not set GPIO 12 to input!\n");
		goto Gpio22Error;
	}

	if (gpio_request(6, "rpi-gpio-6")) {				//6
		printk("Can not allocate GPIO 6\n");
		goto AddError;
	}
	if (gpio_direction_output(6,0)) {
		printk("Can not set GPIO 6 to input!\n");
		goto Gpio6Error;
	}

	if (gpio_request(13, "rpi-gpio-13")) {				//13
		printk("Can not allocate GPIO 13\n");
		goto AddError;
	}
	if (gpio_direction_output(13,0)) {
		printk("Can not set GPIO 13 to input!\n");
		goto Gpio13Error;
	}

	if (gpio_request(19, "rpi-gpio-19")) {				//19
		printk("Can not allocate GPIO 19\n");
		goto AddError;
	}
	if (gpio_direction_output(19,0)) {
		printk("Can not set GPIO 19 to input!\n");
		goto Gpio19Error;
	}
	if (gpio_request(26, "rpi-gpio-26")) {				//26
		printk("Can not allocate GPIO 26\n");
		goto AddError;
	}
	if (gpio_direction_output(26,0)) {
		printk("Can not set GPIO 26 to input!\n");
		goto Gpio26Error;
	}


	return 0;

Gpio27Error:
	gpio_free(27);
Gpio22Error:
	gpio_free(22);
Gpio6Error:
	gpio_free(6);
Gpio13Error:
	gpio_free(13);
Gpio19Error:
	gpio_free(19);
Gpio26Error:
	gpio_free(26);
AddError:
	device_destroy(my_class, my_device_nr);
FileError:
	class_destroy(my_class);
ClassError:
	unregister_chrdev_region(my_device_nr, 1);
	return -1;
}

/**
 * @brief This function is called, when the module is removed from the kernel
 */
static void __exit ModuleExit(void) {
	
	gpio_free(27);
	gpio_free(22);
	gpio_free(6);
	gpio_free(13);
	gpio_free(19);
	gpio_free(26);
	cdev_del(&my_device);
	device_destroy(my_class, my_device_nr);
	class_destroy(my_class);
	unregister_chrdev_region(my_device_nr, 1);
	printk("Goodbye, Kernel\n");
}

module_init(ModuleInit);
module_exit(ModuleExit);


