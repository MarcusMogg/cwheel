add_rules("mode.debug", "mode.release")
add_rules("mode.coverage")

set_toolchains("clang")

option("test")
    set_default(false)
    set_showmenu(true)
    add_cxxflags("-fprofile-arcs -ftest-coverage")
    add_defines("DEBUG")
    set_toolchains("gcc") -- for coverage
option_end()    

includes("src")
includes("test")

target("cwheel")
    set_kind("binary")
    set_languages("cxx20")
    add_deps("cwheel_lib")
    add_files("*.cpp","*.cc")
    add_options("test")

