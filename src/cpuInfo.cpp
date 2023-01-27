// Code by 9prady9
// https://gist.github.com/9prady9/a5e1e8bdbc9dc58b3349

#include <cpuInfo.hpp>

CPUInfo::CPUInfo()
{
    // Get vendor name EAX=0
    CPUID cpuID0(0, 0);
    uint32_t HFS = cpuID0.EAX();
    mVendorId += string((const char *)&cpuID0.EBX(), 4);
    mVendorId += string((const char *)&cpuID0.EDX(), 4);
    mVendorId += string((const char *)&cpuID0.ECX(), 4);
    // Get SSE instructions availability
    CPUID cpuID1(1, 0);
    mIsHTT   = cpuID1.EDX() & AVX_POS;
    mIsSSE   = cpuID1.EDX() & SSE_POS;
    mIsSSE2  = cpuID1.EDX() & SSE2_POS;
    mIsSSE3  = cpuID1.ECX() & SSE3_POS;
    mIsSSE41 = cpuID1.ECX() & SSE41_POS;
    mIsSSE42 = cpuID1.ECX() & SSE41_POS;
    mIsAVX   = cpuID1.ECX() & AVX_POS;
    // Get AVX2 instructions availability
    CPUID cpuID7(7, 0);
    mIsAVX2  = cpuID7.EBX() & AVX2_POS;

    string upVId = mVendorId;
    for_each(upVId.begin(), upVId.end(), [](char& in) { in = ::toupper(in); });
    // Get num of cores
    if (upVId.find("INTEL") != std::string::npos) {
        if(HFS >= 11) {
            for (int lvl=0; lvl<MAX_INTEL_TOP_LVL; ++lvl) {
                    CPUID cpuID4(0x0B, lvl);
                    uint32_t currLevel = (LVL_TYPE & cpuID4.ECX())>>8;
                    switch(currLevel) {
                        case 0x01: mNumSMT     = LVL_CORES & cpuID4.EBX(); break;
                        case 0x02: mNumLogCpus = LVL_CORES & cpuID4.EBX(); break;
                        default: break;
                    }
            }
            mNumCores = mNumLogCpus/mNumSMT;
        } else {
            if (HFS>=1) {
                mNumLogCpus = (cpuID1.EBX() >> 16) & 0xFF;
                if (HFS>=4) {
                    mNumCores = 1 + (CPUID(4, 0).EAX() >> 26) & 0x3F;
                }
            }
            if (mIsHTT) {
                if (!(mNumCores>1)) {
                    mNumCores = 1;
                    mNumLogCpus = (mNumLogCpus >= 2 ? mNumLogCpus : 2);
                }
            } else {
                mNumCores = mNumLogCpus = 1;
            }
        }
    } else if (upVId.find("AMD") != std::string::npos) {
        if (HFS>=1) {
            mNumLogCpus = (cpuID1.EBX() >> 16) & 0xFF;
            if (CPUID(0x80000000, 0).EAX() >=8) {
                mNumCores = 1 + (CPUID(0x80000008, 0).ECX() & 0xFF);
            }
        }
        if (mIsHTT) {
            if (!(mNumCores>1)) {
                mNumCores = 1;
                mNumLogCpus = (mNumLogCpus >= 2 ? mNumLogCpus : 2);
            }
        } else {
            mNumCores = mNumLogCpus = 1;
        }
    } else {
        cout<< "Unexpected vendor id" <<endl;
    }
    // Get processor brand string
    // This seems to be working for both Intel & AMD vendors
    for(int i=0x80000002; i<0x80000005; ++i) {
        CPUID cpuID(i, 0);
        mModelName += string((const char*)&cpuID.EAX(), 4);
        mModelName += string((const char*)&cpuID.EBX(), 4);
        mModelName += string((const char*)&cpuID.ECX(), 4);
        mModelName += string((const char*)&cpuID.EDX(), 4);
    }
}
