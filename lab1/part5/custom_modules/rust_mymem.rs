use kernel::prelude::*;

module! {
    type: RustMymem,
    name: "rust_mymem",
    author: "Guojun Chen",
    description: "A simple module that export hello function",
    license: "GPL",
}

struct RustMymem;

impl kernel::Module for RustMymem {
    fn init(_name: &'static CStr, _module: &'static ThisModule) -> Result<Self> {
        pr_info!("RustMymem (init)\n");
        Ok(RustMymem)
    }
}

pub fn hello() -> usize {
    pr_info!("hello is being called from outside\n");
    6
}

impl Drop for RustMymem {
    fn drop(&mut self) {
        pr_info!("RustMymem (exit)\n");
    }
}