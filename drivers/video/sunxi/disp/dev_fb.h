/*
 * Copyright (C) 2007-2012 Allwinner Technology Co., Ltd.
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

#ifndef __DEV_FB_H__
#define __DEV_FB_H__

#define SUNXI_MAX_FB 2

typedef struct {
	struct device *dev;

	__u32 base_image0;
	__u32 base_image1;
	__u32 base_scaler0;
	__u32 base_scaler1;
	__u32 base_lcdc0;
	__u32 base_lcdc1;
	__u32 base_tvec0;
	__u32 base_tvec1;
	__u32 base_hdmi;
	__u32 base_ccmu;
	__u32 base_sdram;
	__u32 base_pioc;
	__u32 base_pwm;
	__u32 base_iep;
	__disp_init_t disp_init;

	__bool fb_enable[SUNXI_MAX_FB];
	__bool fb_registered[SUNXI_MAX_FB];
	__fb_mode_t fb_mode[SUNXI_MAX_FB];
	/*
	 * [fb_id][0]: screen0 layer handle;
	 * [fb_id][1]: screen1 layer handle
	 */
	__u32 layer_hdl[SUNXI_MAX_FB][2];
	struct fb_info *fbinfo[SUNXI_MAX_FB];
	__disp_fb_create_para_t fb_para[SUNXI_MAX_FB];
	wait_queue_head_t wait[SUNXI_MAX_FB];
	unsigned long wait_count[SUNXI_MAX_FB];
	__u32 pseudo_palette[SUNXI_MAX_FB][16];
#ifdef CONFIG_FB_SUNXI_UMP
	ump_dd_handle ump_wrapped_buffer[SUNXI_MAX_FB][3];
#endif
	/* screen_base address, allocated with disp_malloc() */
	void *malloc_screen_base[SUNXI_MAX_FB];

    struct list_head update_regs_list;
    struct sw_sync_timeline *timeline;
    int timeline_max;
    struct mutex update_regs_list_lock;
    spinlock_t update_reg_lock;
    __bool  b_no_output;
    struct work_struct commit_work;
    struct work_struct vsync_work[2];
    ktime_t vsync_timestamp[2];
    struct work_struct post2_cb_work;
    __u32 reg_active[2];
    struct mutex runtime_lock;
    int blank[2];
    __u32 cb_w_conut;
    __u32 cb_r_conut;
    __u32 cur_count;
    void (*cb_fn)(void *, int);
    void *cb_arg[10];
} fb_info_t;

typedef struct
{
    struct list_head    list;
    setup_dispc_data_t  hwc_data;
} dispc_data_list_t;

extern fb_info_t g_fbi;

#ifdef CONFIG_FB_SUNXI_UMP
extern int (*disp_get_ump_secure_id) (struct fb_info *info, fb_info_t *g_fbi,
				      unsigned long arg, int buf);
extern unsigned long (*disp_get_ump_phy_addr) (unsigned long ump_id);
#endif

__s32 Display_Fb_Request(__u32 fb_id, __disp_fb_create_para_t *fb_para);
__s32 Display_Fb_Release(__u32 fb_id);
__s32 Display_Fb_get_para(__u32 fb_id, __disp_fb_create_para_t *fb_para);

__s32 Display_get_disp_init_para(__disp_init_t *init_para);

__s32 Fb_Exit(void);

#endif /* __DEV_FB_H__ */
