#ifndef ARMPMU_LIB_H
#define ARMPMU_LIB_H

#include <stdint.h>

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
#define PMUSERENR  PMU_REGISTER(c9, c14, 0) // User Enable Register
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

// Events
enum {
    SW_INCR               = 0x00,
    L1I_CACHE_REFILL      = 0x01,
    L1I_TLB_REFILL        = 0x02,
    L1D_CACHE_REFILL      = 0x03,
    L1D_CACHE             = 0x04,
    L1D_TLB_REFILL        = 0x05,
    INST_RETIRED          = 0x08,
    EXC_TAKEN             = 0x09,
    EXC_RETURN            = 0x0A,
    CID_WRITE_RETIRED     = 0x0B,
    BR_MIS_PRED           = 0x10,
    CPU_CYCLES            = 0x11,
    BR_PRED               = 0x12,
    MEM_ACCESS            = 0x13,
    L1I_CACHE             = 0x14,
    L1D_CACHE_WB          = 0x15,
    L2D_CACHE             = 0x16,
    L2D_CACHE_REFILL      = 0x17,
    L2D_CACHE_WB          = 0x18,
    BUS_ACCESS            = 0x19,
    MEMORY_ERROR          = 0x1A,
    INST_SPEC             = 0x1B,
    TTBR_WRITE_RETIRED    = 0x1C,
    BUS_CYCLES            = 0x1D,
    L1D_CACHE_LD          = 0x40,
    L1D_CACHE_ST          = 0x41,
    L1D_CACHE_REFILL_LD   = 0x42,
    L1D_CACHE_REFILL_ST   = 0x43,
    L1D_CACHE_WB_VICTIM   = 0x46,
    L1D_CACHE_WB_CLEAN    = 0x47,
    L1D_CACHE_INVAL       = 0x48,
    L1D_TLB_REFILL_LD     = 0x4C,
    L1D_TLB_REFILL_ST     = 0x4D,
    L2D_CACHE_LD          = 0x50,
    L2D_CACHE_ST          = 0x51,
    L2D_CACHE_REFILL_LD   = 0x52,
    L2D_CACHE_REFILL_ST   = 0x53,
    L2D_CACHE_WB_VICTIM   = 0x56,
    L2D_CACHE_WB_CLEAN    = 0x57,
    L2D_CACHE_INVAL       = 0x58,
    BUS_ACCESS_LD         = 0x60,
    BUS_ACCESS_ST         = 0x61,
    BUS_ACCESS_SHARED     = 0x62,
    BUS_ACCESS_NOT_SHARED = 0x63,
    BUS_ACCESS_NORMAL     = 0x64,
    BUS_ACCESS_PERIPH     = 0x65,
    MEM_ACCESS_LD         = 0x66,
    MEM_ACCESS_ST         = 0x67,
    UNALIGNED_LD_SPEC     = 0x68,
    UNALIGNED_ST_SPEC     = 0x69,
    UNALIGNED_LDST_SPEC   = 0x6A,
    LDREX_SPEC            = 0x6C,
    STREX_PASS_SPEC       = 0x6D,
    STREX_FAIL_SPEC       = 0x6E,
    LD_SPEC               = 0x70,
    ST_SPEC               = 0x71,
    LDST_SPEC             = 0x72,
    DP_SPEC               = 0x73,
    ASE_SPEC              = 0x74,
    VFP_SPEC              = 0x75,
    PC_WRITE_SPEC         = 0x76,
    BR_IMMED_SPEC         = 0x78,
    BR_RETURN_SPEC        = 0x79,
    BR_INDIRECT_SPEC      = 0x7A,
    ISB_SPEC              = 0x7C,
    DSB_SPEC              = 0x7D,
    DMB_SPEC              = 0x7E,
};

// Convenience functions

// Returns the value of the given counter.
uint32_t read_pmn(uint32_t counter);

// Resets all counters.
void reset_pmn();

// Resets the Cycle Count (PMCCNTR).
void reset_ccnt();

#else
#error Unsupported architecture/compiler!
#endif

#endif /* ARMPMU_LIB_H */
