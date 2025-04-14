#include "barretenberg/vm2/tracegen/lib/instruction_spec.hpp"

#include <array>
#include <cstdint>
#include <unordered_map>
#include <vector>

#include "barretenberg/vm2/common/aztec_constants.hpp"
#include "barretenberg/vm2/common/opcodes.hpp"

namespace bb::avm2::tracegen {

const std::unordered_map<ExecutionOpCode, SubtraceInfo> SUBTRACE_INFO_MAP = {
    // Map each ExecutionOpcode to a SubtraceInfo
    { ExecutionOpCode::ADD, { .subtrace_selector = SubtraceSel::ALU, .subtrace_operation_id = 0 } },
    { ExecutionOpCode::SUB, { .subtrace_selector = SubtraceSel::ALU, .subtrace_operation_id = 1 } },
    { ExecutionOpCode::MUL, { .subtrace_selector = SubtraceSel::ALU, .subtrace_operation_id = 2 } },
    { ExecutionOpCode::DIV, { .subtrace_selector = SubtraceSel::ALU, .subtrace_operation_id = 3 } },
    { ExecutionOpCode::FDIV, { .subtrace_selector = SubtraceSel::ALU, .subtrace_operation_id = 4 } },
    { ExecutionOpCode::EQ, { .subtrace_selector = SubtraceSel::ALU, .subtrace_operation_id = 5 } },
    { ExecutionOpCode::LT, { .subtrace_selector = SubtraceSel::ALU, .subtrace_operation_id = 6 } },
    { ExecutionOpCode::LTE, { .subtrace_selector = SubtraceSel::ALU, .subtrace_operation_id = 7 } },
    // Bitwise
    { ExecutionOpCode::AND, { .subtrace_selector = SubtraceSel::BITWISE, .subtrace_operation_id = 0 } },
    { ExecutionOpCode::OR, { .subtrace_selector = SubtraceSel::BITWISE, .subtrace_operation_id = 1 } },
    { ExecutionOpCode::XOR, { .subtrace_selector = SubtraceSel::BITWISE, .subtrace_operation_id = 2 } },
    // Toradixbe
    { ExecutionOpCode::TORADIXBE, { .subtrace_selector = SubtraceSel::TORADIXBE, .subtrace_operation_id = 0 } },
    // ECC
    { ExecutionOpCode::ECADD, { .subtrace_selector = SubtraceSel::ECC, .subtrace_operation_id = 0 } },
    // Data Copy
    { ExecutionOpCode::CALLDATACOPY, { .subtrace_selector = SubtraceSel::DATACOPY, .subtrace_operation_id = 0 } },
    { ExecutionOpCode::RETURNDATACOPY, { .subtrace_selector = SubtraceSel::DATACOPY, .subtrace_operation_id = 1 } },
    // Poseidon2Perm
    { ExecutionOpCode::POSEIDON2PERM, { .subtrace_selector = SubtraceSel::POSEIDON2PERM, .subtrace_operation_id = 0 } },
    // Execution
    { ExecutionOpCode::SET, { .subtrace_selector = SubtraceSel::EXECUTION, .subtrace_operation_id = 1 } },
    { ExecutionOpCode::MOV, { .subtrace_selector = SubtraceSel::EXECUTION, .subtrace_operation_id = 2 } },
    { ExecutionOpCode::JUMP, { .subtrace_selector = SubtraceSel::EXECUTION, .subtrace_operation_id = 3 } },
    { ExecutionOpCode::JUMPI, { .subtrace_selector = SubtraceSel::EXECUTION, .subtrace_operation_id = 4 } },
    { ExecutionOpCode::CALL, { .subtrace_selector = SubtraceSel::EXECUTION, .subtrace_operation_id = 5 } },
    { ExecutionOpCode::STATICCALL, { .subtrace_selector = SubtraceSel::EXECUTION, .subtrace_operation_id = 6 } },
    { ExecutionOpCode::INTERNALCALL, { .subtrace_selector = SubtraceSel::EXECUTION, .subtrace_operation_id = 7 } },
    { ExecutionOpCode::INTERNALRETURN, { .subtrace_selector = SubtraceSel::EXECUTION, .subtrace_operation_id = 8 } },
    { ExecutionOpCode::RETURN, { .subtrace_selector = SubtraceSel::EXECUTION, .subtrace_operation_id = 9 } },
    { ExecutionOpCode::REVERT, { .subtrace_selector = SubtraceSel::EXECUTION, .subtrace_operation_id = 10 } },
    { ExecutionOpCode::SUCCESSCOPY, { .subtrace_selector = SubtraceSel::EXECUTION, .subtrace_operation_id = 11 } },
    { ExecutionOpCode::RETURNDATASIZE, { .subtrace_selector = SubtraceSel::EXECUTION, .subtrace_operation_id = 12 } },
    // Others
    { ExecutionOpCode::SLOAD, { .subtrace_selector = SubtraceSel::OTHERS, .subtrace_operation_id = 0 } },
    { ExecutionOpCode::SSTORE, { .subtrace_selector = SubtraceSel::OTHERS, .subtrace_operation_id = 1 } },
    { ExecutionOpCode::NOTEHASHEXISTS, { .subtrace_selector = SubtraceSel::OTHERS, .subtrace_operation_id = 2 } },
    { ExecutionOpCode::EMITNOTEHASH, { .subtrace_selector = SubtraceSel::OTHERS, .subtrace_operation_id = 3 } },
    { ExecutionOpCode::NULLIFIEREXISTS, { .subtrace_selector = SubtraceSel::OTHERS, .subtrace_operation_id = 4 } },
    { ExecutionOpCode::EMITNULLIFIER, { .subtrace_selector = SubtraceSel::OTHERS, .subtrace_operation_id = 5 } },
    { ExecutionOpCode::L1TOL2MSGEXISTS, { .subtrace_selector = SubtraceSel::OTHERS, .subtrace_operation_id = 6 } },
    { ExecutionOpCode::GETCONTRACTINSTANCE, { .subtrace_selector = SubtraceSel::OTHERS, .subtrace_operation_id = 7 } },
    { ExecutionOpCode::EMITUNENCRYPTEDLOG, { .subtrace_selector = SubtraceSel::OTHERS, .subtrace_operation_id = 8 } },
    { ExecutionOpCode::SENDL2TOL1MSG, { .subtrace_selector = SubtraceSel::OTHERS, .subtrace_operation_id = 9 } },
    { ExecutionOpCode::DEBUGLOG, { .subtrace_selector = SubtraceSel::OTHERS, .subtrace_operation_id = 0 } },
    { ExecutionOpCode::SHA256COMPRESSION, { .subtrace_selector = SubtraceSel::OTHERS, .subtrace_operation_id = 1 } },
    { ExecutionOpCode::KECCAKF1600, { .subtrace_selector = SubtraceSel::OTHERS, .subtrace_operation_id = 2 } }
};

// Maps Execution opcodes to their register + memory accesses
// TODO: This will need to revisited, we will only be sure of the access patterns when we do the opcodes
const std::unordered_map<ExecutionOpCode, uint16_t> REGISTER_INFO_MAP = { {
    { ExecutionOpCode::ADD, RegisterMemInfo().has_inputs(2).has_outputs(1).encode() },
    { ExecutionOpCode::SET, RegisterMemInfo().has_inputs(0).has_outputs(1).encode() },
    { ExecutionOpCode::MOV, RegisterMemInfo().has_inputs(1).has_outputs(1).encode() },
    { ExecutionOpCode::CALL, RegisterMemInfo().has_inputs(3).encode() },
    { ExecutionOpCode::RETURN, RegisterMemInfo().has_inputs(1).encode() },
    { ExecutionOpCode::JUMP, RegisterMemInfo().encode() },
    { ExecutionOpCode::JUMPI, RegisterMemInfo().has_inputs(1).encode() },
} };

} // namespace bb::avm2::tracegen
