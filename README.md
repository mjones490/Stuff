## Stuff

**Stuff** is a repository of routines that I've written over the
years.  These are routines that I've found the need for across
various projects, such as *cmdLineStuff* and *configStuff*, and
things I've written because I thought they'd be nice or convenient
to have, but never really had a use for after they were written,
like *cacheStuff* and *mathStuff*.  Or even stuff I've started to
work on and never finished, like *hashStuff*.

Here is a quick summary of the routines:

* **cacheStuff** - Routines for caching objects.
* **calcStuff** - Routines to read strings of mathmatical expressions
and output the computed answer.
* **cmdlineStuff** - Routines to parse parameters passed in from the 
command line.
* **configStuff** - Routines to read and parse configuration files.
* **hashStuff** - Routines to generate FNV-1A of a given string.
* **listStuff** - Routines manage lists of objects.
* **mathStuff** - A few math routines (duh).
* **stringStuff** - Routines to manage strings.
* **timeStuff** - Routines for time calculations.
* **varStuff** - Routines for handling variables.
* **macroStuff.h** - A few handy C macros.

### cacheStuff

Routines for caching objects.  Similar to memory allocation functions `malloc`
and `free`, these routines manage frequently allocated and deallocated space
for any kind of object.  Each cache holds only one type of object, so requesting
space for a new object simply returns the next address on the free list, without
the need to walk the list for sufficient space for that object.

```struct cache;```

This object contains all the information about the cache itself.

```struct cache *cache_create(size_t obj_size);```

Allocate and return a new `struct cache` object.  The new cache will be configured
to hold objects of `obj_size`.

```void *cache_get_object(struct cache *cache);```

Return a pointer to a free slot in `cache` large enough to hold one object of size
`obj_size`.






