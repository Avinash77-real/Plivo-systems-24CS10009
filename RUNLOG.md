# Assignment Run Log

## Environment Details
* **Implementation Language:** C++
* **Compiler:** g++ 17


---

## Test Run: Profile A (Mild Network)
* **Command:** `python3 run.py --profile profiles/A.json --delay_ms 100`
* **Result:**
  <img width="1113" height="199" alt="{40ACB1D1-F3E8-4051-A1B1-D03674B3E126}" src="https://github.com/user-attachments/assets/28b56ca0-513e-4e46-bc75-f4268322f2a7" />




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
