#ifndef ARMPMU_LIB_H
#define ARMPMU_LIB_H

#if defined(__GNUC__) && defined(__ARM_ARCH_7A__)

#define PMU_REGISTER(CRn, CRm, Op2) \
    #CRn", "#CRm", "#Op2

#define PMXEVCNTR  PMU_REGISTER(c9, c13, 2) // Event Count Register 0 - 5
#define PMCCNTR    PMU_REGISTER(c9, c13, 0) // Cycle Count Register
#define PMXEVTYPER PMU_REGISTER(c9, c13, 1) // Event Type Select Register 0 - 5, 31
#define PMCNTENSET PMU_REGISTER(c9, c12, 1) // Count Enable Set Register
#define PMCNTENCLR PMU_REGISTER(c9, c12, 2) // Count Enable Clear Register
#define PMINTENSET PMU_REGISTER(c9, c14, 1) // Interrupt Enable Set Register
#define PMINTENCLR PMU_REGISTER(c9, c14, 2) // Interrupt Enable Clear Register
#define PMOVSR     PMU_REGISTER(c9, c12, 3) // Overflow Flag Status Register
#define PMSWINC    PMU_REGISTER(c9, c12, 4) // Software Increment Register
#define PMOVSSET   PMU_REGISTER(c9, c14, 3) // Performance Monitor Overflow Status Set Register
#define PMCR       PMU_REGISTER(c9, c12, 0) // Performance Monitor Control Register
#define PMCEID0    PMU_REGISTER(c9, c12, 6) // Common Event Identification Register 0
#define PMCEID1    PMU_REGISTER(c9, c12, 7) // Common Event Identification Register 1
#define PMSELR     PMU_REGISTER(c9, c12, 5) // Event Counter Selection Register

// Move register to coprocessor.
#define xMRC_PMU(REG, PMU_REG) \
	asm volatile("mrc p15, 0, %0, " PMU_REG : "=r"(REG) );

// Move from coprocessor to register.
#define xMCR_PMU(REG, PMU_REG) \
	asm volatile("mcr p15, 0, %0, " PMU_REG : "=r"(REG) );

#define MRC_PMU(REG, PMU_REG) xMRC_PMU(REG, PMU_REG)
#define MCR_PMU(REG, PMU_REG) xMCR_PMU(REG, PMU_REG)

#else
#error Unsupported architecture/compiler!
#endif

#endif /* ARMPMU_LIB_H */
