# Assignment Run Log

## Environment Details
* **Implementation Language:** C++
* **Compiler:** g++ 17


---

## Test Run: Profile A (Mild Network)
* **Command:** `python3 run.py --profile profiles/A.json --delay_ms 100`
* **Result:**
relay done: {'up_bytes': 270108, 'down_bytes': 588, 'up_pkts': 1647, 'down_pkts': 143, 'dropped': 41, 'duplicated': 13}
================ SCORE ================
  frames               : 1500
  deadline misses      : 9  (0.60%)   [cap 1.00%]
  playout delay        : 100 ms   <-- your score if valid; lower wins
  bandwidth overhead   : 1.13x   [cap 2.00x]   (up 270108B, feedback 588B)
  RESULT               : VALID


## Test Run: Profile B (Moderate Network)
* **Command:** `python3 run.py --profile profiles/B.json --delay_ms 200`
* **Result:**
relay done: {'up_bytes': 343908, 'down_bytes': 2512, 'up_pkts': 2097, 'down_pkts': 546, 'dropped': 147, 'duplicated': 28}
================ SCORE ================
  frames               : 1500
  deadline misses      : 12  (0.80%)   [cap 1.00%]
  playout delay        : 200 ms   <-- your score if valid; lower wins
  bandwidth overhead   : 1.44x   [cap 2.00x]   (up 343908B, feedback 2512B)
  RESULT               : VALID