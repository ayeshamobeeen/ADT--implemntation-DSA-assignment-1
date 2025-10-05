Perfect! Here’s a **final polished README** ready to copy, now including your GitHub link at the top for reference. I also formatted it cleanly for submission.

---

# README

## Repository Link

You can find the complete code for all projects here:
[GitHub Repository](https://github.com/ayeshamobeeen/ADT--implemntation-DSA-assignment-1)

---

## Overview

This repository contains three major projects implemented in C++:

1. **UNO.cpp** – A simulation of the UNO card game.
2. **Polynomial.cpp** – An abstract data type (ADT) implementation for polynomials, supporting insertion, addition, multiplication, and differentiation.
3. **TextEditor.cpp** – A simulation of a basic text editor with features like inserting, deleting, copying, pasting, and undo-redo operations.

These projects serve as both exercises in **data structures and object-oriented programming** and as demonstrations of handling **complex ADTs** and **dynamic data management** in C++.

Throughout the implementation of these projects, I faced numerous challenges that required in-depth study of advanced concepts such as **maps, stacks, linked lists, pointers, and object storage strategies**. Below, I outline the problems I faced, the approaches I used, and the concepts employed in detail.

---

## 1. Polynomial.cpp

### Problems Faced

* **Header Restrictions:**
  The provided `Polynomial.h` header could not be changed. All implementations had to respect the existing virtual methods. Initially, I struggled because I couldn’t add protected members or extra functions to simplify storage.

* **Data Storage Challenges:**
  Initially, I tried to use **lists of terms with stacks** to manage term ordering, but the output kept coming in the wrong order. Handling **like terms, zero coefficients, and descending exponent order** proved extremely difficult.

* **Combining Advanced Concepts:**
  To fix ordering issues, I had to learn how to use **`std::map` with custom comparators (`std::greater<int>`)**, which automatically keeps polynomial terms sorted by exponent in descending order. Understanding the difference between **`map` and `list`**, and how `map` handles insertion and automatic sorting, was crucial.

* **Implementation Errors:**

  * Multiple “iterator invalidation” errors when erasing while iterating.
  * Trouble ensuring that addition, multiplication, and derivative functions did not modify original polynomials.
  * Debugging the storage map (`map<const Polynomial*, map<int,int,greater<int>>>`) so that each polynomial instance maintained its own term list without interference.

### Approach

1. **Storage:**
   I used a **map-of-maps**:

   ```cpp
   static map<const Polynomial*, map<int,int,std::greater<int>>> storage;
   ```

   This allowed each `Polynomial` object to maintain its own sorted list of terms, while keeping the header untouched.

2. **Insert Term:**

   * Terms are added using `map[exponent] += coefficient`.
   * Zero terms are removed immediately to avoid clutter.

3. **Addition & Multiplication:**

   * Both operations create a new `Polynomial` object.
   * Terms from each polynomial are combined using `insertTerm` logic to handle like terms.

4. **Derivative:**

   * Simply iterates over each term, multiplies coefficient by exponent, and reduces the exponent by 1.

5. **Output (`toString`):**

   * Iterates over the sorted map.
   * Handles formatting, including signs, omitting coefficient 1, and exponent 0 or 1 cases.

**Concepts Learned:**

* Maps with custom sorting, iterator management, object storage, and careful handling of const-correctness.

---

## 2. UNO.cpp

### Problems Faced

* **Abstract Header Usage:**
  The provided header required abstract class design. Initially, I tried to implement everything in one class, which led to errors like “incomplete type” because of circular dependencies.

* **Card Representation:**
  Deciding how to represent cards efficiently (struct vs class, storing color and number/action) took multiple iterations.

* **Game Logic Complexity:**
  Handling turns, skips, reverses, draw twos, and wild cards involved nested loops and careful state management. Errors often came from **pointer misuse or incorrect loop control**.

* **Inheritance & Virtual Functions:**
  I initially didn’t fully understand abstract classes, so I had to study **virtual functions, overriding, and polymorphism** to implement the game correctly.

### Approach

1. **Card Struct:**
   Used a simple `struct Card` with members for `color` and `value`.

2. **UNOGame Class:**

   * Managed a vector of players, current turn, and discard pile.
   * Methods for drawing cards, playing a card, applying effects, and checking for win conditions.

3. **Polymorphism:**

   * Abstract methods in the header were implemented carefully in `UNOGame`.

**Concepts Learned:**

* Inheritance, virtual functions, polymorphism, managing complex game state, vector usage, and pointer references.

---

## 3. TextEditor.cpp

### Problems Faced

* **Undo-Redo Implementation:**
  Implementing undo and redo stacks required careful **stack management and deep copying**. Initially, I tried arrays and linked lists but encountered off-by-one and memory issues.

* **Cursor and Position Management:**
  Keeping track of the cursor while inserting, deleting, or pasting text caused many boundary errors.

* **Dynamic Memory Issues:**
  I had to learn **dynamic allocation, references, and careful copying of text buffers** to avoid crashes.

### Approach

1. **Data Structures:**

   * Used **stack** for undo and redo operations.
   * Used **linked list or string** to store text efficiently.

2. **Operations:**

   * Insert, delete, copy, paste operations update both text storage and undo stack.
   * Undo pops from undo stack and pushes to redo stack; redo reverses this.

3. **Edge Cases:**

   * Checked for invalid positions, empty clipboard, and empty stacks before operations.

**Concepts Learned:**

* Stacks, linked lists, deep copies, memory safety, and careful logical flow for text operations.

---

## Challenges Across All Projects

* I repeatedly faced **liner errors, pointer issues, and type mismatches**.
* Implementing logic was straightforward, but **correctly mapping it to C++ data structures** was extremely hard.
* I had to **study maps, stacks, linked lists, and abstract classes** in detail to implement these projects successfully.
* Changing headers was **not allowed**, so I had to think carefully about **storage mechanisms that fit the ADT design**.
* Debugging took multiple iterations, especially for **Polynomial.cpp**, where ordering, combining terms, and formatting output required multiple fixes.

---

## General Approach

1. **Understand Requirements:**

   * First, understood what each function in the header was supposed to do.

2. **Design Storage Carefully:**

   * Chose appropriate data structures: map for polynomial, vector/stack for UNO, stack/linked list for text editor.

3. **Iterative Implementation:**

   * Implemented one function at a time.
   * Tested frequently to catch formatting, memory, or logic issues early.

4. **Debug & Study Concepts:**

   * Whenever an error occurred (iterator invalidation, segmentation fault, wrong order), studied the concept in depth and applied corrections.

5. **Final Testing:**

   * Verified outputs matched expectations exactly, especially for polynomials.

---

## Conclusion

These projects were **conceptually straightforward but extremely challenging in implementation**. The main lessons learned include:

* The importance of **choosing the right data structures** for the task.
* Careful handling of **pointers, maps, and iterators**.
* Understanding **abstract classes, inheritance, and virtual functions**.
* The need for **incremental testing** and debugging complex ADTs.

Overall, these projects strengthened my **C++ skills, problem-solving abilities, and understanding of advanced data structures**, and gave me a solid foundation for more complex applications in the future.


