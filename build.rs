use std::env;
use std::fs::File;
use std::io::Write;
use std::path::{PathBuf};

extern crate cc;
#[macro_use] extern crate indoc;

fn ite<A>(p: bool, t: A, f: A) -> A {
    if p {
        t
    } else {
        f
    }
}

fn generate_config(out: &PathBuf) -> PathBuf {
    let path = out.join("FreeRTOSConfig.h");

    let mut f= File::create(&path).unwrap();

    #[cfg(feature = "tick_rate_hz_100")] macro_rules! tick_rate { () => ("100") }
    #[cfg(feature = "tick_rate_hz_250")] macro_rules! tick_rate { () => ("250") }

    #[cfg(feature = "max_priorities_5")] macro_rules! max_priorities { () => ("5") }

    #[cfg(feature = "named_tasks_16")] macro_rules! task_name_len { () => ("16") }
    #[cfg(feature = "named_tasks_32")] macro_rules! task_name_len { () => ("32") }
    #[cfg(feature = "named_tasks_256")] macro_rules! task_name_len { () => ("256") }
    #[cfg(all(not(feature = "named_tasks_16"), not(feature = "named_tasks_32"), not(feature = "named_tasks_256"),))] macro_rules! task_name_len { () => ("0") }

    f.write_all(format!(include_str!("lib/include/FreeRTOSConfig.h.in"),
        preemption = ite(cfg!(feature = "preemption"), "1", "0"),
        task_name_len = task_name_len!(),
        tickless = ite(cfg!(feature = "tickless"), "1", "0"),
        tick_rate_hz = tick_rate!(),
        max_priorities = max_priorities!(),
        mutexes = ite(cfg!(feature = "mutexes"), "1", "0"),
        recursive_mutexes = ite(cfg!(feature = "recursive_mutexes"), "1", "0"),
        counting_semaphore = ite(cfg!(feature = "counting_semaphore"), "1", "0"),
        alternative_api = ite(cfg!(feature = "alternative_api"), "1", "0"),
        static_allocation = ite(cfg!(feature = "static_allocation"), "1", "0"),
        dynamic_allocation = ite(cfg!(feature = "dynamic_allocation"), "1", "0"),
        idle_hook = ite(cfg!(feature = "idle_hook"), "1", "0"),
        tick_hook = ite(cfg!(feature = "tick_hook"), "1", "0"),
        check_stack_overflow = ite(cfg!(feature = "check_stack_overflow"), "1", "0"),
        malloc_failed_hook = ite(cfg!(feature = "malloc_failed_hook"), "1", "0"),
        daemon_hook = ite(cfg!(feature = "daemon_hook"), "1", "0"),
        timers = ite(cfg!(feature = "timers"), "1", "0")
    ).as_bytes()).unwrap();

    path
}



fn main() {
    // Put the linker script somewhere the linker can find it
    let out = &PathBuf::from(env::var_os("OUT_DIR").unwrap());

    let cfg = generate_config(out);
    let cfg = cfg.as_path();

    #[cfg(feature = "port_cm3")] macro_rules! port { () => ("lib/portable/ARM_CM3") }
    #[cfg(feature = "port_cm0")] macro_rules! port { () => ("lib/portable/ARM_CM0") }

    let sources: Vec<&str> = vec![
        "lib/croutine.c",
        "lib/event_groups.c",
        "lib/list.c",
        "lib/queue.c",
        "lib/stream_buffer.c",
        "lib/tasks.c",
        "lib/timers.c",
        concat!(port!(), "/port.c")
    ];


    let headers = vec![
        "lib/include/xFreeRTOS.h",
        cfg.as_os_str().to_str().unwrap()
    ];

    cc::Build::new()
        .files(&sources)
        .out_dir(out)
        .include(out)
        .include("lib/include")
        .include(port!())
        .opt_level_str("s")
        .pic(false)
        .compile("cextension");

    println!("cargo:rustc-link-lib=static=cextension");

    sources.iter().for_each( |f| {
        println!("cargo:rerun-if-changed={}", f);
    });

    let bindings = headers.iter()
        .fold(bindgen::Builder::default(),  |b, f| b.header(f.to_string()))
        .clang_arg(format!("-I{}", out.display()))
        .clang_arg("-Ilib/include")
        .clang_arg(format!("-I{}", port!()))
        .raw_line("#![allow(non_snake_case)]")
        .raw_line("#![allow(non_camel_case_types)]")
        .raw_line("#![allow(non_upper_case_globals)]")
        .use_core()
        .ctypes_prefix("c_types")
        .generate()
        .expect("Unable to generate bindings");

    //Apparantly we shouldn't do this, but CLion doesn't recognize the generated source otherwise...
    let mut f = File::create("src/bindings.rs").unwrap();
//    let mut f = File::create(out_path.join("bindings.rs")).unwrap();

    f.write_all(bindings.to_string().as_bytes()).unwrap();
    f.write_all("\n\n".as_bytes()).unwrap();


    //TODO determine these values on the fly
    f.write_all(indoc!("
        pub mod c_types {
            pub type c_int = i32;
            pub type c_uint = u32;
            pub type c_ulong = u32;
            pub type c_ushort = u16;
            pub type c_short = i16;
            pub type c_schar = i8;
            pub type c_char = i8;
            pub type c_long = i32;
            pub type c_longlong = i64;
            pub type c_ulonglong = u64;
            pub enum c_void {}
        }
      ").as_bytes()).unwrap();

    headers.iter().for_each( |f| {
        println!("cargo:rerun-if-changed={}", f);
    });

}
