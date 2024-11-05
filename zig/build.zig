const Builder = @import("std").build;

pub fn build(b: *Builder) void {
    const target = b.standardTargetOptions(.{});
    const mode = b.standardReleaseOptions();

    // Build Zig-based functions
    //
    //
    // Link with C musl components (assuming musl Makefile compiles these separately)
    const musl_libc = b.addStaticLibrary("muscle", "zig/combined_libc.zig");
    musl_libc.addObject(safe_strcpy);
    musl_libc.addObject(safe_malloc);
    musl_libc.setTarget(target);
    musl_libc.setBuildMode(mode);

    musl_libc.linkLibC(); // Link with musl C library functions
    musl_libc.install();
}
