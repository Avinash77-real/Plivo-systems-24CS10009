# Implementation Notes

**Design Summary:**

Our system uses a circular ring buffer for O(1) packet reordering and batched NACKs to minimize bandwidth overhead. The receiver is time-aware, calculating absolute playout deadlines based on T0 and DELAY_MS to drop dead packets, effectively preventing head-of-line blocking. 

**Recommended Grading Parameters:**

* **Profile A:** Grade at `--delay_ms 120`. This provides the optimal balance between packet recovery time and minimal playback latency.

* **Profile B:** Grade at `--delay_ms 250`. This accounts for the higher round-trip time and increased packet loss inherent in the moderate network profile.

**System Limitations:**

* The system is bounded by the network Round Trip Time (RTT). If the network latency exceeds the jitter buffer depth (8192 packets), retransmissions will arrive after the playout deadline, leading to dropped frames. 

* Extreme jitter spikes beyond our 30ms NACK cooldown threshold may briefly increase bandwidth overhead as the system aggressively re-requests missing segments.