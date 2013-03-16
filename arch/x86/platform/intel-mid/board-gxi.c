/*
 * board-gxi.c: ZTE mfld_pr2 Medfield-based devices
 *
 * Copyright 2012 Motorola Mobility, Inc.
 * Copyright 2013 kamarush@XDA <kamarush42251@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; version 2
 * of the License.
 */

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/sfi.h>
#include <linux/lnw_gpio.h>
#include <linux/i2c.h>
#include <linux/input.h>
#include <linux/gpio.h>
#include <linux/gpio_keys.h>
#include <linux/platform_device.h>
#include <linux/ipc_device.h>
#include <linux/i2c-gpio.h>
#include <linux/ctype.h>

#include <asm/intel-mid.h>
#include <asm/intel_mid_pwm.h>

#include "board-gxi.h"

/*
 * IPC devices
 */

#include "device_libs/platform_ipc.h"
#include "device_libs/platform_pmic_gpio.h"
#include "device_libs/platform_pmic_audio.h"
#include "device_libs/platform_msic_adc.h"
#include "device_libs/platform_msic_battery.h"
#include "device_libs/platform_msic_gpio.h"
#include "device_libs/platform_msic_audio.h"
#include "device_libs/platform_msic_power_btn.h"
#include "device_libs/platform_msic_ocd.h"
#include "device_libs/platform_msic_chrg_led.h"

/*
 * I2C devices
 */

#include "device_libs/platform_max7315.h"
#include "device_libs/platform_tca6416.h"
#include "device_libs/platform_mpu3050.h"
#include "device_libs/platform_lsm303.h"
#include "device_libs/platform_emc1403.h"
#include "device_libs/platform_lis331.h"
#include "device_libs/platform_pn544.h"
#include "device_libs/platform_cyttsp.h"
#include "device_libs/platform_max17042.h"
#include "device_libs/platform_camera.h"
#include "device_libs/platform_mt9e013.h"
#include "device_libs/platform_mt9m114.h"
#include "device_libs/platform_a1026.h"
#include "device_libs/platform_lis3dh.h"
#include "device_libs/platform_ms5607.h"
#include "device_libs/platform_mpu3050.h"
#include "device_libs/platform_ltr502als.h"
#include "device_libs/platform_hmc5883.h"
#include "device_libs/platform_apds990x.h"
#include "device_libs/platform_lm2755.h"
#include "device_libs/platform_lm3554.h"
#include "device_libs/platform_mxt224.h"
#include "device_libs/platform_s3202.h"

/*
 * SPI devices
 */

#include "device_libs/platform_max3111.h"
#include "device_libs/platform_ektf2136.h"

/*
 * HSI devices
 */

#include "device_libs/platform_hsi_modem.h"

/*
 * WIFI devices
 */

/* need here bcmdhd */

/*
 * Bluetooth devices
 */

#include "device_libs/platform_btwilink.h"

/*
 * Miscellaneous devices
 */

#include "device_libs/platform_gxi-sensors.h"

u32 system_rev;

static void __init *no_platform_data(void *info)
{
	return NULL;
}

const struct intel_v4l2_subdev_id v4l2_ids[] = {
	{"mt9e013", RAW_CAMERA, ATOMISP_CAMERA_PORT_PRIMARY},
	{"lc898211", CAMERA_MOTOR, ATOMISP_CAMERA_PORT_PRIMARY},
	{"ov7736", SOC_CAMERA, ATOMISP_CAMERA_PORT_SECONDARY},
	{"lm3556", LED_FLASH, -1},
	{},
};

const struct devs_id __initconst device_ids[] = {
	{"pmic_gpio", SFI_DEV_TYPE_SPI, 1, &pmic_gpio_platform_data, NULL},
	{"pmic_gpio", SFI_DEV_TYPE_IPC, 1, &pmic_gpio_platform_data,
					&ipc_device_handler},
	{"spi_max3111", SFI_DEV_TYPE_SPI, 0, &max3111_platform_data, NULL},
	{"pn544", SFI_DEV_TYPE_I2C, 0, &pn544_platform_data, NULL},
	{"mpu3050", SFI_DEV_TYPE_I2C, 1, &mpu3050_platform_data, NULL},
	{"msic_adc", SFI_DEV_TYPE_IPC, 1, &msic_adc_platform_data,
					&ipc_device_handler},
	{"max17050", SFI_DEV_TYPE_I2C, 1, &max17042_platform_data, NULL},
	{"max17042", SFI_DEV_TYPE_I2C, 1, &max17042_platform_data, NULL},
	{"hsi_ifx_modem", SFI_DEV_TYPE_HSI, 0, &hsi_modem_platform_data, NULL},
	/* MSIC subdevices */
	{"msic_battery", SFI_DEV_TYPE_IPC, 1, &msic_battery_platform_data,
					&ipc_device_handler},
	{"msic_gpio", SFI_DEV_TYPE_IPC, 1, &msic_gpio_platform_data,
					&ipc_device_handler},
	{"msic_audio", SFI_DEV_TYPE_IPC, 1, &msic_audio_platform_data,
					&ipc_device_handler},
	{"msic_power_btn", SFI_DEV_TYPE_IPC, 1, &msic_power_btn_platform_data,
					&ipc_device_handler},
	{"msic_ocd", SFI_DEV_TYPE_IPC, 1, &msic_ocd_platform_data,
					&ipc_device_handler},
	{"msic_chrg_led", SFI_DEV_TYPE_IPC, 1, NULL, &ipc_device_handler},

	/*
	 * I2C devices for camera image subsystem which will not be load into
	 * I2C core while initialize
	 */
	{"mt9e013", SFI_DEV_TYPE_I2C, 0, &mt9e013_platform_data,
					&intel_ignore_i2c_device_register},
	{"mxt224", SFI_DEV_TYPE_I2C, 0, &mxt224_platform_data, NULL},
	{"synaptics_3202", SFI_DEV_TYPE_I2C, 0, &s3202_platform_data},

	{"audience_es305", SFI_DEV_TYPE_I2C, 0, &audience_platform_data,
						NULL},
	{"accel", SFI_DEV_TYPE_I2C, 0, &lis3dh_platform_data, NULL},
	{"compass", SFI_DEV_TYPE_I2C, 0, &hmc5883_platform_data, NULL},
	{"gyro", SFI_DEV_TYPE_I2C, 0, &gyro_platform_data, NULL},
	{"baro", SFI_DEV_TYPE_I2C, 0, &ms5607_platform_data, NULL},
	{"als", SFI_DEV_TYPE_I2C, 0, &ltr502als_platform_data, NULL},
	{"lm3556", SFI_DEV_TYPE_I2C, 0, &no_platform_data,
					&intel_ignore_i2c_device_register},
	{"lm2755", SFI_DEV_TYPE_I2C, 0, &lm2755_led_platform_data, NULL},
	{},
};

#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
static int board_id_proc_show(struct seq_file *m, void *v)
{
	char *bid;

	switch (board_id) {
	case MFLD_BID_CDK:
		bid = "cdk";        break;
	case MFLD_BID_AAVA:
		bid = "aava";       break;
	case MFLD_BID_PR2_PROTO:
	case MFLD_BID_PR2_PNP:
		bid = "pr2_proto";  break;
	case MFLD_BID_PR2_VOLUME:
		bid = "pr2_volume"; break;
	case MFLD_BID_PR3:
	case MFLD_BID_PR3_PNP:
		bid = "pr3";        break;
	default:
		bid = "unknown";    break;
	}
	seq_printf(m, "boardid=%s\n", bid);

	return 0;
}

static int board_id_proc_open(struct inode *inode, struct file *file)
{
	return single_open(file, board_id_proc_show, NULL);
}

static const struct file_operations board_id_proc_fops = {
	.open		= board_id_proc_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

u32 mfld_board_id(void)
{
	return board_id;
}
EXPORT_SYMBOL_GPL(mfld_board_id);

int __init board_proc_init(void)
{
	proc_create("boardid", 0, NULL, &board_id_proc_fops);
	return 0;
}

early_initcall(board_proc_init);

#define FUEL_GAUGE_ALERT_PIN 51  /* GP_AON_051 aka "max17042" */
#define CAMERA_FLASH_PIN 50  /* GP_AON_050 */

static void __init gxi_tcmd_export_gpio(void)
{
	int rc;

	rc = gpio_request(FUEL_GAUGE_ALERT_PIN, "Fuel Gauge Alert");
	if (!rc) {
		gpio_direction_input(FUEL_GAUGE_ALERT_PIN);
		rc = gpio_export(FUEL_GAUGE_ALERT_PIN, 0);
		if (rc) {
			pr_err("%s: Fuel Gauge Alert gpio export failure\n",
					__func__);
			gpio_free(FUEL_GAUGE_ALERT_PIN);
		}
	}
	rc = gpio_request(CAMERA_FLASH_PIN, "Camera Flash");
	if (!rc) {
		gpio_direction_output(CAMERA_FLASH_PIN, 1);
		rc = gpio_export(CAMERA_FLASH_PIN, 0);
		if (rc) {
			pr_err("%s: Camera Flash gpio export failure\n",
					__func__);
			gpio_free(CAMERA_FLASH_PIN);
		}
	}

	return;
}

/* MSIC hardware details */

static int gxi_get_oemr(struct sfi_table_header *table)
{
	struct sfi_table_oemr *oemr;

	oemr = (struct sfi_table_oemr *) table;
	if (!oemr)
		return -EINVAL;

	/*
	 * If system_rev has previously been set (from the command-line,
	 * for example), save the low order word and use the product ID
	 * passed via the SFI.
	 */
	if (system_rev) {
		system_rev &= 0x0000FFFF;
		system_rev |= oemr->boardvev & 0xFFFF0000;
	} else
		system_rev = oemr->boardvev;

	return 0;
}

static int __init handle_hwrev_cmdline_param(char *s)
{
	int shifter = 8;
	u16 hwrev = 0;

	while (*s && shifter >= 0) {
		if (!hwrev) {
			switch (*s) {
			case 'S':
			case 's':
				hwrev = 0x1000;
				break;
			case 'M':
			case 'm':
				hwrev = 0x2000;
				break;
			case 'P':
			case 'p':
				hwrev = 0x8000;
				break;
			case 'D':
			case 'd':
				hwrev = 0x9000;
				break;
			}
		} else {
			switch (*s) {
			case 'A':
			case 'a':
			case 'B':
			case 'b':
			case 'C':
			case 'c':
			case 'D':
			case 'd':
			case 'E':
			case 'e':
			case 'F':
			case 'f':
				hwrev |= (toupper(*s) - 'A' + 0xA) << shifter;
				break;
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				hwrev |= (*s - '0') << shifter;
				break;
			}

			shifter -= 4;
		}

		s++;
	}

	if (hwrev) {
		system_rev &= 0xFFFF0000; /* preserve product ID */
		system_rev |= hwrev; /* replace board revision ID */
	}

	return 1;
}
__setup("androidboot.hwrev=", handle_hwrev_cmdline_param);

#define BOOT_MODE_MAX_LEN 64
static char boot_mode[BOOT_MODE_MAX_LEN + 1];
static int __init board_boot_mode_init(char *s)
{
	strncpy(boot_mode, s, BOOT_MODE_MAX_LEN);
	boot_mode[BOOT_MODE_MAX_LEN] = '\0';
	return 1;
}
__setup("androidboot.mode=", board_boot_mode_init);

int boot_mode_is_factory(void)
{
	return !strncmp(boot_mode, "factory", BOOT_MODE_MAX_LEN);
}
EXPORT_SYMBOL(boot_mode_is_factory);

static int __init gxi_platform_init(void)
{
	sfi_table_parse(SFI_SIG_OEMR, NULL, NULL, gxi_get_oemr);

	gxi_register_board_i2c_devs();
	gxi_sensors_init();
	gxi_tcmd_export_gpio();
	return 0;
}
device_initcall(gxi_platform_init);
