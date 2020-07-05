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

s32 unfreed_malloc_calls = 0;

#ifdef ARM9

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
		iprintf("Nullpointer free detected!\n");
	}
}

void my_start_malloc_invariant(void)
{
	unfreed_malloc_calls = 0;
}

void my_end_malloc_invariant(void)
{
	if(unfreed_malloc_calls != 0) {
		iprintf("Allocation error! Unfreed mallocs: %d\n", (int)unfreed_malloc_calls);
	}
}

void *my_memalign(size_t blocksize, size_t bytes)
{
	void *buf = memalign(blocksize, bytes);
	if( ((u32)buf & blocksize) != 0) {
		iprintf("Memalign error! %p ist not %u-aligned\n", buf, (u16)blocksize);
		return 0;
	} else {
		unfreed_malloc_calls++;
		return buf;
	}
}

#endif

// Reinventing the wheel to save arm7 binary size
void *my_memset(void *s, int c, u32 n)
{
	u8 *t = (u8*)s;
	for(u32 i=0; i<n; ++i) {
		t[i] = c;
	}
	return s;
}

char *my_strncpy(char *dest, const char *src, u32 n)
{
	u32 i=0;
	while((src[i] != 0) && (i < n)) {
		dest[i] = src[i];
		i++;
	}
	if((i<n)&&(src[i]==0)) {
		dest[i] = 0;
	}
	return dest;
}

#ifdef ARM9

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

#endif

u32 my_getUsedRam(void)
{
	struct mallinfo info = mallinfo();

	return info.usmblks + info.uordblks; 
}

u32 my_getFreeDiskSpace(void)
{
	struct statvfs fiData;

	if((statvfs("/",&fiData)) < 0 ) {
		iprintf("stat failed!\n");
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
