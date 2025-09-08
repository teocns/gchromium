To allow a type to be *passed across threads* using its copy constructor,
provide a specialization of `CrossThreadCopier<T>`.

* If the type is not defined in `third_party/blink/`
  ==> Choose one of the existing cross_thread_copier_*.h or add new one,
      and add a forward declaration and a `CrossThreadCopier` specialization
      for the type to the file.
* If the type is defined in `third_party/blink/public/`
  ==> Add a forward declaration and a `CrossThreadCopier` specialization for
      the type to `cross_thread_copier_public.h`.
* If the type is defined in `third_party/blink/`
  ==> Include `cross_thread_copier.h` from the header defining the type, and
      add a `CrossThreadCopier` specialization for the type to the header.


