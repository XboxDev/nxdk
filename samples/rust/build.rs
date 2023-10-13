extern crate core;
use std::env;

fn main() {
    let nxdk_dir = match env::var("NXDK_DIR") {
        Ok(value) => value,
        Err(e) => panic!("Error getting NXDK_DIR variable: {}", e)
    };

    println!("cargo:build-std=core");
    println!("cargo:rustc-link-search={}/lib/xboxkrnl", nxdk_dir);
    println!("cargo:rustc-link-lib=static=libxboxkrnl");
    println!("cargo:rustc-link-search={}/lib/", nxdk_dir);
    println!("cargo:rustc-link-lib=static=libpdclib");
    println!("cargo:rustc-link-lib=static=libwinapi");
    println!("cargo:rustc-link-lib=static=libxboxrt");
    println!("cargo:rustc-link-lib=static=libnxdk_hal");



}