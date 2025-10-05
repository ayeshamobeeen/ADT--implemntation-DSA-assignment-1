# ADT--implemntation-DSA-assignment-1
# ADT Implementations: Polynomial, UNO, and Text Editor

This repository contains three independent Abstract Data Type (ADT) implementations in C++:

* **Polynomial ADT**
* **UNO Game Simulation ADT**
* **Text Editor ADT**

Each ADT demonstrates the design of encapsulated data structures with defined operations while hiding internal implementation details.

---

## Compilation Instructions

To compile and run each program individually:

```bash
g++ Polynomial.cpp -o Polynomial
./Polynomial

g++ UNO.cpp -o UNO
./UNO

g++ TextEditor.cpp -o TextEditor
./TextEditor
```

---

## Approach

The development approach for these ADTs was centered on object-oriented design principles and abstraction:

1. **Defining Interfaces First**

   * Each ADT was defined through its class interface, specifying only the necessary operations (e.g., insertion, display, arithmetic operations, undo/redo).
   * Implementation details such as storage structures were hidden from the user.

2. **Use of Abstraction and Virtual Functions**

   * Abstract methods (`virtual = 0`) were introduced where necessary to enforce polymorphic behavior.
   * This allowed extending the ADTs without rewriting client code.

3. **Incremental Implementation**

   * Initial focus was on creating minimal working versions of each ADT.
   * Features such as term simplification in polynomials, special cards in UNO, and undo/redo in the text editor were then added iteratively.

4. **Testing and Debugging**

   * Each ADT was tested individually in `main` functions before being finalized.
   * Frequent compilation and small test cases were used to isolate errors quickly.

---

## Project Descriptions

### Polynomial ADT

* Supports insertion of terms, string representation, addition, multiplication, and differentiation.
* Polynomial terms are stored in a sorted order based on exponents.

### UNO ADT

* Simulates the basic structure of the UNO card game.
* Provides functionality for drawing and playing cards, including handling of special cards.

### Text Editor ADT

* Implements a simplified text editor.
* Supports insertion, deletion, and undo/redo operations using internal data structures.

---

## Issues Faced and Resolutions

The development process involved several significant challenges:

* **Abstract Classes**: Initially, it was unclear that `virtual = 0` could be used to declare pure virtual functions. This prevented early progress until the concept of abstract base classes was properly understood.

* **Header and Implementation Separation**: A recurring issue was attempting to define functions in `.cpp` files without first declaring them in the header. This highlighted the necessity of strictly separating the class interface (header) from the implementation.

* **Header File Modifications**: The initial intent was to complete the task without modifying header files. However, practical constraints required adjustments to add constructors, destructors, and virtual functions, without which the implementations could not compile.

* **UNO Implementation Complexity**: The UNO ADT was initially over-complicated by attempting patterns such as PIMPL. This approach was abandoned in favor of a more straightforward design using inheritance and overriding.

* **Undo/Redo in Text Editor**: Implementing history management for the text editor required careful consideration of stack-based approaches. This was one of the more difficult aspects, as it required both structural planning and debugging.

---

## Reflections

Through these implementations, the following key lessons were learned:

* Abstract Data Types must emphasize clear interfaces and hidden implementations.
* Virtual functions and inheritance are critical for creating extensible designs.
* Simplifying the design often leads to more effective solutions than over-engineering.
* Debugging and iterative testing are essential when developing data structures from scratch.

---

## Future Work

Potential areas for improvement include:

* Replacing static/global storage mechanisms with private class-level data members.
* Expanding unit testing to cover edge cases and larger inputs.
* Enhancing efficiency in polynomial operations and text editor history management.
* Improving modularity to allow for easier extension of UNO game rules.

---

## Author

Developed by **Ayesha**
Second Semester, Data Structures and Algorithms Practice
