#pragma once

#include <cstdint>
#include <unordered_map>

#include "barretenberg/vm2/common/opcodes.hpp"

namespace bb::avm2::tracegen {
enum class SubtraceSel : uint8_t {
    ALU,
    BITWISE,
    TORADIXBE,
    POSEIDON2PERM,
    ECC,
    DATACOPY,
    EXECUTION,
    // Temp
    OTHERS,
};

struct SubtraceInfo {
    SubtraceSel subtrace_selector;
    uint8_t subtrace_operation_id;
};

// Non traditional builder
class RegisterMemInfo {
  public:
    uint16_t encode() const { return encoded_register_info; }
    RegisterMemInfo& has_inputs(uint16_t num_inputs)
    {
        for (uint16_t i = 0; i < num_inputs; ++i) {
            encoded_register_info |= (read_encoding << (i * 2));
        }
        write_index += num_inputs != 0 ? num_inputs : 2; // Hack in case of no inputs
        return *this;
    }
    RegisterMemInfo& has_outputs(uint16_t num_outputs)
    {
        for (uint16_t i = 0; i < num_outputs; ++i) {
            encoded_register_info |= (write_encoding << (write_index * 2));
            write_index++;
        }
        return *this;
    }

  private:
    static const uint16_t read_encoding = 0b01;
    static const uint16_t write_encoding = 0b11;
    uint16_t encoded_register_info = 0;
    uint16_t write_index = 0;
};

extern const std::unordered_map<ExecutionOpCode, SubtraceInfo> SUBTRACE_INFO_MAP;
extern const std::unordered_map<ExecutionOpCode, uint16_t> REGISTER_INFO_MAP;
} // namespace bb::avm2::tracegen
