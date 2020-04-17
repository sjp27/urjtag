/*
 * $Id$
 *
 * Copyright (C) 2003 ETC s.r.o.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
 * 02111-1307, USA.
 *
 * Written by Marcel Telka <marcel@telka.sk>, 2003.
 *
 */

#include <sysdep.h>

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>

#include <urjtag/error.h>
#include <urjtag/bus.h>

#include <urjtag/cmd.h>

#include "cmd.h"

static int
cmd_showmem_run (urj_chain_t *chain, char *params[])
{
    long unsigned adr;
    long unsigned len;
    int r;

    if (urj_cmd_params (params) != 3)
    {
        urj_error_set (URJ_ERROR_SYNTAX,
                       "%s: #parameters should be %d, not %d",
                       params[0], 3, urj_cmd_params (params));
        return URJ_STATUS_FAIL;
    }

    if (!urj_bus)
    {
        urj_error_set (URJ_ERROR_ILLEGAL_STATE, _("Bus missing"));
        return URJ_STATUS_FAIL;
    }

    if (urj_cmd_get_number (params[1], &adr) != URJ_STATUS_OK
        || urj_cmd_get_number (params[2], &len) != URJ_STATUS_OK)
        return URJ_STATUS_FAIL;

    if(len > 0x1000)
    {
        urj_error_set (URJ_ERROR_SYNTAX, ("LEN too large\n"));
        return URJ_STATUS_FAIL;
    }

    r = urj_bus_showmem (urj_bus, adr, len);

    return r;
}

static void
cmd_showmem_help (void)
{
    urj_log (URJ_LOG_LEVEL_NORMAL,
             _("Usage: %s ADDR LEN\n"
               "Show device memory content starting with ADDR.\n"
               "\n"
               "ADDR       start address of the memory area\n"
               "LEN        memory length (max 4096)\n"
               "\n"
               "ADDR and LEN could be in decimal or hexadecimal (prefixed with 0x) form.\n"),
             "showmem");
}

static void
cmd_showmem_complete (urj_chain_t *chain, char ***matches, size_t *match_cnt,
                      char * const *tokens, const char *text, size_t text_len,
                      size_t token_point)
{
    switch (token_point)
    {
    case 1: /* addr */
        break;

    case 2: /* len */
        break;
    }
}

const urj_cmd_t urj_cmd_showmem = {
    "showmem",
    N_("show content of the memory"),
    cmd_showmem_help,
    cmd_showmem_run,
    cmd_showmem_complete,
};
