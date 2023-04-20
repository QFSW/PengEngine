# Build Configurations

Peng Engine comes with several different build configurations for different purposes.

## Debug

Debug builds should be used when developing new rendering features or when debugging difficult issues.

Optimizations are disabled and use debug versions of the standard library. Additionally, debug builds will cause OpenGL to run under a debug context which provides much more internal logging and validation. Because of this the performance is quite poor.

## Development

Development builds generally be used in most cases as your default configuration when developing your project.

The code generation uses optimizations and the release version of the standard library meaning the generated code will be much closer in performance functionality to a final build that you would distribute. Logs, assertions and profiling support is still included in development builds.

## Release

Release builds should be used for all final testing and performance profiling.

Assertions are removed, and generally Release builds are as close as possible to Master builds while still being usable for validation and profiling. Unlike Master builds, they still emit logs and have profiler support.

## Master

Master builds should only be used for final distribution of a stable build. Logs, assertions, profiling, the console and debug contexts are disabled.