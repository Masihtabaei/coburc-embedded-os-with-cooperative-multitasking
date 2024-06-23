# cobucm - A Minimal Embedded OS With Cooperative Multitasking

## Technologies Used

![C](https://img.shields.io/badge/c-%2300599C.svg?style=for-the-badge&logo=c&logoColor=white)
![Static Badge](https://img.shields.io/badge/armasm_(arm%20Assembly)-red?style=for-the-badge)
![Static Badge](https://img.shields.io/badge/STM32G474_Processor-blue?style=for-the-badge&logo=stmicroelectronics)
![Static Badge](https://img.shields.io/badge/arm_Keil_%C2%B5Vision_IDE-green?style=for-the-badge)
![Static Badge](https://img.shields.io/badge/Doxygen-yellow?style=for-the-badge)
![Git](https://img.shields.io/badge/git-%23F05033.svg?style=for-the-badge&logo=git&logoColor=white)
![GitHub](https://img.shields.io/badge/github-%23121011.svg?style=for-the-badge&logo=github&logoColor=white)


## General Information

Here you will find a co-operative operating system (more precisely: co-operative scheduler) that I implemented as part of the course "Embedded Operating Systems" at the university.
I called it **cobuc**. **cobuc** is a partial blend (**Coburg** \[name of a small city located in Bavaria in Southern Germany\] + **Cooperative Multitasking** )
Feel free to extend/adjust this project so that it meets your goals.

A simple API was developed for process management, which offers the following methods:
1. create (to create new processes)
2. destroy (for deleting processes)
3. yield (to switch processes and contexts)

Process and context switch can be carried out using pure assembler code as well as a combination of C and assembler (controllable by a macro). Project is heavily commented
and documented. You will find the documentation under `documentation/html` (this directory contains an automatically documentation generated by Doxygen).
The operating system was implemented and tested for an Arm-based processor (Arm Cortex M4). A chaser light application (with 8 LEDs) was used for illustration and demonstration.
Two processes each control 4 LEDs (one for the right and one for the left strip). There is also an initial process that takes care of all the configurations and initialisations.
In addition, a fourth process takes care of the delay between the individual steps. By defining new processes, the two strips can be operated at different frequencies.

## Videos

https://github.com/Masihtabaei/embedded-os-with-cooperative-multitasking/assets/40685026/2361a3b6-c0e1-4fb2-92de-5ddf7a549d20

https://github.com/Masihtabaei/embedded-os-with-cooperative-multitasking/assets/40685026/be7caaec-ee7d-4de1-bd18-4a8fcd31f679

https://github.com/Masihtabaei/embedded-os-with-cooperative-multitasking/assets/40685026/c31ec3cf-6f13-47d3-b616-a32fcd1ecf0d

