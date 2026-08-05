I initially thought building an emulator of any kind would be too difficult
for me. I could not be more wrong!

A huge thanks to the following resources that have guided me throughout this
project. It would not have been possible otherwise!

1. [Austin Morlan's Guide For Building a CHIP-8 Emulator](https://austinmorlan.com/posts/chip8_emulator/)

Without this guide, the project would not have started at all. Thank you for
this amazing guide. It explains a lot of the concepts in simple understable
terms and provides code for reference which has been a huge help.

2. [Cowgod's CHIP-8 Technical Reference](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM)

After getting the hang of implementing a few instruction from Austin's guide,
I had this reference open for most of the others. It has simple and clear
text that lays down exactly what is needed to be done.

3. Mattmikolay's [Mastering CHIP-8](https://github.com/mattmikolay/chip-8/wiki/Mastering-CHIP%E2%80%908)
and [CHIP-8 instruction set](https://github.com/mattmikolay/chip-8/wiki/CHIP%E2%80%908-Instruction-Set)

There are some instructions that people don't agree upon. Austin Morlan's guide
mentions them under the section named `The Instructions`. As such, this guide
served as a second technical reference for those instructions.

4. [Emualation Online's Guide on Control Flow and Graphics](https://www.emulationonline.com/systems/chip8/control-flow-and-graphics/)

This helped me clear a few misunderstandings and confusions regarding the way
graphics are stored in CHIP-8.

5. [learncpp tutorials for pointers](https://www.learncpp.com/cpp-tutorial/introduction-to-pointers/)

This site has helped me understand `function pointers` and the background knowledge
going all the way back to `lvalue references`.

I suggest following the tutorial and going back to read upon the lessons it tells
you to read about at the start. `Function pointers` required knowledge of lesson
`12.7 Introduction to pointers` which required knowledge of lesson `lvalue
references` which required knowledge about `value categories in c++`.

I suggest going back even further to any missing information cleared up before
reading the subsequent tutorials. The tutorials themselves are simple to follow
and clear in its explanation.

6. [Alice Goldfuss's Guide for Function Dispatch Tables](https://blog.alicegoldfuss.com/function-dispatch-tables/)

This guide helped me understand the basic idea of function dispatch tables and
sparked my curiosity to understand function pointers which led me down the rabbit
hole of `learncpp`. It is a short but good read.

7. [Lazy Foo's SDL Tutorials](https://lazyfoo.net/tutorials/SDL/)

These tutorials have been a huge help in understanding SDL mechanics. Here, I
understood how Makefiles work in CMake for the first time.

Its explanations are beginner friendly.

---

Those are all the specific resources used that helped me make this project.

