#![no_std]

#![feature(allocator_api)]
#![feature(linkage)]
#![feature(naked_functions)]

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


// This needs to be called without modifying the stack
// Turning this into naked it should basically compiles to a branch instruction (remember goto?)
#[naked]
#[exception]
unsafe fn PendSV() { xPortPendSVHandler() }

// This needs to be called without modifying the stack
// Turning this into naked it should basically compiles to a branch instruction (remember goto?)
#[naked]
#[exception]
unsafe fn SVCall() { vPortSVCHandler() }

#[exception]
unsafe fn SysTick() { xPortSysTickHandler() }

#[cfg(all(feature = "tick_hook", feature = "weak_hooks"))]
#[linkage = "weak"]
#[no_mangle]
pub extern fn vApplicationTickHook() {
}

#[cfg(all(feature = "check_stack_overflow", feature = "weak_hooks"))]
#[linkage = "weak"]
#[no_mangle]
pub extern fn vApplicationStackOverflowHook() {
}

#[cfg(all(feature = "malloc_failed_hook", feature = "weak_hooks"))]
#[linkage = "weak"]
#[no_mangle]
pub extern fn vApplicationMallocFailedHook() {
}


#[cfg(all(feature = "idle_hook", feature = "weak_hooks"))]
#[linkage = "weak"]
#[no_mangle]
pub extern fn vApplicationIdleHook() {
}


#[no_mangle]
unsafe extern fn pvPortMalloc(sz: usize) -> *mut u8 {
    //We actually need to allocate a bit more space so that we can save the allocation size, the default allocator is picky about getting that number back.
    let ptr = Global.alloc(Layout::from_size_align_unchecked(sz+size_of::<usize>(), 4)).unwrap().as_ptr();
    *(ptr as *mut usize) = sz;

    ptr.offset(size_of::<usize>() as isize)
}

#[no_mangle]
unsafe extern fn vPortFree(ptr: *mut u8) {
    //Let's hope we are trying to free a pointer that was allocated by pvPortMalloc!
    let ptr = ptr.offset(-1 *  size_of::<usize>() as isize);
    let sz = *(ptr as *mut usize);

    Global.dealloc(NonNull::new(ptr).unwrap(), Layout::from_size_align_unchecked(sz, 4));
}

