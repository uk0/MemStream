#include <string>
#include <stdexcept>
#include <cassert>

#include <vmmdll.h>

#include "MemStream/FPGA.h"
#include "MemStream/Windows/Registry.h"

namespace memstream::windows {

    Registry::Registry() : Registry(GetDefaultFPGA()){}
    Registry::Registry(FPGA *pFPGA) {
        if(!pFPGA)
            throw std::invalid_argument("invalid fpga");

        this->pFPGA = pFPGA;
    }

    Registry::~Registry() = default;

    bool Registry::Query(const std::string &path, const RegistryType& type, std::string &value) {
        assert(this->pFPGA && "null fpga");

        DWORD dwType = (DWORD)type;
        uint8_t buffer[0x128];
        DWORD dwSize = sizeof(buffer);

        bool ok = VMMDLL_WinReg_QueryValueExU(
                this->pFPGA->getVmm(),
                (char*)path.c_str(),
                &dwType,
                buffer,
                &dwSize);
        if(!ok) return false;

        // copy buffer into value
        //TODO: is `buffer` unicode ?
        //  std::wstring wstr = std::wstring((wchar_t*)buffer);
       //    value = std::string(wstr.begin(), wstr.end());
        value = (const char*)buffer;
        return true;
    }
}