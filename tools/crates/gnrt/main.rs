// Copyright 2022 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

use gnrt_lib::*;

mod download;
mod gen;
mod util;

use anyhow::{Context, Result};
use clap::arg;

fn main() -> Result<()> {
    let args = clap::Command::new("gnrt")
        .subcommand(
            clap::Command::new("gen")
                .about("Generate GN build rules from third_party/rust crates")
                .arg(arg!(--"output-cargo-toml" "Output third_party/rust/Cargo.toml then exit \
                immediately"))
                .arg(
                    arg!(--"for-std" "(WIP) Generate build files for Rust std library instead of \
                third_party/rust"),
                ),
        )
        .subcommand(
            clap::Command::new("download")
                .about("Download the crate with the given name and version to third_party/rust.")
                .arg(arg!(<name> "Name of the crate to download"))
                .arg(
                    arg!(<version> "Version of the crate to download")
                        .value_parser(clap::value_parser!(semver::Version)),
                )
                .arg(
                    arg!(--"security-critical" <YESNO> "Whether the crate is considered to be \
                        security critical."
                    )
                    .value_parser(["yes", "no"])
                    .required(true),
                ),
        )
        .get_matches();

    let paths = paths::ChromiumPaths::new().context("Could not find chromium checkout paths")?;

    match args.subcommand() {
        Some(("gen", args)) => gen::generate(&args, &paths),
        Some(("download", args)) => {
            let security = args.get_one::<String>("security-critical").unwrap() == "yes";
            let name = args.get_one::<String>("name").unwrap();
            let version = args.get_one::<semver::Version>("version").unwrap().clone();
            download::download(name, version, security, &paths)
        }
        _ => unreachable!("Invalid subcommand"),
    }
}
