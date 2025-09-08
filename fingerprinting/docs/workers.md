
# Worker / Worklet types

- Workers are divided into 2 types:
  - **In-process workers (Dedicated Workers)**: A worker of this type always runs in the same renderer process with a parent document that starts the worker.
  - **Out-of-process workers (Shared Workers and Service Workers)**: A worker of this type may run in a different renderer process with a parent document that starts the worker.
- Worklets are divided into 2 types:
  - **Main thread worklets (Paint Worklets and Layout Worklets)**: A worklet of this type runs on the main thread.
  - **Threaded worklets (Audio Worklets and Animation Worklets)**: A worklet of this type runs on a worker thread.

