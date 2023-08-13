add_rules("mode.debug", "mode.release")
add_rules("mode.coverage")

set_languages("cxx23")

option("test")
    set_default(false)
    set_showmenu(true)
    add_cxxflags("-fprofile-arcs -ftest-coverage")
option_end()    

add_cxxflags("-stdlib=libc++", {tools = "clang"})

includes("src")
includes("test")

target("cwheel")
    set_kind("binary")
    add_deps("cwheel_lib")
    add_files("*.cc")
    add_options("test")
