#ifndef _MACROS_H
#define _MACROS_H

#define __bis_w(sfr, mask) __asm__ __volatile__("bis.w %1, %0":"=m" (sfr):"ir" (mask))
#define __bic_w(sfr, mask) __asm__ __volatile__("bic.w %1, %0":"=m" (sfr):"ir" (mask))

#define __bis_b(sfr, mask) __asm__ __volatile__("bis.b %1, %0":"=m" (sfr):"ir" (mask))
#define __bic_b(sfr, mask) __asm__ __volatile__("bic.b %1, %0":"=m" (sfr):"ir" (mask))

#endif
