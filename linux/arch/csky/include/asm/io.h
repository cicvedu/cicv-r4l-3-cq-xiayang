/* SPDX-License-Identifier: GPL-2.0 */

#ifndef __ASM_CSKY_IO_H
#define __ASM_CSKY_IO_H

#include <linux/pgtable.h>
#include <linux/types.h>

/*
 * I/O memory access primitives. Reads are ordered relative to any
 * following Normal memory access. Writes are ordered relative to any prior
 * Normal memory access.
 *
 * For CACHEV1 (807, 810), store instruction could fast retire, so we need
 * another mb() to prevent st fast retire.
 *
 * For CACHEV2 (860), store instruction with PAGE_ATTR_NO_BUFFERABLE won't
 * fast retire.
 */
#define readb(c)		({ u8  __v = readb_relaxed(c); rmb(); __v; })
#define readw(c)		({ u16 __v = readw_relaxed(c); rmb(); __v; })
#define readl(c)		({ u32 __v = readl_relaxed(c); rmb(); __v; })

#ifdef CONFIG_CPU_HAS_CACHEV2
#define writeb(v,c)		({ wmb(); writeb_relaxed((v),(c)); })
#define writew(v,c)		({ wmb(); writew_relaxed((v),(c)); })
#define writel(v,c)		({ wmb(); writel_relaxed((v),(c)); })
#else
#define writeb(v,c)		({ wmb(); writeb_relaxed((v),(c)); mb(); })
#define writew(v,c)		({ wmb(); writew_relaxed((v),(c)); mb(); })
#define writel(v,c)		({ wmb(); writel_relaxed((v),(c)); mb(); })
#endif

/*
 * String version of I/O memory access operations.
 */
extern void __memcpy_fromio(void *, const volatile void __iomem *, size_t);
extern void __memcpy_toio(volatile void __iomem *, const void *, size_t);
extern void __memset_io(volatile void __iomem *, int, size_t);

#define memset_io(c,v,l)        __memset_io((c),(v),(l))
#define memcpy_fromio(a,c,l)    __memcpy_fromio((a),(c),(l))
#define memcpy_toio(c,a,l)      __memcpy_toio((c),(a),(l))

/*
 * I/O memory mapping functions.
 */
#define ioremap_wc(addr, size) \
	ioremap_prot((addr), (size), \
		(_PAGE_IOREMAP & ~_CACHE_MASK) | _CACHE_UNCACHED)

#include <asm-generic/io.h>

#endif /* __ASM_CSKY_IO_H */
