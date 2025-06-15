//
// Created by starry on 25-6-4.
//
/*
 * Copyright (c) 2023 HPMicro
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
#include "hpm_panel.h"
#include "hpm_pixelmux_drv.h"
#include "hpm_mipi_dsi_drv.h"
#include "hpm_mipi_dsi_phy_drv.h"
#include <stdio.h>

typedef struct c2_mipi_cmd_list {
    uint16_t len;
    uint8_t cmd[2];
} c2_mipi_cmd_list_t;

static const c2_mipi_cmd_list_t mipi_panel_cmd[] = {
        {2, {0XE0, 0X00}},
        {2, {0XE1, 0X93}},
        {2, {0XE2, 0X65}},
        {2, {0XE3, 0XF8}},
        {2, {0X80, 0X03}},
        {2, {0XE0, 0X01}},
        {2, {0X00, 0X00}},
        {2, {0X01, 0X77}},
        {2, {0X03, 0X00}},
        {2, {0X04, 0X65}},
        {2, {0X0C, 0X74}},
        {2, {0X17, 0X00}},
        {2, {0X18, 0XB7}},
        {2, {0X19, 0X00}},
        {2, {0X1A, 0X00}},
        {2, {0X1B, 0XB7}},
        {2, {0X1C, 0X00}},
        {2, {0X24, 0XFE}},
        {2, {0X37, 0X19}},
        {2, {0X38, 0X05}},
        {2, {0X39, 0X00}},
        {2, {0X3A, 0X01}},
        {2, {0X3B, 0X01}},
        {2, {0X3C, 0X70}},
        {2, {0X3D, 0XFF}},
        {2, {0X3E, 0XFF}},
        {2, {0X3F, 0XFF}},
        {2, {0X40, 0X06}},
        {2, {0X41, 0XA0}},
        {2, {0X43, 0X1E}},
        {2, {0X44, 0X0F}},
        {2, {0X45, 0X28}},
        {2, {0X4B, 0X04}},
        {2, {0X55, 0X02}},
        {2, {0X56, 0X01}},
        {2, {0X57, 0XA9}},
        {2, {0X58, 0X0A}},
        {2, {0X59, 0X0A}},
        {2, {0X5A, 0X37}},
        {2, {0X5B, 0X19}},
        {2, {0X5D, 0X78}},
        {2, {0X5E, 0X63}},
        {2, {0X5F, 0X54}},
        {2, {0X60, 0X49}},
        {2, {0X61, 0X45}},
        {2, {0X62, 0X38}},
        {2, {0X63, 0X3D}},
        {2, {0X64, 0X28}},
        {2, {0X65, 0X43}},
        {2, {0X66, 0X41}},
        {2, {0X67, 0X43}},
        {2, {0X68, 0X62}},
        {2, {0X69, 0X50}},
        {2, {0X6A, 0X57}},
        {2, {0X6B, 0X49}},
        {2, {0X6C, 0X44}},
        {2, {0X6D, 0X37}},
        {2, {0X6E, 0X23}},
        {2, {0X6F, 0X10}},
        {2, {0X70, 0X78}},
        {2, {0X71, 0X63}},
        {2, {0X72, 0X54}},
        {2, {0X73, 0X49}},
        {2, {0X74, 0X45}},
        {2, {0X75, 0X38}},
        {2, {0X76, 0X3D}},
        {2, {0X77, 0X28}},
        {2, {0X78, 0X43}},
        {2, {0X79, 0X41}},
        {2, {0X7A, 0X43}},
        {2, {0X7B, 0X62}},
        {2, {0X7C, 0X50}},
        {2, {0X7D, 0X57}},
        {2, {0X7E, 0X49}},
        {2, {0X7F, 0X44}},
        {2, {0X80, 0X37}},
        {2, {0X81, 0X23}},
        {2, {0X82, 0X10}},
        {2, {0XE0, 0X02}},
        {2, {0X00, 0X47}},
        {2, {0X01, 0X47}},
        {2, {0X02, 0X45}},
        {2, {0X03, 0X45}},
        {2, {0X04, 0X4B}},
        {2, {0X05, 0X4B}},
        {2, {0X06, 0X49}},
        {2, {0X07, 0X49}},
        {2, {0X08, 0X41}},
        {2, {0X09, 0X1F}},
        {2, {0X0A, 0X1F}},
        {2, {0X0B, 0X1F}},
        {2, {0X0C, 0X1F}},
        {2, {0X0D, 0X1F}},
        {2, {0X0E, 0X1F}},
        {2, {0X0F, 0X5F}},
        {2, {0X10, 0X5F}},
        {2, {0X11, 0X57}},
        {2, {0X12, 0X77}},
        {2, {0X13, 0X35}},
        {2, {0X14, 0X1F}},
        {2, {0X15, 0X1F}},
        {2, {0X16, 0X46}},
        {2, {0X17, 0X46}},
        {2, {0X18, 0X44}},
        {2, {0X19, 0X44}},
        {2, {0X1A, 0X4A}},
        {2, {0X1B, 0X4A}},
        {2, {0X1C, 0X48}},
        {2, {0X1D, 0X48}},
        {2, {0X1E, 0X40}},
        {2, {0X1F, 0X1F}},
        {2, {0X20, 0X1F}},
        {2, {0X21, 0X1F}},
        {2, {0X22, 0X1F}},
        {2, {0X23, 0X1F}},
        {2, {0X24, 0X1F}},
        {2, {0X25, 0X5F}},
        {2, {0X26, 0X5F}},
        {2, {0X27, 0X57}},
        {2, {0X28, 0X77}},
        {2, {0X29, 0X35}},
        {2, {0X2A, 0X1F}},
        {2, {0X2B, 0X1F}},
        {2, {0X58, 0X40}},
        {2, {0X59, 0X00}},
        {2, {0X5A, 0X00}},
        {2, {0X5B, 0X10}},
        {2, {0X5C, 0X06}},
        {2, {0X5D, 0X40}},
        {2, {0X5E, 0X01}},
        {2, {0X5F, 0X02}},
        {2, {0X60, 0X30}},
        {2, {0X61, 0X01}},
        {2, {0X62, 0X02}},
        {2, {0X63, 0X03}},
        {2, {0X64, 0X6B}},
        {2, {0X65, 0X05}},
        {2, {0X66, 0X0C}},
        {2, {0X67, 0X73}},
        {2, {0X68, 0X09}},
        {2, {0X69, 0X03}},
        {2, {0X6A, 0X56}},
        {2, {0X6B, 0X08}},
        {2, {0X6C, 0X00}},
        {2, {0X6D, 0X04}},
        {2, {0X6E, 0X04}},
        {2, {0X6F, 0X88}},
        {2, {0X70, 0X00}},
        {2, {0X71, 0X00}},
        {2, {0X72, 0X06}},
        {2, {0X73, 0X7B}},
        {2, {0X74, 0X00}},
        {2, {0X75, 0XF8}},
        {2, {0X76, 0X00}},
        {2, {0X77, 0XD5}},
        {2, {0X78, 0X2E}},
        {2, {0X79, 0X12}},
        {2, {0X7A, 0X03}},
        {2, {0X7B, 0X00}},
        {2, {0X7C, 0X00}},
        {2, {0X7D, 0X03}},
        {2, {0X7E, 0X7B}},
        {2, {0XE0, 0X04}},
        {2, {0X00, 0X0E}},
        {2, {0X02, 0XB3}},
        {2, {0X09, 0X60}},
        {2, {0X0E, 0X2A}},
        {2, {0X36, 0X59}},
        {2, {0XE0, 0X00}},
        {1, {0X11}},


};
static void delay_send(MIPI_DSI_Type *ptr, uint8_t channel)
{
    int ret;
    uint8_t cmd[2];
    hpm_panel_delay_ms(120);
    cmd[0]=0x29;
    ret = mipi_dsi_dcs_write_buffer(ptr, channel, cmd, 1);
    if (ret <= 0)
        printf("mipi_cmd.cmd: 0x%02X -- failed\n", cmd[0]);
    hpm_panel_delay_ms(50);
}

static void mipi_panel_init_cmd_send(hpm_panel_t *panel)
{
    int ret;
    uint8_t page_cmd[4] = {0xDA, 0x98, 0x81, 0x01};
    uint8_t panel_id[2] = {0x98, 0x81};
    uint8_t rdata;
    MIPI_DSI_Type *mipi_host = panel->hw_if.video.mipi.mipi_host_base;

//    mipi_dsi_dcs_write_buffer(mipi_host, 0, page_cmd, 1);

//    for (int i = 0; i < 2; i++) {
//        mipi_dsi_set_maximum_return_packet_size(mipi_host, 0, 1);
//        ret = mipi_dsi_dcs_read(mipi_host, 0, i, &rdata, 1);
//        if (ret <= 0 || rdata != panel_id[i]) {
//            printf("read id[%d]: 0x%02X -- failed\n", i, rdata);
//            while (1) {
//            }
//        } else {
//            printf("read id[%d]: 0x%02X -- ok\n", i, rdata);
//        }
//        hpm_panel_delay_ms(10);
//    }

    int mipi_cmd_num = sizeof(mipi_panel_cmd) / sizeof(mipi_panel_cmd[0]);
    for (int i = 0; i < mipi_cmd_num; i++) {
        ret = mipi_dsi_dcs_write_buffer(mipi_host, 0, mipi_panel_cmd[i].cmd, mipi_panel_cmd[i].len);
        if (ret <= 0)
            printf("mipi_cmd[%d].cmd: 0x%02X -- failed\n", i, mipi_panel_cmd[i].cmd[0]);
    }

    hpm_panel_delay_ms(10);
    delay_send(mipi_host, 0);

}

static void mipi_panel_host_init(hpm_panel_t *panel)
{
    MIPI_DSI_Type *mipi_host = panel->hw_if.video.mipi.mipi_host_base;
    const hpm_panel_timing_t *timing = &panel->timing;
    mipi_dsi_config_t mipi_cfg;
    mipi_dsi_get_defconfig_on_video(&mipi_cfg);

    mipi_cfg.channel = 0;
    mipi_cfg.lanes = 4;
    mipi_cfg.video_para.pixel_clock_khz = panel->hw_if.lcdc_pixel_clk_khz;
    mipi_cfg.video_para.hactive = timing->hactive;
    mipi_cfg.video_para.hsync_len = timing->hsync_len;
    mipi_cfg.video_para.hback_porch = timing->hback_porch;
    mipi_cfg.video_para.hfront_porch = timing->hfront_porch;
    mipi_cfg.video_para.vsync_len = timing->vsync_len;
    mipi_cfg.video_para.vactive = timing->vactive;
    mipi_cfg.video_para.vback_porch = timing->vback_porch;
    mipi_cfg.video_para.vfront_porch = timing->vfront_porch;

    mipi_dsi_init(mipi_host, &mipi_cfg);
}

static void mipi_panel_phy_init(hpm_panel_t *panel)
{
    MIPI_DSI_Type *mipi_host = panel->hw_if.video.mipi.mipi_host_base;
    MIPI_DSI_PHY_Type *mipi_phy = panel->hw_if.video.mipi.mipi_phy_base;

    mipi_dsi_phy_config_t mipi_phy_cfg = {
            .lanes = 4,
            .lane_mbps = 500
    };
    mipi_dsi_phy_powerdown(mipi_host);
    mipi_dsi_phy_init(mipi_phy, &mipi_phy_cfg);
    mipi_dsi_phy_poweron(mipi_host);
}

static void reset(hpm_panel_t *panel)
{
    if (!panel->hw_if.set_reset_pin_level)
        return;

    panel->hw_if.set_reset_pin_level(0);
    hpm_panel_delay_ms(20);

    panel->hw_if.set_reset_pin_level(1);
    hpm_panel_delay_ms(15);
}

static void init(hpm_panel_t *panel)
{
    if (panel->hw_if.set_video_router)
        panel->hw_if.set_video_router();

    mipi_panel_host_init(panel);
    mipi_panel_phy_init(panel);
    mipi_panel_init_cmd_send(panel);
}

static void power_on(hpm_panel_t *panel)
{
    MIPI_DSI_Type *mipi_host = panel->hw_if.video.mipi.mipi_host_base;

    if (panel->state.power_state != HPM_PANEL_STATE_POWER_ON) {
        mipi_dsi_video_mode_hs_transfer_enable(mipi_host);

        if (panel->hw_if.set_backlight) {
            if (panel->state.backlight_percent == 0)
                panel->state.backlight_percent = 100;
            panel->hw_if.set_backlight(panel->state.backlight_percent);
        }
        panel->state.power_state = HPM_PANEL_STATE_POWER_ON;
    }
}

static void power_off(hpm_panel_t *panel)
{
    MIPI_DSI_Type *mipi_host = panel->hw_if.video.mipi.mipi_host_base;

    if (panel->state.power_state != HPM_PANEL_STATE_POWER_OFF) {
        if (panel->hw_if.set_backlight)
            panel->hw_if.set_backlight(0);
        mipi_dsi_video_mode_hs_transfer_disable(mipi_host);
        panel->state.power_state = HPM_PANEL_STATE_POWER_OFF;
    }
}



hpm_panel_t panel_jd9365da_31b = {
        .name = "jd9365da_31b",
        .if_type = HPM_PANEL_IF_TYPE_MIPI,
        .timing = {
                .pixel_clock_khz = 70000,
                .hactive = 800,
                .hfront_porch = 40,
                .hback_porch = 20,
                .hsync_len = 20,

                .vactive = 1280,
                .vfront_porch = 30,
                .vback_porch = 12,
                .vsync_len = 4,
        },
        .funcs = {
                .reset = reset,
                .init = init,
                .power_on = power_on,
                .power_off = power_off,
        },
};