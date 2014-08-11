/*
 * Copyright (C) 2012 Henrik Nordstrom <henrik@henriknordstrom.net>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <linux/module.h>

#ifndef CONFIG_FB_SUNXI_UMP
#error This file should not be built without UMP enabled
#endif

#include <ump/ump_kernel_interface_ref_drv.h>
#include <ump_kernel_memory_backend.h>

#include "drv_disp_i.h"
#include "dev_disp.h"
#include "dev_fb.h"

static unsigned long _disp_get_ump_phy_addr(unsigned long ump_id)
{
	ump_dd_mem * mem = NULL;

	if (0 != ump_dd_descriptor_mapping_get((int)ump_id, (void**)&mem))
	{
		printk(KERN_ERR "%s: ump_descriptor_mapping_get failed ump_id = %lu\n", __func__, ump_id);
		return 0;
	}

	if (mem == NULL)
	{
		printk(KERN_ERR "%s: no description\n", __func__);
	}

	return (unsigned long) mem->block_array[0].addr;
}
		
static int _disp_get_ump_secure_id(struct fb_info *info, fb_info_t *g_fbi,
				   unsigned long arg, int buf)
{
	u32 __user *psecureid = (u32 __user *) arg;
	ump_secure_id secure_id;

	if (!g_fbi->ump_wrapped_buffer[info->node][buf]) {
		ump_dd_physical_block ump_memory_description;

		ump_memory_description.addr = info->fix.smem_start;
		ump_memory_description.size = info->fix.smem_len;
		g_fbi->ump_wrapped_buffer[info->node][buf] =
			ump_dd_handle_create_from_phys_blocks
			(&ump_memory_description, 1);
	}
	secure_id = ump_dd_secure_id_get(g_fbi->
					 ump_wrapped_buffer[info->node][buf]);
	return put_user((unsigned int)secure_id, psecureid);
}

static int __init disp_ump_module_init(void)
{
	int ret = 0;

	disp_get_ump_secure_id = _disp_get_ump_secure_id;
	disp_get_ump_phy_addr = _disp_get_ump_phy_addr;

	return ret;
}

static void __exit disp_ump_module_exit(void)
{
	disp_get_ump_secure_id = NULL;
	disp_get_ump_phy_addr = NULL;
}

module_init(disp_ump_module_init);
module_exit(disp_ump_module_exit);

MODULE_AUTHOR("Henrik Nordstrom <henrik@henriknordstrom.net>");
MODULE_DESCRIPTION("sunxi display driver MALI UMP module glue");
MODULE_LICENSE("GPL");
