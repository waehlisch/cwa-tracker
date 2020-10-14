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

#include "shell.h"

#include "app.h"



static const shell_command_t _cmds[] = {
    // { "time", "set current system time" },
    // { "test", "foo", _mount },
    { NULL, NULL, NULL }
};

int main(void)
{
    int res;

    puts("CWA Tracker");

    res = stor_init();
    if (res != 0) {
        puts("error: unable to initialize storage");
        return 1;
    }

    res = scanner_init();
    if (res != 0) {
        puts("error: unable to initialize scanner");
        return 1;
    }


    scanner_start();


    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(_cmds, line_buf, SHELL_DEFAULT_BUFSIZE);

    return 0;
}
