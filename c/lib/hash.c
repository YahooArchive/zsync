/*
 *   zsync/lib - library for using the rsync algorithm to determine
 *               which parts of a file you have and which you need.
 *   Copyright (C) 2004 Colin Phipps <cph@moria.org.uk>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.

 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "zsync.h"
#include "internal.h"

static inline int calc_rhash(struct zsync_state* z, struct rsum r) { return r.b & z->hashmask; }

const struct hash_entry* get_first_hash_entry(struct zsync_state* z, struct rsum r) {
  return z->rsum_hash[calc_rhash(z, r)];
}

void add_target_block(struct zsync_state* z, zs_blockid b, struct rsum r, void* checksum)
{
 if (b < z->blocks) {
  /* Get hash entry with checksums for this block */
  struct hash_entry* e = &(z->blockhashes[b]);
  int h;

  /* Enter checksums */
  memcpy(e->checksum, checksum, CHECKSUM_SIZE);
  e->r = r;

  /* Prepend to linked list for this hash entry */
  h = calc_rhash(z, r);
  e->next = z->rsum_hash[h];
  z->rsum_hash[h] = e;
 }
}

