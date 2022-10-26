use kernel::prelude::*;
use rust_mymem;

module! {
    type: MymemBenchmark,
    name: "mymem_benchmark",
    author: "Guojun Chen",
    description: "Rust mymem driver benchmark module",
    license: "GPL",
}

struct MymemBenchmark;
impl kernel::Module for MymemBenchmark {
    fn init(_name: &'static CStr, _module: &'static ThisModule) -> Result<Self> {
        pr_info!("Mymem benchmark (init)\n");
        pr_info!("Call mymem hello: {}\n", rust_mymem::hello());
        Ok(MymemBenchmark)
    }
}

impl Drop for MymemBenchmark {
    fn drop(&mut self) {
        pr_info!("Mymem benchmark (exit)\n");
    }
}