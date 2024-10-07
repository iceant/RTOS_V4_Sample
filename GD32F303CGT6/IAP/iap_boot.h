#ifndef INCLUDED_IAP_BOOT_H
#define INCLUDED_IAP_BOOT_H

typedef void (*IAP_Function_T)(void);
void IAP_JumpToApplication(unsigned int address);

#endif /* INCLUDED_IAP_BOOT_H */
