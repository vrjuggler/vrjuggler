/*
 ** Copyright (c) 1990- 1993, 1996 Open Software Foundation, Inc.
 ** Copyright (c) 1989 by Hewlett-Packard Company, Palo Alto, Ca. &
 ** Digital Equipment Corporation, Maynard, Mass.
 ** Copyright (c) 1998 Microsoft.
 ** To anyone who acknowledges that this file is provided "AS IS"
 ** without any express or implied warranty: permission to use, copy,
 ** modify, and distribute this file for any purpose is hereby
 ** granted without fee, provided that the above copyright notices and
 ** this notice appears in all source code copies, and that none of
 ** the names of Open Software Foundation, Inc., Hewlett-Packard
 ** Company, or Digital Equipment Corporation be used in advertising
 ** or publicity pertaining to distribution of the software without
 ** specific, written prior permission.  Neither Open Software
 ** Foundation, Inc., Hewlett-Packard Company, Microsoft, nor Digital Equipment
 ** Corporation makes any representations about the suitability of
 ** this software for any purpose.
 */

#include "sysdep.h"
#include <stdio.h>
#include "uuid.h"

uuid_t NameSpace_DNS = { /* 6ba7b810-9dad-11d1-80b4-00c04fd430c8 */
    0x6ba7b810,
    0x9dad,
    0x11d1,
    0x80, 0xb4, 0x00, 0xc0, 0x4f, 0xd4, 0x30, 0xc8
  };

/* puid -- print a UUID */
void puid(uuid_t u);

/* Simple driver for UUID generator */
void main(int argc, char **argv) {
  uuid_t u;
  int f;

  uuid_create(&u);
  printf("uuid_create()             -> "); puid(u);

  f = uuid_compare(&u, &u);
  printf("uuid_compare(u,u): %d\n", f);     /* should be 0 */
  f = uuid_compare(&u, &NameSpace_DNS);
  printf("uuid_compare(u, NameSpace_DNS): %d\n", f); /* s.b. 1 */
  f = uuid_compare(&NameSpace_DNS, &u);
  printf("uuid_compare(NameSpace_DNS, u): %d\n", f); /* s.b. -1 */

  uuid_create_from_name(&u, NameSpace_DNS, "www.widgets.com", 15);
  printf("uuid_create_from_name() -> "); puid(u);
};

void puid(uuid_t u) {
  int i;

  printf("%8.8x-%4.4x-%4.4x-%2.2x%2.2x-", u.time_low, u.time_mid,
      u.time_hi_and_version, u.clock_seq_hi_and_reserved,
      u.clock_seq_low);
  for (i = 0; i < 6; i++)
      printf("%2.2x", u.node[i]);
  printf("\n");
};
