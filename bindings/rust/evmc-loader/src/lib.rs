// EVMC: Ethereum Client-VM Connector API.
// Copyright 2019 The EVMC Authors.
// Licensed under the Apache License, Version 2.0.

mod sys;

#[cfg(test)]
mod tests {
use std::ffi::CString;
use std::os::raw::c_char;

    use super::*;

    #[test]
    fn load_fail() {
        let c_str = CString::new("test.so").unwrap();
        unsafe {
            let mut error_code = sys::evmc_loader_error_code::EVMC_LOADER_UNSPECIFIED_ERROR;
            let instance = sys::evmc_load_and_create(c_str.as_ptr() as *const c_char, &mut error_code);
        println!("{:?}", error_code);
        }
    }
}
