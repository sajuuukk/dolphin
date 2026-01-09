## 2024-05-23 - Avoid redundant list construction in hot paths
**Learning:** In Qt/C++, even implicitly shared classes like `QList` (via `KFileItemList`) can have overhead when constructed and populated repeatedly in hot paths like selection changes. `DolphinView::selectedItems()` constructs a new list by iterating over indices.
**Action:** Always check if the data (like selection list) is already available in the context (e.g. signal arguments) before calling a getter that reconstructs it. Pass it down to update functions.
