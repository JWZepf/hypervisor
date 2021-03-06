//
// Copyright (C) 2019 Assured Information Security, Inc.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef EXIT_HANDLER_INTEL_X64_H
#define EXIT_HANDLER_INTEL_X64_H

#include <bfdelegate.h>

#include <list>
#include <array>
#include <memory>

#include <intrinsics.h>

#include "vmcs.h"
#include "../x64/gdt.h"
#include "../x64/idt.h"
#include "../x64/tss.h"

// -----------------------------------------------------------------------------
// Exports
// -----------------------------------------------------------------------------

#include <bfexports.h>

#ifndef STATIC_HVE
#ifdef SHARED_HVE
#define EXPORT_HVE EXPORT_SYM
#else
#define EXPORT_HVE IMPORT_SYM
#endif
#else
#define EXPORT_HVE
#endif

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4251)
#endif

// -----------------------------------------------------------------------------
// Handler Types
// -----------------------------------------------------------------------------

namespace bfvmm::intel_x64
{
class vcpu;
}

using handler_t = bool(gsl::not_null<bfvmm::intel_x64::vcpu *>);
using handler_delegate_t = delegate<handler_t>;
using init_handler_delegate_t = delegate<handler_t>;
using fini_handler_delegate_t = delegate<handler_t>;

// -----------------------------------------------------------------------------
// Helpers
// -----------------------------------------------------------------------------

::x64::msrs::value_type emulate_rdmsr(::x64::msrs::field_type msr);
void emulate_wrmsr(::x64::msrs::field_type msr, ::x64::msrs::value_type val);

uintptr_t emulate_rdgpr(gsl::not_null<bfvmm::intel_x64::vcpu *> vcpu);
void emulate_wrgpr(gsl::not_null<bfvmm::intel_x64::vcpu *> vcpu, uintptr_t val);

// -----------------------------------------------------------------------------
// Exit Handler
// -----------------------------------------------------------------------------

namespace bfvmm::intel_x64
{

/// Exit Handler
///
/// This class is responsible for detecting why a guest exited (i.e. stopped
/// its execution), and handleres the appropriated handler to emulate the
/// instruction that could not execute. Note that this class could be executed
/// a lot, so performance is key here.
///
/// This class works with the VMCS class to provide the bare minimum exit
/// handler needed to execute a 64bit guest, with the TRUE controls being used.
/// In general, the only instruction that needs to be emulated is the CPUID
/// instruction. If more functionality is needed (which is likely), the user
/// can subclass this class, and overload the handlers that are needed. The
/// basics are provided with this class to ease development.
///
class EXPORT_HVE exit_handler
{
public:

    /// Default Constructor
    ///
    /// @expects none
    /// @ensures none
    ///
    /// @param vcpu The vCPU associated with this exit handler
    ///
    exit_handler(gsl::not_null<vcpu *> vcpu);

    /// Destructor
    ///
    /// @expects none
    /// @ensures none
    ///
    VIRTUAL ~exit_handler() = default;

    /// Add Handler Delegate
    ///
    /// Adds a handler to the handler function. When a VM exit occurs, the
    /// handler handler will call the delegate registered by this function as
    /// as needed. Note that the handlers are called in the reverse order they
    /// are registered (i.e. FIFO).
    ///
    /// @note If the delegate has serviced the VM exit, it should return true,
    ///     otherwise it should return false, and the next delegate registered
    ///     for this VM exit will execute, or an unimplemented exit reason
    ///     error will trigger
    ///
    /// @expects none
    /// @ensures none
    ///
    /// @param reason The exit reason for the handler being registered
    /// @param d The delegate being registered
    ///
    VIRTUAL void add_handler(
        ::intel_x64::vmcs::value_type reason,
        const handler_delegate_t &d
    );

    /// Add Exit Delegate
    ///
    /// Adds an exit function to the exit list. Exit functions are executed
    /// right after a vCPU exits for any reason. Use this with care because
    /// this function will be executed a lot.
    ///
    /// Note the return value of the delegate is ignored
    ///
    /// @expects none
    /// @ensures none
    ///
    /// @param d The delegate being registered
    ///
    VIRTUAL void add_exit_handler(
        const handler_delegate_t &d
    );

    /// Add Init Delegate
    ///
    /// Adds an init function to the init list. Init functions are executed
    /// right after a vCPU is started.
    ///
    /// @note The init function is the first VMexit that Bareflank causes
    ///     intentionally. It might not be the first VMexit to occur.
    ///
    /// @expects none
    /// @ensures none
    ///
    /// @param d The delegate being registered
    ///
    VIRTUAL void add_init_handler(
        const handler_delegate_t &d
    );

    /// Add Fini Delegate
    ///
    /// Adds an fini function to the fini list. Fini functions are executed
    /// right before the vCPU is about to stop.
    ///
    /// @expects none
    /// @ensures none
    ///
    /// @param d The delegate being registered
    ///
    VIRTUAL void add_fini_handler(
        const handler_delegate_t &d
    );

    /// Handle
    ///
    /// Handles a VM exit. This function should only be called by the exit
    /// handler entry function, which gets called when a VM exit occurs, and
    /// then calls this function to handler the VM exit to the proper
    /// handler.
    ///
    /// @expects none
    /// @ensures none
    ///
    /// @param exit_handler The exit handler to handler the VM exit to
    ///
    static void handle(
        bfvmm::intel_x64::exit_handler *exit_handler) noexcept;

    /// Get Host TSS
    ///
    /// @expects none
    /// @ensures none
    ///
    /// @return Returns a pointer to the host_tss
    ///
    auto host_tss() noexcept
    { return &m_host_tss; }

    /// Get Host IDT
    ///
    /// @expects none
    /// @ensures none
    ///
    /// @return Returns a pointer to the host_idt
    ///
    auto host_idt() noexcept
    { return &m_host_idt; }

    /// Get Host GDT
    ///
    /// @expects none
    /// @ensures none
    ///
    /// @return Returns a pointer to the host_gdt
    ///
    auto host_gdt() noexcept
    { return &m_host_gdt; }

private:

    void write_host_state();
    void write_guest_state();
    void write_control_state();

protected:

    /// @cond

    x64::tss m_host_tss{};
    x64::gdt m_host_gdt{512};
    x64::idt m_host_idt{256};

    /// @endcond

private:

    bool handle_cpuid(gsl::not_null<bfvmm::intel_x64::vcpu *> vcpu);

private:

    vcpu *m_vcpu;
    std::unique_ptr<gsl::byte[]> m_ist1;
    std::unique_ptr<gsl::byte[]> m_stack;

    std::list<init_handler_delegate_t> m_exit_handlers;
    std::list<init_handler_delegate_t> m_init_handlers;
    std::list<fini_handler_delegate_t> m_fini_handlers;
    std::array<std::list<handler_delegate_t>, 128> m_exit_handlers_array;

public:

    /// @cond

    exit_handler(exit_handler &&) noexcept = default;
    exit_handler &operator=(exit_handler &&) noexcept = default;

    exit_handler(const exit_handler &) = delete;
    exit_handler &operator=(const exit_handler &) = delete;

    /// @endcond
};

}

using exit_handler_t = bfvmm::intel_x64::exit_handler;

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif
