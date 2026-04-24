*This project has been created as part of the 42 curriculum by joflorid.*

## Description

Codexion is a multithreading project that simulates a variation of the classic Dining Philosophers problem. In this simulation, a number of "coders" (threads) need to acquire two "USB dongles" (shared resources) to perform a sequence of tasks: compiling, debugging, and refactoring. The project explores concepts of concurrency, resource management, and scheduling.

The main goal is to implement a simulation that correctly handles thread synchronization to prevent common concurrency issues like deadlocks and race conditions, while also implementing specific scheduling algorithms (FIFO and EDF) to manage resource allocation and prevent starvation.

## Instructions

### Compilation

1.  Navigate to the project's root directory.
2.  Run `make` to compile the source code.

```bash
make
```

This will create an executable named `codexion`.

### Execution

Run the program from the command line with eight arguments:

```bash
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug \
           time_to_refactor number_of_compiles_required dongle_cooldown scheduler
```

*   `number_of_coders`: The number of coders (and dongles) in the simulation.
*   `time_to_burnout`: Time in milliseconds. If a coder doesn't start compiling within this time since their last compilation, they "burn out" and the simulation ends.
*   `time_to_compile`: Time in milliseconds a coder spends compiling.
*   `time_to_debug`: Time in milliseconds a coder spends debugging.
*   `time_to_refactor`: Time in milliseconds a coder spends refactoring.
*   `number_of_compiles_required`: The number of times each coder must compile before the simulation ends successfully.
*   `dongle_cooldown`: Time in milliseconds a dongle is unavailable after being used.
*   `scheduler`: The scheduling policy to use. Must be either `fifo` or `edf`.

## Resources

*   **[Implementing a queue in C - Youtube](https://www.youtube.com/watch?v=Ra6p-Bmajlw)**:
*   **[Dining Philosophers Problem - Wikipedia](https://en.wikipedia.org/wiki/Dining_philosophers_problem)**
*   **[Short introduction to threads (pthreads) - Youtube](https://youtu.be/d9s_d28yJq0?si=kMJe4xZ9sSzgRLvp)**
*	**Some colleagues in 42 Urduliz Campus (juetxeba, ingonzal, gcassi-d, eandres)**

### AI Usage

AI was used to understand certain concepts related to the project and to solve specific problems that were posing obstacles. AI helped define the project’s core architecture.

## Blocking cases handled

*   **Deadlock Prevention**: Deadlocks are prevented by breaking the "circular wait" condition. When a coder needs to acquire two dongles (resources), the mutexes for these dongles are always locked in a globally consistent order. This is achieved by comparing the memory addresses of the dongle structures (`ft_sorted_mutex_lock` in `take_dongles.c`), ensuring that no two threads can be waiting for each other's resources in a circular chain.

*   **Starvation Prevention**: The project implements two scheduling policies to manage access to dongles and prevent starvation:
    *   **FIFO (First-In, First-Out)**: Coders are granted access to dongles in the order they requested them. This is managed by a simple queue for each dongle, ensuring fairness based on arrival time.
    *   **EDF (Earliest Deadline First)**: This scheduler prioritizes coders who are closest to their "burnout" deadline. A priority queue is used for each dongle, where priority is determined by the coder's `prior` value (the timestamp of their potential burnout). This ensures that coders at risk of failing the simulation are served first.

*   **Cooldown Handling**: After a coder finishes compiling and releases their dongles, the dongles enter a cooldown period (`tt_cooldown`). The `ft_release_dongles` function sets an `end_cool` timestamp on each dongle. The `ft_can_take_both` function checks this timestamp, preventing any coder from acquiring the dongle until the cooldown period has elapsed. This check is performed atomically while holding the dongle's mutex.

*   **Precise Burnout Detection**: A dedicated monitor thread (`watcher`) is created to continuously check the status of all coders. In `ft_check_burnout`, this thread iterates through each coder, locks their individual mutex (`m_coder`) to safely read their state, and compares the current time with the time of their last compilation (`st_comp`). If the difference exceeds `tt_burn`, the monitor sets a global `stop_sim` flag and prints a "burned out" message. This check happens frequently (`usleep(100)`) to ensure detection is timely.

*   **Log Serialization**: All output to the console (`printf`) is protected by a single, global print mutex (`m_print`). Before any thread can print a status message, it must lock this mutex. It is unlocked immediately after the print statement. This mechanism guarantees that log lines from different threads are never interleaved, ensuring a clean and readable output.

## Thread synchronization mechanisms

This project uses several `pthreads` primitives to manage concurrency safely.

*   `pthread_mutex_t`: Mutexes are the primary tool for ensuring exclusive access to shared data.
    *   `m_dongle`: Each dongle has its own mutex. A coder must lock the mutexes of both its left and right dongles before it can use them. This prevents two coders from using the same dongle simultaneously.
    *   `m_coder`: Each coder has a mutex to protect its own state variables, such as `st_comp` (last compile time) and `num_comp` (compiles remaining). This is crucial for safe communication between the coder thread and the monitor thread. For example, the monitor locks `m_coder` to read `st_comp` for the burnout check, while the coder thread locks it to update `st_comp` after compiling.
    *   `m_print`: A single mutex used to serialize all calls to `printf`, preventing interleaved output messages.
    *   `m_gen`: A general-purpose mutex protecting the global simulation state in the `t_gen` struct, including the `stop_sim` flag and the `pending_comp` counter.

*   `pthread_cond_t`: A condition variable is used to synchronize the start of the simulation.
    *   `w_align`: All coder threads are created and immediately made to wait on this condition variable (`pthread_cond_wait`). Once all threads are ready, the main thread broadcasts a signal (`pthread_cond_broadcast`) to wake them all up at the same time. This ensures that the simulation starts fairly for all coders.

### Race Condition Prevention Example

A potential race condition exists between the monitor thread checking for burnout and a coder thread updating its status after a compilation.

1.  The monitor thread (`ft_check_burnout`) reads `coder->st_comp`.
2.  The coder thread (`ft_start_compile`) writes to `coder->st_comp`.

This is prevented by using the `coder->m_coder` mutex. The monitor must lock it before reading, and the coder thread must lock it before writing, ensuring the read and write operations are mutually exclusive.

### Thread-Safe Communication

*   **Monitor to Coders**: The monitor communicates a simulation-ending event (like a burnout) by setting the `stop_sim` flag. This flag is protected by the `m_gen` mutex. Each coder thread checks this flag at the beginning of its main loop (`ft_exit_routine`), also while holding the `m_gen` mutex, and exits if it is set.

*   **Coders to Monitor**: Coders signal that they have completed a compilation cycle by decrementing the `pending_comp` counter. This variable is also protected by the `m_gen` mutex to prevent race conditions from multiple coders updating it at once. The monitor thread checks this counter to determine when the simulation has completed successfully.
