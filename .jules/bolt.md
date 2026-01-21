## 2024-05-23 - Avoid redundant list construction in hot paths
**Learning:** In Qt/C++, even implicitly shared classes like `QList` (via `KFileItemList`) can have overhead when constructed and populated repeatedly in hot paths like selection changes. `DolphinView::selectedItems()` constructs a new list by iterating over indices.
**Action:** Always check if the data (like selection list) is already available in the context (e.g. signal arguments) before calling a getter that reconstructs it. Pass it down to update functions.

## 2024-05-23 - QCache Key Optimization
**Learning:** Using `QString` as a key for `QCache` in hot paths forces repeated string allocations and formatting. `QCache` supports custom structs as keys if they implement `operator==` and `qHash`.
**Action:** Replace composite string keys with simple structs implementing `qHash` to eliminate allocation overhead in cache lookups.
