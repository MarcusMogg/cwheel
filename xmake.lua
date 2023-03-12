add_rules("mode.debug", "mode.release")
add_rules("mode.coverage")

option("test")
    set_default(false)
    set_showmenu(true)
    add_cxxflags("-fprofile-arcs -ftest-coverage")
    add_defines("DEBUG")
option_end()    

if is_config("test", true ) then
    set_toolchains("gcc")
else
    set_toolchains("clang")
end

includes("src")
includes("test")

target("cwheel")
    set_kind("binary")
    set_languages("cxx20")
    add_deps("cwheel_lib")
    add_files("*.cpp","*.cc")
    add_options("test")

