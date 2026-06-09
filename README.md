# IoT-Utilization-cpp
Minimal C++ library that computes a machine's utilization rate (稼働率) from a
history of (timestamp, value) IoT readings — base for a SWE-bench task.
The interval primitives in `iot::merge_intervals` / `total_seconds` /
`clamp_interval` are implemented; the analyzer `iot::UtilizationAnalyzer` is
left to implement in src/utilization.cpp.
