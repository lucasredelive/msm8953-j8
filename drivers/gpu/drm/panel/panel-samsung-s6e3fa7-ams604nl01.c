// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2013-2020, The Linux Foundation. All rights reserved.
// Generated with linux-mdss-dsi-panel-driver-generator from vendor device tree:

#include <linux/backlight.h>
#include <linux/delay.h>
#include <linux/gpio/consumer.h>
#include <linux/module.h>
#include <linux/of.h>

#include <video/mipi_display.h>

#include <drm/drm_mipi_dsi.h>
#include <drm/drm_modes.h>
#include <drm/drm_panel.h>

#include "panel-samsung-s6e3fa7-ams604nl01-gamma.h"

struct s6e3fa7_ams604nl01 {
	struct drm_panel panel;
	struct mipi_dsi_device *dsi;
	struct backlight_device *backlight;
	struct gpio_desc *reset_gpio;
	struct s6e3fa7_gamma_ctx *gamma_ctx;
	bool prepared;
	int brightness;
	u8 elvss_params[2];
	u8 irc_params[2];
	u8 mtp_offsets[32];
};

static int s6e3fa7_ams604nl01_bl_update_status(struct backlight_device *bl);
static inline
struct s6e3fa7_ams604nl01 *to_s6e3fa7_ams604nl01(struct drm_panel *panel)
{
	return container_of(panel, struct s6e3fa7_ams604nl01, panel);
}

#define dsi_generic_write_seq(dsi, seq...) do {				\
		static const u8 d[] = { seq };				\
		int ret;						\
		ret = mipi_dsi_generic_write(dsi, d, ARRAY_SIZE(d));	\
		if (ret < 0)						\
			return ret;					\
	} while (0)

static const struct brightness_level {
	short candela;
	short gamma_idx;
	u8 aid1;
	u8 aid2;
	u8 elvss2;
	u8 elvss3;
	u8 vint2;
	u8 irc25_27;
	u8 irc28_30;
	u8 irc31_33;
} brightness_settings[] = {
	{   3,   1, 0x08,  0x93,  0xcc, 0x04,  0x1c,  0x00,  0x00,  0x00 },
	{   3,   1, 0x08,  0x93,  0xcc, 0x04,  0x1c,  0x00,  0x00,  0x00 },
	{   3,   1, 0x08,  0x91,  0xcc, 0x04,  0x1c,  0x00,  0x00,  0x00 },
	{   3,   1, 0x08,  0x8f,  0xcc, 0x04,  0x1c,  0x00,  0x00,  0x00 },
	{   3,   1, 0x08,  0x89,  0xcc, 0x04,  0x1c,  0x00,  0x00,  0x00 },
	{   3,   1, 0x08,  0x87,  0xcc, 0x04,  0x1c,  0x00,  0x00,  0x00 },
	{   3,   1, 0x08,  0x85,  0xcc, 0x04,  0x1c,  0x00,  0x00,  0x00 },
	{   3,   1, 0x08,  0x83,  0xcc, 0x04,  0x1c,  0x00,  0x00,  0x00 },
	{   4,   2, 0x08,  0x81,  0xcc, 0x04,  0x1c,  0x00,  0x00,  0x00 },
	{   4,   2, 0x08,  0x7f,  0xcc, 0x04,  0x1c,  0x00,  0x00,  0x00 },
	{   4,   2, 0x08,  0x7d,  0xcc, 0x04,  0x1c,  0x00,  0x00,  0x00 },
	{   4,   2, 0x08,  0x6e,  0xcc, 0x04,  0x1c,  0x00,  0x00,  0x00 },
	{   4,   2, 0x08,  0x6c,  0xcc, 0x04,  0x1c,  0x00,  0x00,  0x00 },
	{   4,   2, 0x08,  0x6a,  0xcc, 0x04,  0x1c,  0x00,  0x00,  0x00 },
	{   4,   2, 0x08,  0x68,  0xcc, 0x04,  0x1c,  0x00,  0x00,  0x00 },
	{   5,   3, 0x08,  0x66,  0xcc, 0x04,  0x1c,  0x00,  0x00,  0x00 },
	{   5,   3, 0x08,  0x64,  0xcc, 0x04,  0x1c,  0x00,  0x00,  0x00 },
	{   5,   3, 0x08,  0x62,  0xcc, 0x04,  0x1c,  0x00,  0x00,  0x00 },
	{   5,   3, 0x08,  0x5c,  0xcc, 0x04,  0x1c,  0x00,  0x00,  0x00 },
	{   5,   3, 0x08,  0x5a,  0xcc, 0x04,  0x1c,  0x00,  0x00,  0x00 },
	{   5,   3, 0x08,  0x58,  0xcc, 0x04,  0x1c,  0x00,  0x00,  0x00 },
	{   5,   3, 0x08,  0x56,  0xcc, 0x04,  0x1c,  0x00,  0x00,  0x00 },
	{   6,   4, 0x08,  0x54,  0xcc, 0x04,  0x1c,  0x00,  0x00,  0x00 },
	{   6,   4, 0x08,  0x52,  0xcc, 0x04,  0x1c,  0x00,  0x00,  0x00 },
	{   6,   4, 0x08,  0x50,  0xcc, 0x04,  0x1c,  0x00,  0x00,  0x00 },
	{   6,   4, 0x08,  0x4a,  0xcc, 0x04,  0x1c,  0x00,  0x00,  0x00 },
	{   6,   4, 0x08,  0x48,  0xcc, 0x04,  0x1c,  0x00,  0x00,  0x00 },
	{   6,   4, 0x08,  0x46,  0xcc, 0x04,  0x1c,  0x00,  0x00,  0x00 },
	{   6,   4, 0x08,  0x44,  0xcc, 0x04,  0x1c,  0x00,  0x00,  0x00 },
	{   7,   5, 0x08,  0x42,  0xcc, 0x04,  0x1c,  0x00,  0x00,  0x00 },
	{   7,   5, 0x08,  0x40,  0xcc, 0x04,  0x1c,  0x00,  0x00,  0x00 },
	{   7,   5, 0x08,  0x37,  0xcc, 0x04,  0x1c,  0x00,  0x00,  0x00 },
	{   7,   5, 0x08,  0x35,  0xcc, 0x04,  0x1c,  0x00,  0x00,  0x00 },
	{   7,   5, 0x08,  0x33,  0xcc, 0x04,  0x1c,  0x00,  0x00,  0x00 },
	{   7,   5, 0x08,  0x31,  0xcc, 0x04,  0x1c,  0x00,  0x00,  0x00 },
	{   8,   6, 0x08,  0x23,  0xcc, 0x05,  0x1c,  0x00,  0x00,  0x00 },
	{   8,   6, 0x08,  0x15,  0xcc, 0x05,  0x1c,  0x00,  0x00,  0x00 },
	{   9,   7, 0x08,  0x0b,  0xcc, 0x06,  0x1c,  0x00,  0x00,  0x00 },
	{   9,   7, 0x08,  0x01,  0xcc, 0x06,  0x1c,  0x00,  0x00,  0x00 },
	{  10,   8, 0x07,  0xf3,  0xcc, 0x07,  0x1d,  0x02,  0x01,  0x00 },
	{  10,   8, 0x07,  0xe4,  0xcc, 0x07,  0x1d,  0x02,  0x01,  0x00 },
	{  11,   9, 0x07,  0xda,  0xcc, 0x08,  0x1d,  0x02,  0x01,  0x00 },
	{  11,   9, 0x07,  0xd0,  0xcc, 0x08,  0x1d,  0x02,  0x01,  0x00 },
	{  12,  10, 0x07,  0xc2,  0xcc, 0x09,  0x1d,  0x02,  0x01,  0x01 },
	{  12,  10, 0x07,  0xb5,  0xcc, 0x09,  0x1d,  0x02,  0x01,  0x01 },
	{  13,  11, 0x07,  0xa6,  0xcc, 0x0a,  0x1d,  0x02,  0x01,  0x01 },
	{  13,  11, 0x07,  0x97,  0xcc, 0x0a,  0x1d,  0x02,  0x01,  0x01 },
	{  14,  12, 0x07,  0x8c,  0xcc, 0x0b,  0x1d,  0x03,  0x01,  0x00 },
	{  14,  12, 0x07,  0x81,  0xcc, 0x0b,  0x1d,  0x03,  0x01,  0x00 },
	{  15,  13, 0x07,  0x72,  0xcc, 0x0c,  0x1d,  0x03,  0x01,  0x01 },
	{  15,  13, 0x07,  0x64,  0xcc, 0x0c,  0x1d,  0x03,  0x01,  0x01 },
	{  16,  14, 0x07,  0x56,  0xcc, 0x0d,  0x1d,  0x03,  0x01,  0x01 },
	{  16,  14, 0x07,  0x47,  0xcc, 0x0d,  0x1d,  0x03,  0x01,  0x01 },
	{  17,  15, 0x07,  0x3d,  0xcc, 0x0d,  0x1d,  0x03,  0x01,  0x01 },
	{  17,  15, 0x07,  0x33,  0xcc, 0x0d,  0x1d,  0x03,  0x01,  0x01 },
	{  19,  16, 0x07,  0x1a,  0xcc, 0x0d,  0x1d,  0x03,  0x02,  0x01 },
	{  19,  16, 0x07,  0x02,  0xcc, 0x0d,  0x1d,  0x04,  0x01,  0x01 },
	{  20,  17, 0x06,  0xf1,  0xcc, 0x0d,  0x1e,  0x04,  0x01,  0x01 },
	{  21,  18, 0x06,  0xe2,  0xcc, 0x0d,  0x1e,  0x04,  0x01,  0x02 },
	{  21,  18, 0x06,  0xd4,  0xcc, 0x0d,  0x1e,  0x04,  0x01,  0x02 },
	{  22,  19, 0x06,  0xca,  0xcc, 0x0d,  0x1e,  0x04,  0x02,  0x01 },
	{  22,  19, 0x06,  0xc0,  0xcc, 0x0d,  0x1e,  0x04,  0x02,  0x01 },
	{  24,  20, 0x06,  0xa8,  0xcc, 0x0d,  0x1e,  0x04,  0x02,  0x01 },
	{  24,  20, 0x06,  0x90,  0xcc, 0x0d,  0x1e,  0x05,  0x01,  0x02 },
	{  25,  21, 0x06,  0x83,  0xcc, 0x0d,  0x1e,  0x05,  0x01,  0x02 },
	{  25,  21, 0x06,  0x75,  0xcc, 0x0d,  0x1e,  0x05,  0x01,  0x02 },
	{  27,  22, 0x06,  0x5d,  0xcc, 0x0d,  0x1e,  0x05,  0x02,  0x01 },
	{  27,  22, 0x06,  0x45,  0xcc, 0x0d,  0x1e,  0x05,  0x02,  0x02 },
	{  29,  23, 0x06,  0x2d,  0xcc, 0x0d,  0x1e,  0x05,  0x02,  0x02 },
	{  29,  23, 0x06,  0x14,  0xcc, 0x0d,  0x1e,  0x06,  0x01,  0x02 },
	{  30,  24, 0x05,  0xff,  0xcc, 0x0d,  0x1e,  0x06,  0x02,  0x02 },
	{  32,  25, 0x05,  0xe4,  0xcc, 0x0d,  0x1e,  0x06,  0x02,  0x02 },
	{  32,  25, 0x05,  0xc8,  0xcc, 0x0d,  0x1e,  0x06,  0x02,  0x02 },
	{  34,  26, 0x05,  0xaf,  0xcc, 0x0d,  0x1e,  0x06,  0x02,  0x02 },
	{  34,  26, 0x05,  0x96,  0xcc, 0x0d,  0x1e,  0x06,  0x03,  0x02 },
	{  37,  27, 0x05,  0x74,  0xcc, 0x0d,  0x1e,  0x07,  0x02,  0x02 },
	{  37,  27, 0x05,  0x52,  0xcc, 0x0d,  0x1e,  0x07,  0x02,  0x03 },
	{  39,  28, 0x05,  0x3a,  0xcc, 0x0d,  0x1e,  0x07,  0x03,  0x02 },
	{  39,  28, 0x05,  0x22,  0xcc, 0x0d,  0x1e,  0x07,  0x03,  0x02 },
	{  41,  29, 0x05,  0x09,  0xdc, 0x0d,  0x1e,  0x08,  0x02,  0x03 },
	{  41,  29, 0x04,  0xf0,  0xdc, 0x0d,  0x1e,  0x08,  0x02,  0x03 },
	{  44,  30, 0x04,  0xca,  0xdc, 0x0d,  0x1e,  0x08,  0x03,  0x03 },
	{  44,  30, 0x04,  0xa5,  0xdc, 0x0d,  0x1e,  0x08,  0x03,  0x03 },
	{  47,  31, 0x04,  0x7d,  0xdc, 0x0d,  0x1e,  0x09,  0x03,  0x03 },
	{  47,  31, 0x04,  0x55,  0xdc, 0x0d,  0x1e,  0x09,  0x03,  0x03 },
	{  50,  32, 0x04,  0x2e,  0xdc, 0x0d,  0x1e,  0x09,  0x03,  0x04 },
	{  50,  32, 0x04,  0x07,  0xdc, 0x0d,  0x1e,  0x0a,  0x03,  0x03 },
	{  53,  33, 0x03,  0xdf,  0xdc, 0x0d,  0x1e,  0x0a,  0x03,  0x03 },
	{  53,  33, 0x03,  0xb7,  0xdc, 0x0d,  0x1e,  0x0a,  0x03,  0x04 },
	{  56,  34, 0x03,  0x95,  0xdc, 0x0d,  0x1e,  0x0a,  0x04,  0x03 },
	{  56,  34, 0x03,  0x72,  0xdc, 0x0d,  0x1e,  0x0b,  0x03,  0x04 },
	{  60,  35, 0x03,  0x3c,  0xdc, 0x0d,  0x1e,  0x0b,  0x04,  0x03 },
	{  60,  35, 0x03,  0x06,  0xdc, 0x0d,  0x1e,  0x0b,  0x04,  0x04 },
	{  64,  36, 0x02,  0xcf,  0xdc, 0x0d,  0x1e,  0x0c,  0x04,  0x04 },
	{  64,  36, 0x02,  0x97,  0xdc, 0x0d,  0x1e,  0x0c,  0x04,  0x04 },
	{  68,  37, 0x03,  0x04,  0xdc, 0x0d,  0x1e,  0x0d,  0x04,  0x04 },
	{  68,  37, 0x02,  0xd7,  0xdc, 0x0d,  0x1e,  0x0d,  0x04,  0x05 },
	{  72,  38, 0x03,  0x01,  0xdc, 0x0d,  0x1e,  0x0d,  0x05,  0x04 },
	{  72,  38, 0x02,  0xd7,  0xdc, 0x0d,  0x1e,  0x0e,  0x04,  0x05 },
	{  77,  39, 0x03,  0x08,  0xdc, 0x0d,  0x1e,  0x0e,  0x05,  0x05 },
	{  77,  39, 0x02,  0xd7,  0xdc, 0x0d,  0x1e,  0x0f,  0x04,  0x05 },
	{  82,  40, 0x03,  0x05,  0xdc, 0x0d,  0x1e,  0x0f,  0x05,  0x05 },
	{  82,  40, 0x02,  0xd7,  0xdc, 0x0d,  0x1e,  0x10,  0x05,  0x05 },
	{  87,  41, 0x03,  0x02,  0xdc, 0x0d,  0x1e,  0x10,  0x05,  0x06 },
	{  87,  41, 0x02,  0xd7,  0xdc, 0x0d,  0x1e,  0x11,  0x05,  0x06 },
	{  93,  42, 0x03,  0x08,  0xdc, 0x0d,  0x1e,  0x11,  0x06,  0x06 },
	{  93,  42, 0x02,  0xd7,  0xdc, 0x0d,  0x1e,  0x12,  0x05,  0x06 },
	{  98,  43, 0x02,  0xfe,  0xdc, 0x0d,  0x1e,  0x12,  0x06,  0x06 },
	{  98,  43, 0x02,  0xd7,  0xdc, 0x0d,  0x1e,  0x13,  0x06,  0x06 },
	{ 105,  44, 0x03,  0x09,  0xdc, 0x0d,  0x1e,  0x13,  0x07,  0x06 },
	{ 105,  44, 0x02,  0xd7,  0xdc, 0x0d,  0x1e,  0x14,  0x07,  0x06 },
	{ 111,  45, 0x03,  0x00,  0xdc, 0x0d,  0x1e,  0x15,  0x06,  0x07 },
	{ 111,  45, 0x02,  0xd7,  0xdc, 0x0d,  0x1e,  0x15,  0x07,  0x07 },
	{ 119,  46, 0x03,  0x0a,  0xdc, 0x0d,  0x1e,  0x16,  0x07,  0x07 },
	{ 119,  46, 0x02,  0xd7,  0xdc, 0x0d,  0x1e,  0x17,  0x07,  0x08 },
	{ 126,  47, 0x03,  0x01,  0xdc, 0x0d,  0x1e,  0x17,  0x08,  0x08 },
	{ 126,  47, 0x02,  0xd7,  0xdc, 0x0d,  0x1e,  0x18,  0x08,  0x08 },
	{ 134,  48, 0x03,  0x04,  0xdc, 0x0d,  0x1e,  0x19,  0x08,  0x08 },
	{ 134,  48, 0x02,  0xd7,  0xdc, 0x0d,  0x1e,  0x1a,  0x08,  0x08 },
	{ 143,  49, 0x03,  0x07,  0xdc, 0x0d,  0x1e,  0x1a,  0x09,  0x09 },
	{ 143,  49, 0x02,  0xd7,  0xdc, 0x0d,  0x1e,  0x1b,  0x09,  0x09 },
	{ 152,  50, 0x03,  0x04,  0xdc, 0x0d,  0x1e,  0x1c,  0x09,  0x0a },
	{ 152,  50, 0x02,  0xd7,  0xdc, 0x0d,  0x1e,  0x1d,  0x09,  0x0a },
	{ 162,  51, 0x03,  0x06,  0xdc, 0x0d,  0x1e,  0x1e,  0x0a,  0x0a },
	{ 162,  51, 0x02,  0xd7,  0xdc, 0x0d,  0x1e,  0x1f,  0x0a,  0x0a },
	{ 172,  52, 0x03,  0x03,  0xdc, 0x0d,  0x1e,  0x20,  0x0a,  0x0b },
	{ 172,  52, 0x02,  0xd7,  0xdc, 0x0d,  0x1e,  0x21,  0x0a,  0x0b },
	{ 183,  53, 0x03,  0x04,  0xdc, 0x0d,  0x1e,  0x22,  0x0b,  0x0b },
	{ 183,  53, 0x02,  0xd7,  0xdc, 0x0d,  0x1e,  0x23,  0x0b,  0x0c },
	{ 195,  54, 0x02,  0xc9,  0xdc, 0x0d,  0x1e,  0x23,  0x0c,  0x0c },
	{ 195,  54, 0x02,  0xba,  0xdc, 0x0d,  0x1e,  0x23,  0x0c,  0x0c },
	{ 195,  54, 0x02,  0xac,  0xdc, 0x0d,  0x1e,  0x24,  0x0b,  0x0d },
	{ 195,  54, 0x02,  0x9e,  0xdc, 0x0d,  0x1e,  0x24,  0x0c,  0x0c },
	{ 195,  54, 0x02,  0x90,  0xdc, 0x0d,  0x1e,  0x25,  0x0b,  0x0d },
	{ 195,  54, 0x02,  0x81,  0xdc, 0x0d,  0x1e,  0x25,  0x0c,  0x0c },
	{ 195,  54, 0x02,  0x73,  0xdc, 0x0d,  0x1e,  0x25,  0x0c,  0x0d },
	{ 207,  55, 0x02,  0x63,  0xdc, 0x0d,  0x1e,  0x26,  0x0c,  0x0c },
	{ 207,  55, 0x02,  0x54,  0xdc, 0x0d,  0x1e,  0x26,  0x0c,  0x0d },
	{ 207,  55, 0x02,  0x44,  0xdc, 0x0d,  0x1e,  0x26,  0x0c,  0x0d },
	{ 207,  55, 0x02,  0x34,  0xdc, 0x0d,  0x1e,  0x26,  0x0d,  0x0d },
	{ 207,  55, 0x02,  0x24,  0xdc, 0x0d,  0x1e,  0x27,  0x0c,  0x0e },
	{ 207,  55, 0x02,  0x15,  0xdc, 0x0d,  0x1e,  0x27,  0x0d,  0x0d },
	{ 207,  55, 0x02,  0x05,  0xdc, 0x0d,  0x1e,  0x27,  0x0d,  0x0e },
	{ 220,  56, 0x01,  0xf2,  0xdc, 0x0d,  0x1e,  0x28,  0x0d,  0x0d },
	{ 220,  56, 0x01,  0xe0,  0xdc, 0x0d,  0x1e,  0x28,  0x0d,  0x0e },
	{ 220,  56, 0x01,  0xcd,  0xdc, 0x0d,  0x1e,  0x29,  0x0d,  0x0d },
	{ 220,  56, 0x01,  0xba,  0xdc, 0x0d,  0x1e,  0x29,  0x0d,  0x0e },
	{ 220,  56, 0x01,  0xa7,  0xdc, 0x0d,  0x1e,  0x29,  0x0e,  0x0d },
	{ 220,  56, 0x01,  0x95,  0xdc, 0x0d,  0x1e,  0x2a,  0x0d,  0x0e },
	{ 220,  56, 0x01,  0x82,  0xdc, 0x0d,  0x1e,  0x2a,  0x0e,  0x0e },
	{ 234,  57, 0x01,  0x72,  0xdc, 0x0c,  0x1e,  0x2a,  0x0e,  0x0e },
	{ 234,  57, 0x01,  0x62,  0xdc, 0x0c,  0x1e,  0x2b,  0x0e,  0x0e },
	{ 234,  57, 0x01,  0x52,  0xdc, 0x0c,  0x1e,  0x2b,  0x0e,  0x0e },
	{ 234,  57, 0x01,  0x42,  0xdc, 0x0c,  0x1e,  0x2b,  0x0e,  0x0f },
	{ 234,  57, 0x01,  0x32,  0xdc, 0x0c,  0x1e,  0x2c,  0x0e,  0x0f },
	{ 234,  57, 0x01,  0x22,  0xdc, 0x0c,  0x1e,  0x2c,  0x0e,  0x0f },
	{ 234,  57, 0x01,  0x12,  0xdc, 0x0c,  0x1e,  0x2c,  0x0f,  0x0e },
	{ 234,  57, 0x01,  0x02,  0xdc, 0x0c,  0x1e,  0x2d,  0x0e,  0x0f },
	{ 249,  58, 0x01,  0x4a,  0xdc, 0x0c,  0x1e,  0x2d,  0x0f,  0x0f },
	{ 249,  58, 0x01,  0x3b,  0xdc, 0x0c,  0x1e,  0x2d,  0x0f,  0x0f },
	{ 249,  58, 0x01,  0x2c,  0xdc, 0x0c,  0x1e,  0x2e,  0x0f,  0x0f },
	{ 249,  58, 0x01,  0x1d,  0xdc, 0x0c,  0x1e,  0x2e,  0x0f,  0x10 },
	{ 249,  58, 0x01,  0x0e,  0xdc, 0x0c,  0x1e,  0x2e,  0x0f,  0x10 },
	{ 249,  58, 0x00,  0xfe,  0xdc, 0x0c,  0x1e,  0x2f,  0x0f,  0x10 },
	{ 249,  58, 0x00,  0xef,  0xdc, 0x0c,  0x1e,  0x2f,  0x0f,  0x10 },
	{ 249,  58, 0x00,  0xe0,  0xdc, 0x0c,  0x1e,  0x2f,  0x10,  0x10 },
	{ 265,  59, 0x01,  0x4c,  0xdc, 0x0b,  0x1e,  0x30,  0x0f,  0x10 },
	{ 265,  59, 0x01,  0x3f,  0xdc, 0x0b,  0x1e,  0x30,  0x10,  0x10 },
	{ 265,  59, 0x01,  0x31,  0xdc, 0x0b,  0x1e,  0x30,  0x10,  0x10 },
	{ 265,  59, 0x01,  0x24,  0xdc, 0x0b,  0x1e,  0x31,  0x10,  0x10 },
	{ 265,  59, 0x01,  0x16,  0xdc, 0x0b,  0x1e,  0x31,  0x10,  0x11 },
	{ 265,  59, 0x01,  0x09,  0xdc, 0x0b,  0x1e,  0x32,  0x10,  0x10 },
	{ 265,  59, 0x00,  0xfb,  0xdc, 0x0b,  0x1e,  0x32,  0x10,  0x11 },
	{ 265,  59, 0x00,  0xee,  0xdc, 0x0b,  0x1e,  0x32,  0x10,  0x11 },
	{ 265,  59, 0x00,  0xe0,  0xdc, 0x0b,  0x1e,  0x32,  0x11,  0x11 },
	{ 282,  60, 0x01,  0x4c,  0xdc, 0x0a,  0x1e,  0x33,  0x10,  0x12 },
	{ 282,  60, 0x01,  0x3f,  0xdc, 0x0a,  0x1e,  0x33,  0x11,  0x11 },
	{ 282,  60, 0x01,  0x31,  0xdc, 0x0a,  0x1e,  0x34,  0x10,  0x12 },
	{ 282,  60, 0x01,  0x24,  0xdc, 0x0a,  0x1e,  0x34,  0x11,  0x11 },
	{ 282,  60, 0x01,  0x16,  0xdc, 0x0a,  0x1e,  0x34,  0x11,  0x12 },
	{ 282,  60, 0x01,  0x09,  0xdc, 0x0a,  0x1e,  0x35,  0x11,  0x11 },
	{ 282,  60, 0x00,  0xfb,  0xdc, 0x0a,  0x1e,  0x35,  0x11,  0x12 },
	{ 282,  60, 0x00,  0xee,  0xdc, 0x0a,  0x1e,  0x35,  0x12,  0x12 },
	{ 282,  60, 0x00,  0xe0,  0xdc, 0x0a,  0x1e,  0x36,  0x11,  0x12 },
	{ 300,  61, 0x01,  0x4d,  0xdc, 0x09,  0x1e,  0x36,  0x12,  0x12 },
	{ 300,  61, 0x01,  0x41,  0xdc, 0x09,  0x1e,  0x36,  0x12,  0x13 },
	{ 300,  61, 0x01,  0x35,  0xdc, 0x09,  0x1e,  0x37,  0x11,  0x13 },
	{ 300,  61, 0x01,  0x29,  0xdc, 0x09,  0x1e,  0x37,  0x12,  0x13 },
	{ 300,  61, 0x01,  0x1c,  0xdc, 0x09,  0x1e,  0x37,  0x12,  0x13 },
	{ 300,  61, 0x01,  0x10,  0xdc, 0x09,  0x1e,  0x38,  0x12,  0x13 },
	{ 300,  61, 0x01,  0x04,  0xdc, 0x09,  0x1e,  0x38,  0x12,  0x13 },
	{ 300,  61, 0x00,  0xf8,  0xdc, 0x09,  0x1e,  0x38,  0x13,  0x13 },
	{ 300,  61, 0x00,  0xec,  0xdc, 0x09,  0x1e,  0x39,  0x12,  0x13 },
	{ 300,  61, 0x00,  0xe0,  0xdc, 0x09,  0x1e,  0x39,  0x13,  0x13 },
	{ 316,  62, 0x01,  0x39,  0xdc, 0x09,  0x1e,  0x3a,  0x12,  0x14 },
	{ 316,  62, 0x01,  0x2d,  0xdc, 0x09,  0x1e,  0x3a,  0x13,  0x13 },
	{ 316,  62, 0x01,  0x20,  0xdc, 0x09,  0x1e,  0x3a,  0x13,  0x14 },
	{ 316,  62, 0x01,  0x13,  0xdc, 0x09,  0x1e,  0x3b,  0x13,  0x14 },
	{ 316,  62, 0x01,  0x06,  0xdc, 0x09,  0x1e,  0x3b,  0x13,  0x14 },
	{ 316,  62, 0x00,  0xfa,  0xdc, 0x09,  0x1e,  0x3b,  0x14,  0x14 },
	{ 316,  62, 0x00,  0xed,  0xdc, 0x09,  0x1e,  0x3c,  0x13,  0x14 },
	{ 316,  62, 0x00,  0xe0,  0xdc, 0x09,  0x1e,  0x3c,  0x14,  0x14 },
	{ 333,  63, 0x01,  0x3b,  0xdc, 0x08,  0x1e,  0x3d,  0x13,  0x15 },
	{ 333,  63, 0x01,  0x30,  0xdc, 0x08,  0x1e,  0x3d,  0x14,  0x14 },
	{ 333,  63, 0x01,  0x25,  0xdc, 0x08,  0x1e,  0x3d,  0x14,  0x15 },
	{ 333,  63, 0x01,  0x19,  0xdc, 0x08,  0x1e,  0x3e,  0x14,  0x15 },
	{ 333,  63, 0x01,  0x0e,  0xdc, 0x08,  0x1e,  0x3e,  0x14,  0x15 },
	{ 333,  63, 0x01,  0x02,  0xdc, 0x08,  0x1e,  0x3e,  0x15,  0x15 },
	{ 333,  63, 0x00,  0xf7,  0xdc, 0x08,  0x1e,  0x3f,  0x14,  0x15 },
	{ 333,  63, 0x00,  0xeb,  0xdc, 0x08,  0x1e,  0x3f,  0x15,  0x15 },
	{ 333,  63, 0x00,  0xe0,  0xdc, 0x08,  0x1e,  0x3f,  0x15,  0x15 },
	{ 350,  64, 0x01,  0x37,  0xdc, 0x07,  0x1e,  0x40,  0x15,  0x15 },
	{ 350,  64, 0x01,  0x2c,  0xdc, 0x07,  0x1e,  0x40,  0x15,  0x16 },
	{ 350,  64, 0x01,  0x21,  0xdc, 0x07,  0x1e,  0x41,  0x15,  0x15 },
	{ 350,  64, 0x01,  0x16,  0xdc, 0x07,  0x1e,  0x41,  0x15,  0x16 },
	{ 350,  64, 0x01,  0x0c,  0xdc, 0x07,  0x1e,  0x41,  0x15,  0x16 },
	{ 350,  64, 0x01,  0x01,  0xdc, 0x07,  0x1e,  0x42,  0x15,  0x16 },
	{ 350,  64, 0x00,  0xf6,  0xdc, 0x07,  0x1e,  0x42,  0x15,  0x17 },
	{ 350,  64, 0x00,  0xeb,  0xdc, 0x07,  0x1e,  0x42,  0x16,  0x16 },
	{ 350,  64, 0x00,  0xe0,  0xdc, 0x07,  0x1e,  0x43,  0x15,  0x17 },
	{ 357,  65, 0x00,  0xfe,  0xdc, 0x06,  0x1e,  0x43,  0x16,  0x16 },
	{ 357,  65, 0x00,  0xf4,  0xdc, 0x06,  0x1e,  0x43,  0x16,  0x17 },
	{ 357,  65, 0x00,  0xea,  0xdc, 0x06,  0x1e,  0x44,  0x16,  0x16 },
	{ 357,  65, 0x00,  0xe0,  0xdc, 0x06,  0x1e,  0x44,  0x16,  0x17 },
	{ 365,  66, 0x01,  0x01,  0xdc, 0x06,  0x1e,  0x44,  0x17,  0x17 },
	{ 365,  66, 0x00,  0xf6,  0xdc, 0x06,  0x1e,  0x45,  0x16,  0x17 },
	{ 365,  66, 0x00,  0xeb,  0xdc, 0x06,  0x1e,  0x45,  0x17,  0x17 },
	{ 365,  66, 0x00,  0xe0,  0xdc, 0x06,  0x1e,  0x46,  0x16,  0x18 },
	{ 372,  67, 0x00,  0xd0,  0xdc, 0x06,  0x1e,  0x46,  0x17,  0x17 },
	{ 372,  67, 0x00,  0xc1,  0xdc, 0x06,  0x1e,  0x46,  0x17,  0x18 },
	{ 372,  67, 0x00,  0xb1,  0xdc, 0x06,  0x1e,  0x47,  0x17,  0x18 },
	{ 380,  68, 0x00,  0xa6,  0xdc, 0x05,  0x1e,  0x47,  0x17,  0x18 },
	{ 380,  68, 0x00,  0x9a,  0xdc, 0x05,  0x1e,  0x48,  0x17,  0x18 },
	{ 380,  68, 0x00,  0x8e,  0xdc, 0x05,  0x1e,  0x48,  0x17,  0x19 },
	{ 380,  68, 0x00,  0x83,  0xdc, 0x05,  0x1e,  0x48,  0x18,  0x18 },
	{ 387,  69, 0x00,  0x77,  0xdc, 0x05,  0x1e,  0x49,  0x17,  0x19 },
	{ 387,  69, 0x00,  0x6b,  0xdc, 0x05,  0x1e,  0x49,  0x18,  0x19 },
	{ 387,  69, 0x00,  0x5f,  0xdc, 0x05,  0x1e,  0x49,  0x18,  0x19 },
	{ 387,  69, 0x00,  0x53,  0xdc, 0x05,  0x1e,  0x4a,  0x18,  0x19 },
	{ 395,  70, 0x00,  0x44,  0xdc, 0x05,  0x1e,  0x4a,  0x18,  0x19 },
	{ 395,  70, 0x00,  0x34,  0xdc, 0x05,  0x1e,  0x4a,  0x19,  0x19 },
	{ 395,  70, 0x00,  0x25,  0xdc, 0x05,  0x1e,  0x4b,  0x18,  0x19 },
	{ 395,  70, 0x00,  0x16,  0xdc, 0x05,  0x1e,  0x4b,  0x19,  0x19 },
	{ 403,  71, 0x00,  0x31,  0xdc, 0x05,  0x1e,  0x4c,  0x18,  0x1a },
	{ 403,  71, 0x00,  0x26,  0xdc, 0x05,  0x1e,  0x4c,  0x19,  0x19 },
	{ 403,  71, 0x00,  0x1b,  0xdc, 0x05,  0x1e,  0x4c,  0x19,  0x1a },
	{ 403,  71, 0x00,  0x10,  0xdc, 0x05,  0x1e,  0x4d,  0x19,  0x1a },
	{ 412,  72, 0x00,  0x34,  0xdc, 0x04,  0x1e,  0x4d,  0x19,  0x1a },
	{ 412,  72, 0x00,  0x28,  0xdc, 0x04,  0x1e,  0x4e,  0x19,  0x1a },
	{ 412,  72, 0x00,  0x1c,  0xdc, 0x04,  0x1e,  0x4e,  0x19,  0x1b },
	{ 412,  72, 0x00,  0x10,  0xdc, 0x04,  0x1e,  0x4e,  0x1a,  0x1a },
	{ 420,  73, 0x00,  0x30,  0xdc, 0x04,  0x1e,  0x4f,  0x19,  0x1b },
	{ 420,  73, 0x00,  0x25,  0xdc, 0x04,  0x1e,  0x4f,  0x1a,  0x1a },
	{ 420,  73, 0x00,  0x1b,  0xdc, 0x04,  0x1e,  0x4f,  0x1a,  0x1b },
	{ 420,  73, 0x00,  0x10,  0xdc, 0x04,  0x1e,  0x50,  0x1a,  0x1b },
};

#define dsi_dcs_write_seq(dsi, seq...) do {				\
		static const u8 d[] = { seq };				\
		int ret;						\
		ret = mipi_dsi_dcs_write_buffer(dsi, d, ARRAY_SIZE(d));	\
		if (ret < 0)						\
			return ret;					\
	} while (0)

#define LEVEL1_ON	0xf0, 0x5a, 0x5a
#define LEVEL1_OFF	0xf0, 0xa5, 0xa5

static void s6e3fa7_ams604nl01_reset(struct s6e3fa7_ams604nl01 *ctx)
{
	gpiod_set_value_cansleep(ctx->reset_gpio, 1);
	usleep_range(10000, 11000);
	gpiod_set_value_cansleep(ctx->reset_gpio, 0);
	usleep_range(5000, 6000);
	gpiod_set_value_cansleep(ctx->reset_gpio, 1);
	usleep_range(10000, 11000);
}

static int s6e3fa7_ams604nl01_on(struct drm_panel *panel)
{
	struct s6e3fa7_ams604nl01 *ctx = to_s6e3fa7_ams604nl01(panel);
	struct mipi_dsi_device *dsi = ctx->dsi;
	struct device *dev = &dsi->dev;
	int ret;

	dsi->mode_flags |= MIPI_DSI_MODE_LPM;

	ret = mipi_dsi_dcs_exit_sleep_mode(dsi);
	if (ret < 0) {
		dev_err(dev, "Failed to exit sleep mode: %d\n", ret);
		return ret;
	}
	msleep(20);

	dsi_generic_write_seq(dsi, 0x35, 0x00);  // TE Vsync On

	dsi_generic_write_seq(dsi, LEVEL1_ON);

	dsi_generic_write_seq(dsi, 0xcc, 0x4c); // PCD Setting

	dsi_generic_write_seq(dsi, 0xed, 0x44); // ERR_FG

	dsi_generic_write_seq(dsi, 0xb9, 0x00, 0x00, 0x14, // TSP SYNC
			0x00, 0x18, 0x00, 0x00, 0x00, 0x00,
			0x11, 0x01, 0x02, 0x40, 0x02, 0x40);

	dsi_generic_write_seq(dsi, 0xc5, 0x09, 0x10, 0xc8, // FFC SYNC
			0x21, 0x67, 0x11, 0x26, 0xd4);

	dsi_generic_write_seq(dsi, 0xf4, 0xbb, 0x1e, // AVC 2.0
			0x19, 0x3a, 0x9f, 0x0f, 0x09, 0xc0,
			0x00, 0xb4, 0x37, 0x70, 0x79, 0x69);

	dsi_generic_write_seq(dsi, 0xb0, 0x0e); // SAVE 5C enable
	dsi_generic_write_seq(dsi, 0xf2, 0x80);

	if (!ctx->gamma_ctx) {
		int offset;
		u8 offset_cmd [] = { 0xb0, 0x00 };
		/* Read ELVSS DATA (B5[16:17]) */
		dsi_generic_write_seq(dsi, 0xb0, 0x16); // Set read offset
		mipi_dsi_dcs_read(dsi, 0xb5, ctx->elvss_params, 2);

		/* Read IRC DATA (B8[0:1]) */
		dsi_generic_write_seq(dsi, 0xb0, 0x0); // Set read offset
		mipi_dsi_dcs_read(dsi, 0xb8, ctx->irc_params, 2);

		/* Read MTP OFFSET (C8[0:32]) */
		for (offset = 0; offset < 32; offset += 8) {
			offset_cmd[1] = offset;
			ret = mipi_dsi_generic_write(dsi, offset_cmd, ARRAY_SIZE(offset_cmd)); // Set read offset
			if (ret < 0)
				return ret;
			mipi_dsi_dcs_read(dsi, 0xc8, &ctx->mtp_offsets[offset], 8);
		}

		dsi_generic_write_seq(dsi, LEVEL1_OFF);

		ctx->gamma_ctx = s6e3fa7_gamma_init(ctx->mtp_offsets, ARRAY_SIZE(ctx->mtp_offsets));

		if (IS_ERR_OR_NULL(ctx->gamma_ctx)) {
			return PTR_ERR(ctx->gamma_ctx);
		}

		dev_dbg(&dsi->dev, "ELVSS params: 0x%02x 0x%02x\n",
				ctx->elvss_params[0], ctx->elvss_params[1]);

		dev_dbg(&dsi->dev, "IRC params: 0x%02x 0x%02x\n",
				ctx->irc_params[0], ctx->irc_params[1]);

	} else dsi_generic_write_seq(dsi, LEVEL1_OFF);

	s6e3fa7_ams604nl01_bl_update_status(ctx->backlight);

	msleep(80);
	ret = mipi_dsi_dcs_set_display_on(dsi);
	if (ret < 0) {
		dev_err(dev, "Failed to set display on: %d\n", ret);
		return ret;
	}

	return 0;
}

static int s6e3fa7_ams604nl01_off(struct drm_panel *panel)
{
	struct s6e3fa7_ams604nl01 *ctx = to_s6e3fa7_ams604nl01(panel);
	struct mipi_dsi_device *dsi = ctx->dsi;
	struct device *dev = &dsi->dev;
	int ret;

	dsi->mode_flags &= ~MIPI_DSI_MODE_LPM;

	ret = mipi_dsi_dcs_set_display_off(dsi);
	if (ret < 0) {
		dev_err(dev, "Failed to set display off: %d\n", ret);
		return ret;
	}
	usleep_range(10000, 11000);

	ret = mipi_dsi_dcs_enter_sleep_mode(dsi);
	if (ret < 0) {
		dev_err(dev, "Failed to enter sleep mode: %d\n", ret);
		return ret;
	}
	msleep(120);

	return 0;
}

static int s6e3fa7_ams604nl01_prepare(struct drm_panel *panel)
{
	struct s6e3fa7_ams604nl01 *ctx = to_s6e3fa7_ams604nl01(panel);
	struct mipi_dsi_device *dsi = ctx->dsi;
	struct device *dev = &dsi->dev;
	int ret;

	if (ctx->prepared)
		return 0;

	s6e3fa7_ams604nl01_reset(ctx);

	ret = s6e3fa7_ams604nl01_on(panel);
	if (ret < 0) {
		dev_err(dev, "Failed to initialize panel: %d\n", ret);
		gpiod_set_value_cansleep(ctx->reset_gpio, 0);
		return ret;
	}

	ctx->prepared = true;
	return 0;
}

static int s6e3fa7_ams604nl01_unprepare(struct drm_panel *panel)
{
	struct s6e3fa7_ams604nl01 *ctx = to_s6e3fa7_ams604nl01(panel);
	struct device *dev = &ctx->dsi->dev;
	int ret;

	if (!ctx->prepared)
		return 0;

	ret = s6e3fa7_ams604nl01_off(panel);
	if (ret < 0)
		dev_err(dev, "Failed to un-initialize panel: %d\n", ret);

	gpiod_set_value_cansleep(ctx->reset_gpio, 0);

	ctx->prepared = false;
	return 0;
}

static const struct drm_display_mode s6e3fa7_ams604nl01_fhd_mode = {
	.clock = (1080 + 100 + 24 + 36) * (2220 + 4 + 2 + 14) * 60 / 1000,
	.hdisplay = 1080,
	.hsync_start = 1080 + 100,
	.hsync_end = 1080 + 100 + 24,
	.htotal = 1080 + 100 + 24 + 36,
	.vdisplay = 2220,
	.vsync_start = 2220 + 4,
	.vsync_end = 2220 + 4 + 2,
	.vtotal = 2220 + 4 + 2 + 14,
	.vrefresh = 60,
	.width_mm = 67,
	.height_mm = 138,
};

static int s6e3fa7_ams604nl01_get_modes(struct drm_panel *panel,
					struct drm_connector *connector)
{
	struct drm_display_mode *mode;

	mode = drm_mode_duplicate(connector->dev, &s6e3fa7_ams604nl01_fhd_mode);
	if (!mode)
		return -ENOMEM;

	drm_mode_set_name(mode);

	mode->type = DRM_MODE_TYPE_DRIVER | DRM_MODE_TYPE_PREFERRED;
	connector->display_info.width_mm = mode->width_mm;
	connector->display_info.height_mm = mode->height_mm;
	drm_mode_probed_add(connector, mode);

	return 1;
}

static const struct drm_panel_funcs s6e3fa7_ams604nl01_panel_funcs = {
	.prepare = s6e3fa7_ams604nl01_prepare,
	.unprepare = s6e3fa7_ams604nl01_unprepare,
	.get_modes = s6e3fa7_ams604nl01_get_modes,
};

static int s6e3fa7_ams604nl01_bl_update_status(struct backlight_device *bl)
{
	struct mipi_dsi_device *dsi = bl_get_data(bl);
	struct s6e3fa7_ams604nl01 *ctx = mipi_dsi_get_drvdata(dsi);
	const struct brightness_level *settings;
	u16 brightness = bl->props.brightness;
	u8 aid_cmd[] =  { 0xb1, 0x00, 0x10 };
	u8 elvss_cmd[] = {
		0xb5, 0x14, 0xcc, 0x0d, 0x01, 0x34, 0x67, 0x9a,
		0xcd, 0x01, 0x22, 0x33, 0x44, 0xc0, 0x00, 0x09,
		0x99, 0x33, 0x13, 0x01, 0x11, 0x10, 0x00, 0x00,
	};
	u8 vint_cmd[] = { 0xf4, 0xbb, 0x1e };
	u8 gamma_cmd[] = {  0xca,  /* 1) Gamma 2.2 Setting */
		0x07, 0x00, 0x00,  /* (VTR << 4, V255 R[8],G[8],B[8]), (VTG << 4, VTB), V255 R[7:0] */
		0x00, 0x00, 0x00,  /* V255 G[7:0],empty, V255 B[7:0] */
		0x80, 0x80, 0x80,  /* V203 R,G,B */
		0x80, 0x80, 0x80,  /* V151 R,G,B */
		0x80, 0x80, 0x80,  /*  V87 R,G,B */
		0x80, 0x80, 0x80,  /*  V51 R,G,B */
		0x80, 0x80, 0x80,  /*  V35 R,G,B */
		0x80, 0x80, 0x80,  /*  V23 R,G,B */
		0x80, 0x80, 0x80,  /*  V11 R,G,B */
		0x80, 0x80, 0x80,  /*   V7 R,G,B */
		0x80, 0x80, 0x80,  /*   V1 R,G,B */
	};

	u8 irc_cmd[] = { 0xb8, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x80, 0x00, 0x00, 0x00,
		0x80, 0x61, 0x2c, 0x30, 0x4e, 0xc4,
		0x33, 0x69, 0x12, 0x7a, 0xc7, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	};
	int ret, i;

	if (bl->props.power != FB_BLANK_UNBLANK ||
	    bl->props.fb_blank != FB_BLANK_UNBLANK ||
	    bl->props.state & (BL_CORE_SUSPENDED | BL_CORE_FBBLANK))
		brightness = 0;

	if (brightness < 0 || brightness > bl->props.max_brightness) {
		dev_err(&dsi->dev, "%s brightness out of range", __func__);
		return -EINVAL;
	}

	if (IS_ERR_OR_NULL(ctx->gamma_ctx)) {
		ctx->brightness = brightness;
		return 0;
	}

	elvss_cmd[23] = ctx->elvss_params[0];
	irc_cmd[1] = ctx->irc_params[0];
	irc_cmd[2] = ctx->irc_params[1];

	settings = &brightness_settings[brightness];

	s6e3fa7_get_gamma(ctx->gamma_ctx, settings->gamma_idx, &gamma_cmd[1], ARRAY_SIZE(gamma_cmd) - 1);

	aid_cmd[1] = clamp((u8)settings->aid1, (u8) 0,(u8)  8);
	aid_cmd[2] = settings->aid2;
	vint_cmd[2] = settings->vint2;
	elvss_cmd[2] = settings->elvss2;
	elvss_cmd[3] = settings->elvss3;
	for (i = 0; i < 3; i ++) {
		irc_cmd[25 + i] = settings->irc25_27;
		irc_cmd[28 + i] = settings->irc28_30;
		irc_cmd[31 + i] = settings->irc31_33;
	}

	dsi_generic_write_seq(dsi, LEVEL1_ON);

	ret = mipi_dsi_generic_write(dsi, aid_cmd, ARRAY_SIZE(aid_cmd));
	if (ret < 0)
		goto fail;

	ret = mipi_dsi_generic_write(dsi, elvss_cmd, ARRAY_SIZE(elvss_cmd));
	if (ret < 0)
		goto fail;

	ret = mipi_dsi_generic_write(dsi, vint_cmd, ARRAY_SIZE(vint_cmd));
	if (ret < 0)
		goto fail;

	ret = mipi_dsi_generic_write(dsi, irc_cmd, ARRAY_SIZE(irc_cmd));
	if (ret < 0)
		goto fail;

	ret = mipi_dsi_generic_write(dsi, gamma_cmd, ARRAY_SIZE(gamma_cmd));
	if (ret < 0)
		goto fail;

	dsi_generic_write_seq(dsi, 0xf7, 0x03); // Gamma, LTPS(AID) update
	ctx->brightness = brightness;
fail:
	dsi_generic_write_seq(dsi, LEVEL1_OFF);
	if (ret < 0) {
		dev_err(&dsi->dev, "%s mipi_dsi_generic_write failed: %d", __func__, ret);
		return ret;
	}

	return 0;
}

static int s6e3fa7_ams604nl01_bl_get_brightness(struct backlight_device *bl)
{
	struct mipi_dsi_device *dsi = bl_get_data(bl);
	struct s6e3fa7_ams604nl01 *ctx = mipi_dsi_get_drvdata(dsi);
	return ctx->brightness;
}

static const struct backlight_ops s6e3fa7_ams604nl01_bl_ops = {
	.update_status = s6e3fa7_ams604nl01_bl_update_status,
	.get_brightness = s6e3fa7_ams604nl01_bl_get_brightness,
};

static struct backlight_device *
s6e3fa7_ams604nl01_create_backlight(struct mipi_dsi_device *dsi)
{
	struct device *dev = &dsi->dev;
	struct backlight_properties props = {
		.type = BACKLIGHT_RAW,
		.brightness = 100,
		.max_brightness = ARRAY_SIZE(brightness_settings) - 1,
	};

	return devm_backlight_device_register(dev, dev_name(dev), dev, dsi,
					      &s6e3fa7_ams604nl01_bl_ops, &props);
}

static int s6e3fa7_ams604nl01_probe(struct mipi_dsi_device *dsi)
{
	struct device *dev = &dsi->dev;
	struct s6e3fa7_ams604nl01 *ctx;
	int ret;

	ctx = devm_kzalloc(dev, sizeof(*ctx), GFP_KERNEL);
	if (!ctx)
		return -ENOMEM;

	ctx->reset_gpio = devm_gpiod_get(dev, "reset", GPIOD_OUT_LOW);
	if (IS_ERR(ctx->reset_gpio)) {
		ret = PTR_ERR(ctx->reset_gpio);
		dev_err(dev, "Failed to get reset-gpios: %d\n", ret);
		return ret;
	}

	ctx->dsi = dsi;
	mipi_dsi_set_drvdata(dsi, ctx);

	dsi->lanes = 4;
	dsi->format = MIPI_DSI_FMT_RGB888;
	dsi->mode_flags = MIPI_DSI_MODE_VIDEO_BURST |
			  MIPI_DSI_CLOCK_NON_CONTINUOUS;

	drm_panel_init(&ctx->panel, dev, &s6e3fa7_ams604nl01_panel_funcs,
		       DRM_MODE_CONNECTOR_DSI);

	ctx->backlight = s6e3fa7_ams604nl01_create_backlight(dsi);
	if (IS_ERR(ctx->backlight)) {
		ret = PTR_ERR(ctx->backlight);
		dev_err(dev, "Failed to create backlight: %d\n", ret);
		return ret;
	}

	ret = drm_panel_add(&ctx->panel);
	if (ret < 0) {
		dev_err(dev, "Failed to add panel: %d\n", ret);
		return ret;
	}

	ret = mipi_dsi_attach(dsi);
	if (ret < 0) {
		dev_err(dev, "Failed to attach to DSI host: %d\n", ret);
		return ret;
	}

	return 0;
}

static int s6e3fa7_ams604nl01_remove(struct mipi_dsi_device *dsi)
{
	struct s6e3fa7_ams604nl01 *ctx = mipi_dsi_get_drvdata(dsi);
	int ret;

	if (!IS_ERR_OR_NULL(ctx->gamma_ctx))
		s6e3fa7_gamma_destroy(ctx->gamma_ctx);

	ret = mipi_dsi_detach(dsi);
	if (ret < 0)
		dev_err(&dsi->dev, "Failed to detach from DSI host: %d\n", ret);

	drm_panel_remove(&ctx->panel);

	return 0;
}

static const struct of_device_id s6e3fa7_ams604nl01_of_match[] = {
	{ .compatible = "samsung,s6e3fa7-ams604nl01-fhd" },
	{ }
};
MODULE_DEVICE_TABLE(of, s6e3fa7_ams604nl01_of_match);

static struct mipi_dsi_driver s6e3fa7_ams604nl01_driver = {
	.probe = s6e3fa7_ams604nl01_probe,
	.remove = s6e3fa7_ams604nl01_remove,
	.driver = {
		.name = "panel-s6e3fa7-ams604nl01-fhd",
		.of_match_table = s6e3fa7_ams604nl01_of_match,
	},
};
module_mipi_dsi_driver(s6e3fa7_ams604nl01_driver);

MODULE_AUTHOR("linux-mdss-dsi-panel-driver-generator with help of <junak.pub@gmail.com>");
MODULE_DESCRIPTION("DRM driver for ss_dsi_panel_S6E3FA7_AMS604NL01");
MODULE_LICENSE("GPL v2");
