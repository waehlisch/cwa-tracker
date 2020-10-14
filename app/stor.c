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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#include "fmt.h"
#include "random.h"
#include "vfs.h"
#include "fs/fatfs.h"
#include "mtd_sdcard.h"
#include "sdcard_spi.h"
#include "sdcard_spi_params.h"


#include "app.h"

#include "xtimer.h"

/* Configure MTD device for the first SD card */
extern sdcard_spi_t sdcard_spi_devs[ARRAY_SIZE(sdcard_spi_params)];
static mtd_sdcard_t mtd_sdcard_dev = {
    .base = {
        .driver = &mtd_sdcard_driver
    },
    .sd_card = &sdcard_spi_devs[0],
    .params = &sdcard_spi_params[0],
};
mtd_dev_t *fatfs_mtd_devs[FF_VOLUMES];
static fatfs_desc_t fs_desc;
static vfs_mount_t flash_mount = {
    .fs = &fatfs_file_system,
    .mount_point = "/f",
    .private_data = &fs_desc,
};

#define FSBUF_SIZE          16384U      /* 15kb buffer */
#define WRITE_DELAY         (US_PER_SEC)

#define PRIO        (THREAD_PRIORITY_MAIN - 1)
static char _stack[THREAD_STACKSIZE_DEFAULT];


static char _fname[30];

static mutex_t _buflock = MUTEX_INIT;
static char _inbuf[FSBUF_SIZE];
static char _fsbuf[FSBUF_SIZE];
static size_t _inbuf_pos;


static void _fswrite(void)
{
    size_t len;

    uint32_t now = xtimer_now_usec();

    /* copy buffer and clear inbuf */
    mutex_lock(&_buflock);
    memcpy(_fsbuf, _inbuf, _inbuf_pos);
    len = _inbuf_pos;
    _inbuf_pos = 0;
    mutex_unlock(&_buflock);

    /* write data to FS */
    int f = vfs_open(_fname, (O_CREAT | O_WRONLY | O_APPEND), 0);
    if (f < 0) {
        printf("err: unable to open file '%s'\n", _fname);
        return;
    }

    int n = vfs_write(f, _fsbuf, len);
    if (n < 0) {
        printf("err: unable to write\n");
    }
    else if ((size_t)n != len) {
        printf("err: size written is not the given\n");
    }
    vfs_close(f);

    uint32_t dur = xtimer_now_usec() - now;
    printf("stor: written %i byte to FS in %uus\n", len, (unsigned)dur);
}

static void *_write_thread(void *arg)
{
    (void)arg;

    while (1) {
        xtimer_usleep(WRITE_DELAY);
        if (_inbuf_pos > 0) {
            _fswrite();
        }
    }

    return NULL;
}

int stor_init(void)
{
    int res;

    fatfs_mtd_devs[fs_desc.vol_idx] = (mtd_dev_t*)&mtd_sdcard_dev;

    puts("Mounting FS (SD-Card)");
    res = vfs_mount(&flash_mount);
    if (res != 0) {
        puts("error: unable to mount SD-Card");
        return res;
    }
    else {
        puts("... success");
    }

    _fname[0] = '/';
    _fname[1] = 'f';
    _fname[2] = '/';
    uint8_t tmp[4];
    random_bytes(tmp, 4);
    fmt_bytes_hex(&_fname[3], tmp, 4);
    _fname[11] = '\0';

    printf("out file is %s\n", _fname);

    thread_create(_stack, sizeof(_stack), PRIO, THREAD_CREATE_STACKTEST,
                  _write_thread, NULL, "stor");
    return res;
}

int stor_write_ln(char *line, size_t len)
{
    mutex_lock(&_buflock);
    if ((_inbuf_pos + len) > FSBUF_SIZE) {
        printf("err: stor write ln: buffer full, dropping data\n");
        mutex_unlock(&_buflock);
        return 1;
    }
    memcpy(&_inbuf[_inbuf_pos], line, len);
    _inbuf_pos += len;
    mutex_unlock(&_buflock);

    return 0;
}

