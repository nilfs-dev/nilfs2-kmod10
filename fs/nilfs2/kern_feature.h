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
# endif
#endif

/*
 * defaults
 */

/*
 * defaults dependent to kernel versions
 */
#ifdef LINUX_VERSION_CODE
#endif /* LINUX_VERSION_CODE */


#include <linux/fs.h>

#endif /* NILFS_KERN_FEATURE_H */
