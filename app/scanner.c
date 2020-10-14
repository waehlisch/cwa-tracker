/*
 * Copyright (C) 2020 Freie Universität Berlin
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     cwa-tracker
 * @{
 *
 * @file
 * @brief       Corona-Warn-App Tracker
 *
 * @author      Hauke Petersen <hauke.petersen@fu-berlin.de>
 *
 * @}
 */

#include "fmt.h"
#include "nimble_scanner.h"
#include "net/bluetil/addr.h"

#include "app.h"

#include "periph/rtt.h"

static const struct ble_gap_disc_params _scan_params = {
    .itvl = SCANNER_ITVL,
    .window = SCANNER_WIN,
    .filter_policy = 0,
    .limited = 0,
    .passive = 1,                               /* only scan passive: no SCAN_REQ */
    .filter_duplicates = 0,
};

static char tmp[200];

static void _on_disc(uint8_t type,
                     const ble_addr_t *addr, int8_t rssi,
                     const uint8_t *ad, size_t ad_len)
{
    size_t pos = 0;

    uint32_t ticks = rtt_get_counter();
    uint32_t sec = RTT_TICKS_TO_SEC(ticks);
    uint32_t ms = RTT_TICKS_TO_MS(ticks) % 1000;

    pos += fmt_u32_dec(&tmp[pos], sec);
    tmp[pos++] = '.';
    pos += fmt_u32_dec(&tmp[pos], ms);
    tmp[pos++] = ';';
    pos += fmt_u32_dec(&tmp[pos], (uint32_t)type);
    tmp[pos++] = ';';
    pos += fmt_u32_dec(&tmp[pos], (uint32_t)addr->type);
    tmp[pos++] = '-';
    bluetil_addr_sprint(&tmp[pos], addr->val);
    pos += BLUETIL_ADDR_STRLEN - 1;
    tmp[pos++] = ';';
    pos += snprintf(&tmp[pos], 4, "%i", (int)rssi);
    // pos += fmt_s16_dec(&tmp[pos], (int16_t)rssi);
    tmp[pos++] = ';';
    pos += fmt_bytes_hex(&tmp[pos], ad, ad_len);
    tmp[pos++] = '\n';
    tmp[pos] = '\0';

    stor_write_ln(tmp, pos);

    // printf("%s", tmp);
}


int scanner_init(void)
{
    int res;

    res = nimble_scanner_init(&_scan_params, _on_disc);

    return res;
}

int scanner_start(void)
{
    return nimble_scanner_start();
}

#include "app.h"


