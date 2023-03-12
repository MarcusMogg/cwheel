add_rules("mode.debug", "mode.release")
add_rules("mode.coverage")

option("test")
    set_default(false)
    set_showmenu(true)
    add_cxxflags("-fprofile-arcs -ftest-coverage")
    add_defines("DEBUG")
option_end()    

set_toolchains("clang")

includes("src")
includes("test")

target("cwheel")
    set_kind("binary")
    set_languages("cxx20")
    set_toolchains("clang")
    add_deps("cwheel_lib")
    add_files("*.cpp","*.cc")

