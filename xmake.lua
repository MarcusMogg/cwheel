add_rules("mode.debug", "mode.release")
add_rules("mode.coverage")

option("test")
    set_default(false)
    set_showmenu(true)
    add_cxxflags("-fprofile-arcs -ftest-coverage")
option_end()    

if has_config("test") then
    set_toolchains("gcc")
end

includes("src")
includes("test")

target("cwheel")
    set_kind("binary")
    set_languages("cxx20")
    add_deps("cwheel_lib")
    add_files("*.cpp","*.cc")
    add_options("test")

