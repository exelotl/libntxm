/*
 * libNTXM - XM Player Library for the Nintendo DS
 *
 *    Copyright (C) 2005-2008 Tobias Weyand (0xtob)
 *                         me@nitrotracker.tobw.net
 *
 */

/***** BEGIN LICENSE BLOCK *****
 * 
 * Version: Noncommercial zLib License / GPL 3.0
 * 
 * The contents of this file are subject to the Noncommercial zLib License 
 * (the "License"); you may not use this file except in compliance with
 * the License. You should have recieved a copy of the license with this package.
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied.
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either of the GNU General Public License Version 3 or later (the "GPL"),
 * in which case the provisions of the GPL are applicable instead of those above.
 * If you wish to allow use of your version of this file only under the terms of
 * either the GPL, and not to allow others to use your version of this file under
 * the terms of the Noncommercial zLib License, indicate your decision by
 * deleting the provisions above and replace them with the notice and other
 * provisions required by the GPL. If you do not delete the provisions above,
 * a recipient may use your version of this file under the terms of any one of
 * the GPL or the Noncommercial zLib License.
 * 
 ***** END LICENSE BLOCK *****/

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <nds.h>

#include "ntxm/ntxmtools.h"
#include <sys/statvfs.h>

#ifdef ARM9

#ifdef DEBUG
s32 unfreed_malloc_calls = 0;
u32 remaining_bytes = 0;

void *my_malloc(size_t size)
{
	void *ptr = malloc(size);
	if(ptr!=0) {
		unfreed_malloc_calls++;
	}
	return ptr;
}

void my_free(void *ptr)
{
	if(ptr!=0) {
		unfreed_malloc_calls--;
		free(ptr);
	} else {
		my_dprintf("Nullpointer free detected!\n");
	}
}

void my_start_malloc_invariant(void)
{
	unfreed_malloc_calls = 0;
}

void my_end_malloc_invariant(void)
{
	if(unfreed_malloc_calls != 0) {
		my_dprintf("Allocation error! Unfreed mallocs: %d\n", (int)unfreed_malloc_calls);
	}
}

void *my_memalign(size_t blocksize, size_t bytes)
{
	void *buf = memalign(blocksize, bytes);
	if( ((u32)buf & blocksize) != 0) {
		my_dprintf("Memalign error! %p ist not %u-aligned\n", buf, (u16)blocksize);
		return 0;
	} else {
		unfreed_malloc_calls++;
		return buf;
	}
}
#endif /* DEBUG */

bool my_file_exists(const char *filename)
{
	bool res;
	FILE* f = fopen(filename,"r");
	if(f == NULL) {
		res = false;
	} else {
		fclose(f);
		res = true;
	}

	return res;
}

#endif /* ARM9 */

u32 my_getUsedRam(void)
{
	struct mallinfo info = mallinfo();

	return info.usmblks + info.uordblks; 
}

#ifdef ARM9

u32 my_getFreeDiskSpace(void)
{
	struct statvfs fiData;

	if((statvfs("/",&fiData)) < 0 ) {
		my_dprintf("stat failed!\n");
		return 0;
	} else {
		return fiData.f_bsize*fiData.f_bfree;
	}
}

u32 my_getFileSize(const char *filename)
{
	FILE *file = fopen(filename, "r");
	fseek(file, 0, SEEK_END);
	u32 filesize = ftell(file);
	fclose(file);
	return filesize;
}

#endif

void ntxm_unsigned2signed_8(uint8_t *buffer, size_t count)
{
	uint32_t *buf32 = (uint32_t*) buffer;
	size_t count32 = count >> 2;
	size_t i;

	for (i = 0; i < count32; i++) {
		buf32[i] ^= 0x80808080;
	}
	i <<= 2;
	for (; i < count; i++) {
		buffer[i] ^= 0x80;
	}
}

void ntxm_unsigned2signed_16(uint16_t *buffer, size_t count)
{
	uint32_t *buf32 = (uint32_t*) buffer;
	size_t count32 = count >> 1;

	for (size_t i = 0; i < count32; i++) {
		buf32[i] ^= 0x80008000;
	}
	if (count & 1) buffer[count - 1] ^= 0x8000;
}
