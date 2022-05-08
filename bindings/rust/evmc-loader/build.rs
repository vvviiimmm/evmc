// EVMC: Ethereum Client-VM Connector API.
// Copyright 2019 The EVMC Authors.
// Licensed under the Apache License, Version 2.0.

extern crate bindgen;
extern crate cmake;

use cmake::Config;
use std::env;
use std::path::{Path, PathBuf};

fn gen_bindings() {
    let bindings = bindgen::Builder::default()
        .header("loader.h")
        .generate_comments(false)
        // do not generate an empty enum for EVMC_ABI_VERSION
        .constified_enum("")
        // generate Rust enums for each evmc enum
        .rustified_enum("*")
        // force deriving the Hash trait on basic types (address, bytes32)
        .derive_hash(true)
        // force deriving the PratialEq trait on basic types (address, bytes32)
        .derive_partialeq(true)
        .allowlist_type("evmc_.*")
        .allowlist_function("evmc_.*")
        // TODO: consider removing this
        .size_t_is_usize(true)
        .generate()
        .expect("Unable to generate bindings");

    let out_path = PathBuf::from(env::var("OUT_DIR").unwrap());
    bindings
        .write_to_file(out_path.join("bindings.rs"))
        .expect("Couldn't write bindings!");
}

fn gen_loader() {
    let build_dir = Config::new("../../../").build();
    let loader_path = Path::new(&build_dir).join("build/lib/loader");
    println!("cargo:rustc-link-search=native={}", loader_path.display());
    println!("cargo:rustc-link-lib=static=evmc-loader");
}

fn main() {
    gen_loader();
    gen_bindings();
}
