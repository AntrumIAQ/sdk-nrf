/*
 * Copyright (c) 2021 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#pragma once

#include <cstdint>

#include "bolt_lock_manager.h"
#include "led_widget.h"

struct AppEvent {
	enum LockEventType : uint8_t { Lock, Unlock, Toggle, CompleteLockAction };

	enum FunctionEventType : uint8_t { FunctionPress = CompleteLockAction + 1, FunctionRelease, FunctionTimer };

	enum UpdateLedStateEventType : uint8_t { UpdateLedState = FunctionTimer + 1 };

	enum OtherEventType : uint8_t {
		StartBleAdvertising = UpdateLedState + 1,
#ifdef CONFIG_MCUMGR_SMP_BT
		StartSMPAdvertising
#endif
	};

	AppEvent() = default;
	AppEvent(LockEventType type, BoltLockManager::OperationSource source) : Type(type), LockEvent{ source } {}
	AppEvent(FunctionEventType type, uint8_t buttonNumber) : Type(type), FunctionEvent{ buttonNumber } {}
	AppEvent(UpdateLedStateEventType type, LEDWidget *ledWidget) : Type(type), UpdateLedStateEvent{ ledWidget } {}
	explicit AppEvent(OtherEventType type) : Type(type) {}

	uint8_t Type;

	union {
		struct {
			BoltLockManager::OperationSource Source;
		} LockEvent;
		struct {
			LEDWidget *LedWidget;
		} UpdateLedStateEvent;
		struct {
			uint8_t ButtonNumber;
		} FunctionEvent;
	};
};
