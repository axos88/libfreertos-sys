use std::env;
use std::fs::File;
use std::io::Write;
use std::path::{PathBuf};
extern crate cc;
#[macro_use] extern crate indoc;

fn bool2int(p: bool) -> &'static str {
    if p {
        "1"
    } else {
        "0"
    }
}

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

    f.write_all(format!(include_str!("libFreeRTOS/include/FreeRTOSConfig.h.in"),
        preemption = bool2int(cfg!(feature = "preemption")),
        max_task_name_len = ite(cfg!(feature = "named_tasks"), 16, 0)
    ).as_bytes()).unwrap();

    path
}



fn main() {
    // Put the linker script somewhere the linker can find it
    let out = &PathBuf::from(env::var_os("OUT_DIR").unwrap());

    let cfg = generate_config(out);
    let cfg = cfg.as_path();

    let sources: Vec<&str> = vec![
        "libFreeRTOS/src/croutine.c",
        "libFreeRTOS/src/heap.c",
        "libFreeRTOS/src/list.c",
        "libFreeRTOS/src/port.c",
        "libFreeRTOS/src/queue.c",
        "libFreeRTOS/src/tasks.c",
        "libFreeRTOS/src/shims.c"
    ];

    let headers = vec![
        "libFreeRTOS/include/xFreeRTOS.h",
        cfg.as_os_str().to_str().unwrap()
    ];

    cc::Build::new()
        .files(&sources)
        .out_dir(out)
        .include(out)
        .include("libFreeRTOS/include")
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
        .clang_arg("-IlibFreeRTOS/include")
        .use_core()
        .ctypes_prefix("c_types")
        .generate()
        .expect("Unable to generate bindings");

    //Apparantly we shouldn't do this, but CLion doesn't reckognize the generated source otherwise...
    let mut f = File::create("src/bindings.rs").unwrap();
//    let mut f = File::create(out_path.join("bindings.rs")).unwrap();

    f.write_all(bindings.to_string().as_bytes()).unwrap();
    f.write_all("\n\n".as_bytes()).unwrap();

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
            pub enum c_void {}
        }
      ").as_bytes()).unwrap();

    headers.iter().for_each( |f| {
        println!("cargo:rerun-if-changed={}", f);
    });
}
