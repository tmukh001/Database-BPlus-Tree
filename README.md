# Database System Principles B+ Tree Indexing for NBA Stats  
**(Block-Based Storage + Range Search)**

A C-based implementation of disk-optimized storage and indexing for NBA match statistics, using **fixed-size data blocks** and a **custom B+ Tree index**. Enables fast **range search** on field goal percentage via structured binary files.

This project was developed as part of the *Database System Principles* course at NTU Singapore.

---

## 📦 Project Overview

This project consists of three core modules:

1. **Storage Engine**  
   - Reads `games.txt` containing NBA match data  
   - Segments records into fixed-size binary blocks (`data.db`)  
   - Tracks metadata for efficient retrieval

2. **Indexing Component**  
   - Implements a B+ Tree stored in `index.db`  
   - Keys = `FG_PCT_home` values  
   - Each node written and read directly from disk  
   - Supports dynamic insertion and node splitting

3. **Search Component**  
   - Brute-force scan for baseline comparison  
   - Indexed range search using the B+ Tree  
   - Reports retrieval stats (I/O cost, timing, average values)

---

## ⚙️ Functionality Summary

| Feature                     | Status     |
|----------------------------|------------|
| Block-based file storage   | ✅ Complete |
| Brute-force range search   | ✅ Complete |
| B+ Tree index on disk      | ✅ Complete |
| Leaf/internal splitting    | ✅ Complete |
| Recursive parent updates   | ✅ Complete |
| Indexed range query        | ✅ Complete |
| Statistics & I/O metrics   | ✅ Complete |

---

## 💡 What This Project Demonstrates

- Proficiency in **C systems programming** with low-level file I/O  
- Hands-on understanding of **index structures** in database systems  
- Use of **disk-resident data structures** and record packing  
- Measurement of algorithmic performance trade-offs

---

## 📊 Sample Output

```
Storage Statistics:
-------------------
Size of a record: 64 bytes
Number of records: 27000
Records per block: 8
Number of blocks: 3375

B+ Tree Statistics:
-------------------
Max keys per node: 4
Number of nodes: 271
Levels in tree: 3
Root keys: [0.48, 0.53]
...
```

---

## 🙋 Contributors

- **Tathagato Mukherjee**  
- **[Group 04]** — Full contributor list in "Group 04 - Project 1.pdf"

