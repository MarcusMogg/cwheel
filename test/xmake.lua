add_rules("mode.debug", "mode.release")

add_requires("gtest")

target("cwheel_test")
    set_kind("binary")
    set_languages("cxx20")
    set_toolchains("clang")
    add_files("*.cc")
    add_packages("gtest")
    add_deps("cwheel_lib")
    add_cxxflags("-fprofile-instr-generate", "-fcoverage-mapping") -- https://clang.llvm.org/docs/SourceBasedCodeCoverage.html
    add_ldflags("-fprofile-instr-generate")

