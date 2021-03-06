/*
 * Copyright (C) 2020 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "test.h"

#include "level_zero/tools/test/unit_tests/sources/sysman/linux/mock_sysman_fixture.h"

namespace L0 {
namespace ult {

using MockDeviceSysmanGetTest = Test<DeviceFixture>;
TEST_F(MockDeviceSysmanGetTest, GivenValidSysmanHandleSetInDeviceStructWhenGetThisSysmanHandleThenHandlesShouldBeSimilar) {
    SysmanDeviceImp *sysman = new SysmanDeviceImp(device->toHandle());
    device->setSysmanHandle(sysman);
    EXPECT_EQ(sysman, device->getSysmanHandle());
}

TEST_F(SysmanDeviceFixture, GivenValidDeviceHandleInSysmanInitThenValidSysmanHandleReceived) {
    ze_device_handle_t hSysman = device->toHandle();
    auto pSysmanDevice = L0::SysmanDeviceHandleContext::init(hSysman);
    EXPECT_NE(pSysmanDevice, nullptr);
    delete pSysmanDevice;
    pSysmanDevice = nullptr;
}

TEST_F(SysmanDeviceFixture, GivenSetValidDrmHandleForDeviceWhenDoingOsSysmanDeviceInitThenSameDrmHandleIsRetrieved) {
    EXPECT_EQ(&pLinuxSysmanImp->getDrm(), device->getOsInterface().get()->getDrm());
}

TEST_F(SysmanDeviceFixture, GivenCreateFsAccessHandleWhenCallinggetFsAccessThenCreatedFsAccessHandleWillBeRetrieved) {
    if (pLinuxSysmanImp->pFsAccess != nullptr) {
        //delete previously allocated pFsAccess
        delete pLinuxSysmanImp->pFsAccess;
        pLinuxSysmanImp->pFsAccess = nullptr;
    }
    pLinuxSysmanImp->pFsAccess = FsAccess::create();
    EXPECT_EQ(&pLinuxSysmanImp->getFsAccess(), pLinuxSysmanImp->pFsAccess);
}

TEST_F(SysmanDeviceFixture, GivenValidPathnameWhenCallingFsAccessExistsThenSuccessIsReturned) {
    auto FsAccess = pLinuxSysmanImp->getFsAccess();

    char cwd[PATH_MAX];
    std::string path = getcwd(cwd, PATH_MAX);
    EXPECT_TRUE(FsAccess.fileExists(path));
}

TEST_F(SysmanDeviceFixture, GivenInvalidPathnameWhenCallingFsAccessExistsThenErrorIsReturned) {
    auto FsAccess = pLinuxSysmanImp->getFsAccess();

    std::string path = "noSuchFileOrDirectory";
    EXPECT_FALSE(FsAccess.fileExists(path));
}

TEST_F(SysmanDeviceFixture, GivenCreateSysfsAccessHandleWhenCallinggetSysfsAccessThenCreatedSysfsAccessHandleHandleWillBeRetrieved) {
    if (pLinuxSysmanImp->pSysfsAccess != nullptr) {
        //delete previously allocated pSysfsAccess
        delete pLinuxSysmanImp->pSysfsAccess;
        pLinuxSysmanImp->pSysfsAccess = nullptr;
    }
    pLinuxSysmanImp->pSysfsAccess = SysfsAccess::create("");
    EXPECT_EQ(&pLinuxSysmanImp->getSysfsAccess(), pLinuxSysmanImp->pSysfsAccess);
}

TEST_F(SysmanDeviceFixture, GivenCreateProcfsAccessHandleWhenCallinggetProcfsAccessThenCreatedProcfsAccessHandleWillBeRetrieved) {
    if (pLinuxSysmanImp->pProcfsAccess != nullptr) {
        //delete previously allocated pProcfsAccess
        delete pLinuxSysmanImp->pProcfsAccess;
        pLinuxSysmanImp->pProcfsAccess = nullptr;
    }
    pLinuxSysmanImp->pProcfsAccess = ProcfsAccess::create();
    EXPECT_EQ(&pLinuxSysmanImp->getProcfsAccess(), pLinuxSysmanImp->pProcfsAccess);
}

TEST_F(SysmanDeviceFixture, GivenValidPidWhenCallingProcfsAccessIsAliveThenSuccessIsReturned) {
    auto ProcfsAccess = pLinuxSysmanImp->getProcfsAccess();

    EXPECT_TRUE(ProcfsAccess.isAlive(getpid()));
}

TEST_F(SysmanDeviceFixture, GivenInvalidPidWhenCallingProcfsAccessIsAliveThenErrorIsReturned) {
    auto ProcfsAccess = pLinuxSysmanImp->getProcfsAccess();

    EXPECT_FALSE(ProcfsAccess.isAlive(reinterpret_cast<::pid_t>(-1)));
}

TEST_F(SysmanDeviceFixture, GivenPmtHandleWhenCallinggetPlatformMonitoringTechAccessThenCreatedPmtHandleWillBeRetrieved) {
    if (pLinuxSysmanImp->pPmt != nullptr) {
        //delete previously allocated pPmt
        delete pLinuxSysmanImp->pPmt;
        pLinuxSysmanImp->pPmt = nullptr;
    }
    pLinuxSysmanImp->pPmt = new PlatformMonitoringTech();
    EXPECT_EQ(&pLinuxSysmanImp->getPlatformMonitoringTechAccess(), pLinuxSysmanImp->pPmt);
}

TEST_F(SysmanDeviceFixture, GivenValidDeviceHandleVerifyThatSameHandleIsRetrievedFromOsSpecificCode) {
    EXPECT_EQ(pLinuxSysmanImp->getDeviceHandle(), device);
}

TEST_F(SysmanDeviceFixture, GivenPmuInterfaceHandleWhenCallinggetPmuInterfaceThenCreatedPmuInterfaceHandleWillBeRetrieved) {
    if (pLinuxSysmanImp->pPmuInterface != nullptr) {
        //delete previously allocated pPmuInterface
        delete pLinuxSysmanImp->pPmuInterface;
        pLinuxSysmanImp->pPmuInterface = nullptr;
    }
    pLinuxSysmanImp->pPmuInterface = PmuInterface::create(pLinuxSysmanImp);
    EXPECT_EQ(pLinuxSysmanImp->getPmuInterface(), pLinuxSysmanImp->pPmuInterface);
}

TEST_F(SysmanMultiDeviceFixture, GivenValidDeviceHandleHavingSubdevicesWhenValidatingSysmanHandlesForSubdevicesThenSysmanHandleForSubdeviceWillBeSameAsSysmanHandleForDevice) {
    uint32_t count = 0;
    EXPECT_EQ(ZE_RESULT_SUCCESS, device->getSubDevices(&count, nullptr));
    std::vector<ze_device_handle_t> subDeviceHandles(count, nullptr);
    EXPECT_EQ(ZE_RESULT_SUCCESS, device->getSubDevices(&count, subDeviceHandles.data()));
    for (auto subDeviceHandle : subDeviceHandles) {
        L0::DeviceImp *subDeviceHandleImp = static_cast<DeviceImp *>(Device::fromHandle(subDeviceHandle));
        EXPECT_EQ(subDeviceHandleImp->getSysmanHandle(), device->getSysmanHandle());
    }
}

} // namespace ult
} // namespace L0
