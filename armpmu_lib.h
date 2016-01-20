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

// Read register from coprocessor.
#define xMRC_PMU(REG, PMU_REG) \
	asm volatile("mrc p15, 0, %0, " PMU_REG : "=r"(REG) );

// Write register to coprocessor.
#define xMCR_PMU(REG, PMU_REG) \
	asm volatile("mcr p15, 0, %0, " PMU_REG : /* no out */ : "r"(REG) );

#define MRC_PMU(REG, PMU_REG) xMRC_PMU(REG, PMU_REG)
#define MCR_PMU(REG, PMU_REG) xMCR_PMU(REG, PMU_REG)

// Events as X Macro
#define ARMPMU_EVENT_LIST \
    X(SW_INCR, 0x00) \
    X(L1I_CACHE_REFILL, 0x01) \
    X(L1I_TLB_REFILL, 0x02) \
    X(L1D_CACHE_REFILL, 0x03) \
    X(L1D_CACHE, 0x04) \
    X(L1D_TLB_REFILL, 0x05) \
    X(INST_RETIRED, 0x08) \
    X(EXC_TAKEN, 0x09) \
    X(EXC_RETURN, 0x0A) \
    X(CID_WRITE_RETIRED, 0x0B) \
    X(BR_MIS_PRED, 0x10) \
    X(CPU_CYCLES, 0x11) \
    X(BR_PRED, 0x12) \
    X(MEM_ACCESS, 0x13) \
    X(L1I_CACHE, 0x14) \
    X(L1D_CACHE_WB, 0x15) \
    X(L2D_CACHE, 0x16) \
    X(L2D_CACHE_REFILL, 0x17) \
    X(L2D_CACHE_WB, 0x18) \
    X(BUS_ACCESS, 0x19) \
    X(MEMORY_ERROR, 0x1A) \
    X(INST_SPEC, 0x1B) \
    X(TTBR_WRITE_RETIRED, 0x1C) \
    X(BUS_CYCLES, 0x1D) \
    X(L1D_CACHE_LD, 0x40) \
    X(L1D_CACHE_ST, 0x41) \
    X(L1D_CACHE_REFILL_LD, 0x42) \
    X(L1D_CACHE_REFILL_ST, 0x43) \
    X(L1D_CACHE_WB_VICTIM, 0x46) \
    X(L1D_CACHE_WB_CLEAN, 0x47) \
    X(L1D_CACHE_INVAL, 0x48) \
    X(L1D_TLB_REFILL_LD, 0x4C) \
    X(L1D_TLB_REFILL_ST, 0x4D) \
    X(L2D_CACHE_LD, 0x50) \
    X(L2D_CACHE_ST, 0x51) \
    X(L2D_CACHE_REFILL_LD, 0x52) \
    X(L2D_CACHE_REFILL_ST, 0x53) \
    X(L2D_CACHE_WB_VICTIM, 0x56) \
    X(L2D_CACHE_WB_CLEAN, 0x57) \
    X(L2D_CACHE_INVAL, 0x58) \
    X(BUS_ACCESS_LD, 0x60) \
    X(BUS_ACCESS_ST, 0x61) \
    X(BUS_ACCESS_SHARED, 0x62) \
    X(BUS_ACCESS_NOT_SHARED, 0x63) \
    X(BUS_ACCESS_NORMAL, 0x64) \
    X(BUS_ACCESS_PERIPH, 0x65) \
    X(MEM_ACCESS_LD, 0x66) \
    X(MEM_ACCESS_ST, 0x67) \
    X(UNALIGNED_LD_SPEC, 0x68) \
    X(UNALIGNED_ST_SPEC, 0x69) \
    X(UNALIGNED_LDST_SPEC, 0x6A) \
    X(LDREX_SPEC, 0x6C) \
    X(STREX_PASS_SPEC, 0x6D) \
    X(STREX_FAIL_SPEC, 0x6E) \
    X(LD_SPEC, 0x70) \
    X(ST_SPEC, 0x71) \
    X(LDST_SPEC, 0x72) \
    X(DP_SPEC, 0x73) \
    X(ASE_SPEC, 0x74) \
    X(VFP_SPEC, 0x75) \
    X(PC_WRITE_SPEC, 0x76) \
    X(BR_IMMED_SPEC, 0x78) \
    X(BR_RETURN_SPEC, 0x79) \
    X(BR_INDIRECT_SPEC, 0x7A) \
    X(ISB_SPEC, 0x7C) \
    X(DSB_SPEC, 0x7D) \
    X(DMB_SPEC, 0x7E)

enum {
#define X(NAME, VALUE) NAME = VALUE,
    ARMPMU_EVENT_LIST
#undef X
};

// Convenience functions

// Enables performance monitoring.
void enable_pmn();
// Disables performance monitoring.
void disable_pmn();

// Sets the given counter to monitor a particular event.
void set_pmn(uint32_t counter, uint32_t event);

// Returns the value of the given counter.
uint32_t read_pmn(uint32_t counter);

// Resets all counters.
void reset_pmn();

// Resets the Cycle Count (PMCCNTR).
void reset_ccnt();

// Returns an event name from its value.
const char * pmn_event_name(int event);

#else
#error Unsupported architecture/compiler!
#endif

#endif /* ARMPMU_LIB_H */
