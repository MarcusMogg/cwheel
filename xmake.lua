add_rules("mode.debug", "mode.release")

includes("src")
includes("test")

target("cwheel")
    set_kind("binary")
    set_languages("cxx20")
    set_toolchains("clang")
    add_deps("cwheel_lib")
    add_files("*.cpp","*.cc")

