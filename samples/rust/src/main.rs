#![no_std] // don't link the Rust standard library
#![no_main] // disable all Rust-level entry points

use core::panic::PanicInfo;

#[link (name="libnxdk_hal")]
extern "C" {
    fn XVideoSetMode(_: i32, _: i32, _: i32, _:i32);
    fn debugPrint(_: &str);
}

#[no_mangle]
fn main() -> i32 {
    unsafe {
        // 640x480, 32bpp, 60Hz
        XVideoSetMode(640, 480, 32, 60);
        debugPrint("Hello from Rust");
    }
    loop {

    }
}

// This function is called on panic.
#[panic_handler]
fn panic(_info: &PanicInfo) -> ! {
    loop {}
}