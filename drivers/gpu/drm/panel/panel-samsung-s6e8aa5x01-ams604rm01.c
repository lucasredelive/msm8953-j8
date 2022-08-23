// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2013, The Linux Foundation. All rights reserved.

static const struct drm_display_mode s6e8aa5x01_ams604rm01_mode = {
	.clock = (720 + 172 + 12 + 20) * (1480 + 14 + 5 + 5) * 60 / 1000,
	.hdisplay = 720,
	.hsync_start = 720 + 172,
	.hsync_end = 720 + 172 + 12,
	.htotal = 720 + 172 + 12 + 20,
	.vdisplay = 1480,
	.vsync_start = 1480 + 14,
	.vsync_end = 1480 + 14 + 5,
	.vtotal = 1480 + 14 + 5 + 5,
	.width_mm = 67,
	.height_mm = 138,
};

static const struct panel_desc_dsi s6e8aa5x01_ams604rm01 = {
	.desc = {
		.modes = &s6e8aa5x01_ams604rm01_mode,
		.num_modes = 1,
		.bpc = 8,
		.size = {
			.width = 67,
			.height = 138,
		},
		.connector_type = DRM_MODE_CONNECTOR_DSI,
	},
	.flags = MIPI_DSI_MODE_VIDEO | MIPI_DSI_MODE_VIDEO_BURST |
		 MIPI_DSI_MODE_VIDEO_NO_HSA,
	.format = MIPI_DSI_FMT_RGB888,
	.lanes = 4,
};
