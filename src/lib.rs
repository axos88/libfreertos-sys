#![no_std]

#[macro_use] extern crate cortex_m_rt;

extern "C" {
    fn xPortSysTickHandler();
    fn xPortPendSVHandler();
    fn vPortSVCHandler();
}


#[exception]
fn PendSV() {
    unsafe { xPortPendSVHandler() };
}

#[exception]
fn SysTick() {
    unsafe { xPortSysTickHandler() };
}

#[exception]
fn SVCall() {
    unsafe { vPortSVCHandler() };
}