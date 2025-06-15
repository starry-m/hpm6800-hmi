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

typedef struct c_mipi_cmd_list {
    uint16_t len;
    uint8_t cmd[50];
} c_mipi_cmd_list_t;

static const c_mipi_cmd_list_t mipi_panel_cmd[] = {
        {4, {0XDF, 0X95, 0X22, 0XB7}},
        {8, {0XB0, 0X01, 0X23, 0X06, 0X88, 0X65, 0X06, 0X00}},
        {3, {0XB2, 0X01, 0X1D}},
        {3, {0XB3, 0X01, 0X1D}},
        {7, {0XB7, 0X00, 0XE0, 0X00, 0X00, 0XE0, 0X00}},
        {4, {0XCF, 0X49, 0X24, 0X3F}},
        {5, {0XB9, 0X22, 0XFC, 0X33, 0X00}},
        {3, {0XC0, 0X04, 0X04}},
        {4, {0XC1, 0X00, 0X10, 0X00}},
        {23, {0XC3, 0X3C, 0X01, 0X01, 0X01, 0X01, 0X09, 0X1D, 0X20, 0X34, 0X37, 0X4B, 0X08, 0X03, 0X03, 0X03, 0X02, 0X01, 0X01, 0X01, 0X01, 0X01, 0X4F}},
        {6, {0XC4, 0X36, 0XC0, 0X56, 0X08, 0X07}},
        {9, {0XBB, 0X01, 0X01, 0X24, 0X6A, 0X0A, 0X46, 0X14, 0X05}},
        {48, {0XC8, 0X00, 0XFF, 0XF9, 0XF5, 0XE9, 0XDF, 0XCE, 0XD7, 0XC2, 0XBA, 0X9B, 0XA7, 0X9D, 0X92, 0X83, 0X65, 0X78, 0X64, 0X5B, 0X4A, 0X34, 0X26, 0X20, 0X1E, 0XFF, 0XF9, 0XF5, 0XE9, 0XDF, 0XCE, 0XD7, 0XC2, 0XBA, 0X9B, 0XA7, 0X9D, 0X92, 0X83, 0X65, 0X78, 0X64, 0X5B, 0X4A, 0X34, 0X26, 0X20, 0X1E}},
        {48, {0XC9, 0X00, 0XFF, 0XF9, 0XF5, 0XE9, 0XDF, 0XCE, 0XD7, 0XC2, 0XBA, 0X9B, 0XA7, 0X9D, 0X92, 0X83, 0X65, 0X78, 0X64, 0X5B, 0X4A, 0X34, 0X26, 0X20, 0X1E, 0XFF, 0XF9, 0XF5, 0XE9, 0XDF, 0XCE, 0XD7, 0XC2, 0XBA, 0X9B, 0XA7, 0X9D, 0X92, 0X83, 0X65, 0X78, 0X64, 0X5B, 0X4A, 0X34, 0X26, 0X20, 0X1E}},
        {48, {0XCA, 0X00, 0XFF, 0XF9, 0XF5, 0XE9, 0XDF, 0XCE, 0XD7, 0XC2, 0XBA, 0X9B, 0XA7, 0X9D, 0X92, 0X83, 0X65, 0X78, 0X64, 0X5B, 0X4A, 0X34, 0X26, 0X20, 0X1E, 0XFF, 0XF9, 0XF5, 0XE9, 0XDF, 0XCE, 0XD7, 0XC2, 0XBA, 0X9B, 0XA7, 0X9D, 0X92, 0X83, 0X65, 0X78, 0X64, 0X5B, 0X4A, 0X34, 0X26, 0X20, 0X1E}},
        {22, {0XD0, 0X00, 0X1F, 0X1E, 0X1F, 0X01, 0X05, 0X0B, 0X09, 0X07, 0X1F, 0X1F, 0X1F, 0X21, 0X22, 0X23, 0X1F, 0X1F, 0X38, 0X00, 0X38, 0X01}},
        {22, {0XD1, 0X00, 0X1F, 0X1E, 0X1F, 0X00, 0X04, 0X0A, 0X08, 0X06, 0X1F, 0X1F, 0X1F, 0X21, 0X22, 0X23, 0X1F, 0X1F, 0X38, 0X00, 0X38, 0X01}},
        {22, {0XD2, 0X00, 0X1F, 0X1E, 0X1F, 0X00, 0X06, 0X08, 0X0A, 0X04, 0X1F, 0X1F, 0X1F, 0X21, 0X22, 0X23, 0X1F, 0X1F, 0X38, 0X00, 0X38, 0X01}},
        {22, {0XD3, 0X00, 0X1F, 0X1E, 0X1F, 0X01, 0X07, 0X09, 0X0B, 0X05, 0X1F, 0X1F, 0X1F, 0X21, 0X22, 0X23, 0X1F, 0X1F, 0X38, 0X00, 0X38, 0X01}},
        {33, {0XD4, 0X01, 0X07, 0X80, 0X15, 0X05, 0X00, 0X01, 0X02, 0X04, 0X50, 0X37, 0X87, 0X30, 0X01, 0X02, 0X01, 0X49, 0X70, 0X06, 0X04, 0X4F, 0X06, 0X00, 0X01, 0X00, 0X00, 0X01, 0X00, 0X00, 0X06, 0X02, 0X02}},
        {34, {0XD5, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X80, 0X00, 0X00, 0X01, 0X49, 0X00, 0X07, 0X50, 0X00, 0X77, 0X99, 0X00, 0X49, 0XA0, 0X1F, 0X00, 0X00, 0X00, 0X06, 0X03, 0XC0, 0X00, 0X02, 0X06, 0X08, 0X08}},
        {3, {0XBE, 0X5A, 0X62}},
        {5, {0XDD, 0X2A, 0XA3, 0X2A, 0XE9}},
        {2, {0XDE, 0X02}},
        {9, {0XC2, 0X00, 0X20, 0X38, 0X38, 0X9F, 0X43, 0X98, 0X00}},
        {3, {0XC1, 0X04, 0X14}},
        {4, {0XCF, 0X00, 0X00, 0X04}},
        {2, {0XDE, 0X03}},
        {2, {0XC7, 0X50}},
        {2, {0XDE, 0X00}},
        {1,{0X35}},
        {1,{0X11}}


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
    hpm_panel_delay_ms(120);
    cmd[0]=0x35;
    cmd[1]=0x00;
    ret = mipi_dsi_dcs_write_buffer(ptr, channel, cmd, 2);
    if (ret <= 0)
        printf("mipi_cmd.cmd: 0x%02X  -- failed\n", cmd[0]);
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

hpm_panel_t panel_jd95522z_31b = {
    .name = "jd95522z_31b",
    .if_type = HPM_PANEL_IF_TYPE_MIPI,
    .timing = {
        .pixel_clock_khz = 71000,
        .hactive = 1080,
        .hfront_porch = 80,
        .hback_porch = 20,
        .hsync_len = 20,

        .vactive = 1920,
        .vfront_porch = 8,
        .vback_porch = 6,
        .vsync_len = 2,
    },
    .funcs = {
        .reset = reset,
        .init = init,
        .power_on = power_on,
        .power_off = power_off,
    },
};
