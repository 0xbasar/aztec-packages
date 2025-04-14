#include "barretenberg/vm2/tracegen/execution_trace.hpp"
#include "gmock/gmock.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <cstdint>

#include "barretenberg/vm2/common/aztec_constants.hpp"
#include "barretenberg/vm2/constraining/flavor_settings.hpp"
#include "barretenberg/vm2/constraining/full_row.hpp"
#include "barretenberg/vm2/tracegen/range_check_trace.hpp"
#include "barretenberg/vm2/tracegen/test_trace_container.hpp"

namespace bb::avm2::tracegen {
namespace {

using testing::AllOf;
using testing::Contains;
using testing::Field;

using R = TestTraceContainer::Row;
using FF = R::FF;

TEST(ExecutionTraceGenTest, RegisterAllocation)
{
    TestTraceContainer trace;
    ExecutionTraceBuilder builder;

    // Some inputs
    ExecInstructionSpec spec = {
        .num_addresses = 3, .gas_cost = { .base_l2 = AVM_ADD_BASE_L2_GAS, .base_da = 0, .dyn_l2 = 0, .dyn_da = 0 }
    };
    simulation::AddressingEvent addressing_event{
        .instruction = simulation::Instruction(),
        .spec = &spec,
    };

    auto ex_event = simulation::ExecutionEvent::allocate();
    ex_event.inputs = { TaggedValue::from_tag(ValueTag::U16, 5), TaggedValue::from_tag(ValueTag::U16, 3) };
    ex_event.output = { TaggedValue::from_tag(ValueTag::U16, 8) };
    ex_event.opcode = ExecutionOpCode::ADD;

    ex_event.addressing_event = addressing_event;

    builder.process({ ex_event }, trace);
    // todo: Test doesnt check the other register fields are zeroed out.
    EXPECT_THAT(trace.as_rows(),
                AllOf(Contains(Field(&R::execution_sel, 1)),
                      Contains(Field(&R::execution_alu_sel, 1)),
                      Contains(Field(&R::execution_reg1, 5)),
                      Contains(Field(&R::execution_reg2, 3)),
                      Contains(Field(&R::execution_reg3, 8)),
                      Contains(Field(&R::execution_mem_tag1, /*U16=*/3)),
                      Contains(Field(&R::execution_mem_tag2, /*U16=*/3)),
                      Contains(Field(&R::execution_mem_tag3, /*U16=*/3)),
                      Contains(Field(&R::execution_mem_op1, 1)),
                      Contains(Field(&R::execution_mem_op2, 1)),
                      Contains(Field(&R::execution_mem_op3, 1)),
                      Contains(Field(&R::execution_rw1, 0)),
                      Contains(Field(&R::execution_rw2, 0)),
                      Contains(Field(&R::execution_rw3, 1))));
}
} // namespace
} // namespace bb::avm2::tracegen
