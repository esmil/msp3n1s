#ifndef _MACROS_H
#define _MACROS_H

#define __bis_w(mask, sfr) __asm__ __volatile__("bis.w %0, %1"::"i" (mask), "im" (sfr))
#define __bic_w(mask, sfr) __asm__ __volatile__("bic.w %0, %1"::"i" (mask), "im" (sfr))

#define __bis_b(mask, sfr) __asm__ __volatile__("bis.b %0, %1"::"i" (mask), "im" (sfr))
#define __bic_b(mask, sfr) __asm__ __volatile__("bic.b %0, %1"::"i" (mask), "im" (sfr))

#endif
