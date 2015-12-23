#ifndef ARMPMU_LIB_H
#define ARMPMU_LIB_H

#if defined(__GNUC__) && defined(__ARM_ARCH_7A__)

// Move register to coprocessor.
#define MRC_PMU(REG, CRn, CRm, Op2) \
	asm volatile("mrc p15, 0, %0, "#CRn", "#CRm", "#Op2 : "=r"(REG) );

// Move from coprocessor to register.
#define MCR_PMU(REG, CRn, CRm, Op2) \
	asm volatile("mcr p15, 0, %0, "#CRn", "#CRm", "#Op2 : "=r"(REG) );

#else
#error Unsupported architecture/compiler!
#endif

#endif /* ARMPMU_LIB_H */
