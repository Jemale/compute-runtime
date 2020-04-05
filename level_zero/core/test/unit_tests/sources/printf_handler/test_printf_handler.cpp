/*
 * Copyright (C) 2020 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "shared/test/unit_test/mocks/mock_device.h"

#include "test.h"

#include "level_zero/core/source/printf_handler/printf_handler.h"
#include "level_zero/core/test/unit_tests/mocks/mock_device.h"

namespace L0 {
namespace ult {

TEST(PrintfHandler, whenPrintfBufferIscreatedThenCorrectAllocationTypeIsUsed) {
    NEO::Device *neoDevice(NEO::MockDevice::createWithNewExecutionEnvironment<NEO::MockDevice>(NEO::defaultHwInfo.get(), 0));
    Mock<L0::DeviceImp> l0Device(neoDevice, neoDevice->getExecutionEnvironment());

    auto allocation = PrintfHandler::createPrintfBuffer(&l0Device);
    EXPECT_EQ(NEO::GraphicsAllocation::AllocationType::PRINTF_SURFACE, allocation->getAllocationType());
    neoDevice->getMemoryManager()->freeGraphicsMemory(allocation);
}

} // namespace ult
} // namespace L0