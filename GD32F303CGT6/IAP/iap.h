#ifndef INCLUDED_IAP_H
#define INCLUDED_IAP_H

#ifndef INCLUDED_OS_CONFIG_H
#include <os_config.h>
#endif /*INCLUDED_OS_CONFIG_H*/

#ifndef INCLUDED_OS_KERNEL_H
#include <os_kernel.h>
#endif /*INCLUDED_OS_KERNEL_H*/

/* -------------------------------------------------------------------------------------------------------------- */
/*  */

#if defined(IAP_USED_IN_APPLICATION)
#ifndef INCLUDED_IAP_APP_H
#include <iap_app.h>
#endif /*INCLUDED_IAP_APP_H*/
#endif

#if defined(IAP_USED_IN_BOOTLOADER)
#ifndef INCLUDED_IAP_BOOT_H
#include <iap_boot.h>
#endif /*INCLUDED_IAP_BOOT_H*/
#endif


#endif /* INCLUDED_IAP_H */
