#ifndef __LORA_H__
#define __LORA_H__

#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/mutex.h>

/* I/O control by each command. */
#define LORA_IOC_MAGIC '\x66'

#define LORA_SET_STATE		(_IOW(LORA_IOC_MAGIC, 0, int))
#define LORA_GET_STATE		(_IOR(LORA_IOC_MAGIC, 1, int))
#define LORA_SET_FREQUENCY	(_IOW(LORA_IOC_MAGIC, 2, int))
#define LORA_GET_FREQUENCY	(_IOR(LORA_IOC_MAGIC, 3, int))
#define LORA_SET_POWER		(_IOW(LORA_IOC_MAGIC, 4, int))
#define LORA_GET_POWER		(_IOR(LORA_IOC_MAGIC, 5, int))
#define LORA_SET_BANDWIDTH	(_IOW(LORA_IOC_MAGIC, 6, int))
#define LORA_GET_BANDWIDTH	(_IOR(LORA_IOC_MAGIC, 7, int))


/* List the state of the LoRa device. */
#define LORA_SLEEP		0
#define LORA_STANDBY		1
#define LORA_TX			2
#define LORA_RX			3

struct lora_data;

/* The structure lists the LoRa device's operations. */
struct lora_operations {
	/* Set & get the state of the LoRa device. */
	long (*setState)(struct lora_data *, void __user *);
	long (*getState)(struct lora_data *, void __user *);
	/* Set & get the carrier frequency. */
	long (*setFreq)(struct lora_data *, void __user *);
	long (*getFreq)(struct lora_data *, void __user *);
	/* Set & get the PA power. */
	long (*setPower)(struct lora_data *, void __user *);
	long (*getPower)(struct lora_data *, void __user *);
	/* Set & get the RF bandwith. */
	long (*setBW)(struct lora_data *, void __user *);
	long (*getBW)(struct lora_data *, void __user *);
	/* Read from the LoRa device's communication. */
	ssize_t (*read)(struct lora_data *, const char __user *, size_t);
	/* Write to the LoRa device's communication. */
	ssize_t (*write)(struct lora_data *, const char __user *, size_t);
};

struct lora_data {
	dev_t devt;
	void *lora_device;
	struct list_head device_entry;
	struct lora_operations *ops;
	uint8_t *tx_buf;
	uint8_t *rx_buf;
	uint8_t tx_buflen;
	uint8_t rx_buflen;
	uint8_t bufmaxlen;
	uint8_t users;
	struct mutex buf_lock;
	uint8_t node_adr;
	uint8_t packet_num;
};

struct lora_driver {
	char *name;
	int major;
	int minor_start;
	int num; // Max number of minor
	struct cdev lora_cdev;
	struct class *lora_class;
	struct module *owner;
};

#define MAX_PACKETLENGTH		123
#define LORA_OFFSET_PAYLOADLENGTH	5

struct lora_packet {
	uint8_t dst;
	uint8_t src;
	uint8_t packet_num;
	uint8_t len;
	uint8_t data[MAX_PACKETLENGTH];
	uint8_t retry;
};

#endif
