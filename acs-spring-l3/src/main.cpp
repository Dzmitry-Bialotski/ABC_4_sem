#include "Cpu.h"
#include "Memory.h"
#include "BaseTypes.h"

#include <optional>

int main()
{
    MemoryStorage mem ;
    mem.LoadElf("/home/dimasiandro/Desktop/ABC/acs-spring-l3/program");
    std::unique_ptr<IMem> memModelPtr(new UncachedMem (mem));
    Cpu cpu{*memModelPtr};
    cpu.Reset(0x200);

    int32_t print_int = 0;
    fprintf(stderr, "   Uncashed:\n");
    while (true)
    {
        cpu.Clock();
        memModelPtr->Clock();
        std::optional<CpuToHostData> msg = cpu.GetMessage();
        if (!msg)
            continue;

        auto type = msg.value().unpacked.type;
        auto data = msg.value().unpacked.data;

        if(type == CpuToHostType::ExitCode) {
            if(data == 0) {
                fprintf(stderr, "PASSED\n");
                break;
            } else {
                fprintf(stderr, "FAILED: exit code = %d\n", data);
                return data;
            }
        } else if(type == CpuToHostType::PrintChar) {
            fprintf(stderr, "%c", (char)data);
        } else if(type == CpuToHostType::PrintIntLow) {
            print_int = uint32_t(data);
        } else if(type == CpuToHostType::PrintIntHigh) {
            print_int |= uint32_t(data) << 16;
            fprintf(stderr, "%d", print_int);
        }
    }

    MemoryStorage cashed_mem ;
    cashed_mem.LoadElf("/home/dimasiandro/Desktop/ABC/acs-spring-l3/program");
    std::unique_ptr<IMem> cashed_memModelPtr(new CachedMem (cashed_mem));
    Cpu cashed_cpu{*cashed_memModelPtr};
    cashed_cpu.Reset(0x200);
    fprintf(stderr, "   Cashed:\n");
    int32_t cashed_print_int = 0;
    while (true)
    {
        cashed_cpu.Clock();
        cashed_memModelPtr->Clock();
        std::optional<CpuToHostData> msg = cashed_cpu.GetMessage();
        if (!msg)
            continue;

        auto type = msg.value().unpacked.type;
        auto data = msg.value().unpacked.data;

        if(type == CpuToHostType::ExitCode) {
            if(data == 0) {
                fprintf(stderr, "PASSED\n");
                break;
            } else {
                fprintf(stderr, "FAILED: exit code = %d\n", data);
                return data;
            }
        } else if(type == CpuToHostType::PrintChar) {
            fprintf(stderr, "%c", (char)data);
        } else if(type == CpuToHostType::PrintIntLow) {
            cashed_print_int = uint32_t(data);
        } else if(type == CpuToHostType::PrintIntHigh) {
            cashed_print_int |= uint32_t(data) << 16;
            fprintf(stderr, "%d", cashed_print_int);
        }
    }
}
