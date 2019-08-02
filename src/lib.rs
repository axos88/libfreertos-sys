#![no_std]

#![feature(allocator_api)]
#![feature(linkage)]

extern crate alloc;

mod bindings;

pub use bindings::*;
use core::ffi::c_void;
use alloc::alloc::Global;
use core::alloc::{Alloc, Layout};
use core::ptr::NonNull;
use core::mem::size_of;

#[macro_use] extern crate cortex_m_rt;

extern "C" {
    fn xPortSysTickHandler();
    fn xPortPendSVHandler();
    fn vPortSVCHandler();
}


//#[exception]
//fn PendSV() {
//    unsafe { xPortPendSVHandler() };
//}

#[exception]
fn SysTick() {
    unsafe { xPortSysTickHandler() };
}

//#[exception]
//fn SVCall() {
//    unsafe { vPortSVCHandler() };
//}

#[linkage = "weak"]
#[no_mangle]
pub extern fn vApplicationTickHook() {
}

#[linkage = "weak"]
#[no_mangle]
pub extern fn vApplicationStackOverflowHook() {
}

#[linkage = "weak"]
#[no_mangle]
pub extern fn vApplicationMallocFailedHook() {
}


#[linkage = "weak"]
#[no_mangle]
pub extern fn vApplicationIdleHook() {
}

#[no_mangle]
unsafe extern fn malloc(sz: usize) -> *mut u8 {
    let ptr = Global.alloc(Layout::from_size_align_unchecked(sz+size_of::<usize>(), 4)).unwrap().as_ptr();
    *(ptr as *mut usize) = sz;

    ptr.offset(size_of::<usize>() as isize)
}

#[no_mangle]
unsafe extern fn free(ptr: *mut u8) {
    let ptr = ptr.offset(-1 *  size_of::<usize>() as isize);
    let sz = *(ptr as *mut usize);

    Global.dealloc(NonNull::new(ptr).unwrap(), Layout::from_size_align_unchecked(sz, 4));
}

