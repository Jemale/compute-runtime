/*
 * Copyright (C) 2017-2020 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#pragma once
#include <cstdint>

namespace NEO {

extern long(__stdcall *notifyAubCaptureImpl)(void *csrHandle, uint64_t gfxAddress, size_t gfxSize, bool allocate);
template <typename GfxFamily>
struct DeviceCallbacks {
    static long __stdcall notifyAubCapture(void *csrHandle, uint64_t gfxAddress, size_t gfxSize, bool allocate);
};

template <typename GfxFamily>
struct TTCallbacks {
    using MI_LOAD_REGISTER_IMM = typename GfxFamily::MI_LOAD_REGISTER_IMM;

    static int __stdcall writeL3Address(void *queueHandle, uint64_t l3GfxAddress, uint64_t regOffset);
};

} // namespace NEO
