#include <linux/init.h>
#include <linux/input.h>
#include <linux/keyreset.h>
#include <linux/platform_device.h>
#include <linux/mfd/intel_msic.h>

#include <asm/intel_scu_ipc.h>

static struct keyreset_platform_data gxi_reset_keys_pdata = {
	.delay = 7000,
	.keys_down = {
		KEY_POWER,
		KEY_VOLUMEUP,
		0
	},
};

struct platform_device gxi_keyreset_device = {
	.name	= KEYRESET_NAME,
	.dev	= {
		.platform_data = &gxi_reset_keys_pdata,
	},
};

static int __init gxi_keyreset_init(int mode)
{
	/* Disable power off on long press power button */
	intel_scu_ipc_iowrite8(INTEL_MSIC_PBCONFIG, 0x8);
	platform_device_register(&gxi_keyreset_device);
	return 0;
}
late_initcall(gxi_keyreset_init);
