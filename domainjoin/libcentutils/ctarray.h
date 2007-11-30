/*
 * Copyright (C) Centeris Corporation 2004-2007
 * Copyright (C) Likewise Software 2007.  
 * All rights reserved.
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/* ex: set tabstop=4 expandtab shiftwidth=4: */  
#ifndef __CT_ARRAY_H__
#define __CT_ARRAY_H__
    
/**
 * @brief A dynamically resizeable array
 *
 * This encapsulates a dynamically resizeable array of a user defined type. The
 * data pointer requires casting to the user's preferred type.
 */ 
typedef struct  {
	void *data;
	
    /**
     * The number of items in the array in terms of the type this array holds,
     * not in terms of bytes.
     */ 
	 size_t size;
	
    /**
     * The number of items that can be stored without having to reallocate
     * memory. This is in items, not bytes
     */ 
	 size_t capacity;
} DynamicArray;
CENTERROR CTArrayConstruct(DynamicArray * array, size_t itemSize);

/**
 * Change the available space in the array
 *
 * @errcode
 * @canfail
 */ 
    CENTERROR CTSetCapacity(DynamicArray * array, size_t itemSize,
			    size_t capacity);

/**
 * Insert one or more items into the array at any position.
 *
 * @errcode
 * @canfail
 */ 
    CENTERROR CTArrayInsert(DynamicArray * array, int insertPos, int itemSize,
			    void *data, size_t dataLen);

/**
 * Append one or more items to the end of the array.
 *
 * @errcode
 * @canfail
 */ 
    CENTERROR CTArrayAppend(DynamicArray * array, int itemSize, void *data,
			    size_t dataLen);

/**
 * Remove one or more items from the array at any position. This will not
 * shrink the allocated memory (capacity stays the same).
 *
 * @errcode
 * @canfail
 */ 
    CENTERROR CTArrayRemove(DynamicArray * array, int removePos, int itemSize,
			    size_t dataLen);

/**
 * Pop items off of the head of the list. They first get copied into the user
 * supplied buffer, then they are removed from the front of the array.
 *
 * @return the number of items removed
 */ 
    size_t CTArrayRemoveHead(DynamicArray * array, int itemSize, void *store,
			     size_t dataLen);

/**
 * Free the memory associated with a dynamic array, and zero out the pointers.
 * The dynamic array essentialy becomes a zero length array. The object can be
 * reused by appending new data after it has been freed.
 *
 * @wontfail
 */ 
void CTArrayFree(DynamicArray * array);

#endif	/*  */
