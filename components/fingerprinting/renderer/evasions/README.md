## Definition 

Evasions are monkey-patches targeting a specific JavaScript API feature (eg WebGL, WebAudio, etc). They are inspired to puppeteer-extra-plugin-stealth/evasions*



## Implementation

Each evasion is a `Hook`. In JavaScript terms, it translates to a callable [function].

An isolated V8 script execution context is created in which the monkey-patching process executes. This ensures that our patch utilities are not exposed to the global context


## Workflow for deploying the patch


- **Establish a new v8::Context**: Create a new, isolated V8 context for your private scope where you can expose native functions.
  
- **Create an ObjectTemplate**: Define a v8::ObjectTemplate to represent the sandboxed global object for the new context.

- **Bind PatchValue to the Object**: Attach `PatchValue` to the object template so that it can be accessed within the isolated context, but no association is created with the global scope.

- **Instantiate the Context with Object**: Initialize the new v8::Context with the object template.

- **Engage Scoped Context Execution**: Use `v8::Context::Scope` to enter the local context before executing any JavaScript.

- **Introduce Native Function**:
  - Use `v8::FunctionTemplate::New(isolate, PatchValue)` to expose the native `PatchValue` function to JavaScript in this private scope.
  
- **Wrap and Inject JavaScript Code**: Compile and execute the JavaScript code within this new context ensuring it calls `PatchValue` appropriately.

- **Execute Script in Context**: Compile and run your JavaScript code within the newly created context where `PatchValue` is visible.



