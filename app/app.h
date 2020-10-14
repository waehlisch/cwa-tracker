/*
 * Copyright (C) 2020 Freie Universität Berlin
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @defgroup    cwa-tracker Corona Warn App Tracker
 * @brief       Corona-Warn-App Tracker
 *
 * @{
 * @file
 * @brief       Corona-Warn-App Tracker
 *
 * @author      Hauke Petersen <hauke.petersen@fu-berlin.de>
 */

#ifndef APP_H
#define APP_H

#ifdef __cplusplus
extern "C" {
#endif

#define SCANNER_ITVL        BLE_GAP_SCAN_ITVL_MS(60)
#define SCANNER_WIN         BLE_GAP_SCAN_WIN_MS(60)

int scanner_init(void);
int scanner_start(void);

int stor_init(void);
int stor_write_ln(char *line, size_t len);

#ifdef __cplusplus
}
#endif

#endif /* APP_H */
/** @} */
