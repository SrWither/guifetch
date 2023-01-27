// Code by 9prady9
// https://gist.github.com/9prady9/a5e1e8bdbc9dc58b3349

#include <algorithm>
#include <iostream>
#include <string>

#ifdef _WIN32
#include <limits.h>
#include <intrin.h>
typedef unsigned __int32  uint32_t;
#else
#include <stdint.h>
#endif

using namespace std;

#define MAX_INTEL_TOP_LVL 4

class CPUID {
    uint32_t regs[4];

    public:
    explicit CPUID(unsigned funcId, unsigned subFuncId) {
#ifdef _WIN32
        __cpuidex((int *)regs, (int)funcId, (int)subFuncId);

#else
        asm volatile
            ("cpuid" : "=a" (regs[0]), "=b" (regs[1]), "=c" (regs[2]), "=d" (regs[3])
             : "a" (funcId), "c" (subFuncId));
        // ECX is set to zero for CPUID function 4
#endif
    }

    const uint32_t &EAX() const {return regs[0];}
    const uint32_t &EBX() const {return regs[1];}
    const uint32_t &ECX() const {return regs[2];}
    const uint32_t &EDX() const {return regs[3];}
};

class CPUInfo {
    public:
        CPUInfo();
        string  vendor()            const { return mVendorId;   }
        string  model()             const { return mModelName;  }
        int     cores()             const { return mNumCores;   }
        float   cpuSpeedInMHz()     const { return mCPUMHz;     }
        bool    isSSE()             const { return mIsSSE;      }
        bool    isSSE2()            const { return mIsSSE2;     }
        bool    isSSE3()            const { return mIsSSE3;     }
        bool    isSSE41()           const { return mIsSSE41;    }
        bool    isSSE42()           const { return mIsSSE42;    }
        bool    isAVX()             const { return mIsAVX;      }
        bool    isAVX2()            const { return mIsAVX2;     }
        bool    isHyperThreaded()   const { return mIsHTT;      }
        int     logicalCpus()       const { return mNumLogCpus; }

    private:
        // Bit positions for data extractions
        static const uint32_t SSE_POS   = 0x02000000;
        static const uint32_t SSE2_POS  = 0x04000000;
        static const uint32_t SSE3_POS  = 0x00000001;
        static const uint32_t SSE41_POS = 0x00080000;
        static const uint32_t SSE42_POS = 0x00100000;
        static const uint32_t AVX_POS   = 0x10000000;
        static const uint32_t AVX2_POS  = 0x00000020;
        static const uint32_t LVL_NUM   = 0x000000FF;
        static const uint32_t LVL_TYPE  = 0x0000FF00;
        static const uint32_t LVL_CORES = 0x0000FFFF;

        // Attributes
        string mVendorId;
        string mModelName;
        int    mNumSMT;
        int    mNumCores;
        int    mNumLogCpus;
        float  mCPUMHz;
        bool   mIsHTT;
        bool   mIsSSE;
        bool   mIsSSE2;
        bool   mIsSSE3;
        bool   mIsSSE41;
        bool   mIsSSE42;
        bool   mIsAVX;
        bool   mIsAVX2;
};
