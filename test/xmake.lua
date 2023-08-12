add_rules("mode.debug", "mode.release")

add_requires("gtest")

target("cwheel_test")
    set_kind("binary")
    add_files("*.cc")
    add_packages("gtest")
    add_deps("cwheel_lib")
    add_options("test")