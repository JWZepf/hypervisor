//
// Bareflank Hypervisor
// Copyright (C) 2015 Assured Information Security, Inc.
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA

#include <catch/catch.hpp>
#include <intrinsics/x86/intel_x64.h>
#include <hippomocks.h>

#ifdef _HIPPOMOCKS__ENABLE_CFUNC_MOCKING_SUPPORT

using namespace intel_x64;

TEST_CASE("test name goes here")
{
    CHECK(true);
}

std::map<msrs::field_type, msrs::value_type> g_msrs;

extern "C" uint64_t
test_read_msr(uint32_t addr) noexcept
{ return g_msrs[addr]; }

extern "C" void
test_write_msr(uint32_t addr, uint64_t val) noexcept
{ g_msrs[addr] = val; }

std::map<cpuid::field_type, cpuid::value_type> g_ecx_cpuid;

struct cpuid_regs {
    cpuid::value_type ecx;
};

struct cpuid_regs g_regs;

extern "C" uint32_t
test_cpuid_ecx(uint32_t val) noexcept
{ return g_ecx_cpuid[val]; }

static void
setup_intrinsics(MockRepository &mocks)
{
    mocks.OnCallFunc(_read_msr).Do(test_read_msr);
    mocks.OnCallFunc(_write_msr).Do(test_write_msr);

    mocks.OnCallFunc(_cpuid_ecx).Do(test_cpuid_ecx);
}

TEST_CASE("msrs_ia32_x2apic_apicid")
{
    MockRepository mocks;
    setup_intrinsics(mocks);

    using namespace msrs::ia32_x2apic_apicid;

    g_msrs[addr] = 0xFFFFFFFFFFFFFFFFULL;
    CHECK(get() == 0xFFFFFFFFFFFFFFFFULL);
    dump(0);
}

TEST_CASE("msrs_ia32_x2apic_version")
{
    MockRepository mocks;
    setup_intrinsics(mocks);

    using namespace msrs::ia32_x2apic_version;

    g_msrs[addr] = 0xFFFFFFFFFFFFFFFFULL;
    CHECK(get() == 0xFFFFFFFFFFFFFFFFULL);
    dump(0);
}

TEST_CASE("msrs_ia32_x2apic_tpr")
{
    MockRepository mocks;
    setup_intrinsics(mocks);

    using namespace msrs::ia32_x2apic_tpr;

    set(0xFFFFFFFFFFFFFFFFULL);
    CHECK(get() == 0xFFFFFFFFFFFFFFFFULL);
    dump(0);
}

TEST_CASE("msrs_ia32_x2apic_ppr")
{
    MockRepository mocks;
    setup_intrinsics(mocks);

    using namespace msrs::ia32_x2apic_ppr;

    g_msrs[addr] = 0xFFFFFFFFFFFFFFFFULL;
    CHECK(get() == 0xFFFFFFFFFFFFFFFFULL);
    dump(0);
}

TEST_CASE("msrs_ia32_x2apic_eoi")
{
    MockRepository mocks;
    setup_intrinsics(mocks);

    using namespace msrs::ia32_x2apic_eoi;

    set(0xFFFFFFFFFFFFFFFFULL);
    CHECK(g_msrs[addr] == 0xFFFFFFFFFFFFFFFFULL);
}

TEST_CASE("msrs_ia32_x2apic_ldr")
{
    MockRepository mocks;
    setup_intrinsics(mocks);

    using namespace msrs::ia32_x2apic_ldr;

    g_msrs[addr] = 0xFFFFFFFFFFFFFFFFULL;
    CHECK(get() == 0xFFFFFFFFFFFFFFFFULL);
    dump(0);
}

TEST_CASE("msrs_ia32_x2apic_ldr_logical_id")
{
    MockRepository mocks;
    setup_intrinsics(mocks);

    using namespace msrs::ia32_x2apic_ldr;

    g_msrs[addr] = 0xFFFFFFFFFFFFFFFFULL;
    CHECK(logical_id::get() == (logical_id::mask >> logical_id::from));

    g_msrs[addr] = 0xFFFFFFFFFFFFFFFFULL;
    CHECK(logical_id::get(logical_id::mask) == (logical_id::mask >> logical_id::from));
}

TEST_CASE("msrs_ia32_x2apic_ldr_cluster_id")
{
    MockRepository mocks;
    setup_intrinsics(mocks);

    using namespace msrs::ia32_x2apic_ldr;

    g_msrs[addr] = 0xFFFFFFFFFFFFFFFFULL;
    CHECK(cluster_id::get() == (cluster_id::mask >> cluster_id::from));

    g_msrs[addr] = 0xFFFFFFFFFFFFFFFFULL;
    CHECK(cluster_id::get(cluster_id::mask) == (cluster_id::mask >> cluster_id::from));
}

TEST_CASE("msrs_ia32_x2apic_sivr")
{
    MockRepository mocks;
    setup_intrinsics(mocks);

    using namespace msrs::ia32_x2apic_sivr;

    set(0xFFFFFFFFFFFFFFFFULL);
    CHECK(get() == 0xFFFFFFFFFFFFFFFFULL);
    dump(0);
}

TEST_CASE("msrs_ia32_x2apic_sivr_vector")
{
    MockRepository mocks;
    setup_intrinsics(mocks);

    using namespace msrs::ia32_x2apic_sivr;

    vector::set(0xFFFFFFFFFFFFFFFFULL);
    CHECK(vector::get() == (vector::mask >> vector::from));

    vector::set(vector::mask, 0xFFFFFFFFFFFFFFFFULL);
    CHECK(vector::get(vector::mask) == (vector::mask >> vector::from));
}

TEST_CASE("msrs_ia32_x2apic_sivr_apic_enable_bit")
{
    MockRepository mocks;
    setup_intrinsics(mocks);

    using namespace msrs::ia32_x2apic_sivr;

    apic_enable_bit::enable();
    CHECK(apic_enable_bit::is_enabled());
    apic_enable_bit::disable();
    CHECK(apic_enable_bit::is_disabled());

    apic_enable_bit::enable(apic_enable_bit::mask);
    CHECK(apic_enable_bit::is_enabled(apic_enable_bit::mask));
    apic_enable_bit::disable(0x0);
    CHECK(apic_enable_bit::is_disabled(0x0));
}

TEST_CASE("msrs_ia32_x2apic_sivr_focus_checking")
{
    MockRepository mocks;
    setup_intrinsics(mocks);

    using namespace msrs::ia32_x2apic_sivr;

    focus_checking::enable();
    CHECK(focus_checking::is_enabled());
    focus_checking::disable();
    CHECK(focus_checking::is_disabled());

    focus_checking::enable(0x0);
    CHECK(focus_checking::is_enabled(0x0));
    focus_checking::disable(focus_checking::mask);
    CHECK(focus_checking::is_disabled(focus_checking::mask));
}

TEST_CASE("msrs_ia32_x2apic_sivr_suppress_eoi_broadcast")
{
    MockRepository mocks;
    setup_intrinsics(mocks);

    using namespace msrs::ia32_x2apic_sivr;

    suppress_eoi_broadcast::enable();
    CHECK(suppress_eoi_broadcast::is_enabled());
    suppress_eoi_broadcast::disable();
    CHECK(suppress_eoi_broadcast::is_disabled());

    suppress_eoi_broadcast::enable(suppress_eoi_broadcast::mask);
    CHECK(suppress_eoi_broadcast::is_enabled(suppress_eoi_broadcast::mask));
    suppress_eoi_broadcast::disable(0x0);
    CHECK(suppress_eoi_broadcast::is_disabled(0x0));
}

TEST_CASE("msrs_ia32_x2apic_isr0")
{
    MockRepository mocks;
    setup_intrinsics(mocks);

    using namespace msrs::ia32_x2apic_isr0;

    g_msrs[addr] = 0xFFFFFFFFFFFFFFFFULL;
    CHECK(get() == 0xFFFFFFFFFFFFFFFFULL);
    dump(0);
}

TEST_CASE("msrs_ia32_x2apic_isr1")
{
    MockRepository mocks;
    setup_intrinsics(mocks);

    using namespace msrs::ia32_x2apic_isr1;

    g_msrs[addr] = 0xFFFFFFFFFFFFFFFFULL;
    CHECK(get() == 0xFFFFFFFFFFFFFFFFULL);
    dump(0);
}

TEST_CASE("msrs_ia32_x2apic_isr2")
{
    MockRepository mocks;
    setup_intrinsics(mocks);

    using namespace msrs::ia32_x2apic_isr2;

    g_msrs[addr] = 0xFFFFFFFFFFFFFFFFULL;
    CHECK(get() == 0xFFFFFFFFFFFFFFFFULL);
    dump(0);
}

TEST_CASE("msrs_ia32_x2apic_isr3")
{
    MockRepository mocks;
    setup_intrinsics(mocks);

    using namespace msrs::ia32_x2apic_isr3;

    g_msrs[addr] = 0xFFFFFFFFFFFFFFFFULL;
    CHECK(get() == 0xFFFFFFFFFFFFFFFFULL);
    dump(0);
}

TEST_CASE("msrs_ia32_x2apic_isr4")
{
    MockRepository mocks;
    setup_intrinsics(mocks);

    using namespace msrs::ia32_x2apic_isr4;

    g_msrs[addr] = 0xFFFFFFFFFFFFFFFFULL;
    CHECK(get() == 0xFFFFFFFFFFFFFFFFULL);
    dump(0);
}

TEST_CASE("msrs_ia32_x2apic_isr5")
{
    MockRepository mocks;
    setup_intrinsics(mocks);

    using namespace msrs::ia32_x2apic_isr5;

    g_msrs[addr] = 0xFFFFFFFFFFFFFFFFULL;
    CHECK(get() == 0xFFFFFFFFFFFFFFFFULL);
    dump(0);
}

TEST_CASE("msrs_ia32_x2apic_isr6")
{
    MockRepository mocks;
    setup_intrinsics(mocks);

    using namespace msrs::ia32_x2apic_isr6;

    g_msrs[addr] = 0xFFFFFFFFFFFFFFFFULL;
    CHECK(get() == 0xFFFFFFFFFFFFFFFFULL);
    dump(0);
}

TEST_CASE("msrs_ia32_x2apic_isr7")
{
    MockRepository mocks;
    setup_intrinsics(mocks);

    using namespace msrs::ia32_x2apic_isr7;

    g_msrs[addr] = 0xFFFFFFFFFFFFFFFFULL;
    CHECK(get() == 0xFFFFFFFFFFFFFFFFULL);
    dump(0);
}

TEST_CASE("msrs_ia32_x2apic_tmr0")
{
    MockRepository mocks;
    setup_intrinsics(mocks);

    using namespace msrs::ia32_x2apic_tmr0;

    g_msrs[addr] = 0xFFFFFFFFFFFFFFFFULL;
    CHECK(get() == 0xFFFFFFFFFFFFFFFFULL);
    dump(0);
}

TEST_CASE("msrs_ia32_x2apic_tmr1")
{
    MockRepository mocks;
    setup_intrinsics(mocks);

    using namespace msrs::ia32_x2apic_tmr1;

    g_msrs[addr] = 0xFFFFFFFFFFFFFFFFULL;
    CHECK(get() == 0xFFFFFFFFFFFFFFFFULL);
    dump(0);
}

TEST_CASE("msrs_ia32_x2apic_tmr2")
{
    MockRepository mocks;
    setup_intrinsics(mocks);

    using namespace msrs::ia32_x2apic_tmr2;

    g_msrs[addr] = 0xFFFFFFFFFFFFFFFFULL;
    CHECK(get() == 0xFFFFFFFFFFFFFFFFULL);
    dump(0);
}

TEST_CASE("msrs_ia32_x2apic_tmr3")
{
    MockRepository mocks;
    setup_intrinsics(mocks);

    using namespace msrs::ia32_x2apic_tmr3;

    g_msrs[addr] = 0xFFFFFFFFFFFFFFFFULL;
    CHECK(get() == 0xFFFFFFFFFFFFFFFFULL);
    dump(0);
}

TEST_CASE("msrs_ia32_x2apic_tmr4")
{
    MockRepository mocks;
    setup_intrinsics(mocks);

    using namespace msrs::ia32_x2apic_tmr4;

    g_msrs[addr] = 0xFFFFFFFFFFFFFFFFULL;
    CHECK(get() == 0xFFFFFFFFFFFFFFFFULL);
    dump(0);
}

TEST_CASE("msrs_ia32_x2apic_tmr5")
{
    MockRepository mocks;
    setup_intrinsics(mocks);

    using namespace msrs::ia32_x2apic_tmr5;

    g_msrs[addr] = 0xFFFFFFFFFFFFFFFFULL;
    CHECK(get() == 0xFFFFFFFFFFFFFFFFULL);
    dump(0);
}

TEST_CASE("msrs_ia32_x2apic_tmr6")
{
    MockRepository mocks;
    setup_intrinsics(mocks);

    using namespace msrs::ia32_x2apic_tmr6;

    g_msrs[addr] = 0xFFFFFFFFFFFFFFFFULL;
    CHECK(get() == 0xFFFFFFFFFFFFFFFFULL);
    dump(0);
}

TEST_CASE("msrs_ia32_x2apic_tmr7")
{
    MockRepository mocks;
    setup_intrinsics(mocks);

    using namespace msrs::ia32_x2apic_tmr7;

    g_msrs[addr] = 0xFFFFFFFFFFFFFFFFULL;
    CHECK(get() == 0xFFFFFFFFFFFFFFFFULL);
    dump(0);
}

TEST_CASE("msrs_ia32_x2apic_irr0")
{
    MockRepository mocks;
    setup_intrinsics(mocks);

    using namespace msrs::ia32_x2apic_irr0;

    g_msrs[addr] = 0xFFFFFFFFFFFFFFFFULL;
    CHECK(get() == 0xFFFFFFFFFFFFFFFFULL);
    dump(0);
}

TEST_CASE("msrs_ia32_x2apic_irr1")
{
    MockRepository mocks;
    setup_intrinsics(mocks);

    using namespace msrs::ia32_x2apic_irr1;

    g_msrs[addr] = 0xFFFFFFFFFFFFFFFFULL;
    CHECK(get() == 0xFFFFFFFFFFFFFFFFULL);
    dump(0);
}

TEST_CASE("msrs_ia32_x2apic_irr2")
{
    MockRepository mocks;
    setup_intrinsics(mocks);

    using namespace msrs::ia32_x2apic_irr2;

    g_msrs[addr] = 0xFFFFFFFFFFFFFFFFULL;
    CHECK(get() == 0xFFFFFFFFFFFFFFFFULL);
    dump(0);
}

TEST_CASE("msrs_ia32_x2apic_irr3")
{
    MockRepository mocks;
    setup_intrinsics(mocks);

    using namespace msrs::ia32_x2apic_irr3;

    g_msrs[addr] = 0xFFFFFFFFFFFFFFFFULL;
    CHECK(get() == 0xFFFFFFFFFFFFFFFFULL);
    dump(0);
}

TEST_CASE("msrs_ia32_x2apic_irr4")
{
    MockRepository mocks;
    setup_intrinsics(mocks);

    using namespace msrs::ia32_x2apic_irr4;

    g_msrs[addr] = 0xFFFFFFFFFFFFFFFFULL;
    CHECK(get() == 0xFFFFFFFFFFFFFFFFULL);
    dump(0);
}

TEST_CASE("msrs_ia32_x2apic_irr5")
{
    MockRepository mocks;
    setup_intrinsics(mocks);

    using namespace msrs::ia32_x2apic_irr5;

    g_msrs[addr] = 0xFFFFFFFFFFFFFFFFULL;
    CHECK(get() == 0xFFFFFFFFFFFFFFFFULL);
    dump(0);
}

TEST_CASE("msrs_ia32_x2apic_irr6")
{
    MockRepository mocks;
    setup_intrinsics(mocks);

    using namespace msrs::ia32_x2apic_irr6;

    g_msrs[addr] = 0xFFFFFFFFFFFFFFFFULL;
    CHECK(get() == 0xFFFFFFFFFFFFFFFFULL);
    dump(0);
}

TEST_CASE("msrs_ia32_x2apic_irr7")
{
    MockRepository mocks;
    setup_intrinsics(mocks);

    using namespace msrs::ia32_x2apic_irr7;

    g_msrs[addr] = 0xFFFFFFFFFFFFFFFFULL;
    CHECK(get() == 0xFFFFFFFFFFFFFFFFULL);
    dump(0);
}

TEST_CASE("msrs_ia32_x2apic_esr")
{
    MockRepository mocks;
    setup_intrinsics(mocks);

    using namespace msrs::ia32_x2apic_esr;

    set(0xFFFFFFFFFFFFFFFFULL);
    CHECK(get() == 0xFFFFFFFFFFFFFFFFULL);
    dump(0);
}

TEST_CASE("msrs_ia32_x2apic_lvt_cmci")
{
    MockRepository mocks;
    setup_intrinsics(mocks);

    using namespace msrs::ia32_x2apic_lvt_cmci;

    set(0xFFFFFFFFFFFFFFFFULL);
    CHECK(get() == 0xFFFFFFFFFFFFFFFFULL);
    dump(0);
}

TEST_CASE("msrs_ia32_x2apic_lvt_cmci_vector")
{
    MockRepository mocks;
    setup_intrinsics(mocks);

    using namespace msrs::ia32_x2apic_lvt_cmci;

    vector::set(0xFFFFFFFFFFFFFFFFULL);
    CHECK(vector::get() == (vector::mask >> vector::from));

    vector::set(vector::mask, 0xFFFFFFFFFFFFFFFFULL);
    CHECK(vector::get(vector::mask) == (vector::mask >> vector::from));
}

TEST_CASE("msrs_ia32_x2apic_lvt_cmci_delivery_mode")
{
    MockRepository mocks;
    setup_intrinsics(mocks);

    using namespace msrs::ia32_x2apic_lvt_cmci;

    delivery_mode::set(0xFFFFFFFFFFFFFFFFULL);
    CHECK(delivery_mode::get() == (delivery_mode::mask >> delivery_mode::from));

    delivery_mode::set(delivery_mode::mask, 0xFFFFFFFFFFFFFFFFULL);
    CHECK(delivery_mode::get(delivery_mode::mask) == (delivery_mode::mask >> delivery_mode::from));

    delivery_mode::set(0x0ULL);
    delivery_mode::dump(0);
    delivery_mode::set(0x2ULL);
    delivery_mode::dump(0);
    delivery_mode::set(0x4ULL);
    delivery_mode::dump(0);
    delivery_mode::set(0x5ULL);
    delivery_mode::dump(0);
    delivery_mode::set(0x7ULL);
    delivery_mode::dump(0);
    delivery_mode::set(0x1ULL);
    delivery_mode::dump(0);
}

TEST_CASE("msrs_ia32_x2apic_lvt_cmci_delivery_status")
{
    MockRepository mocks;
    setup_intrinsics(mocks);

    using namespace msrs::ia32_x2apic_lvt_cmci;

    delivery_status::set(0xFFFFFFFFFFFFFFFFULL);
    CHECK(delivery_status::get() == (delivery_status::mask >> delivery_status::from));

    delivery_status::set(delivery_status::mask, 0xFFFFFFFFFFFFFFFFULL);
    CHECK(delivery_status::get(delivery_status::mask) == (delivery_status::mask >> delivery_status::from));

    delivery_status::set(0x0000000000000000ULL);
    dump(0);
    delivery_status::set(0xFFFFFFFFFFFFFFFFULL);
    dump(0);
}

TEST_CASE("msrs_ia32_x2apic_lvt_cmci_mask_bit")
{
    MockRepository mocks;
    setup_intrinsics(mocks);

    using namespace msrs::ia32_x2apic_lvt_cmci;

    mask_bit::enable();
    CHECK(mask_bit::is_enabled());
    mask_bit::disable();
    CHECK(mask_bit::is_disabled());

    mask_bit::enable(mask_bit::mask);
    CHECK(mask_bit::is_enabled(mask_bit::mask));
    mask_bit::disable(0x0);
    CHECK(mask_bit::is_disabled(0x0));
}

TEST_CASE("msrs_ia32_x2apic_icr")
{
    MockRepository mocks;
    setup_intrinsics(mocks);

    using namespace msrs::ia32_x2apic_icr;

    set(0xFFFFFFFFFFFFFFFFULL);
    CHECK(get() == 0xFFFFFFFFFFFFFFFFULL);
    dump(0);
}

TEST_CASE("msrs_ia32_x2apic_icr_vector")
{
    MockRepository mocks;
    setup_intrinsics(mocks);

    using namespace msrs::ia32_x2apic_icr;

    vector::set(0xFFFFFFFFFFFFFFFFULL);
    CHECK(vector::get() == (vector::mask >> vector::from));

    vector::set(vector::mask, 0xFFFFFFFFFFFFFFFFULL);
    CHECK(vector::get(vector::mask) == (vector::mask >> vector::from));
}

TEST_CASE("msrs_ia32_x2apic_icr_delivery_mode")
{
    MockRepository mocks;
    setup_intrinsics(mocks);

    using namespace msrs::ia32_x2apic_icr;

    delivery_mode::set(0xFFFFFFFFFFFFFFFFULL);
    CHECK(delivery_mode::get() == (delivery_mode::mask >> delivery_mode::from));

    delivery_mode::set(delivery_mode::mask, 0xFFFFFFFFFFFFFFFFULL);
    CHECK(delivery_mode::get(delivery_mode::mask) == (delivery_mode::mask >> delivery_mode::from));

    delivery_mode::set(0x0ULL);
    delivery_mode::dump(0);
    delivery_mode::set(0x2ULL);
    delivery_mode::dump(0);
    delivery_mode::set(0x4ULL);
    delivery_mode::dump(0);
    delivery_mode::set(0x5ULL);
    delivery_mode::dump(0);
    delivery_mode::set(0x7ULL);
    delivery_mode::dump(0);
    delivery_mode::set(0x1ULL);
    delivery_mode::dump(0);
}

TEST_CASE("msrs_ia32_x2apic_icr_destination_mode")
{
    MockRepository mocks;
    setup_intrinsics(mocks);

    using namespace msrs::ia32_x2apic_icr;

    destination_mode::set(0xFFFFFFFFFFFFFFFFULL);
    CHECK(destination_mode::get() == (destination_mode::mask >> destination_mode::from));

    destination_mode::set(destination_mode::mask, 0xFFFFFFFFFFFFFFFFULL);
    CHECK(destination_mode::get(destination_mode::mask) == (destination_mode::mask >> destination_mode::from));

    destination_mode::set(0x0000000000000000ULL);
    dump(0);
    destination_mode::set(0xFFFFFFFFFFFFFFFFULL);
    dump(0);
}

TEST_CASE("msrs_ia32_x2apic_icr_delivery_status")
{
    MockRepository mocks;
    setup_intrinsics(mocks);

    using namespace msrs::ia32_x2apic_icr;

    delivery_status::set(0xFFFFFFFFFFFFFFFFULL);
    CHECK(delivery_status::get() == (delivery_status::mask >> delivery_status::from));

    delivery_status::set(delivery_status::mask, 0xFFFFFFFFFFFFFFFFULL);
    CHECK(delivery_status::get(delivery_status::mask) == (delivery_status::mask >> delivery_status::from));

    delivery_status::set(0x0000000000000000ULL);
    dump(0);
    delivery_status::set(0xFFFFFFFFFFFFFFFFULL);
    dump(0);
}

TEST_CASE("msrs_ia32_x2apic_icr_level")
{
    MockRepository mocks;
    setup_intrinsics(mocks);

    using namespace msrs::ia32_x2apic_icr;

    level::enable();
    CHECK(level::is_enabled());
    level::disable();
    CHECK(level::is_disabled());

    level::enable(level::mask);
    CHECK(level::is_enabled(level::mask));
    level::disable(0x0);
    CHECK(level::is_disabled(0x0));
}

TEST_CASE("msrs_ia32_x2apic_icr_trigger_mode")
{
    MockRepository mocks;
    setup_intrinsics(mocks);

    using namespace msrs::ia32_x2apic_icr;

    trigger_mode::set(0xFFFFFFFFFFFFFFFFULL);
    CHECK(trigger_mode::get() == (trigger_mode::mask >> trigger_mode::from));

    trigger_mode::set(trigger_mode::mask, 0xFFFFFFFFFFFFFFFFULL);
    CHECK(trigger_mode::get(trigger_mode::mask) == (trigger_mode::mask >> trigger_mode::from));

    trigger_mode::set(0x0000000000000000ULL);
    dump(0);
    trigger_mode::set(0xFFFFFFFFFFFFFFFFULL);
    dump(0);
}

TEST_CASE("msrs_ia32_x2apic_icr_destination_shorthand")
{
    MockRepository mocks;
    setup_intrinsics(mocks);

    using namespace msrs::ia32_x2apic_icr;

    destination_shorthand::set(0xFFFFFFFFFFFFFFFFULL);
    CHECK(destination_shorthand::get() == (destination_shorthand::mask >> destination_shorthand::from));

    destination_shorthand::set(destination_shorthand::mask, 0xFFFFFFFFFFFFFFFFULL);
    CHECK(destination_shorthand::get(destination_shorthand::mask) == (destination_shorthand::mask >> destination_shorthand::from));

    destination_shorthand::set(0x0ULL);
    destination_shorthand::dump(0);
    destination_shorthand::set(0x1ULL);
    destination_shorthand::dump(0);
    destination_shorthand::set(0x2ULL);
    destination_shorthand::dump(0);
    destination_shorthand::set(0x3ULL);
    destination_shorthand::dump(0);
}

TEST_CASE("msrs_ia32_x2apic_icr_destination_field")
{
    MockRepository mocks;
    setup_intrinsics(mocks);

    using namespace msrs::ia32_x2apic_icr;

    destination_field::set(0xFFFFFFFFFFFFFFFFULL);
    CHECK(destination_field::get() == (destination_field::mask >> destination_field::from));

    destination_field::set(destination_field::mask, 0xFFFFFFFFFFFFFFFFULL);
    CHECK(destination_field::get(destination_field::mask) == (destination_field::mask >> destination_field::from));
}

TEST_CASE("msrs_ia32_x2apic_lvt_timer")
{
    MockRepository mocks;
    setup_intrinsics(mocks);

    using namespace msrs::ia32_x2apic_lvt_timer;

    set(0xFFFFFFFFFFFFFFFFULL);
    CHECK(get() == 0xFFFFFFFFFFFFFFFFULL);
    dump(0);
}

TEST_CASE("msrs_ia32_x2apic_lvt_timer_vector")
{
    MockRepository mocks;
    setup_intrinsics(mocks);

    using namespace msrs::ia32_x2apic_lvt_timer;

    vector::set(0xFFFFFFFFFFFFFFFFULL);
    CHECK(vector::get() == (vector::mask >> vector::from));

    vector::set(vector::mask, 0xFFFFFFFFFFFFFFFFULL);
    CHECK(vector::get(vector::mask) == (vector::mask >> vector::from));
}

TEST_CASE("msrs_ia32_x2apic_lvt_timer_delivery_status")
{
    MockRepository mocks;
    setup_intrinsics(mocks);

    using namespace msrs::ia32_x2apic_lvt_timer;

    delivery_status::set(0xFFFFFFFFFFFFFFFFULL);
    CHECK(delivery_status::get() == (delivery_status::mask >> delivery_status::from));

    delivery_status::set(delivery_status::mask, 0xFFFFFFFFFFFFFFFFULL);
    CHECK(delivery_status::get(delivery_status::mask) == (delivery_status::mask >> delivery_status::from));

    delivery_status::set(0x0000000000000000ULL);
    dump(0);
    delivery_status::set(0xFFFFFFFFFFFFFFFFULL);
    dump(0);
}

TEST_CASE("msrs_ia32_x2apic_lvt_timer_mask_bit")
{
    MockRepository mocks;
    setup_intrinsics(mocks);

    using namespace msrs::ia32_x2apic_lvt_timer;

    mask_bit::enable();
    CHECK(mask_bit::is_enabled());
    mask_bit::disable();
    CHECK(mask_bit::is_disabled());

    mask_bit::enable(mask_bit::mask);
    CHECK(mask_bit::is_enabled(mask_bit::mask));
    mask_bit::disable(0x0);
    CHECK(mask_bit::is_disabled(0x0));
}

TEST_CASE("msrs_ia32_x2apic_lvt_timer_timer_mode")
{
    MockRepository mocks;
    setup_intrinsics(mocks);

    using namespace msrs::ia32_x2apic_lvt_timer;

    timer_mode::set(0xFFFFFFFFFFFFFFFFULL);
    CHECK(timer_mode::get() == (timer_mode::mask >> timer_mode::from));

    timer_mode::set(timer_mode::mask, 0xFFFFFFFFFFFFFFFFULL);
    CHECK(timer_mode::get(timer_mode::mask) == (timer_mode::mask >> timer_mode::from));

    timer_mode::set(0x0ULL);
    timer_mode::dump(0);
    timer_mode::set(0x1ULL);
    timer_mode::dump(0);
    timer_mode::set(0x2ULL);
    timer_mode::dump(0);
    timer_mode::set(0x3ULL);
    timer_mode::dump(0);
}

TEST_CASE("msrs_ia32_x2apic_lvt_thermal")
{
    MockRepository mocks;
    setup_intrinsics(mocks);

    using namespace msrs::ia32_x2apic_lvt_thermal;

    set(0xFFFFFFFFFFFFFFFFULL);
    CHECK(get() == 0xFFFFFFFFFFFFFFFFULL);
    dump(0);
}

TEST_CASE("msrs_ia32_x2apic_lvt_thermal_vector")
{
    MockRepository mocks;
    setup_intrinsics(mocks);

    using namespace msrs::ia32_x2apic_lvt_thermal;

    vector::set(0xFFFFFFFFFFFFFFFFULL);
    CHECK(vector::get() == (vector::mask >> vector::from));

    vector::set(vector::mask, 0xFFFFFFFFFFFFFFFFULL);
    CHECK(vector::get(vector::mask) == (vector::mask >> vector::from));
}

TEST_CASE("msrs_ia32_x2apic_lvt_thermal_delivery_mode")
{
    MockRepository mocks;
    setup_intrinsics(mocks);

    using namespace msrs::ia32_x2apic_lvt_thermal;

    delivery_mode::set(0xFFFFFFFFFFFFFFFFULL);
    CHECK(delivery_mode::get() == (delivery_mode::mask >> delivery_mode::from));

    delivery_mode::set(delivery_mode::mask, 0xFFFFFFFFFFFFFFFFULL);
    CHECK(delivery_mode::get(delivery_mode::mask) == (delivery_mode::mask >> delivery_mode::from));

    delivery_mode::set(0x0ULL);
    delivery_mode::dump(0);
    delivery_mode::set(0x2ULL);
    delivery_mode::dump(0);
    delivery_mode::set(0x4ULL);
    delivery_mode::dump(0);
    delivery_mode::set(0x5ULL);
    delivery_mode::dump(0);
    delivery_mode::set(0x7ULL);
    delivery_mode::dump(0);
    delivery_mode::set(0x1ULL);
    delivery_mode::dump(0);
}

TEST_CASE("msrs_ia32_x2apic_lvt_thermal_delivery_status")
{
    MockRepository mocks;
    setup_intrinsics(mocks);

    using namespace msrs::ia32_x2apic_lvt_thermal;

    delivery_status::set(0xFFFFFFFFFFFFFFFFULL);
    CHECK(delivery_status::get() == (delivery_status::mask >> delivery_status::from));

    delivery_status::set(delivery_status::mask, 0xFFFFFFFFFFFFFFFFULL);
    CHECK(delivery_status::get(delivery_status::mask) == (delivery_status::mask >> delivery_status::from));

    delivery_status::set(0x0000000000000000ULL);
    dump(0);
    delivery_status::set(0xFFFFFFFFFFFFFFFFULL);
    dump(0);
}

TEST_CASE("msrs_ia32_x2apic_lvt_thermal_mask_bit")
{
    MockRepository mocks;
    setup_intrinsics(mocks);

    using namespace msrs::ia32_x2apic_lvt_thermal;

    mask_bit::enable();
    CHECK(mask_bit::is_enabled());
    mask_bit::disable();
    CHECK(mask_bit::is_disabled());

    mask_bit::enable(mask_bit::mask);
    CHECK(mask_bit::is_enabled(mask_bit::mask));
    mask_bit::disable(0x0);
    CHECK(mask_bit::is_disabled(0x0));
}

TEST_CASE("msrs_ia32_x2apic_lvt_pmi")
{
    MockRepository mocks;
    setup_intrinsics(mocks);

    using namespace msrs::ia32_x2apic_lvt_pmi;

    set(0xFFFFFFFFFFFFFFFFULL);
    CHECK(get() == 0xFFFFFFFFFFFFFFFFULL);
    dump(0);
}

TEST_CASE("msrs_ia32_x2apic_lvt_pmi_vector")
{
    MockRepository mocks;
    setup_intrinsics(mocks);

    using namespace msrs::ia32_x2apic_lvt_pmi;

    vector::set(0xFFFFFFFFFFFFFFFFULL);
    CHECK(vector::get() == (vector::mask >> vector::from));

    vector::set(vector::mask, 0xFFFFFFFFFFFFFFFFULL);
    CHECK(vector::get(vector::mask) == (vector::mask >> vector::from));
}

TEST_CASE("msrs_ia32_x2apic_lvt_pmi_delivery_mode")
{
    MockRepository mocks;
    setup_intrinsics(mocks);

    using namespace msrs::ia32_x2apic_lvt_pmi;

    delivery_mode::set(0xFFFFFFFFFFFFFFFFULL);
    CHECK(delivery_mode::get() == (delivery_mode::mask >> delivery_mode::from));

    delivery_mode::set(delivery_mode::mask, 0xFFFFFFFFFFFFFFFFULL);
    CHECK(delivery_mode::get(delivery_mode::mask) == (delivery_mode::mask >> delivery_mode::from));

    delivery_mode::set(0x0ULL);
    delivery_mode::dump(0);
    delivery_mode::set(0x2ULL);
    delivery_mode::dump(0);
    delivery_mode::set(0x4ULL);
    delivery_mode::dump(0);
    delivery_mode::set(0x5ULL);
    delivery_mode::dump(0);
    delivery_mode::set(0x7ULL);
    delivery_mode::dump(0);
    delivery_mode::set(0x1ULL);
    delivery_mode::dump(0);
}

TEST_CASE("msrs_ia32_x2apic_lvt_pmi_delivery_status")
{
    MockRepository mocks;
    setup_intrinsics(mocks);

    using namespace msrs::ia32_x2apic_lvt_pmi;

    delivery_status::set(0xFFFFFFFFFFFFFFFFULL);
    CHECK(delivery_status::get() == (delivery_status::mask >> delivery_status::from));

    delivery_status::set(delivery_status::mask, 0xFFFFFFFFFFFFFFFFULL);
    CHECK(delivery_status::get(delivery_status::mask) == (delivery_status::mask >> delivery_status::from));

    delivery_status::set(0x0000000000000000ULL);
    dump(0);
}

TEST_CASE("msrs_ia32_x2apic_lvt_pmi_mask_bit")
{
    MockRepository mocks;
    setup_intrinsics(mocks);

    using namespace msrs::ia32_x2apic_lvt_pmi;

    mask_bit::enable();
    CHECK(mask_bit::is_enabled());
    mask_bit::disable();
    CHECK(mask_bit::is_disabled());

    mask_bit::enable(mask_bit::mask);
    CHECK(mask_bit::is_enabled(mask_bit::mask));
    mask_bit::disable(0x0);
    CHECK(mask_bit::is_disabled(0x0));
}

TEST_CASE("msrs_ia32_x2apic_lvt_lint0")
{
    MockRepository mocks;
    setup_intrinsics(mocks);

    using namespace msrs::ia32_x2apic_lvt_lint0;

    set(0xFFFFFFFFFFFFFFFFULL);
    CHECK(get() == 0xFFFFFFFFFFFFFFFFULL);
    dump(0);
}

TEST_CASE("msrs_ia32_x2apic_lvt_lint0_vector")
{
    MockRepository mocks;
    setup_intrinsics(mocks);

    using namespace msrs::ia32_x2apic_lvt_lint0;

    vector::set(0xFFFFFFFFFFFFFFFFULL);
    CHECK(vector::get() == (vector::mask >> vector::from));

    vector::set(vector::mask, 0xFFFFFFFFFFFFFFFFULL);
    CHECK(vector::get(vector::mask) == (vector::mask >> vector::from));
}

TEST_CASE("msrs_ia32_x2apic_lvt_lint0_delivery_mode")
{
    MockRepository mocks;
    setup_intrinsics(mocks);

    using namespace msrs::ia32_x2apic_lvt_lint0;

    delivery_mode::set(0xFFFFFFFFFFFFFFFFULL);
    CHECK(delivery_mode::get() == (delivery_mode::mask >> delivery_mode::from));

    delivery_mode::set(delivery_mode::mask, 0xFFFFFFFFFFFFFFFFULL);
    CHECK(delivery_mode::get(delivery_mode::mask) == (delivery_mode::mask >> delivery_mode::from));

    delivery_mode::set(0x0ULL);
    delivery_mode::dump(0);
    delivery_mode::set(0x2ULL);
    delivery_mode::dump(0);
    delivery_mode::set(0x4ULL);
    delivery_mode::dump(0);
    delivery_mode::set(0x5ULL);
    delivery_mode::dump(0);
    delivery_mode::set(0x7ULL);
    delivery_mode::dump(0);
    delivery_mode::set(0x1ULL);
    delivery_mode::dump(0);
}

TEST_CASE("msrs_ia32_x2apic_lvt_lint0_delivery_status")
{
    MockRepository mocks;
    setup_intrinsics(mocks);

    using namespace msrs::ia32_x2apic_lvt_lint0;

    delivery_status::set(0xFFFFFFFFFFFFFFFFULL);
    CHECK(delivery_status::get() == (delivery_status::mask >> delivery_status::from));

    delivery_status::set(delivery_status::mask, 0xFFFFFFFFFFFFFFFFULL);
    CHECK(delivery_status::get(delivery_status::mask) == (delivery_status::mask >> delivery_status::from));

    delivery_status::set(0x0000000000000000ULL);
    dump(0);
    delivery_status::set(0xFFFFFFFFFFFFFFFFULL);
    dump(0);
}

TEST_CASE("msrs_ia32_x2apic_lvt_lint0_polarity")
{
    MockRepository mocks;
    setup_intrinsics(mocks);

    using namespace msrs::ia32_x2apic_lvt_lint0;

    polarity::set(0xFFFFFFFFFFFFFFFFULL);
    CHECK(polarity::get() == (polarity::mask >> polarity::from));

    polarity::set(polarity::mask, 0xFFFFFFFFFFFFFFFFULL);
    CHECK(polarity::get(polarity::mask) == (polarity::mask >> polarity::from));

    polarity::set(0x0000000000000000ULL);
    dump(0);
    polarity::set(0xFFFFFFFFFFFFFFFFULL);
    dump(0);
}

TEST_CASE("msrs_ia32_x2apic_lvt_lint0_remote_irr")
{
    MockRepository mocks;
    setup_intrinsics(mocks);

    using namespace msrs::ia32_x2apic_lvt_lint0;

    g_msrs[addr] = 0xFFFFFFFFFFFFFFFFULL;
    CHECK(remote_irr::get() == (remote_irr::mask >> remote_irr::from));

    g_msrs[addr] = 0xFFFFFFFFFFFFFFFFULL;
    CHECK(remote_irr::get(remote_irr::mask) == (remote_irr::mask >> remote_irr::from));
}

TEST_CASE("msrs_ia32_x2apic_lvt_lint0_trigger_mode")
{
    MockRepository mocks;
    setup_intrinsics(mocks);

    using namespace msrs::ia32_x2apic_lvt_lint0;

    trigger_mode::set(0xFFFFFFFFFFFFFFFFULL);
    CHECK(trigger_mode::get() == (trigger_mode::mask >> trigger_mode::from));

    trigger_mode::set(trigger_mode::mask, 0xFFFFFFFFFFFFFFFFULL);
    CHECK(trigger_mode::get(trigger_mode::mask) == (trigger_mode::mask >> trigger_mode::from));

    trigger_mode::set(0x0000000000000000ULL);
    dump(0);
    trigger_mode::set(0xFFFFFFFFFFFFFFFFULL);
    dump(0);
}

TEST_CASE("msrs_ia32_x2apic_lvt_lint0_mask_bit")
{
    MockRepository mocks;
    setup_intrinsics(mocks);

    using namespace msrs::ia32_x2apic_lvt_lint0;

    mask_bit::enable();
    CHECK(mask_bit::is_enabled());
    mask_bit::disable();
    CHECK(mask_bit::is_disabled());

    mask_bit::enable(mask_bit::mask);
    CHECK(mask_bit::is_enabled(mask_bit::mask));
    mask_bit::disable(0x0);
    CHECK(mask_bit::is_disabled(0x0));
}

TEST_CASE("msrs_ia32_x2apic_lvt_lint1")
{
    MockRepository mocks;
    setup_intrinsics(mocks);

    using namespace msrs::ia32_x2apic_lvt_lint1;

    set(0xFFFFFFFFFFFFFFFFULL);
    CHECK(get() == 0xFFFFFFFFFFFFFFFFULL);
    dump(0);
}

TEST_CASE("msrs_ia32_x2apic_lvt_lint1_vector")
{
    MockRepository mocks;
    setup_intrinsics(mocks);

    using namespace msrs::ia32_x2apic_lvt_lint1;

    vector::set(0xFFFFFFFFFFFFFFFFULL);
    CHECK(vector::get() == (vector::mask >> vector::from));

    vector::set(vector::mask, 0xFFFFFFFFFFFFFFFFULL);
    CHECK(vector::get(vector::mask) == (vector::mask >> vector::from));
}

TEST_CASE("msrs_ia32_x2apic_lvt_lint1_delivery_mode")
{
    MockRepository mocks;
    setup_intrinsics(mocks);

    using namespace msrs::ia32_x2apic_lvt_lint1;

    delivery_mode::set(0xFFFFFFFFFFFFFFFFULL);
    CHECK(delivery_mode::get() == (delivery_mode::mask >> delivery_mode::from));

    delivery_mode::set(delivery_mode::mask, 0xFFFFFFFFFFFFFFFFULL);
    CHECK(delivery_mode::get(delivery_mode::mask) == (delivery_mode::mask >> delivery_mode::from));

    delivery_mode::set(0x0ULL);
    delivery_mode::dump(0);
    delivery_mode::set(0x2ULL);
    delivery_mode::dump(0);
    delivery_mode::set(0x4ULL);
    delivery_mode::dump(0);
    delivery_mode::set(0x5ULL);
    delivery_mode::dump(0);
    delivery_mode::set(0x7ULL);
    delivery_mode::dump(0);
    delivery_mode::set(0x1ULL);
    delivery_mode::dump(0);
}

TEST_CASE("msrs_ia32_x2apic_lvt_lint1_delivery_status")
{
    MockRepository mocks;
    setup_intrinsics(mocks);

    using namespace msrs::ia32_x2apic_lvt_lint1;

    delivery_status::set(0xFFFFFFFFFFFFFFFFULL);
    CHECK(delivery_status::get() == (delivery_status::mask >> delivery_status::from));

    delivery_status::set(delivery_status::mask, 0xFFFFFFFFFFFFFFFFULL);
    CHECK(delivery_status::get(delivery_status::mask) == (delivery_status::mask >> delivery_status::from));

    delivery_status::set(0x0000000000000000ULL);
    dump(0);
    delivery_status::set(0xFFFFFFFFFFFFFFFFULL);
    dump(0);
}

TEST_CASE("msrs_ia32_x2apic_lvt_lint1_polarity")
{
    MockRepository mocks;
    setup_intrinsics(mocks);

    using namespace msrs::ia32_x2apic_lvt_lint1;

    polarity::set(0xFFFFFFFFFFFFFFFFULL);
    CHECK(polarity::get() == (polarity::mask >> polarity::from));

    polarity::set(polarity::mask, 0xFFFFFFFFFFFFFFFFULL);
    CHECK(polarity::get(polarity::mask) == (polarity::mask >> polarity::from));

    polarity::set(0x0000000000000000ULL);
    dump(0);
    polarity::set(0xFFFFFFFFFFFFFFFFULL);
    dump(0);
}

TEST_CASE("msrs_ia32_x2apic_lvt_lint1_remote_irr")
{
    MockRepository mocks;
    setup_intrinsics(mocks);

    using namespace msrs::ia32_x2apic_lvt_lint1;

    g_msrs[addr] = 0xFFFFFFFFFFFFFFFFULL;
    CHECK(remote_irr::get() == (remote_irr::mask >> remote_irr::from));

    g_msrs[addr] = 0xFFFFFFFFFFFFFFFFULL;
    CHECK(remote_irr::get(remote_irr::mask) == (remote_irr::mask >> remote_irr::from));
}

TEST_CASE("msrs_ia32_x2apic_lvt_lint1_trigger_mode")
{
    MockRepository mocks;
    setup_intrinsics(mocks);

    using namespace msrs::ia32_x2apic_lvt_lint1;

    trigger_mode::set(0xFFFFFFFFFFFFFFFFULL);
    CHECK(trigger_mode::get() == (trigger_mode::mask >> trigger_mode::from));

    trigger_mode::set(trigger_mode::mask, 0xFFFFFFFFFFFFFFFFULL);
    CHECK(trigger_mode::get(trigger_mode::mask) == (trigger_mode::mask >> trigger_mode::from));

    trigger_mode::set(0x0000000000000000ULL);
    dump(0);
    trigger_mode::set(0xFFFFFFFFFFFFFFFFULL);
    dump(0);
}

TEST_CASE("msrs_ia32_x2apic_lvt_lint1_mask_bit")
{
    MockRepository mocks;
    setup_intrinsics(mocks);

    using namespace msrs::ia32_x2apic_lvt_lint1;

    mask_bit::enable();
    CHECK(mask_bit::is_enabled());
    mask_bit::disable();
    CHECK(mask_bit::is_disabled());

    mask_bit::enable(mask_bit::mask);
    CHECK(mask_bit::is_enabled(mask_bit::mask));
    mask_bit::disable(0x0);
    CHECK(mask_bit::is_disabled(0x0));
}

TEST_CASE("msrs_ia32_x2apic_lvt_error")
{
    MockRepository mocks;
    setup_intrinsics(mocks);

    using namespace msrs::ia32_x2apic_lvt_error;

    set(0xFFFFFFFFFFFFFFFFULL);
    CHECK(get() == 0xFFFFFFFFFFFFFFFFULL);
    dump(0);
}

TEST_CASE("msrs_ia32_x2apic_lvt_error_vector")
{
    MockRepository mocks;
    setup_intrinsics(mocks);

    using namespace msrs::ia32_x2apic_lvt_error;

    vector::set(0xFFFFFFFFFFFFFFFFULL);
    CHECK(vector::get() == (vector::mask >> vector::from));

    vector::set(vector::mask, 0xFFFFFFFFFFFFFFFFULL);
    CHECK(vector::get(vector::mask) == (vector::mask >> vector::from));
}

TEST_CASE("msrs_ia32_x2apic_lvt_error_delivery_status")
{
    MockRepository mocks;
    setup_intrinsics(mocks);

    using namespace msrs::ia32_x2apic_lvt_error;

    delivery_status::set(0xFFFFFFFFFFFFFFFFULL);
    CHECK(delivery_status::get() == (delivery_status::mask >> delivery_status::from));

    delivery_status::set(delivery_status::mask, 0xFFFFFFFFFFFFFFFFULL);
    CHECK(delivery_status::get(delivery_status::mask) == (delivery_status::mask >> delivery_status::from));

    delivery_status::set(0x0000000000000000ULL);
    dump(0);
    delivery_status::set(0xFFFFFFFFFFFFFFFFULL);
    dump(0);
}

TEST_CASE("msrs_ia32_x2apic_init_count")
{
    MockRepository mocks;
    setup_intrinsics(mocks);

    using namespace msrs::ia32_x2apic_init_count;

    set(0xFFFFFFFFFFFFFFFFULL);
    CHECK(get() == 0xFFFFFFFFFFFFFFFFULL);
    dump(0);
}

TEST_CASE("msrs_ia32_x2apic_cur_count")
{
    MockRepository mocks;
    setup_intrinsics(mocks);

    using namespace msrs::ia32_x2apic_cur_count;

    g_msrs[addr] = 0xFFFFFFFFFFFFFFFFULL;
    CHECK(get() == 0xFFFFFFFFFFFFFFFFULL);
    dump(0);
}

TEST_CASE("msrs_ia32_x2apic_div_conf")
{
    MockRepository mocks;
    setup_intrinsics(mocks);

    using namespace msrs::ia32_x2apic_div_conf;

    set(0xFFFFFFFFFFFFFFFFULL);
    CHECK(get() == 0xFFFFFFFFFFFFFFFFULL);
    dump(0);
}

TEST_CASE("msrs_ia32_x2apic_div_conf_div_val")
{
    MockRepository mocks;
    setup_intrinsics(mocks);

    using namespace msrs::ia32_x2apic_div_conf;

    div_val::set(0xFFFFFFFFFFFFFFFFULL);
    CHECK(div_val::get() == (div_val::mask >> div_val::from));

    div_val::set(div_val::mask, 0xFFFFFFFFFFFFFFFFULL);
    CHECK(div_val::get(div_val::mask) == (div_val::mask >> div_val::from));

    div_val::set(div_val::div_by_2);
    CHECK(div_val::get() == div_val::div_by_2);
    div_val::dump(0);

    div_val::set(div_val::div_by_4);
    CHECK(div_val::get() == div_val::div_by_4);
    div_val::dump(0);

    div_val::set(div_val::div_by_8);
    CHECK(div_val::get() == div_val::div_by_8);
    div_val::dump(0);

    div_val::set(div_val::div_by_16);
    CHECK(div_val::get() == div_val::div_by_16);
    div_val::dump(0);

    div_val::set(div_val::div_by_32);
    CHECK(div_val::get() == div_val::div_by_32);
    div_val::dump(0);

    div_val::set(div_val::div_by_64);
    CHECK(div_val::get() == div_val::div_by_64);
    div_val::dump(0);

    div_val::set(div_val::div_by_128);
    CHECK(div_val::get() == div_val::div_by_128);
    div_val::dump(0);

    div_val::set(div_val::div_by_1);
    CHECK(div_val::get() == div_val::div_by_1);
    div_val::dump(0);
}

TEST_CASE("msrs_ia32_x2apic_self_ipi")
{
    MockRepository mocks;
    setup_intrinsics(mocks);

    using namespace msrs::ia32_x2apic_self_ipi;

    set(0xFFFFFFFFFFFFFFFFULL);
    CHECK(g_msrs[addr] == 0xFFFFFFFFFFFFFFFFULL);
}

TEST_CASE("msrs_ia32_x2apic_self_ipi_vector")
{
    MockRepository mocks;
    setup_intrinsics(mocks);

    using namespace msrs::ia32_x2apic_self_ipi;

    set(0x0000000000000000ULL);
    vector::set(0xFFFFFFFFFFFFFFFFULL);
    CHECK(g_msrs[addr] == vector::mask);

    vector::set(vector::mask, 0xFFFFFFFFFFFFFFFFULL);
    CHECK(g_msrs[addr] == vector::mask);
}

TEST_CASE("x2apic_supported")
{
    MockRepository mocks;
    setup_intrinsics(mocks);

    g_ecx_cpuid[cpuid::feature_information::addr] =
        cpuid::feature_information::ecx::x2apic::mask;
    CHECK(x2apic::supported());

    g_ecx_cpuid[cpuid::feature_information::addr] = 0x0;
    CHECK_FALSE(x2apic::supported());
}

TEST_CASE("x2apic_control_validate_gpa_op")
{
    MockRepository mocks;
    setup_intrinsics(mocks);
    x2apic_control ctrl;

    CHECK(ctrl.validate_gpa_op(0xFEE00000ULL, lapic_control::read) == -1);
    CHECK(ctrl.validate_gpa_op(0xFEE00020ULL, lapic_control::write) == -1);
    CHECK(ctrl.validate_gpa_op(0xFEE000B0ULL, lapic_control::read) == -1);
    CHECK(ctrl.validate_gpa_op(0xFEE00020ULL, lapic_control::read) == 0x2U);
}

TEST_CASE("x2apic_control_validate_msr_op")
{
    MockRepository mocks;
    setup_intrinsics(mocks);
    x2apic_control ctrl;

    CHECK(ctrl.validate_msr_op(0x00000000ULL, lapic_control::read) == -1);
    CHECK(ctrl.validate_msr_op(0xFFFFFFFFULL, lapic_control::read) == -1);
    CHECK(ctrl.validate_msr_op(0x00000800ULL, lapic_control::read) == -1);
    CHECK(ctrl.validate_msr_op(0x00000802ULL, lapic_control::write) == -1);
    CHECK(ctrl.validate_msr_op(0x0000080BULL, lapic_control::read) == -1);
    CHECK(ctrl.validate_msr_op(0x00000802ULL, lapic_control::read) == 0x2U);
}

TEST_CASE("x2apic_control_read_register")
{
    MockRepository mocks;
    setup_intrinsics(mocks);
    x2apic_control ctrl;

    g_msrs[msrs::ia32_x2apic_apicid::addr] = 0xFFFFFFFFFFFFFFFFULL;
    CHECK(ctrl.read_register(0x02U) == 0xFFFFFFFFFFFFFFFFULL);

    g_msrs[msrs::ia32_x2apic_apicid::addr] = 0x0ULL;
    CHECK(ctrl.read_register(0x02U) == 0x0ULL);
}

TEST_CASE("x2apic_control_write_register")
{
    MockRepository mocks;
    setup_intrinsics(mocks);
    x2apic_control ctrl;

    ctrl.write_register(0x02U, 0xFFFFFFFFFFFFFFFFULL);
    CHECK(ctrl.read_id() == 0xFFFFFFFFFFFFFFFFULL);

    ctrl.write_register(0x02U, 0x0ULL);
    CHECK(ctrl.read_id() == 0x0ULL);
}

TEST_CASE("x2apic_control_read_id")
{
    MockRepository mocks;
    setup_intrinsics(mocks);
    x2apic_control ctrl;

    g_msrs[msrs::ia32_x2apic_apicid::addr] = 0xFFFFFFFFFFFFFFFFULL;
    CHECK(ctrl.read_id() == 0xFFFFFFFFFFFFFFFFULL);

    g_msrs[msrs::ia32_x2apic_apicid::addr] = 0x0ULL;
    CHECK(ctrl.read_id() == 0x0ULL);
}

TEST_CASE("x2apic_control_read_version")
{
    MockRepository mocks;
    setup_intrinsics(mocks);
    x2apic_control ctrl;

    g_msrs[msrs::ia32_x2apic_version::addr] = 0xFFFFFFFFFFFFFFFFULL;
    CHECK(ctrl.read_version() == 0xFFFFFFFFFFFFFFFFULL);

    g_msrs[msrs::ia32_x2apic_version::addr] = 0x0ULL;
    CHECK(ctrl.read_version() == 0x0ULL);
}

TEST_CASE("x2apic_control_read_tpr")
{
    MockRepository mocks;
    setup_intrinsics(mocks);
    x2apic_control ctrl;

    msrs::ia32_x2apic_tpr::set(0xFFFFFFFFFFFFFFFFULL);
    CHECK(ctrl.read_tpr() == 0xFFFFFFFFFFFFFFFFULL);

    msrs::ia32_x2apic_tpr::set(0x0ULL);
    CHECK(ctrl.read_tpr() == 0x0ULL);
}

TEST_CASE("x2apic_control_read_ldr")
{
    MockRepository mocks;
    setup_intrinsics(mocks);
    x2apic_control ctrl;

    g_msrs[msrs::ia32_x2apic_ldr::addr] = 0xFFFFFFFFFFFFFFFFULL;
    CHECK(ctrl.read_ldr() == 0xFFFFFFFFFFFFFFFFULL);

    g_msrs[msrs::ia32_x2apic_ldr::addr] = 0x0ULL;
    CHECK(ctrl.read_ldr() == 0x0ULL);
}

TEST_CASE("x2apic_control_read_svr")
{
    MockRepository mocks;
    setup_intrinsics(mocks);
    x2apic_control ctrl;

    msrs::ia32_x2apic_sivr::set(0xFFFFFFFFFFFFFFFFULL);
    CHECK(ctrl.read_svr() == 0xFFFFFFFFFFFFFFFFULL);

    msrs::ia32_x2apic_sivr::set(0x0ULL);
    CHECK(ctrl.read_svr() == 0x0ULL);
}

TEST_CASE("x2apic_control_read_icr")
{
    MockRepository mocks;
    setup_intrinsics(mocks);
    x2apic_control ctrl;

    msrs::ia32_x2apic_icr::set(0xFFFFFFFFFFFFFFFFULL);
    CHECK(ctrl.read_icr() == 0xFFFFFFFFFFFFFFFFULL);

    msrs::ia32_x2apic_icr::set(0x0ULL);
    CHECK(ctrl.read_icr() == 0x0ULL);
}

TEST_CASE("x2apic_control_read_isr0")
{
    MockRepository mocks;
    setup_intrinsics(mocks);
    x2apic_control ctrl;

    g_msrs[msrs::ia32_x2apic_isr0::addr] = 0xFFFFFFFFFFFFFFFFULL;
    CHECK(ctrl.read_isr(ctrl.idx0) == 0xFFFFFFFFFFFFFFFFULL);

    g_msrs[msrs::ia32_x2apic_isr0::addr] = 0x0ULL;
    CHECK(ctrl.read_isr(ctrl.idx0) == 0x0ULL);
}

TEST_CASE("x2apic_control_read_isr1")
{
    MockRepository mocks;
    setup_intrinsics(mocks);
    x2apic_control ctrl;

    g_msrs[msrs::ia32_x2apic_isr1::addr] = 0xFFFFFFFFFFFFFFFFULL;
    CHECK(ctrl.read_isr(ctrl.idx1) == 0xFFFFFFFFFFFFFFFFULL);

    g_msrs[msrs::ia32_x2apic_isr1::addr] = 0x0ULL;
    CHECK(ctrl.read_isr(ctrl.idx1) == 0x0ULL);
}

TEST_CASE("x2apic_control_read_isr2")
{
    MockRepository mocks;
    setup_intrinsics(mocks);
    x2apic_control ctrl;

    g_msrs[msrs::ia32_x2apic_isr2::addr] = 0xFFFFFFFFFFFFFFFFULL;
    CHECK(ctrl.read_isr(ctrl.idx2) == 0xFFFFFFFFFFFFFFFFULL);

    g_msrs[msrs::ia32_x2apic_isr2::addr] = 0x0ULL;
    CHECK(ctrl.read_isr(ctrl.idx2) == 0x0ULL);
}

TEST_CASE("x2apic_control_read_isr3")
{
    MockRepository mocks;
    setup_intrinsics(mocks);
    x2apic_control ctrl;

    g_msrs[msrs::ia32_x2apic_isr3::addr] = 0xFFFFFFFFFFFFFFFFULL;
    CHECK(ctrl.read_isr(ctrl.idx3) == 0xFFFFFFFFFFFFFFFFULL);

    g_msrs[msrs::ia32_x2apic_isr3::addr] = 0x0ULL;
    CHECK(ctrl.read_isr(ctrl.idx3) == 0x0ULL);
}

TEST_CASE("x2apic_control_read_isr4")
{
    MockRepository mocks;
    setup_intrinsics(mocks);
    x2apic_control ctrl;

    g_msrs[msrs::ia32_x2apic_isr4::addr] = 0xFFFFFFFFFFFFFFFFULL;
    CHECK(ctrl.read_isr(ctrl.idx4) == 0xFFFFFFFFFFFFFFFFULL);

    g_msrs[msrs::ia32_x2apic_isr4::addr] = 0x0ULL;
    CHECK(ctrl.read_isr(ctrl.idx4) == 0x0ULL);
}

TEST_CASE("x2apic_control_read_isr5")
{
    MockRepository mocks;
    setup_intrinsics(mocks);
    x2apic_control ctrl;

    g_msrs[msrs::ia32_x2apic_isr5::addr] = 0xFFFFFFFFFFFFFFFFULL;
    CHECK(ctrl.read_isr(ctrl.idx5) == 0xFFFFFFFFFFFFFFFFULL);

    g_msrs[msrs::ia32_x2apic_isr5::addr] = 0x0ULL;
    CHECK(ctrl.read_isr(ctrl.idx5) == 0x0ULL);
}

TEST_CASE("x2apic_control_read_isr6")
{
    MockRepository mocks;
    setup_intrinsics(mocks);
    x2apic_control ctrl;

    g_msrs[msrs::ia32_x2apic_isr6::addr] = 0xFFFFFFFFFFFFFFFFULL;
    CHECK(ctrl.read_isr(ctrl.idx6) == 0xFFFFFFFFFFFFFFFFULL);

    g_msrs[msrs::ia32_x2apic_isr6::addr] = 0x0ULL;
    CHECK(ctrl.read_isr(ctrl.idx6) == 0x0ULL);
}

TEST_CASE("x2apic_control_read_isr7")
{
    MockRepository mocks;
    setup_intrinsics(mocks);
    x2apic_control ctrl;

    g_msrs[msrs::ia32_x2apic_isr7::addr] = 0xFFFFFFFFFFFFFFFFULL;
    CHECK(ctrl.read_isr(ctrl.idx7) == 0xFFFFFFFFFFFFFFFFULL);

    g_msrs[msrs::ia32_x2apic_isr7::addr] = 0x0ULL;
    CHECK(ctrl.read_isr(ctrl.idx7) == 0x0ULL);
}

TEST_CASE("x2apic_control_read_tmr0")
{
    MockRepository mocks;
    setup_intrinsics(mocks);
    x2apic_control ctrl;

    g_msrs[msrs::ia32_x2apic_tmr0::addr] = 0xFFFFFFFFFFFFFFFFULL;
    CHECK(ctrl.read_tmr(ctrl.idx0) == 0xFFFFFFFFFFFFFFFFULL);

    g_msrs[msrs::ia32_x2apic_tmr0::addr] = 0x0ULL;
    CHECK(ctrl.read_tmr(ctrl.idx0) == 0x0ULL);
}

TEST_CASE("x2apic_control_read_tmr1")
{
    MockRepository mocks;
    setup_intrinsics(mocks);
    x2apic_control ctrl;

    g_msrs[msrs::ia32_x2apic_tmr1::addr] = 0xFFFFFFFFFFFFFFFFULL;
    CHECK(ctrl.read_tmr(ctrl.idx1) == 0xFFFFFFFFFFFFFFFFULL);

    g_msrs[msrs::ia32_x2apic_tmr1::addr] = 0x0ULL;
    CHECK(ctrl.read_tmr(ctrl.idx1) == 0x0ULL);
}

TEST_CASE("x2apic_control_read_tmr2")
{
    MockRepository mocks;
    setup_intrinsics(mocks);
    x2apic_control ctrl;

    g_msrs[msrs::ia32_x2apic_tmr2::addr] = 0xFFFFFFFFFFFFFFFFULL;
    CHECK(ctrl.read_tmr(ctrl.idx2) == 0xFFFFFFFFFFFFFFFFULL);

    g_msrs[msrs::ia32_x2apic_tmr2::addr] = 0x0ULL;
    CHECK(ctrl.read_tmr(ctrl.idx2) == 0x0ULL);
}

TEST_CASE("x2apic_control_read_tmr3")
{
    MockRepository mocks;
    setup_intrinsics(mocks);
    x2apic_control ctrl;

    g_msrs[msrs::ia32_x2apic_tmr3::addr] = 0xFFFFFFFFFFFFFFFFULL;
    CHECK(ctrl.read_tmr(ctrl.idx3) == 0xFFFFFFFFFFFFFFFFULL);

    g_msrs[msrs::ia32_x2apic_tmr3::addr] = 0x0ULL;
    CHECK(ctrl.read_tmr(ctrl.idx3) == 0x0ULL);
}

TEST_CASE("x2apic_control_read_tmr4")
{
    MockRepository mocks;
    setup_intrinsics(mocks);
    x2apic_control ctrl;

    g_msrs[msrs::ia32_x2apic_tmr4::addr] = 0xFFFFFFFFFFFFFFFFULL;
    CHECK(ctrl.read_tmr(ctrl.idx4) == 0xFFFFFFFFFFFFFFFFULL);

    g_msrs[msrs::ia32_x2apic_tmr4::addr] = 0x0ULL;
    CHECK(ctrl.read_tmr(ctrl.idx4) == 0x0ULL);
}

TEST_CASE("x2apic_control_read_tmr5")
{
    MockRepository mocks;
    setup_intrinsics(mocks);
    x2apic_control ctrl;

    g_msrs[msrs::ia32_x2apic_tmr5::addr] = 0xFFFFFFFFFFFFFFFFULL;
    CHECK(ctrl.read_tmr(ctrl.idx5) == 0xFFFFFFFFFFFFFFFFULL);

    g_msrs[msrs::ia32_x2apic_tmr5::addr] = 0x0ULL;
    CHECK(ctrl.read_tmr(ctrl.idx5) == 0x0ULL);
}

TEST_CASE("x2apic_control_read_tmr6")
{
    MockRepository mocks;
    setup_intrinsics(mocks);
    x2apic_control ctrl;

    g_msrs[msrs::ia32_x2apic_tmr6::addr] = 0xFFFFFFFFFFFFFFFFULL;
    CHECK(ctrl.read_tmr(ctrl.idx6) == 0xFFFFFFFFFFFFFFFFULL);

    g_msrs[msrs::ia32_x2apic_tmr6::addr] = 0x0ULL;
    CHECK(ctrl.read_tmr(ctrl.idx6) == 0x0ULL);
}

TEST_CASE("x2apic_control_read_tmr7")
{
    MockRepository mocks;
    setup_intrinsics(mocks);
    x2apic_control ctrl;

    g_msrs[msrs::ia32_x2apic_tmr7::addr] = 0xFFFFFFFFFFFFFFFFULL;
    CHECK(ctrl.read_tmr(ctrl.idx7) == 0xFFFFFFFFFFFFFFFFULL);

    g_msrs[msrs::ia32_x2apic_tmr7::addr] = 0x0ULL;
    CHECK(ctrl.read_tmr(ctrl.idx7) == 0x0ULL);
}

TEST_CASE("x2apic_control_read_irr0")
{
    MockRepository mocks;
    setup_intrinsics(mocks);
    x2apic_control ctrl;

    g_msrs[msrs::ia32_x2apic_irr0::addr] = 0xFFFFFFFFFFFFFFFFULL;
    CHECK(ctrl.read_irr(ctrl.idx0) == 0xFFFFFFFFFFFFFFFFULL);

    g_msrs[msrs::ia32_x2apic_irr0::addr] = 0x0ULL;
    CHECK(ctrl.read_irr(ctrl.idx0) == 0x0ULL);
}

TEST_CASE("x2apic_control_read_irr1")
{
    MockRepository mocks;
    setup_intrinsics(mocks);
    x2apic_control ctrl;

    g_msrs[msrs::ia32_x2apic_irr1::addr] = 0xFFFFFFFFFFFFFFFFULL;
    CHECK(ctrl.read_irr(ctrl.idx1) == 0xFFFFFFFFFFFFFFFFULL);

    g_msrs[msrs::ia32_x2apic_irr1::addr] = 0x0ULL;
    CHECK(ctrl.read_irr(ctrl.idx1) == 0x0ULL);
}

TEST_CASE("x2apic_control_read_irr2")
{
    MockRepository mocks;
    setup_intrinsics(mocks);
    x2apic_control ctrl;

    g_msrs[msrs::ia32_x2apic_irr2::addr] = 0xFFFFFFFFFFFFFFFFULL;
    CHECK(ctrl.read_irr(ctrl.idx2) == 0xFFFFFFFFFFFFFFFFULL);

    g_msrs[msrs::ia32_x2apic_irr2::addr] = 0x0ULL;
    CHECK(ctrl.read_irr(ctrl.idx2) == 0x0ULL);
}

TEST_CASE("x2apic_control_read_irr3")
{
    MockRepository mocks;
    setup_intrinsics(mocks);
    x2apic_control ctrl;

    g_msrs[msrs::ia32_x2apic_irr3::addr] = 0xFFFFFFFFFFFFFFFFULL;
    CHECK(ctrl.read_irr(ctrl.idx3) == 0xFFFFFFFFFFFFFFFFULL);

    g_msrs[msrs::ia32_x2apic_irr3::addr] = 0x0ULL;
    CHECK(ctrl.read_irr(ctrl.idx3) == 0x0ULL);
}

TEST_CASE("x2apic_control_read_irr4")
{
    MockRepository mocks;
    setup_intrinsics(mocks);
    x2apic_control ctrl;

    g_msrs[msrs::ia32_x2apic_irr4::addr] = 0xFFFFFFFFFFFFFFFFULL;
    CHECK(ctrl.read_irr(ctrl.idx4) == 0xFFFFFFFFFFFFFFFFULL);

    g_msrs[msrs::ia32_x2apic_irr4::addr] = 0x0ULL;
    CHECK(ctrl.read_irr(ctrl.idx4) == 0x0ULL);
}

TEST_CASE("x2apic_control_read_irr5")
{
    MockRepository mocks;
    setup_intrinsics(mocks);
    x2apic_control ctrl;

    g_msrs[msrs::ia32_x2apic_irr5::addr] = 0xFFFFFFFFFFFFFFFFULL;
    CHECK(ctrl.read_irr(ctrl.idx5) == 0xFFFFFFFFFFFFFFFFULL);

    g_msrs[msrs::ia32_x2apic_irr5::addr] = 0x0ULL;
    CHECK(ctrl.read_irr(ctrl.idx5) == 0x0ULL);
}

TEST_CASE("x2apic_control_read_irr6")
{
    MockRepository mocks;
    setup_intrinsics(mocks);
    x2apic_control ctrl;

    g_msrs[msrs::ia32_x2apic_irr6::addr] = 0xFFFFFFFFFFFFFFFFULL;
    CHECK(ctrl.read_irr(ctrl.idx6) == 0xFFFFFFFFFFFFFFFFULL);

    g_msrs[msrs::ia32_x2apic_irr6::addr] = 0x0ULL;
    CHECK(ctrl.read_irr(ctrl.idx6) == 0x0ULL);
}

TEST_CASE("x2apic_control_read_irr7")
{
    MockRepository mocks;
    setup_intrinsics(mocks);
    x2apic_control ctrl;

    g_msrs[msrs::ia32_x2apic_irr7::addr] = 0xFFFFFFFFFFFFFFFFULL;
    CHECK(ctrl.read_irr(ctrl.idx7) == 0xFFFFFFFFFFFFFFFFULL);

    g_msrs[msrs::ia32_x2apic_irr7::addr] = 0x0ULL;
    CHECK(ctrl.read_irr(ctrl.idx7) == 0x0ULL);
}

TEST_CASE("x2apic_control_read_lvt_cmci")
{
    MockRepository mocks;
    setup_intrinsics(mocks);
    x2apic_control ctrl;

    msrs::ia32_x2apic_lvt_cmci::set(0xFFFFFFFFFFFFFFFFULL);
    CHECK(ctrl.read_lvt(ctrl.cmci) == 0xFFFFFFFFFFFFFFFFULL);

    msrs::ia32_x2apic_lvt_cmci::set(0x0ULL);
    CHECK(ctrl.read_lvt(ctrl.cmci) == 0x0ULL);
}

TEST_CASE("x2apic_control_read_lvt_timer")
{
    MockRepository mocks;
    setup_intrinsics(mocks);
    x2apic_control ctrl;

    msrs::ia32_x2apic_lvt_timer::set(0xFFFFFFFFFFFFFFFFULL);
    CHECK(ctrl.read_lvt(ctrl.timer) == 0xFFFFFFFFFFFFFFFFULL);

    msrs::ia32_x2apic_lvt_timer::set(0x0ULL);
    CHECK(ctrl.read_lvt(ctrl.timer) == 0x0ULL);
}

TEST_CASE("x2apic_control_read_lvt_thermal")
{
    MockRepository mocks;
    setup_intrinsics(mocks);
    x2apic_control ctrl;

    msrs::ia32_x2apic_lvt_thermal::set(0xFFFFFFFFFFFFFFFFULL);
    CHECK(ctrl.read_lvt(ctrl.thermal) == 0xFFFFFFFFFFFFFFFFULL);

    msrs::ia32_x2apic_lvt_thermal::set(0x0ULL);
    CHECK(ctrl.read_lvt(ctrl.thermal) == 0x0ULL);
}

TEST_CASE("x2apic_control_read_lvt_pmi")
{
    MockRepository mocks;
    setup_intrinsics(mocks);
    x2apic_control ctrl;

    msrs::ia32_x2apic_lvt_pmi::set(0xFFFFFFFFFFFFFFFFULL);
    CHECK(ctrl.read_lvt(ctrl.perf) == 0xFFFFFFFFFFFFFFFFULL);

    msrs::ia32_x2apic_lvt_pmi::set(0x0ULL);
    CHECK(ctrl.read_lvt(ctrl.perf) == 0x0ULL);
}

TEST_CASE("x2apic_control_read_lvt_lint0")
{
    MockRepository mocks;
    setup_intrinsics(mocks);
    x2apic_control ctrl;

    msrs::ia32_x2apic_lvt_lint0::set(0xFFFFFFFFFFFFFFFFULL);
    CHECK(ctrl.read_lvt(ctrl.lint0) == 0xFFFFFFFFFFFFFFFFULL);

    msrs::ia32_x2apic_lvt_lint0::set(0x0ULL);
    CHECK(ctrl.read_lvt(ctrl.lint0) == 0x0ULL);
}

TEST_CASE("x2apic_control_read_lvt_lint1")
{
    MockRepository mocks;
    setup_intrinsics(mocks);
    x2apic_control ctrl;

    msrs::ia32_x2apic_lvt_lint1::set(0xFFFFFFFFFFFFFFFFULL);
    CHECK(ctrl.read_lvt(ctrl.lint1) == 0xFFFFFFFFFFFFFFFFULL);

    msrs::ia32_x2apic_lvt_lint1::set(0x0ULL);
    CHECK(ctrl.read_lvt(ctrl.lint1) == 0x0ULL);
}

TEST_CASE("x2apic_control_read_lvt_error")
{
    MockRepository mocks;
    setup_intrinsics(mocks);
    x2apic_control ctrl;

    msrs::ia32_x2apic_lvt_error::set(0xFFFFFFFFFFFFFFFFULL);
    CHECK(ctrl.read_lvt(ctrl.error) == 0xFFFFFFFFFFFFFFFFULL);

    msrs::ia32_x2apic_lvt_error::set(0x0ULL);
    CHECK(ctrl.read_lvt(ctrl.error) == 0x0ULL);
}

TEST_CASE("x2apic_control_read_count_initial")
{
    MockRepository mocks;
    setup_intrinsics(mocks);
    x2apic_control ctrl;

    g_msrs[msrs::ia32_x2apic_init_count::addr] = 0xFFFFFFFFFFFFFFFFULL;
    CHECK(ctrl.read_count(ctrl.initial) == 0xFFFFFFFFFFFFFFFFULL);

    g_msrs[msrs::ia32_x2apic_init_count::addr] = 0x0ULL;
    CHECK(ctrl.read_count(ctrl.initial) == 0x0ULL);
}

TEST_CASE("x2apic_control_read_count_current")
{
    MockRepository mocks;
    setup_intrinsics(mocks);
    x2apic_control ctrl;

    g_msrs[msrs::ia32_x2apic_cur_count::addr] = 0xFFFFFFFFFFFFFFFFULL;
    CHECK(ctrl.read_count(ctrl.current) == 0xFFFFFFFFFFFFFFFFULL);

    g_msrs[msrs::ia32_x2apic_cur_count::addr] = 0x0ULL;
    CHECK(ctrl.read_count(ctrl.current) == 0x0ULL);
}

TEST_CASE("x2apic_control_read_div_config")
{
    MockRepository mocks;
    setup_intrinsics(mocks);
    x2apic_control ctrl;

    msrs::ia32_x2apic_div_conf::set(0xFFFFFFFFFFFFFFFFULL);
    CHECK(ctrl.read_div_config() == 0xFFFFFFFFFFFFFFFFULL);

    msrs::ia32_x2apic_div_conf::set(0x0ULL);
    CHECK(ctrl.read_div_config() == 0x0ULL);
}

TEST_CASE("x2apic_control_write_eoi")
{
    MockRepository mocks;
    setup_intrinsics(mocks);
    x2apic_control ctrl;

    ctrl.write_eoi();
    CHECK(g_msrs[msrs::ia32_x2apic_eoi::addr] == 0x0ULL);
}

TEST_CASE("x2apic_control_write_tpr")
{
    MockRepository mocks;
    setup_intrinsics(mocks);
    x2apic_control ctrl;

    ctrl.write_tpr(0xFFFFFFFFULL);
    CHECK(msrs::ia32_x2apic_tpr::get() == 0xFFFFFFFFULL);

    ctrl.write_tpr(0x0ULL);
    CHECK(msrs::ia32_x2apic_tpr::get() == 0x0ULL);
}

TEST_CASE("x2apic_control_write_svr")
{
    MockRepository mocks;
    setup_intrinsics(mocks);
    x2apic_control ctrl;

    ctrl.write_svr(0xFFFFFFFFULL);
    CHECK(msrs::ia32_x2apic_sivr::get() == 0xFFFFFFFFULL);

    ctrl.write_svr(0x0ULL);
    CHECK(msrs::ia32_x2apic_sivr::get() == 0x0ULL);
}

TEST_CASE("x2apic_control_write_icr")
{
    MockRepository mocks;
    setup_intrinsics(mocks);
    x2apic_control ctrl;

    ctrl.write_icr(0xFFFFFFFFULL);
    CHECK(msrs::ia32_x2apic_icr::get() == 0xFFFFFFFFULL);

    ctrl.write_icr(0x0ULL);
    CHECK(msrs::ia32_x2apic_icr::get() == 0x0ULL);
}

TEST_CASE("x2apic_control_write_lvt_cmci")
{
    MockRepository mocks;
    setup_intrinsics(mocks);
    x2apic_control ctrl;

    ctrl.write_lvt(ctrl.cmci, 0xFFFFFFFFULL);
    CHECK(msrs::ia32_x2apic_lvt_cmci::get() == 0xFFFFFFFFULL);

    ctrl.write_lvt(ctrl.cmci, 0x0ULL);
    CHECK(msrs::ia32_x2apic_lvt_cmci::get() == 0x0ULL);
}

TEST_CASE("x2apic_control_write_lvt_timer")
{
    MockRepository mocks;
    setup_intrinsics(mocks);
    x2apic_control ctrl;

    ctrl.write_lvt(ctrl.timer, 0xFFFFFFFFULL);
    CHECK(msrs::ia32_x2apic_lvt_timer::get() == 0xFFFFFFFFULL);

    ctrl.write_lvt(ctrl.timer, 0x0ULL);
    CHECK(msrs::ia32_x2apic_lvt_timer::get() == 0x0ULL);
}

TEST_CASE("x2apic_control_write_lvt_thermal")
{
    MockRepository mocks;
    setup_intrinsics(mocks);
    x2apic_control ctrl;

    ctrl.write_lvt(ctrl.thermal, 0xFFFFFFFFULL);
    CHECK(msrs::ia32_x2apic_lvt_thermal::get() == 0xFFFFFFFFULL);

    ctrl.write_lvt(ctrl.thermal, 0x0ULL);
    CHECK(msrs::ia32_x2apic_lvt_thermal::get() == 0x0ULL);
}

TEST_CASE("x2apic_control_write_lvt_perf")
{
    MockRepository mocks;
    setup_intrinsics(mocks);
    x2apic_control ctrl;

    ctrl.write_lvt(ctrl.perf, 0xFFFFFFFFULL);
    CHECK(msrs::ia32_x2apic_lvt_pmi::get() == 0xFFFFFFFFULL);

    ctrl.write_lvt(ctrl.perf, 0x0ULL);
    CHECK(msrs::ia32_x2apic_lvt_pmi::get() == 0x0ULL);
}

TEST_CASE("x2apic_control_write_lvt_lint0")
{
    MockRepository mocks;
    setup_intrinsics(mocks);
    x2apic_control ctrl;

    ctrl.write_lvt(ctrl.lint0, 0xFFFFFFFFULL);
    CHECK(msrs::ia32_x2apic_lvt_lint0::get() == 0xFFFFFFFFULL);

    ctrl.write_lvt(ctrl.lint0, 0x0ULL);
    CHECK(msrs::ia32_x2apic_lvt_lint0::get() == 0x0ULL);
}

TEST_CASE("x2apic_control_write_lvt_lint1")
{
    MockRepository mocks;
    setup_intrinsics(mocks);
    x2apic_control ctrl;

    ctrl.write_lvt(ctrl.lint1, 0xFFFFFFFFULL);
    CHECK(msrs::ia32_x2apic_lvt_lint1::get() == 0xFFFFFFFFULL);

    ctrl.write_lvt(ctrl.lint1, 0x0ULL);
    CHECK(msrs::ia32_x2apic_lvt_lint1::get() == 0x0ULL);
}

TEST_CASE("x2apic_control_write_lvt_error")
{
    MockRepository mocks;
    setup_intrinsics(mocks);
    x2apic_control ctrl;

    ctrl.write_lvt(ctrl.error, 0xFFFFFFFFULL);
    CHECK(msrs::ia32_x2apic_lvt_error::get() == 0xFFFFFFFFULL);

    ctrl.write_lvt(ctrl.error, 0x0ULL);
    CHECK(msrs::ia32_x2apic_lvt_error::get() == 0x0ULL);
}

TEST_CASE("x2apic_control_write_init_count")
{
    MockRepository mocks;
    setup_intrinsics(mocks);
    x2apic_control ctrl;

    ctrl.write_init_count(0xFFFFFFFFULL);
    CHECK(msrs::ia32_x2apic_init_count::get() == 0xFFFFFFFFULL);

    ctrl.write_init_count(0x0ULL);
    CHECK(msrs::ia32_x2apic_init_count::get() == 0x0ULL);
}

TEST_CASE("x2apic_control_write_div_config")
{
    MockRepository mocks;
    setup_intrinsics(mocks);
    x2apic_control ctrl;

    ctrl.write_div_config(0xFFFFFFFFULL);
    CHECK(msrs::ia32_x2apic_div_conf::get() == 0xFFFFFFFFULL);

    ctrl.write_div_config(0x0ULL);
    CHECK(msrs::ia32_x2apic_div_conf::get() == 0x0ULL);
}

TEST_CASE("x2apic_control_write_self_ipi")
{
    MockRepository mocks;
    setup_intrinsics(mocks);
    x2apic_control ctrl;

    ctrl.write_self_ipi(0xFFULL);
    CHECK(g_msrs[msrs::ia32_x2apic_self_ipi::addr] == 0xFFULL);

    ctrl.write_self_ipi(0x88ULL);
    CHECK(g_msrs[msrs::ia32_x2apic_self_ipi::addr] == 0x88ULL);
}

TEST_CASE("x2apic_control_level_triggered")
{
    MockRepository mocks;
    setup_intrinsics(mocks);
    x2apic_control ctrl;

    g_msrs[msrs::ia32_x2apic_tmr0::addr] = 0x0ULL;
    CHECK_FALSE(ctrl.level_triggered(1));
    g_msrs[msrs::ia32_x2apic_tmr0::addr] = 0x2ULL;
    CHECK(ctrl.level_triggered(1));

    g_msrs[msrs::ia32_x2apic_tmr1::addr] = 0x0ULL;
    CHECK_FALSE(ctrl.level_triggered(34));
    g_msrs[msrs::ia32_x2apic_tmr1::addr] = 0x4ULL;
    CHECK(ctrl.level_triggered(34));

    g_msrs[msrs::ia32_x2apic_tmr2::addr] = 0x0ULL;
    CHECK_FALSE(ctrl.level_triggered(67));
    g_msrs[msrs::ia32_x2apic_tmr2::addr] = 0x8ULL;
    CHECK(ctrl.level_triggered(67));

    g_msrs[msrs::ia32_x2apic_tmr3::addr] = 0x0ULL;
    CHECK_FALSE(ctrl.level_triggered(100));
    g_msrs[msrs::ia32_x2apic_tmr3::addr] = 0x10ULL;
    CHECK(ctrl.level_triggered(100));

    g_msrs[msrs::ia32_x2apic_tmr4::addr] = 0x0ULL;
    CHECK_FALSE(ctrl.level_triggered(133));
    g_msrs[msrs::ia32_x2apic_tmr4::addr] = 0x20ULL;
    CHECK(ctrl.level_triggered(133));

    g_msrs[msrs::ia32_x2apic_tmr5::addr] = 0x0ULL;
    CHECK_FALSE(ctrl.level_triggered(166));
    g_msrs[msrs::ia32_x2apic_tmr5::addr] = 0x40ULL;
    CHECK(ctrl.level_triggered(166));

    g_msrs[msrs::ia32_x2apic_tmr6::addr] = 0x0ULL;
    CHECK_FALSE(ctrl.level_triggered(199));
    g_msrs[msrs::ia32_x2apic_tmr6::addr] = 0x80ULL;
    CHECK(ctrl.level_triggered(199));

    g_msrs[msrs::ia32_x2apic_tmr7::addr] = 0x0ULL;
    CHECK_FALSE(ctrl.level_triggered(232));
    g_msrs[msrs::ia32_x2apic_tmr7::addr] = 0x100ULL;
    CHECK(ctrl.level_triggered(232));
}

TEST_CASE("x2apic_control_in_service")
{
    MockRepository mocks;
    setup_intrinsics(mocks);
    x2apic_control ctrl;

    g_msrs[msrs::ia32_x2apic_isr0::addr] = 0x0ULL;
    CHECK_FALSE(ctrl.in_service(1));
    g_msrs[msrs::ia32_x2apic_isr0::addr] = 0x2ULL;
    CHECK(ctrl.in_service(1));

    g_msrs[msrs::ia32_x2apic_isr1::addr] = 0x0ULL;
    CHECK_FALSE(ctrl.in_service(34));
    g_msrs[msrs::ia32_x2apic_isr1::addr] = 0x4ULL;
    CHECK(ctrl.in_service(34));

    g_msrs[msrs::ia32_x2apic_isr2::addr] = 0x0ULL;
    CHECK_FALSE(ctrl.in_service(67));
    g_msrs[msrs::ia32_x2apic_isr2::addr] = 0x8ULL;
    CHECK(ctrl.in_service(67));

    g_msrs[msrs::ia32_x2apic_isr3::addr] = 0x0ULL;
    CHECK_FALSE(ctrl.in_service(100));
    g_msrs[msrs::ia32_x2apic_isr3::addr] = 0x10ULL;
    CHECK(ctrl.in_service(100));

    g_msrs[msrs::ia32_x2apic_isr4::addr] = 0x0ULL;
    CHECK_FALSE(ctrl.in_service(133));
    g_msrs[msrs::ia32_x2apic_isr4::addr] = 0x20ULL;
    CHECK(ctrl.in_service(133));

    g_msrs[msrs::ia32_x2apic_isr5::addr] = 0x0ULL;
    CHECK_FALSE(ctrl.in_service(166));
    g_msrs[msrs::ia32_x2apic_isr5::addr] = 0x40ULL;
    CHECK(ctrl.in_service(166));

    g_msrs[msrs::ia32_x2apic_isr6::addr] = 0x0ULL;
    CHECK_FALSE(ctrl.in_service(199));
    g_msrs[msrs::ia32_x2apic_isr6::addr] = 0x80ULL;
    CHECK(ctrl.in_service(199));

    g_msrs[msrs::ia32_x2apic_isr7::addr] = 0x0ULL;
    CHECK_FALSE(ctrl.in_service(232));
    g_msrs[msrs::ia32_x2apic_isr7::addr] = 0x100ULL;
    CHECK(ctrl.in_service(232));
}

#endif
