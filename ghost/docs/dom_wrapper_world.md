

# Consistency in DOM Wrappers 
The same DOM wrapper is returned to JavaScript as long as the underlying C++ DOM object is alive. We should not return different DOM wrappers for the same C++ DOM object.


As a result, we have multiple DOM wrapper storages in one isolate. The mapping of the main world is written in ScriptWrappable. If ScriptWrappable::main_world_wrapper_ has a non-empty value, it is a DOM wrapper of the C++ DOM object of the main world. The mapping of other worlds are written in DOMDataStore.



```
Here is an example of an underlying C++ DOM object being referenced by different worlds in Chromium:

// In blink/renderer/core/dom/element.h

class Element : public ContainerNode {
  // ...

  // Raw pointer to the corresponding V8 wrapper object.
  WrapperTypeInfo* wrapper_type_info_;

}

// In blink/renderer/platform/bindings/dom_wrapper_world.cc

void DOMWrapperWorld::SetWrapperTypeInfo(ScriptWrappable* script_wrappable, 
                                         WrapperTypeInfo* wrapper_type_info) {
  script_wrappable->wrapper_type_info_ = wrapper_type_info; 
}

// SetWrapperTypeInfo is called when creating the V8 wrapper for each world.
// This stores a pointer to the WrapperTypeInfo for that world on the 
// underlying C++ object.

// So for an Element:
// - main world stores main world WrapperTypeInfo
// - isolated world 1 stores isolated world 1 WrapperTypeInfo 
// - isolated world 2 stores isolated world 2 WrapperTypeInfo
// etc.

// The same underlying Element C++ object is accessible from all worlds, 
// but with separate V8 wrapper objects for each world.
```


This allows the different worlds to operate on the same underlying C++ DOM object, while keeping the JavaScript objects separate per world for security.
