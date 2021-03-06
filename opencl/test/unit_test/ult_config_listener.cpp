/*
 * Copyright (C) 2018-2020 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "opencl/test/unit_test/ult_config_listener.h"

#include "shared/source/execution_environment/execution_environment.h"
#include "shared/source/memory_manager/memory_manager.h"
#include "shared/test/unit_test/helpers/default_hw_info.h"
#include "shared/test/unit_test/helpers/ult_hw_config.h"

#include "opencl/source/platform/platform.h"
#include "opencl/test/unit_test/mocks/mock_platform.h"

#include "third_party/aub_stream/headers/aubstream.h"

namespace NEO {
namespace GlobalMockSipProgram {
void resetAllocationState();
}
} // namespace NEO

void NEO::UltConfigListener::OnTestStart(const ::testing::TestInfo &testInfo) {
    GlobalMockSipProgram::resetAllocationState();
    referencedHwInfo = *defaultHwInfo;
    auto executionEnvironment = constructPlatform()->peekExecutionEnvironment();
    executionEnvironment->prepareRootDeviceEnvironments(1);
    executionEnvironment->rootDeviceEnvironments[0]->setHwInfo(defaultHwInfo.get());
    executionEnvironment->calculateMaxOsContextCount();
    executionEnvironment->rootDeviceEnvironments[0]->initGmm();
}
void NEO::UltConfigListener::OnTestEnd(const ::testing::TestInfo &testInfo) {
    // Clear global platform that it shouldn't be reused between tests
    platformsImpl->clear();
    MemoryManager::maxOsContextCount = 0u;
    aub_stream::injectMMIOList(aub_stream::MMIOList{});

    // Ensure that global state is restored
    UltHwConfig expectedState{};
    static_assert(sizeof(UltHwConfig) == 9 * sizeof(bool), ""); // Ensure that there is no internal padding
    EXPECT_EQ(0, memcmp(&expectedState, &ultHwConfig, sizeof(UltHwConfig)));
    EXPECT_EQ(0, memcmp(&referencedHwInfo, defaultHwInfo.get(), sizeof(HardwareInfo)));
}
