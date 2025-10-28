/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Kernel dependent features
 */

#ifndef NILFS_KERN_FEATURE_H
#define NILFS_KERN_FEATURE_H

#include <linux/version.h>
#ifndef LINUX_VERSION_CODE
# include <generated/uapi/linux/version.h>
#endif

/*
 * Please define as 0/1 here if you want to override
 */

/*
 * for Red Hat Enterprise Linux 10.x clones
 */
#if defined(RHEL_MAJOR) && (RHEL_MAJOR == 10)
# if defined(RHEL_RELEASE_N)
#  if (RHEL_RELEASE_N >= 27)
#   define	HAVE_FOLIO_BASED_WRITE_BEGIN_END		1
#  endif
#  if (RHEL_RELEASE_N >= 144)
#   define	HAVE_TIMER_CONTAINER_OF				1
#  endif
# else /* !defined(RHEL_RELEASE_N) */
#  define	HAVE_FOLIO_BASED_WRITE_BEGIN_END		1
#  if (RHEL_MINOR > 1)				/* RHEL_RELEASE_N >= 125 */
#   define	HAVE_TIMER_CONTAINER_OF				1
#  endif
# endif
#endif

/*
 * defaults
 */

/*
 * defaults dependent to kernel versions
 */
#ifdef LINUX_VERSION_CODE
/*
 * write_begin and write_end interfaces were converted to folio-based
 * in kernel 6.12-rc1.
 */
#ifndef HAVE_FOLIO_BASED_WRITE_BEGIN_END
# define HAVE_FOLIO_BASED_WRITE_BEGIN_END \
	(LINUX_VERSION_CODE >= KERNEL_VERSION(6, 12, 0))
#endif
/*
 * from_timer() was replaced with timer_container_of() in kernel 6.16.
 */
#ifndef HAVE_TIMER_CONTAINER_OF
# define HAVE_TIMER_CONTAINER_OF \
	(LINUX_VERSION_CODE >= KERNEL_VERSION(6, 16, 0))
#endif
#endif /* LINUX_VERSION_CODE */


#include <linux/fs.h>

#if HAVE_FOLIO_BASED_WRITE_BEGIN_END
#define compat___block_write_begin(folio, pos, len, get_block) \
	__block_write_begin(folio, pos, len, get_block)
#define compat_block_write_begin(mapping, pos, len, foliop, get_block) \
	block_write_begin(mapping, pos, len, foliop, get_block)
#define compat_block_write_end(file, mapping, pos, len, copied, folio, data) \
	block_write_end(file, mapping, pos, len, copied, folio, data)
#else /* HAVE_FOLIO_BASED_WRITE_BEGIN_END */
#define compat___block_write_begin(folio, pos, len, get_block) \
	__block_write_begin(&(folio)->page, pos, len, get_block)
#define compat_block_write_begin(mapping, pos, len, foliop, get_block)	\
	({								\
		struct page *__page;					\
		int __rc;						\
		__rc = block_write_begin(mapping, pos, len, &__page,	\
					 get_block);			\
		if (likely(!__rc))					\
			*(foliop) = page_folio(__page);			\
		__rc;							\
	})
#define compat_block_write_end(file, mapping, pos, len, copied, folio, data) \
	block_write_end(file, mapping, pos, len, copied, &(folio)->page, data)
#endif /* HAVE_FOLIO_BASED_WRITE_BEGIN_END */

#if !HAVE_TIMER_CONTAINER_OF
#define timer_container_of(var, callback_timer, timer_fieldname) \
	from_timer(var, callback_timer, timer_fieldname)
#endif /* !HAVE_TIMER_CONTAINER_OF */

#endif /* NILFS_KERN_FEATURE_H */
