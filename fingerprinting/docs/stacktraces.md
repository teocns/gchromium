
Thread #84 DedicatedWorker thread:
 blink::NavigatorUA::userAgentData() navigator_ua.cc:18
 blink::(anonymous namespace)::v8_worker_navigator::UserAgentDataAttributeGetCallback(v8::FunctionCallbackInfo<v8::Value> const&) v8_worker_navigator.cc:496
 <unknown> <unknown>:0
 <unknown> <unknown>:0
 <unknown> <unknown>:0
 <unknown> <unknown>:0
 <unknown> <unknown>:0
 <unknown> <unknown>:0
 <unknown> <unknown>:0
 <unknown> <unknown>:0
 <unknown> <unknown>:0
 <unknown> <unknown>:0
 v8::internal::GeneratedCode<unsigned long, unsigned long, unsigned long, unsigned long, unsigned long, long, unsigned long**>::Call(unsigned long, unsigned long, unsigned long, unsigned long, long, unsigned long**) [opt] simulator.h:154
 v8::internal::(anonymous namespace)::Invoke(v8::internal::Isolate*, v8::internal::(anonymous namespace)::InvokeParams const&) [opt] execution.cc:427
 v8::internal::Execution::CallScript(v8::internal::Isolate*, v8::internal::Handle<v8::internal::JSFunction>, v8::internal::Handle<v8::internal::Object>, v8::internal::Handle<v8::internal::Object>) [opt] execution.cc:540
 v8::Script::Run(v8::Local<v8::Context>, v8::Local<v8::Data>) [opt] api.cc:2326
 blink::V8ScriptRunner::RunCompiledScript(v8::Isolate*, v8::Local<v8::Script>, v8::Local<v8::Data>, blink::ExecutionContext*) v8_script_runner.cc:408
 blink::V8ScriptRunner::CompileAndRunScript(blink::ScriptState*, blink::ClassicScript*, blink::ExecuteScriptPolicy, blink::V8ScriptRunner::RethrowErrorsOption) v8_script_runner.cc:521
 blink::ClassicScript::RunScriptOnScriptStateAndReturnValue(blink::ScriptState*, blink::ExecuteScriptPolicy, blink::V8ScriptRunner::RethrowErrorsOption) classic_script.cc:219
 blink::WorkerGlobalScope::RunWorkerScript() worker_global_scope.cc:514
 blink::WorkerGlobalScope::WorkerScriptFetchFinished(blink::Script&, absl::optional<v8_inspector::V8StackTraceId>) worker_global_scope.cc:471
 blink::WorkerGlobalScope::EvaluateClassicScript(blink::KURL const&, WTF::String, std::__Cr::unique_ptr<WTF::Vector<unsigned char, 0u, WTF::PartitionAllocator>, std::__Cr::default_delete<WTF::Vector<unsigned char, 0u, WTF::PartitionAllocator>>>, v8_inspector::V8StackTraceId const&) worker_global_scope.cc:455
 blink::WorkerThread::EvaluateClassicScriptOnWorkerThread(blink::KURL const&, WTF::String, std::__Cr::unique_ptr<WTF::Vector<unsigned char, 0u, WTF::PartitionAllocator>, std::__Cr::default_delete<WTF::Vector<unsigned char, 0u, WTF::PartitionAllocator>>>, v8_inspector::V8StackTraceId const&) worker_thread.cc:745
 void base::internal::FunctorTraits<void (blink::WorkerThread::*)(blink::KURL const&, WTF::String, std::__Cr::unique_ptr<WTF::Vector<unsigned char, 0u, WTF::PartitionAllocator>, std::__Cr::default_delete<WTF::Vector<unsigned char, 0u, WTF::PartitionAllocator>>>, v8_inspector::V8StackTraceId const&), void>::Invoke<void (blink::WorkerThread::*)(blink::KURL const&, WTF::String, std::__Cr::unique_ptr<WTF::Vector<unsigned char, 0u, WTF::PartitionAllocator>, std::__Cr::default_delete<WTF::Vector<unsigned char, 0u, WTF::PartitionAllocator>>>, v8_inspector::V8StackTraceId const&), blink::WorkerThread*, blink::KURL, WTF::String, std::__Cr::unique_ptr<WTF::Vector<unsigned char, 0u, WTF::PartitionAllocator>, std::__Cr::default_delete<WTF::Vector<unsigned char, 0u, WTF::PartitionAllocator>>>, v8_inspector::V8StackTraceId>(void (blink::WorkerThread::*)(blink::KURL const&, WTF::String, std::__Cr::unique_ptr<WTF::Vector<unsigned char, 0u, WTF::PartitionAllocator>, std::__Cr::default_delete<WTF::Vector<unsigned char, 0u, WTF::PartitionAllocator>>>, v8_inspector::V8StackTraceId const&), blink::WorkerThread*&&, blink::KURL&&, WTF::String&&, std::__Cr::unique_ptr<WTF::Vector<unsigned char, 0u, WTF::PartitionAllocator>, std::__Cr::default_delete<WTF::Vector<unsigned char, 0u, WTF::PartitionAllocator>>>&&, v8_inspector::V8StackTraceId&&) bind_internal.h:746
 void base::internal::InvokeHelper<false, void, 0ul, 1ul, 2ul, 3ul, 4ul>::MakeItSo<void (blink::WorkerThread::*)(blink::KURL const&, WTF::String, std::__Cr::unique_ptr<WTF::Vector<unsigned char, 0u, WTF::PartitionAllocator>, std::__Cr::default_delete<WTF::Vector<unsigned char, 0u, WTF::PartitionAllocator>>>, v8_inspector::V8StackTraceId const&), std::__Cr::tuple<WTF::CrossThreadUnretainedWrapper<blink::WorkerThread>, blink::KURL, WTF::String, std::__Cr::unique_ptr<WTF::Vector<unsigned char, 0u, WTF::PartitionAllocator>, std::__Cr::default_delete<WTF::Vector<unsigned char, 0u, WTF::PartitionAllocator>>>, v8_inspector::V8StackTraceId>>(void (blink::WorkerThread::*&&)(blink::KURL const&, WTF::String, std::__Cr::unique_ptr<WTF::Vector<unsigned char, 0u, WTF::PartitionAllocator>, std::__Cr::default_delete<WTF::Vector<unsigned char, 0u, WTF::PartitionAllocator>>>, v8_inspector::V8StackTraceId const&), std::__Cr::tuple<WTF::CrossThreadUnretainedWrapper<blink::WorkerThread>, blink::KURL, WTF::String, std::__Cr::unique_ptr<WTF::Vector<unsigned char, 0u, WTF::PartitionAllocator>, std::__Cr::default_delete<WTF::Vector<unsigned char, 0u, WTF::PartitionAllocator>>>, v8_inspector::V8StackTraceId>&&) bind_internal.h:925
 void base::internal::Invoker<base::internal::BindState<void (blink::WorkerThread::*)(blink::KURL const&, WTF::String, std::__Cr::unique_ptr<WTF::Vector<unsigned char, 0u, WTF::PartitionAllocator>, std::__Cr::default_delete<WTF::Vector<unsigned char, 0u, WTF::PartitionAllocator>>>, v8_inspector::V8StackTraceId const&), WTF::CrossThreadUnretainedWrapper<blink::WorkerThread>, blink::KURL, WTF::String, std::__Cr::unique_ptr<WTF::Vector<unsigned char, 0u, WTF::PartitionAllocator>, std::__Cr::default_delete<WTF::Vector<unsigned char, 0u, WTF::PartitionAllocator>>>, v8_inspector::V8StackTraceId>, void ()>::RunImpl<void (blink::WorkerThread::*)(blink::KURL const&, WTF::String, std::__Cr::unique_ptr<WTF::Vector<unsigned char, 0u, WTF::PartitionAllocator>, std::__Cr::default_delete<WTF::Vector<unsigned char, 0u, WTF::PartitionAllocator>>>, v8_inspector::V8StackTraceId const&), std::__Cr::tuple<WTF::CrossThreadUnretainedWrapper<blink::WorkerThread>, blink::KURL, WTF::String, std::__Cr::unique_ptr<WTF::Vector<unsigned char, 0u, WTF::PartitionAllocator>, std::__Cr::default_delete<WTF::Vector<unsigned char, 0u, WTF::PartitionAllocator>>>, v8_inspector::V8StackTraceId>, 0ul, 1ul, 2ul, 3ul, 4ul>(void (blink::WorkerThread::*&&)(blink::KURL const&, WTF::String, std::__Cr::unique_ptr<WTF::Vector<unsigned char, 0u, WTF::PartitionAllocator>, std::__Cr::default_delete<WTF::Vector<unsigned char, 0u, WTF::PartitionAllocator>>>, v8_inspector::V8StackTraceId const&), std::__Cr::tuple<WTF::CrossThreadUnretainedWrapper<blink::WorkerThread>, blink::KURL, WTF::String, std::__Cr::unique_ptr<WTF::Vector<unsigned char, 0u, WTF::PartitionAllocator>, std::__Cr::default_delete<WTF::Vector<unsigned char, 0u, WTF::PartitionAllocator>>>, v8_inspector::V8StackTraceId>&&, std::__Cr::integer_sequence<unsigned long, 0ul, 1ul, 2ul, 3ul, 4ul>) bind_internal.h:1025
 base::internal::Invoker<base::internal::BindState<void (blink::WorkerThread::*)(blink::KURL const&, WTF::String, std::__Cr::unique_ptr<WTF::Vector<unsigned char, 0u, WTF::PartitionAllocator>, std::__Cr::default_delete<WTF::Vector<unsigned char, 0u, WTF::PartitionAllocator>>>, v8_inspector::V8StackTraceId const&), WTF::CrossThreadUnretainedWrapper<blink::WorkerThread>, blink::KURL, WTF::String, std::__Cr::unique_ptr<WTF::Vector<unsigned char, 0u, WTF::PartitionAllocator>, std::__Cr::default_delete<WTF::Vector<unsigned char, 0u, WTF::PartitionAllocator>>>, v8_inspector::V8StackTraceId>, void ()>::RunOnce(base::internal::BindStateBase*) bind_internal.h:976
 base::OnceCallback<void ()>::Run() && callback.h:152
 base::TaskAnnotator::RunTaskImpl(base::PendingTask&) task_annotator.cc:186
 void base::TaskAnnotator::RunTask<base::sequence_manager::internal::ThreadControllerWithMessagePumpImpl::DoWorkImpl(base::LazyNow*)::$_0>(perfetto::StaticString, base::PendingTask&, base::sequence_manager::internal::ThreadControllerWithMessagePumpImpl::DoWorkImpl(base::LazyNow*)::$_0&&) task_annotator.h:88
 base::sequence_manager::internal::ThreadControllerWithMessagePumpImpl::DoWorkImpl(base::LazyNow*) thread_controller_with_message_pump_impl.cc:486
 base::sequence_manager::internal::ThreadControllerWithMessagePumpImpl::DoWork() thread_controller_with_message_pump_impl.cc:351
 base::MessagePumpDefault::Run(base::MessagePump::Delegate*) message_pump_default.cc:40
 base::sequence_manager::internal::ThreadControllerWithMessagePumpImpl::Run(bool, base::TimeDelta) thread_controller_with_message_pump_impl.cc:651
 base::RunLoop::Run(base::Location const&) run_loop.cc:134
 blink::scheduler::NonMainThreadImpl::SimpleThreadImpl::Run() non_main_thread_impl.cc:182
 base::SimpleThread::ThreadMain() simple_thread.cc:78
 base::(anonymous namespace)::ThreadFunc(void*) platform_thread_posix.cc:101
 _pthread_start <unknown>:35

Thread #1 CrBrowserMain:
 std::__Cr::__libcpp_numeric_limits<int, true>::lowest() limits:211
 std::__Cr::numeric_limits<int>::lowest() limits:460
 int base::internal::CommonMin<int, int>() safe_conversions_impl.h:833
 int base::internal::CommonMaxOrMin<int, int>(bool) safe_conversions_impl.h:841
 int base::internal::ClampedAddOp<int, int, void>::Do<int>(int, int) clamped_math_impl.h:90
 base::internal::ClampedNumeric<int> base::internal::ClampedNumeric<int>::MathOp<base::internal::ClampedAddOp, int, int>(int, int) clamped_math.h:157
 base::internal::ClampedNumeric<base::internal::MathWrapper<base::internal::ClampedAddOp, int, int>::type> base::internal::ClampMathOp<base::internal::ClampedAddOp, int, int>(int, int) clamped_math.h:209
 auto base::internal::ClampAdd<int, int>(int, int) clamped_math.h:222
 gfx::Point::operator+=(gfx::Vector2d const&) point.h:64
 gfx::Rect::Inset(gfx::Insets const&) rect.cc:118
 gfx::Rect::Inset(int) rect.h:146
 cc::PictureLayerTilingSet::ComputeSoonBorderRect(gfx::Rect const&, float) picture_layer_tiling_set.cc:485
 cc::PictureLayerTilingSet::UpdatePriorityRects(gfx::Rect const&, double, float) picture_layer_tiling_set.cc:516
 cc::PictureLayerTilingSet::UpdateTilePriorities(gfx::Rect const&, float, double, cc::Occlusion const&, bool) picture_layer_tiling_set.cc:545
 cc::PictureLayerImpl::UpdateTiles() picture_layer_impl.cc:666
 cc::LayerTreeImpl::UpdateDrawProperties(bool, std::__Cr::vector<cc::LayerImpl*, std::__Cr::allocator<cc::LayerImpl*>>*) layer_tree_impl.cc:1695
 cc::LayerTreeHostImpl::UpdateSyncTreeAfterCommitOrImplSideInvalidation() layer_tree_host_impl.cc:778
 cc::LayerTreeHostImpl::CommitComplete() layer_tree_host_impl.cc:739
 cc::SingleThreadProxy::DoPostCommit() single_thread_proxy.cc:254
 cc::SingleThreadProxy::ScheduledActionPostCommit() single_thread_proxy.cc:1179
 cc::Scheduler::ProcessScheduledActions() scheduler.cc:907
 cc::Scheduler::NotifyReadyToCommit(std::__Cr::unique_ptr<cc::BeginMainFrameMetrics, std::__Cr::default_delete<cc::BeginMainFrameMetrics>>) scheduler.cc:210
 cc::SingleThreadProxy::DoPainting(viz::BeginFrameArgs const&) single_thread_proxy.cc:1132
 cc::SingleThreadProxy::BeginMainFrame(viz::BeginFrameArgs const&) single_thread_proxy.cc:1081
 void base::internal::FunctorTraits<void (cc::SingleThreadProxy::*)(viz::BeginFrameArgs const&), void>::Invoke<void (cc::SingleThreadProxy::*)(viz::BeginFrameArgs const&), base::WeakPtr<cc::SingleThreadProxy> const&, viz::BeginFrameArgs>(void (cc::SingleThreadProxy::*)(viz::BeginFrameArgs const&), base::WeakPtr<cc::SingleThreadProxy> const&, viz::BeginFrameArgs&&) bind_internal.h:746
 void base::internal::InvokeHelper<true, void, 0ul, 1ul>::MakeItSo<void (cc::SingleThreadProxy::*)(viz::BeginFrameArgs const&), std::__Cr::tuple<base::WeakPtr<cc::SingleThreadProxy>, viz::BeginFrameArgs>>(void (cc::SingleThreadProxy::*&&)(viz::BeginFrameArgs const&), std::__Cr::tuple<base::WeakPtr<cc::SingleThreadProxy>, viz::BeginFrameArgs>&&) bind_internal.h:953
 void base::internal::Invoker<base::internal::BindState<void (cc::SingleThreadProxy::*)(viz::BeginFrameArgs const&), base::WeakPtr<cc::SingleThreadProxy>, viz::BeginFrameArgs>, void ()>::RunImpl<void (cc::SingleThreadProxy::*)(viz::BeginFrameArgs const&), std::__Cr::tuple<base::WeakPtr<cc::SingleThreadProxy>, viz::BeginFrameArgs>, 0ul, 1ul>(void (cc::SingleThreadProxy::*&&)(viz::BeginFrameArgs const&), std::__Cr::tuple<base::WeakPtr<cc::SingleThreadProxy>, viz::BeginFrameArgs>&&, std::__Cr::integer_sequence<unsigned long, 0ul, 1ul>) bind_internal.h:1025
 base::internal::Invoker<base::internal::BindState<void (cc::SingleThreadProxy::*)(viz::BeginFrameArgs const&), base::WeakPtr<cc::SingleThreadProxy>, viz::BeginFrameArgs>, void ()>::RunOnce(base::internal::BindStateBase*) bind_internal.h:976
 base::OnceCallback<void ()>::Run() && callback.h:152
 ui::(anonymous namespace)::WrappedTask::Run() window_resize_helper_mac.cc:149
 void base::internal::FunctorTraits<void (ui::(anonymous namespace)::WrappedTask::*)(), void>::Invoke<void (ui::(anonymous namespace)::WrappedTask::*)(), std::__Cr::unique_ptr<ui::(anonymous namespace)::WrappedTask, std::__Cr::default_delete<ui::(anonymous namespace)::WrappedTask>>>(void (ui::(anonymous namespace)::WrappedTask::*)(), std::__Cr::unique_ptr<ui::(anonymous namespace)::WrappedTask, std::__Cr::default_delete<ui::(anonymous namespace)::WrappedTask>>&&) bind_internal.h:746
 void base::internal::InvokeHelper<false, void, 0ul>::MakeItSo<void (ui::(anonymous namespace)::WrappedTask::*)(), std::__Cr::tuple<std::__Cr::unique_ptr<ui::(anonymous namespace)::WrappedTask, std::__Cr::default_delete<ui::(anonymous namespace)::WrappedTask>>>>(void (ui::(anonymous namespace)::WrappedTask::*&&)(), std::__Cr::tuple<std::__Cr::unique_ptr<ui::(anonymous namespace)::WrappedTask, std::__Cr::default_delete<ui::(anonymous namespace)::WrappedTask>>>&&) bind_internal.h:925
 void base::internal::Invoker<base::internal::BindState<void (ui::(anonymous namespace)::WrappedTask::*)(), std::__Cr::unique_ptr<ui::(anonymous namespace)::WrappedTask, std::__Cr::default_delete<ui::(anonymous namespace)::WrappedTask>>>, void ()>::RunImpl<void (ui::(anonymous namespace)::WrappedTask::*)(), std::__Cr::tuple<std::__Cr::unique_ptr<ui::(anonymous namespace)::WrappedTask, std::__Cr::default_delete<ui::(anonymous namespace)::WrappedTask>>>, 0ul>(void (ui::(anonymous namespace)::WrappedTask::*&&)(), std::__Cr::tuple<std::__Cr::unique_ptr<ui::(anonymous namespace)::WrappedTask, std::__Cr::default_delete<ui::(anonymous namespace)::WrappedTask>>>&&, std::__Cr::integer_sequence<unsigned long, 0ul>) bind_internal.h:1025
 base::internal::Invoker<base::internal::BindState<void (ui::(anonymous namespace)::WrappedTask::*)(), std::__Cr::unique_ptr<ui::(anonymous namespace)::WrappedTask, std::__Cr::default_delete<ui::(anonymous namespace)::WrappedTask>>>, void ()>::RunOnce(base::internal::BindStateBase*) bind_internal.h:976
 base::OnceCallback<void ()>::Run() && callback.h:152
 base::TaskAnnotator::RunTaskImpl(base::PendingTask&) task_annotator.cc:186
 void base::TaskAnnotator::RunTask<base::sequence_manager::internal::ThreadControllerWithMessagePumpImpl::DoWorkImpl(base::LazyNow*)::$_0>(perfetto::StaticString, base::PendingTask&, base::sequence_manager::internal::ThreadControllerWithMessagePumpImpl::DoWorkImpl(base::LazyNow*)::$_0&&) task_annotator.h:88
 base::sequence_manager::internal::ThreadControllerWithMessagePumpImpl::DoWorkImpl(base::LazyNow*) thread_controller_with_message_pump_impl.cc:486
 base::sequence_manager::internal::ThreadControllerWithMessagePumpImpl::DoWork() thread_controller_with_message_pump_impl.cc:351
 base::MessagePumpCFRunLoopBase::RunWork() message_pump_mac.mm:449
 invocation function for block in base::MessagePumpCFRunLoopBase::RunWorkSource(void*) message_pump_mac.mm:422
 base::mac::CallWithEHFrame(void () block_pointer) call_with_eh_frame_asm.S:65
 base::MessagePumpCFRunLoopBase::RunWorkSource(void*) message_pump_mac.mm:421
 __CFRUNLOOP_IS_CALLING_OUT_TO_A_SOURCE0_PERFORM_FUNCTION__ <unknown>:8
 __CFRunLoopDoSource0 <unknown>:45
 __CFRunLoopDoSources0 <unknown>:62
 __CFRunLoopRun <unknown>:208
 CFRunLoopRunSpecific <unknown>:153
 RunCurrentEventLoopInMode <unknown>:74
 ReceiveNextEventCommon <unknown>:163
 _BlockUntilNextEventMatchingListInModeWithFilter <unknown>:20
 _DPSNextEvent <unknown>:166
 -[NSApplication(NSEventRouting) _nextEventMatchingEventMask:untilDate:inMode:dequeue:] <unknown>:180
 __71-[BrowserCrApplication nextEventMatchingMask:untilDate:inMode:dequeue:]_block_invoke chrome_browser_application_mac.mm:248
 base::mac::CallWithEHFrame(void () block_pointer) call_with_eh_frame_asm.S:65
 -[BrowserCrApplication nextEventMatchingMask:untilDate:inMode:dequeue:] chrome_browser_application_mac.mm:247
 -[NSApplication run] <unknown>:120
 base::MessagePumpNSApplication::DoRun(base::MessagePump::Delegate*) message_pump_mac.mm:811
 base::MessagePumpCFRunLoopBase::Run(base::MessagePump::Delegate*) message_pump_mac.mm:167
 base::sequence_manager::internal::ThreadControllerWithMessagePumpImpl::Run(bool, base::TimeDelta) thread_controller_with_message_pump_impl.cc:651
 base::RunLoop::Run(base::Location const&) run_loop.cc:134
 content::BrowserMainLoop::RunMainMessageLoop() browser_main_loop.cc:1076
 content::BrowserMainRunnerImpl::Run() browser_main_runner_impl.cc:158
 content::BrowserMain(content::MainFunctionParams) browser_main.cc:34
 content::RunBrowserProcessMain(content::MainFunctionParams, content::ContentMainDelegate*) content_main_runner_impl.cc:685
 content::ContentMainRunnerImpl::RunBrowser(content::MainFunctionParams, bool) content_main_runner_impl.cc:1266
 content::ContentMainRunnerImpl::Run() content_main_runner_impl.cc:1116
 content::RunContentProcess(content::ContentMainParams, content::ContentMainRunner*) content_main.cc:326
 content::ContentMain(content::ContentMainParams) content_main.cc:343
 ::ChromeMain(int, const char **) chrome_main.cc:187
 main chrome_exe_main_mac.cc:216
 start <unknown>:591

Thread #2 StackSamplingProfiler:
 mach_msg2_trap <unknown>:3
 mach_msg2_internal <unknown>:21
 mach_msg_overwrite <unknown>:120
 mach_msg <unknown>:7
 base::WaitableEvent::TimedWaitImpl(base::TimeDelta) waitable_event_mac.cc:110
 base::WaitableEvent::TimedWait(base::TimeDelta) waitable_event.cc:39
 base::MessagePumpDefault::Run(base::MessagePump::Delegate*) message_pump_default.cc:58
 base::sequence_manager::internal::ThreadControllerWithMessagePumpImpl::Run(bool, base::TimeDelta) thread_controller_with_message_pump_impl.cc:651
 base::RunLoop::Run(base::Location const&) run_loop.cc:134
 base::Thread::Run(base::RunLoop*) thread.cc:342
 base::Thread::ThreadMain() thread.cc:414
 base::(anonymous namespace)::ThreadFunc(void*) platform_thread_posix.cc:101
 _pthread_start <unknown>:35

Thread #4:
 __workq_kernreturn <unknown>:3

Thread #5:
 mach_msg2_trap <unknown>:3
 mach_msg2_internal <unknown>:21
 mach_msg_overwrite <unknown>:120
 mach_msg <unknown>:7
 crashpad::(anonymous namespace)::MachMessageWithDeadlineInternal(mach_msg_header_t*, int, unsigned int, unsigned int, unsigned long long, unsigned int, bool) mach_message.cc:116
 crashpad::MachMessageWithDeadline(mach_msg_header_t*, int, unsigned int, unsigned int, unsigned long long, unsigned int, bool) mach_message.cc:187
 crashpad::(anonymous namespace)::MachMessageAllocateReceive(crashpad::(anonymous namespace)::MachMessageBuffer*, int, unsigned int, unsigned int, unsigned long long, unsigned int, bool) mach_message_server.cc:105
 crashpad::MachMessageServer::Run(crashpad::MachMessageServer::Interface*, unsigned int, int, crashpad::MachMessageServer::Persistent, crashpad::MachMessageServer::ReceiveLarge, unsigned int) mach_message_server.cc:166
 crashpad::(anonymous namespace)::HandlerStarter::RestartThreadMain(void*) crashpad_client_mac.cc:412
 _pthread_start <unknown>:35

Thread #8 HangWatcher:
 mach_msg2_trap <unknown>:3
 mach_msg2_internal <unknown>:21
 mach_msg_overwrite <unknown>:120
 mach_msg <unknown>:7
 base::WaitableEvent::TimedWaitImpl(base::TimeDelta) waitable_event_mac.cc:110
 base::WaitableEvent::TimedWait(base::TimeDelta) waitable_event.cc:39
 base::HangWatcher::Wait() hang_watcher.cc:574
 base::HangWatcher::Run() hang_watcher.cc:624
 base::DelegateSimpleThread::Run() simple_thread.cc:101
 base::SimpleThread::ThreadMain() simple_thread.cc:78
 base::(anonymous namespace)::ThreadFunc(void*) platform_thread_posix.cc:101
 _pthread_start <unknown>:35

Thread #9 ThreadPoolServiceThread:
 kevent64 <unknown>:3
 base::MessagePumpKqueue::DoInternalWork(base::MessagePump::Delegate*, base::MessagePump::Delegate::NextWorkInfo*) message_pump_kqueue.cc:427
 base::MessagePumpKqueue::RunSimplified(base::MessagePump::Delegate*) message_pump_kqueue.cc:211
 base::MessagePumpKqueue::Run(base::MessagePump::Delegate*) message_pump_kqueue.cc:163
 base::sequence_manager::internal::ThreadControllerWithMessagePumpImpl::Run(bool, base::TimeDelta) thread_controller_with_message_pump_impl.cc:651
 base::RunLoop::Run(base::Location const&) run_loop.cc:134
 base::Thread::Run(base::RunLoop*) thread.cc:342
 base::internal::ServiceThread::Run(base::RunLoop*) service_thread.cc:15
 base::Thread::ThreadMain() thread.cc:414
 base::(anonymous namespace)::ThreadFunc(void*) platform_thread_posix.cc:101
 _pthread_start <unknown>:35

Thread #10 ThreadPoolForegroundWorker:
 mach_msg2_trap <unknown>:3
 mach_msg2_internal <unknown>:21
 mach_msg_overwrite <unknown>:120
 mach_msg <unknown>:7
 base::WaitableEvent::TimedWaitImpl(base::TimeDelta) waitable_event_mac.cc:110
 base::WaitableEvent::TimedWait(base::TimeDelta) waitable_event.cc:39
 base::internal::WorkerThread::Delegate::WaitForWork(base::WaitableEvent*) worker_thread.cc:132
 base::internal::WorkerThread::RunWorker() worker_thread.cc:466
 base::internal::WorkerThread::RunPooledWorker() worker_thread.cc:356
 base::internal::WorkerThread::ThreadMain() worker_thread.cc:336
 base::(anonymous namespace)::ThreadFunc(void*) platform_thread_posix.cc:101
 _pthread_start <unknown>:35

Thread #11 ThreadPoolUtilityWorker:
 mach_msg2_trap <unknown>:3
 mach_msg2_internal <unknown>:21
 mach_msg_overwrite <unknown>:120
 mach_msg <unknown>:7
 base::WaitableEvent::TimedWaitImpl(base::TimeDelta) waitable_event_mac.cc:110
 base::WaitableEvent::TimedWait(base::TimeDelta) waitable_event.cc:39
 base::internal::WorkerThread::Delegate::WaitForWork(base::WaitableEvent*) worker_thread.cc:132
 base::internal::WorkerThread::RunWorker() worker_thread.cc:466
 base::internal::WorkerThread::RunPooledWorker() worker_thread.cc:356
 base::internal::WorkerThread::ThreadMain() worker_thread.cc:336
 base::(anonymous namespace)::ThreadFunc(void*) platform_thread_posix.cc:101
 _pthread_start <unknown>:35

Thread #12 ThreadPoolBackgroundWorker:
 mach_msg2_trap <unknown>:3
 mach_msg2_internal <unknown>:21
 mach_msg_overwrite <unknown>:120
 mach_msg <unknown>:7
 base::WaitableEvent::TimedWaitImpl(base::TimeDelta) waitable_event_mac.cc:110
 base::WaitableEvent::TimedWait(base::TimeDelta) waitable_event.cc:39
 base::internal::WorkerThread::Delegate::WaitForWork(base::WaitableEvent*) worker_thread.cc:132
 base::internal::WorkerThread::RunWorker() worker_thread.cc:466
 base::internal::WorkerThread::RunBackgroundPooledWorker() worker_thread.cc:361
 base::internal::WorkerThread::ThreadMain() worker_thread.cc:315
 base::(anonymous namespace)::ThreadFunc(void*) platform_thread_posix.cc:101
 _pthread_start <unknown>:35

Thread #13 ThreadPoolForegroundWorker:
 mach_msg2_trap <unknown>:3
 mach_msg2_internal <unknown>:21
 mach_msg_overwrite <unknown>:120
 mach_msg <unknown>:7
 base::WaitableEvent::TimedWaitImpl(base::TimeDelta) waitable_event_mac.cc:110
 base::WaitableEvent::TimedWait(base::TimeDelta) waitable_event.cc:39
 base::internal::WorkerThread::Delegate::WaitForWork(base::WaitableEvent*) worker_thread.cc:132
 base::internal::WorkerThread::RunWorker() worker_thread.cc:466
 base::internal::WorkerThread::RunPooledWorker() worker_thread.cc:356
 base::internal::WorkerThread::ThreadMain() worker_thread.cc:336
 base::(anonymous namespace)::ThreadFunc(void*) platform_thread_posix.cc:101
 _pthread_start <unknown>:35

Thread #14 Chrome_IOThread:
 kevent64 <unknown>:3
 base::MessagePumpKqueue::DoInternalWork(base::MessagePump::Delegate*, base::MessagePump::Delegate::NextWorkInfo*) message_pump_kqueue.cc:427
 base::MessagePumpKqueue::RunSimplified(base::MessagePump::Delegate*) message_pump_kqueue.cc:211
 base::MessagePumpKqueue::Run(base::MessagePump::Delegate*) message_pump_kqueue.cc:163
 base::sequence_manager::internal::ThreadControllerWithMessagePumpImpl::Run(bool, base::TimeDelta) thread_controller_with_message_pump_impl.cc:651
 base::RunLoop::Run(base::Location const&) run_loop.cc:134
 base::Thread::Run(base::RunLoop*) thread.cc:342
 content::BrowserProcessIOThread::IOThreadRun(base::RunLoop*) browser_process_io_thread.cc:119
 content::BrowserProcessIOThread::Run(base::RunLoop*) browser_process_io_thread.cc:91
 base::Thread::ThreadMain() thread.cc:414
 base::(anonymous namespace)::ThreadFunc(void*) platform_thread_posix.cc:101
 _pthread_start <unknown>:35

Thread #15 MemoryInfra:
 mach_msg2_trap <unknown>:3
 mach_msg2_internal <unknown>:21
 mach_msg_overwrite <unknown>:120
 mach_msg <unknown>:7
 base::WaitableEvent::TimedWaitImpl(base::TimeDelta) waitable_event_mac.cc:110
 base::WaitableEvent::TimedWait(base::TimeDelta) waitable_event.cc:39
 base::WaitableEvent::Wait() waitable_event.cc:23
 base::MessagePumpDefault::Run(base::MessagePump::Delegate*) message_pump_default.cc:56
 base::sequence_manager::internal::ThreadControllerWithMessagePumpImpl::Run(bool, base::TimeDelta) thread_controller_with_message_pump_impl.cc:651
 base::RunLoop::Run(base::Location const&) run_loop.cc:134
 base::Thread::Run(base::RunLoop*) thread.cc:342
 base::Thread::ThreadMain() thread.cc:414
 base::(anonymous namespace)::ThreadFunc(void*) platform_thread_posix.cc:101
 _pthread_start <unknown>:35

Thread #16 NetworkConfigWatcher:
 mach_msg2_trap <unknown>:3
 mach_msg2_internal <unknown>:21
 mach_msg_overwrite <unknown>:120
 mach_msg <unknown>:7
 __CFRunLoopServiceMachPort <unknown>:41
 __CFRunLoopRun <unknown>:303
 CFRunLoopRunSpecific <unknown>:153
 -[NSRunLoop(NSRunLoop) runMode:beforeDate:] <unknown>:54
 base::MessagePumpNSRunLoop::DoRun(base::MessagePump::Delegate*) message_pump_mac.mm:720
 base::MessagePumpCFRunLoopBase::Run(base::MessagePump::Delegate*) message_pump_mac.mm:167
 base::sequence_manager::internal::ThreadControllerWithMessagePumpImpl::Run(bool, base::TimeDelta) thread_controller_with_message_pump_impl.cc:651
 base::RunLoop::Run(base::Location const&) run_loop.cc:134
 base::Thread::Run(base::RunLoop*) thread.cc:342
 base::Thread::ThreadMain() thread.cc:414
 base::(anonymous namespace)::ThreadFunc(void*) platform_thread_posix.cc:101
 _pthread_start <unknown>:35

Thread #17 CrShutdownDetector:
 read <unknown>:3
 (anonymous namespace)::ShutdownDetector::ThreadMain() shutdown_signal_handlers_posix.cc:135
 base::(anonymous namespace)::ThreadFunc(void*) platform_thread_posix.cc:101
 _pthread_start <unknown>:35

Thread #18 NetworkConfigWatcher:
 mach_msg2_trap <unknown>:3
 mach_msg2_internal <unknown>:21
 mach_msg_overwrite <unknown>:120
 mach_msg <unknown>:7
 __CFRunLoopServiceMachPort <unknown>:41
 __CFRunLoopRun <unknown>:303
 CFRunLoopRunSpecific <unknown>:153
 -[NSRunLoop(NSRunLoop) runMode:beforeDate:] <unknown>:54
 base::MessagePumpNSRunLoop::DoRun(base::MessagePump::Delegate*) message_pump_mac.mm:720
 base::MessagePumpCFRunLoopBase::Run(base::MessagePump::Delegate*) message_pump_mac.mm:167
 base::sequence_manager::internal::ThreadControllerWithMessagePumpImpl::Run(bool, base::TimeDelta) thread_controller_with_message_pump_impl.cc:651
 base::RunLoop::Run(base::Location const&) run_loop.cc:134
 base::Thread::Run(base::RunLoop*) thread.cc:342
 base::Thread::ThreadMain() thread.cc:414
 base::(anonymous namespace)::ThreadFunc(void*) platform_thread_posix.cc:101
 _pthread_start <unknown>:35

Thread #19 ThreadPoolForegroundWorker:
 mach_msg2_trap <unknown>:3
 mach_msg2_internal <unknown>:21
 mach_msg_overwrite <unknown>:120
 mach_msg <unknown>:7
 base::WaitableEvent::TimedWaitImpl(base::TimeDelta) waitable_event_mac.cc:110
 base::WaitableEvent::TimedWait(base::TimeDelta) waitable_event.cc:39
 base::internal::WorkerThread::Delegate::WaitForWork(base::WaitableEvent*) worker_thread.cc:132
 base::internal::WorkerThread::RunWorker() worker_thread.cc:466
 base::internal::WorkerThread::RunPooledWorker() worker_thread.cc:356
 base::internal::WorkerThread::ThreadMain() worker_thread.cc:336
 base::(anonymous namespace)::ThreadFunc(void*) platform_thread_posix.cc:101
 _pthread_start <unknown>:35

Thread #20 ThreadPoolForegroundWorker:
 mach_msg2_trap <unknown>:3
 mach_msg2_internal <unknown>:21
 mach_msg_overwrite <unknown>:120
 mach_msg <unknown>:7
 base::WaitableEvent::TimedWaitImpl(base::TimeDelta) waitable_event_mac.cc:110
 base::WaitableEvent::TimedWait(base::TimeDelta) waitable_event.cc:39
 base::internal::WorkerThread::Delegate::WaitForWork(base::WaitableEvent*) worker_thread.cc:132
 base::internal::WorkerThread::RunWorker() worker_thread.cc:466
 base::internal::WorkerThread::RunPooledWorker() worker_thread.cc:356
 base::internal::WorkerThread::ThreadMain() worker_thread.cc:336
 base::(anonymous namespace)::ThreadFunc(void*) platform_thread_posix.cc:101
 _pthread_start <unknown>:35

Thread #21 ThreadPoolForegroundWorker:
 mach_msg2_trap <unknown>:3
 mach_msg2_internal <unknown>:21
 mach_msg_overwrite <unknown>:120
 mach_msg <unknown>:7
 base::WaitableEvent::TimedWaitImpl(base::TimeDelta) waitable_event_mac.cc:110
 base::WaitableEvent::TimedWait(base::TimeDelta) waitable_event.cc:39
 base::internal::WorkerThread::Delegate::WaitForWork(base::WaitableEvent*) worker_thread.cc:132
 base::internal::WorkerThread::RunWorker() worker_thread.cc:466
 base::internal::WorkerThread::RunPooledWorker() worker_thread.cc:356
 base::internal::WorkerThread::ThreadMain() worker_thread.cc:336
 base::(anonymous namespace)::ThreadFunc(void*) platform_thread_posix.cc:101
 _pthread_start <unknown>:35

Thread #22 NetworkNotificationThreadMac:
 mach_msg2_trap <unknown>:3
 mach_msg2_internal <unknown>:21
 mach_msg_overwrite <unknown>:120
 mach_msg <unknown>:7
 __CFRunLoopServiceMachPort <unknown>:41
 __CFRunLoopRun <unknown>:303
 CFRunLoopRunSpecific <unknown>:153
 -[NSRunLoop(NSRunLoop) runMode:beforeDate:] <unknown>:54
 base::MessagePumpNSRunLoop::DoRun(base::MessagePump::Delegate*) message_pump_mac.mm:720
 base::MessagePumpCFRunLoopBase::Run(base::MessagePump::Delegate*) message_pump_mac.mm:167
 base::sequence_manager::internal::ThreadControllerWithMessagePumpImpl::Run(bool, base::TimeDelta) thread_controller_with_message_pump_impl.cc:651
 base::RunLoop::Run(base::Location const&) run_loop.cc:134
 base::Thread::Run(base::RunLoop*) thread.cc:342
 base::Thread::ThreadMain() thread.cc:414
 base::(anonymous namespace)::ThreadFunc(void*) platform_thread_posix.cc:101
 _pthread_start <unknown>:35

Thread #23 ThreadPoolForegroundWorker:
 mach_msg2_trap <unknown>:3
 mach_msg2_internal <unknown>:21
 mach_msg_overwrite <unknown>:120
 mach_msg <unknown>:7
 base::WaitableEvent::TimedWaitImpl(base::TimeDelta) waitable_event_mac.cc:110
 base::WaitableEvent::TimedWait(base::TimeDelta) waitable_event.cc:39
 base::internal::WorkerThread::Delegate::WaitForWork(base::WaitableEvent*) worker_thread.cc:132
 base::internal::WorkerThread::RunWorker() worker_thread.cc:466
 base::internal::WorkerThread::RunPooledWorker() worker_thread.cc:356
 base::internal::WorkerThread::ThreadMain() worker_thread.cc:336
 base::(anonymous namespace)::ThreadFunc(void*) platform_thread_posix.cc:101
 _pthread_start <unknown>:35

Thread #24 ThreadPoolUtilityWorker:
 mach_msg2_trap <unknown>:3
 mach_msg2_internal <unknown>:21
 mach_msg_overwrite <unknown>:120
 mach_msg <unknown>:7
 base::WaitableEvent::TimedWaitImpl(base::TimeDelta) waitable_event_mac.cc:110
 base::WaitableEvent::TimedWait(base::TimeDelta) waitable_event.cc:39
 base::internal::WorkerThread::Delegate::WaitForWork(base::WaitableEvent*) worker_thread.cc:132
 base::internal::WorkerThread::RunWorker() worker_thread.cc:466
 base::internal::WorkerThread::RunPooledWorker() worker_thread.cc:356
 base::internal::WorkerThread::ThreadMain() worker_thread.cc:336
 base::(anonymous namespace)::ThreadFunc(void*) platform_thread_posix.cc:101
 _pthread_start <unknown>:35

Thread #25 Chrome_InProcGpuThread:
 angle::BitSetT<64ul, unsigned long long, unsigned long>::Iterator::operator==(angle::BitSetT<64ul, unsigned long long, unsigned long>::Iterator const&) const bitset_utils.h:438
 angle::BitSetT<64ul, unsigned long long, unsigned long>::Iterator::operator!=(angle::BitSetT<64ul, unsigned long long, unsigned long>::Iterator const&) const bitset_utils.h:445
 angle::BitSetArray<96ul>::Iterator::operator!=(angle::BitSetArray<96ul>::Iterator const&) const bitset_utils.h:826
 gl::State::syncActiveTextures(gl::Context const*, gl::Command) State.cpp:3479
 gl::State::syncDirtyObjects(gl::Context const*, angle::BitSetT<13ul, unsigned long long, unsigned long> const&, gl::Command) State.h:1277
 gl::Context::syncDirtyObjects(angle::BitSetT<13ul, unsigned long long, unsigned long> const&, gl::Command) Context.inl.h:121
 gl::Context::prepareForDraw(gl::PrimitiveMode) Context.inl.h:131
 gl::Context::drawElementsInstancedBaseVertexBaseInstance(gl::PrimitiveMode, int, gl::DrawElementsType, void const*, int, int, unsigned int) Context.cpp:7177
 ::GL_DrawElementsInstancedBaseVertexBaseInstanceEXT(GLenum, GLsizei, GLenum, const void *, GLsizei, GLint, GLuint) entry_points_gles_ext_autogen.cpp:5058
 ::glDrawElementsInstancedBaseVertexBaseInstanceEXT(GLenum, GLsizei, GLenum, const void *, GLsizei, GLint, GLuint) libGLESv2_autogen.cpp:4055
 GrGLFunction<void (unsigned int, int, unsigned int, void const*, int, int, unsigned int)> gl::init::(anonymous namespace)::bind_impl<true, true, false, void, unsigned int, int, unsigned int, void const*, int, int, unsigned int>(void (*)(unsigned int, int, unsigned int, void const*, int, int, unsigned int), gl::ProgressReporter*)::'lambda'(unsigned int, int, unsigned int, void const*, int, int, unsigned int)::operator()(unsigned int, int, unsigned int, void const*, int, int, unsigned int) const create_gr_gl_interface.cc:158
 GrGLFunction<void (unsigned int, int, unsigned int, void const*, int, int, unsigned int)>::GrGLFunction<GrGLFunction<void (unsigned int, int, unsigned int, void const*, int, int, unsigned int)> gl::init::(anonymous namespace)::bind_impl<true, true, false, void, unsigned int, int, unsigned int, void const*, int, int, unsigned int>(void (*)(unsigned int, int, unsigned int, void const*, int, int, unsigned int), gl::ProgressReporter*)::'lambda'(unsigned int, int, unsigned int, void const*, int, int, unsigned int)>(GrGLFunction<void (unsigned int, int, unsigned int, void const*, int, int, unsigned int)> gl::init::(anonymous namespace)::bind_impl<true, true, false, void, unsigned int, int, unsigned int, void const*, int, int, unsigned int>(void (*)(unsigned int, int, unsigned int, void const*, int, int, unsigned int), gl::ProgressReporter*)::'lambda'(unsigned int, int, unsigned int, void const*, int, int, unsigned int))::'lambda'(void const*, unsigned int, int, unsigned int, void const*, int, int, unsigned int)::operator()(void const*, unsigned int, int, unsigned int, void const*, int, int, unsigned int) const GrGLFunctions.h:288
 GrGLFunction<void (unsigned int, int, unsigned int, void const*, int, int, unsigned int)>::GrGLFunction<GrGLFunction<void (unsigned int, int, unsigned int, void const*, int, int, unsigned int)> gl::init::(anonymous namespace)::bind_impl<true, true, false, void, unsigned int, int, unsigned int, void const*, int, int, unsigned int>(void (*)(unsigned int, int, unsigned int, void const*, int, int, unsigned int), gl::ProgressReporter*)::'lambda'(unsigned int, int, unsigned int, void const*, int, int, unsigned int)>(GrGLFunction<void (unsigned int, int, unsigned int, void const*, int, int, unsigned int)> gl::init::(anonymous namespace)::bind_impl<true, true, false, void, unsigned int, int, unsigned int, void const*, int, int, unsigned int>(void (*)(unsigned int, int, unsigned int, void const*, int, int, unsigned int), gl::ProgressReporter*)::'lambda'(unsigned int, int, unsigned int, void const*, int, int, unsigned int))::'lambda'(void const*, unsigned int, int, unsigned int, void const*, int, int, unsigned int)::__invoke(void const*, unsigned int, int, unsigned int, void const*, int, int, unsigned int) GrGLFunctions.h:286
 GrGLFunction<void (unsigned int, int, unsigned int, void const*, int, int, unsigned int)>::operator()(unsigned int, int, unsigned int, void const*, int, int, unsigned int) const GrGLFunctions.h:294
 GrGLOpsRenderPass::onDrawIndexedInstanced(int, int, int, int, int) GrGLOpsRenderPass.cpp:253
 GrOpsRenderPass::drawIndexedInstanced(int, int, int, int, int) GrOpsRenderPass.cpp:262
 GrOpFlushState::drawIndexedInstanced(int, int, int, int, int) GrOpFlushState.h:249
 skgpu::ganesh::PathWedgeTessellator::draw(GrOpFlushState*) const PathTessellator.cpp:278
 skgpu::ganesh::PathStencilCoverOp::onExecute(GrOpFlushState*, SkRect const&) PathStencilCoverOp.cpp:341
 GrOp::execute(GrOpFlushState*, SkRect const&) GrOp.h:193
 skgpu::ganesh::OpsTask::onExecute(GrOpFlushState*) OpsTask.cpp:648
 skgpu::ganesh::AtlasRenderTask::onExecute(GrOpFlushState*) AtlasRenderTask.cpp:181
 GrRenderTask::execute(GrOpFlushState*) GrRenderTask.h:42
 GrDrawingManager::executeRenderTasks(GrOpFlushState*) GrDrawingManager.cpp:272
 GrDrawingManager::flush(SkSpan<GrSurfaceProxy*>, SkSurfaces::BackendSurfaceAccess, GrFlushInfo const&, skgpu::MutableTextureState const*) GrDrawingManager.cpp:194
 GrDrawingManager::flushSurfaces(SkSpan<GrSurfaceProxy*>, SkSurfaces::BackendSurfaceAccess, GrFlushInfo const&, skgpu::MutableTextureState const*) GrDrawingManager.cpp:521
 GrDirectContextPriv::flushSurfaces(SkSpan<GrSurfaceProxy*>, SkSurfaces::BackendSurfaceAccess, GrFlushInfo const&, skgpu::MutableTextureState const*) GrDirectContextPriv.cpp:67
 GrDirectContextPriv::flushSurface(GrSurfaceProxy*, SkSurfaces::BackendSurfaceAccess, GrFlushInfo const&, skgpu::MutableTextureState const*) GrDirectContextPriv.h:64
 GrDirectContext::flush(SkSurface*, GrFlushInfo const&, skgpu::MutableTextureState const*) GrDirectContext.cpp:542
 skgpu::ganesh::Flush(SkSurface*) SkSurface_Ganesh.cpp:768
 gpu::raster::RasterDecoderImpl::FlushSurface(gpu::SkiaImageRepresentation::ScopedWriteAccess*) raster_decoder.cc:758
 gpu::raster::RasterDecoderImpl::DoEndRasterCHROMIUM() raster_decoder.cc:2950
 gpu::raster::RasterDecoderImpl::HandleEndRasterCHROMIUM(unsigned int, void const volatile*) raster_decoder_autogen.h:161
 gpu::error::Error gpu::raster::RasterDecoderImpl::DoCommandsImpl<false>(unsigned int, void const volatile*, int, int*) raster_decoder.cc:1519
 gpu::raster::RasterDecoderImpl::DoCommands(unsigned int, void const volatile*, int, int*) raster_decoder.cc:1579
 gpu::CommandBufferService::Flush(int, gpu::AsyncAPIInterface*) command_buffer_service.cc:232
 gpu::CommandBufferStub::OnAsyncFlush(int, unsigned int, std::__Cr::vector<gpu::SyncToken, std::__Cr::allocator<gpu::SyncToken>> const&) command_buffer_stub.cc:508
 gpu::CommandBufferStub::ExecuteDeferredRequest(gpu::mojom::DeferredCommandBufferRequestParams&) command_buffer_stub.cc:154
 gpu::GpuChannel::ExecuteDeferredRequest(mojo::StructPtr<gpu::mojom::DeferredRequestParams>) gpu_channel.cc:716
 void base::internal::FunctorTraits<void (gpu::GpuChannel::*)(mojo::StructPtr<gpu::mojom::DeferredRequestParams>), void>::Invoke<void (gpu::GpuChannel::*)(mojo::StructPtr<gpu::mojom::DeferredRequestParams>), base::WeakPtr<gpu::GpuChannel> const&, mojo::StructPtr<gpu::mojom::DeferredRequestParams>>(void (gpu::GpuChannel::*)(mojo::StructPtr<gpu::mojom::DeferredRequestParams>), base::WeakPtr<gpu::GpuChannel> const&, mojo::StructPtr<gpu::mojom::DeferredRequestParams>&&) bind_internal.h:746
 void base::internal::InvokeHelper<true, void, 0ul, 1ul>::MakeItSo<void (gpu::GpuChannel::*)(mojo::StructPtr<gpu::mojom::DeferredRequestParams>), std::__Cr::tuple<base::WeakPtr<gpu::GpuChannel>, mojo::StructPtr<gpu::mojom::DeferredRequestParams>>>(void (gpu::GpuChannel::*&&)(mojo::StructPtr<gpu::mojom::DeferredRequestParams>), std::__Cr::tuple<base::WeakPtr<gpu::GpuChannel>, mojo::StructPtr<gpu::mojom::DeferredRequestParams>>&&) bind_internal.h:953
 void base::internal::Invoker<base::internal::BindState<void (gpu::GpuChannel::*)(mojo::StructPtr<gpu::mojom::DeferredRequestParams>), base::WeakPtr<gpu::GpuChannel>, mojo::StructPtr<gpu::mojom::DeferredRequestParams>>, void ()>::RunImpl<void (gpu::GpuChannel::*)(mojo::StructPtr<gpu::mojom::DeferredRequestParams>), std::__Cr::tuple<base::WeakPtr<gpu::GpuChannel>, mojo::StructPtr<gpu::mojom::DeferredRequestParams>>, 0ul, 1ul>(void (gpu::GpuChannel::*&&)(mojo::StructPtr<gpu::mojom::DeferredRequestParams>), std::__Cr::tuple<base::WeakPtr<gpu::GpuChannel>, mojo::StructPtr<gpu::mojom::DeferredRequestParams>>&&, std::__Cr::integer_sequence<unsigned long, 0ul, 1ul>) bind_internal.h:1025
 base::internal::Invoker<base::internal::BindState<void (gpu::GpuChannel::*)(mojo::StructPtr<gpu::mojom::DeferredRequestParams>), base::WeakPtr<gpu::GpuChannel>, mojo::StructPtr<gpu::mojom::DeferredRequestParams>>, void ()>::RunOnce(base::internal::BindStateBase*) bind_internal.h:976
 base::OnceCallback<void ()>::Run() && callback.h:152
 gpu::SchedulerDfs::ExecuteSequence(base::IdType<gpu::SyncPointOrderData, unsigned int, 0u, 1u>) scheduler_dfs.cc:762
 gpu::SchedulerDfs::RunNextTask() scheduler_dfs.cc:673
 void base::internal::FunctorTraits<void (gpu::SchedulerDfs::*)(), void>::Invoke<void (gpu::SchedulerDfs::*)(), gpu::SchedulerDfs*>(void (gpu::SchedulerDfs::*)(), gpu::SchedulerDfs*&&) bind_internal.h:746
 void base::internal::InvokeHelper<false, void, 0ul>::MakeItSo<void (gpu::SchedulerDfs::*)(), std::__Cr::tuple<base::internal::UnretainedWrapper<gpu::SchedulerDfs, base::unretained_traits::MayNotDangle, (base::RawPtrTraits)0>>>(void (gpu::SchedulerDfs::*&&)(), std::__Cr::tuple<base::internal::UnretainedWrapper<gpu::SchedulerDfs, base::unretained_traits::MayNotDangle, (base::RawPtrTraits)0>>&&) bind_internal.h:925
 void base::internal::Invoker<base::internal::BindState<void (gpu::SchedulerDfs::*)(), base::internal::UnretainedWrapper<gpu::SchedulerDfs, base::unretained_traits::MayNotDangle, (base::RawPtrTraits)0>>, void ()>::RunImpl<void (gpu::SchedulerDfs::*)(), std::__Cr::tuple<base::internal::UnretainedWrapper<gpu::SchedulerDfs, base::unretained_traits::MayNotDangle, (base::RawPtrTraits)0>>, 0ul>(void (gpu::SchedulerDfs::*&&)(), std::__Cr::tuple<base::internal::UnretainedWrapper<gpu::SchedulerDfs, base::unretained_traits::MayNotDangle, (base::RawPtrTraits)0>>&&, std::__Cr::integer_sequence<unsigned long, 0ul>) bind_internal.h:1025
 base::internal::Invoker<base::internal::BindState<void (gpu::SchedulerDfs::*)(), base::internal::UnretainedWrapper<gpu::SchedulerDfs, base::unretained_traits::MayNotDangle, (base::RawPtrTraits)0>>, void ()>::RunOnce(base::internal::BindStateBase*) bind_internal.h:976
 base::OnceCallback<void ()>::Run() && callback.h:152
 base::TaskAnnotator::RunTaskImpl(base::PendingTask&) task_annotator.cc:186
 void base::TaskAnnotator::RunTask<base::sequence_manager::internal::ThreadControllerWithMessagePumpImpl::DoWorkImpl(base::LazyNow*)::$_0>(perfetto::StaticString, base::PendingTask&, base::sequence_manager::internal::ThreadControllerWithMessagePumpImpl::DoWorkImpl(base::LazyNow*)::$_0&&) task_annotator.h:88
 base::sequence_manager::internal::ThreadControllerWithMessagePumpImpl::DoWorkImpl(base::LazyNow*) thread_controller_with_message_pump_impl.cc:486
 base::sequence_manager::internal::ThreadControllerWithMessagePumpImpl::DoWork() thread_controller_with_message_pump_impl.cc:351
 base::MessagePumpCFRunLoopBase::RunWork() message_pump_mac.mm:449
 invocation function for block in base::MessagePumpCFRunLoopBase::RunWorkSource(void*) message_pump_mac.mm:422
 base::mac::CallWithEHFrame(void () block_pointer) call_with_eh_frame_asm.S:65
 base::MessagePumpCFRunLoopBase::RunWorkSource(void*) message_pump_mac.mm:421
 __CFRUNLOOP_IS_CALLING_OUT_TO_A_SOURCE0_PERFORM_FUNCTION__ <unknown>:8
 __CFRunLoopDoSource0 <unknown>:45
 __CFRunLoopDoSources0 <unknown>:62
 __CFRunLoopRun <unknown>:208
 CFRunLoopRunSpecific <unknown>:153
 -[NSRunLoop(NSRunLoop) runMode:beforeDate:] <unknown>:54
 base::MessagePumpNSRunLoop::DoRun(base::MessagePump::Delegate*) message_pump_mac.mm:720
 base::MessagePumpCFRunLoopBase::Run(base::MessagePump::Delegate*) message_pump_mac.mm:167
 base::sequence_manager::internal::ThreadControllerWithMessagePumpImpl::Run(bool, base::TimeDelta) thread_controller_with_message_pump_impl.cc:651
 base::RunLoop::Run(base::Location const&) run_loop.cc:134
 base::Thread::Run(base::RunLoop*) thread.cc:342
 base::Thread::ThreadMain() thread.cc:414
 base::(anonymous namespace)::ThreadFunc(void*) platform_thread_posix.cc:101
 _pthread_start <unknown>:35

Thread #26 Chrome_ChildIOThread:
 kevent64 <unknown>:3
 base::MessagePumpKqueue::DoInternalWork(base::MessagePump::Delegate*, base::MessagePump::Delegate::NextWorkInfo*) message_pump_kqueue.cc:427
 base::MessagePumpKqueue::RunSimplified(base::MessagePump::Delegate*) message_pump_kqueue.cc:211
 base::MessagePumpKqueue::Run(base::MessagePump::Delegate*) message_pump_kqueue.cc:163
 base::sequence_manager::internal::ThreadControllerWithMessagePumpImpl::Run(bool, base::TimeDelta) thread_controller_with_message_pump_impl.cc:651
 base::RunLoop::Run(base::Location const&) run_loop.cc:134
 base::Thread::Run(base::RunLoop*) thread.cc:342
 content::(anonymous namespace)::ChildIOThread::Run(base::RunLoop*) child_process.cc:57
 base::Thread::ThreadMain() thread.cc:414
 base::(anonymous namespace)::ThreadFunc(void*) platform_thread_posix.cc:101
 _pthread_start <unknown>:35

Thread #27 CacheThread_BlockFile:
 kevent64 <unknown>:3
 base::MessagePumpKqueue::DoInternalWork(base::MessagePump::Delegate*, base::MessagePump::Delegate::NextWorkInfo*) message_pump_kqueue.cc:427
 base::MessagePumpKqueue::RunSimplified(base::MessagePump::Delegate*) message_pump_kqueue.cc:211
 base::MessagePumpKqueue::Run(base::MessagePump::Delegate*) message_pump_kqueue.cc:163
 base::sequence_manager::internal::ThreadControllerWithMessagePumpImpl::Run(bool, base::TimeDelta) thread_controller_with_message_pump_impl.cc:651
 base::RunLoop::Run(base::Location const&) run_loop.cc:134
 base::Thread::Run(base::RunLoop*) thread.cc:342
 base::Thread::ThreadMain() thread.cc:414
 base::(anonymous namespace)::ThreadFunc(void*) platform_thread_posix.cc:101
 _pthread_start <unknown>:35

Thread #28 CompositorTileWorker1:
 __psynch_cvwait <unknown>:3
 _pthread_cond_wait <unknown>:308
 base::ConditionVariable::Wait() condition_variable_posix.cc:79
 cc::SingleThreadTaskGraphRunner::Run() single_thread_task_graph_runner.cc:132
 base::DelegateSimpleThread::Run() simple_thread.cc:101
 base::SimpleThread::ThreadMain() simple_thread.cc:78
 base::(anonymous namespace)::ThreadFunc(void*) platform_thread_posix.cc:101
 _pthread_start <unknown>:35

Thread #29 AudioThread:
 mach_msg2_trap <unknown>:3
 mach_msg2_internal <unknown>:21
 mach_msg_overwrite <unknown>:120
 mach_msg <unknown>:7
 base::WaitableEvent::TimedWaitImpl(base::TimeDelta) waitable_event_mac.cc:110
 base::WaitableEvent::TimedWait(base::TimeDelta) waitable_event.cc:39
 base::WaitableEvent::Wait() waitable_event.cc:23
 base::MessagePumpDefault::Run(base::MessagePump::Delegate*) message_pump_default.cc:56
 base::sequence_manager::internal::ThreadControllerWithMessagePumpImpl::Run(bool, base::TimeDelta) thread_controller_with_message_pump_impl.cc:651
 base::RunLoop::Run(base::Location const&) run_loop.cc:134
 base::Thread::Run(base::RunLoop*) thread.cc:342
 base::Thread::ThreadMain() thread.cc:414
 base::(anonymous namespace)::ThreadFunc(void*) platform_thread_posix.cc:101
 _pthread_start <unknown>:35

Thread #30 ThreadPoolSingleThreadSharedUtility0:
 mach_msg2_trap <unknown>:3
 mach_msg2_internal <unknown>:21
 mach_msg_overwrite <unknown>:120
 mach_msg <unknown>:7
 base::WaitableEvent::TimedWaitImpl(base::TimeDelta) waitable_event_mac.cc:110
 base::WaitableEvent::TimedWait(base::TimeDelta) waitable_event.cc:39
 base::internal::WorkerThread::Delegate::WaitForWork(base::WaitableEvent*) worker_thread.cc:132
 base::internal::WorkerThread::RunWorker() worker_thread.cc:435
 base::internal::WorkerThread::RunSharedWorker() worker_thread.cc:366
 base::internal::WorkerThread::ThreadMain() worker_thread.cc:339
 base::(anonymous namespace)::ThreadFunc(void*) platform_thread_posix.cc:101
 _pthread_start <unknown>:35

Thread #31 VizCompositorThread:
 mach_msg2_trap <unknown>:3
 mach_msg2_internal <unknown>:21
 mach_msg_overwrite <unknown>:120
 mach_msg <unknown>:7
 base::WaitableEvent::TimedWaitImpl(base::TimeDelta) waitable_event_mac.cc:110
 base::WaitableEvent::TimedWait(base::TimeDelta) waitable_event.cc:39
 base::MessagePumpDefault::Run(base::MessagePump::Delegate*) message_pump_default.cc:58
 base::sequence_manager::internal::ThreadControllerWithMessagePumpImpl::Run(bool, base::TimeDelta) thread_controller_with_message_pump_impl.cc:651
 base::RunLoop::Run(base::Location const&) run_loop.cc:134
 base::Thread::Run(base::RunLoop*) thread.cc:342
 base::Thread::ThreadMain() thread.cc:414
 base::(anonymous namespace)::ThreadFunc(void*) platform_thread_posix.cc:101
 _pthread_start <unknown>:35

Thread #32 NetworkService:
 kevent64 <unknown>:3
 base::MessagePumpKqueue::DoInternalWork(base::MessagePump::Delegate*, base::MessagePump::Delegate::NextWorkInfo*) message_pump_kqueue.cc:427
 base::MessagePumpKqueue::RunSimplified(base::MessagePump::Delegate*) message_pump_kqueue.cc:211
 base::MessagePumpKqueue::Run(base::MessagePump::Delegate*) message_pump_kqueue.cc:163
 base::sequence_manager::internal::ThreadControllerWithMessagePumpImpl::Run(bool, base::TimeDelta) thread_controller_with_message_pump_impl.cc:651
 base::RunLoop::Run(base::Location const&) run_loop.cc:134
 base::Thread::Run(base::RunLoop*) thread.cc:342
 base::Thread::ThreadMain() thread.cc:414
 base::(anonymous namespace)::ThreadFunc(void*) platform_thread_posix.cc:101
 _pthread_start <unknown>:35

Thread #33 ThreadPoolUtilityWorker:
 mach_msg2_trap <unknown>:3
 mach_msg2_internal <unknown>:21
 mach_msg_overwrite <unknown>:120
 mach_msg <unknown>:7
 base::WaitableEvent::TimedWaitImpl(base::TimeDelta) waitable_event_mac.cc:110
 base::WaitableEvent::TimedWait(base::TimeDelta) waitable_event.cc:39
 base::internal::WorkerThread::Delegate::WaitForWork(base::WaitableEvent*) worker_thread.cc:132
 base::internal::WorkerThread::RunWorker() worker_thread.cc:466
 base::internal::WorkerThread::RunPooledWorker() worker_thread.cc:356
 base::internal::WorkerThread::ThreadMain() worker_thread.cc:336
 base::(anonymous namespace)::ThreadFunc(void*) platform_thread_posix.cc:101
 _pthread_start <unknown>:35

Thread #36 ThreadPoolUtilityWorker:
 mach_msg2_trap <unknown>:3
 mach_msg2_internal <unknown>:21
 mach_msg_overwrite <unknown>:120
 mach_msg <unknown>:7
 base::WaitableEvent::TimedWaitImpl(base::TimeDelta) waitable_event_mac.cc:110
 base::WaitableEvent::TimedWait(base::TimeDelta) waitable_event.cc:39
 base::internal::WorkerThread::Delegate::WaitForWork(base::WaitableEvent*) worker_thread.cc:132
 base::internal::WorkerThread::RunWorker() worker_thread.cc:466
 base::internal::WorkerThread::RunPooledWorker() worker_thread.cc:356
 base::internal::WorkerThread::ThreadMain() worker_thread.cc:336
 base::(anonymous namespace)::ThreadFunc(void*) platform_thread_posix.cc:101
 _pthread_start <unknown>:35

Thread #37 NetworkConfigWatcher:
 mach_msg2_trap <unknown>:3
 mach_msg2_internal <unknown>:21
 mach_msg_overwrite <unknown>:120
 mach_msg <unknown>:7
 __CFRunLoopServiceMachPort <unknown>:41
 __CFRunLoopRun <unknown>:303
 CFRunLoopRunSpecific <unknown>:153
 -[NSRunLoop(NSRunLoop) runMode:beforeDate:] <unknown>:54
 base::MessagePumpNSRunLoop::DoRun(base::MessagePump::Delegate*) message_pump_mac.mm:720
 base::MessagePumpCFRunLoopBase::Run(base::MessagePump::Delegate*) message_pump_mac.mm:167
 base::sequence_manager::internal::ThreadControllerWithMessagePumpImpl::Run(bool, base::TimeDelta) thread_controller_with_message_pump_impl.cc:651
 base::RunLoop::Run(base::Location const&) run_loop.cc:134
 base::Thread::Run(base::RunLoop*) thread.cc:342
 base::Thread::ThreadMain() thread.cc:414
 base::(anonymous namespace)::ThreadFunc(void*) platform_thread_posix.cc:101
 _pthread_start <unknown>:35

Thread #38 ThreadPoolSingleThreadSharedForeground1:
 mach_msg2_trap <unknown>:3
 mach_msg2_internal <unknown>:21
 mach_msg_overwrite <unknown>:120
 mach_msg <unknown>:7
 base::WaitableEvent::TimedWaitImpl(base::TimeDelta) waitable_event_mac.cc:110
 base::WaitableEvent::TimedWait(base::TimeDelta) waitable_event.cc:39
 base::internal::WorkerThread::Delegate::WaitForWork(base::WaitableEvent*) worker_thread.cc:132
 base::internal::WorkerThread::RunWorker() worker_thread.cc:466
 base::internal::WorkerThread::RunSharedWorker() worker_thread.cc:366
 base::internal::WorkerThread::ThreadMain() worker_thread.cc:339
 base::(anonymous namespace)::ThreadFunc(void*) platform_thread_posix.cc:101
 _pthread_start <unknown>:35

Thread #39 ThreadPoolSingleThreadSharedBackgroundBlocking2:
 mach_msg2_trap <unknown>:3
 mach_msg2_internal <unknown>:21
 mach_msg_overwrite <unknown>:120
 mach_msg <unknown>:7
 base::WaitableEvent::TimedWaitImpl(base::TimeDelta) waitable_event_mac.cc:110
 base::WaitableEvent::TimedWait(base::TimeDelta) waitable_event.cc:39
 base::internal::WorkerThread::Delegate::WaitForWork(base::WaitableEvent*) worker_thread.cc:132
 base::internal::WorkerThread::RunWorker() worker_thread.cc:466
 base::internal::WorkerThread::RunBackgroundSharedWorker() worker_thread.cc:371
 base::internal::WorkerThread::ThreadMain() worker_thread.cc:318
 base::(anonymous namespace)::ThreadFunc(void*) platform_thread_posix.cc:101
 _pthread_start <unknown>:35

Thread #40 NetworkConfigWatcher:
 mach_msg2_trap <unknown>:3
 mach_msg2_internal <unknown>:21
 mach_msg_overwrite <unknown>:120
 mach_msg <unknown>:7
 __CFRunLoopServiceMachPort <unknown>:41
 __CFRunLoopRun <unknown>:303
 CFRunLoopRunSpecific <unknown>:153
 -[NSRunLoop(NSRunLoop) runMode:beforeDate:] <unknown>:54
 base::MessagePumpNSRunLoop::DoRun(base::MessagePump::Delegate*) message_pump_mac.mm:720
 base::MessagePumpCFRunLoopBase::Run(base::MessagePump::Delegate*) message_pump_mac.mm:167
 base::sequence_manager::internal::ThreadControllerWithMessagePumpImpl::Run(bool, base::TimeDelta) thread_controller_with_message_pump_impl.cc:651
 base::RunLoop::Run(base::Location const&) run_loop.cc:134
 base::Thread::Run(base::RunLoop*) thread.cc:342
 base::Thread::ThreadMain() thread.cc:414
 base::(anonymous namespace)::ThreadFunc(void*) platform_thread_posix.cc:101
 _pthread_start <unknown>:35

Thread #41 ThreadPoolSingleThreadSharedUtilityBlocking3:
 mach_msg2_trap <unknown>:3
 mach_msg2_internal <unknown>:21
 mach_msg_overwrite <unknown>:120
 mach_msg <unknown>:7
 base::WaitableEvent::TimedWaitImpl(base::TimeDelta) waitable_event_mac.cc:110
 base::WaitableEvent::TimedWait(base::TimeDelta) waitable_event.cc:39
 base::internal::WorkerThread::Delegate::WaitForWork(base::WaitableEvent*) worker_thread.cc:132
 base::internal::WorkerThread::RunWorker() worker_thread.cc:466
 base::internal::WorkerThread::RunSharedWorker() worker_thread.cc:366
 base::internal::WorkerThread::ThreadMain() worker_thread.cc:339
 base::(anonymous namespace)::ThreadFunc(void*) platform_thread_posix.cc:101
 _pthread_start <unknown>:35

Thread #42 Service Discovery Thread:
 mach_msg2_trap <unknown>:3
 mach_msg2_internal <unknown>:21
 mach_msg_overwrite <unknown>:120
 mach_msg <unknown>:7
 __CFRunLoopServiceMachPort <unknown>:41
 __CFRunLoopRun <unknown>:303
 CFRunLoopRunSpecific <unknown>:153
 -[NSRunLoop(NSRunLoop) runMode:beforeDate:] <unknown>:54
 base::MessagePumpNSRunLoop::DoRun(base::MessagePump::Delegate*) message_pump_mac.mm:720
 base::MessagePumpCFRunLoopBase::Run(base::MessagePump::Delegate*) message_pump_mac.mm:167
 base::sequence_manager::internal::ThreadControllerWithMessagePumpImpl::Run(bool, base::TimeDelta) thread_controller_with_message_pump_impl.cc:651
 base::RunLoop::Run(base::Location const&) run_loop.cc:134
 base::Thread::Run(base::RunLoop*) thread.cc:342
 base::Thread::ThreadMain() thread.cc:414
 base::(anonymous namespace)::ThreadFunc(void*) platform_thread_posix.cc:101
 _pthread_start <unknown>:35

Thread #43 com.apple.CFSocket.private:
 __select <unknown>:3
 __CFSocketManager <unknown>:160
 _pthread_start <unknown>:35

Thread #44 Chrome_InProcRendererThread:
 blink::ExecutionContext::GetSecurityOrigin() const execution_context.cc:394
 blink::(anonymous namespace)::CanAccessWindowInternal(blink::LocalDOMWindow const*, blink::DOMWindow const*, blink::DOMWindow::CrossDocumentAccessPolicy*) binding_security.cc:120
 blink::(anonymous namespace)::CanAccessWindow(blink::LocalDOMWindow const*, blink::DOMWindow const*, blink::ExceptionState*) binding_security.cc:194
 blink::BindingSecurity::ShouldAllowAccessTo(blink::LocalDOMWindow const*, blink::DOMWindow const*) binding_security.cc:224
 blink::V8Window::NamedPropertyGetterCustom(WTF::AtomicString const&, v8::PropertyCallbackInfo<v8::Value> const&) v8_window_custom.cc:262
 blink::(anonymous namespace)::v8_window::NamedPropsObjNamedGetterCallback(v8::Local<v8::Name>, v8::PropertyCallbackInfo<v8::Value> const&) v8_window.cc:16421
 v8::internal::PropertyCallbackArguments::CallNamedGetter(v8::internal::Handle<v8::internal::InterceptorInfo>, v8::internal::Handle<v8::internal::Name>) [opt] api-arguments-inl.h:145
 v8::internal::(anonymous namespace)::GetPropertyAttributesWithInterceptorInternal(v8::internal::LookupIterator*, v8::internal::Handle<v8::internal::InterceptorInfo>) [opt] js-objects.cc:1298
 v8::internal::JSObject::GetPropertyAttributesWithInterceptor(v8::internal::LookupIterator*) [opt] js-objects.cc:3798
 v8::internal::JSReceiver::HasProperty(v8::internal::LookupIterator*) [opt] js-objects.cc:111
 v8::internal::LoadIC::Load(v8::internal::Handle<v8::internal::Object>, v8::internal::Handle<v8::internal::Name>, bool, v8::internal::Handle<v8::internal::Object>) [opt] ic.cc:465
 v8::internal::KeyedLoadIC::Load(v8::internal::Handle<v8::internal::Object>, v8::internal::Handle<v8::internal::Object>) [opt] ic.cc:1500
 v8::internal::__RT_impl_Runtime_KeyedHasIC_Miss(v8::internal::Arguments<(v8::internal::ArgumentsType)0>, v8::internal::Isolate*) [opt] ic.cc:3424
 v8::internal::Runtime_KeyedHasIC_Miss(int, unsigned long*, v8::internal::Isolate*) [opt] ic.cc:3407
 <unknown> <unknown>:0
 <unknown> <unknown>:0
 <unknown> <unknown>:0
 <unknown> <unknown>:0
 <unknown> <unknown>:0
 v8::internal::GeneratedCode<unsigned long, unsigned long, unsigned long, unsigned long, unsigned long, long, unsigned long**>::Call(unsigned long, unsigned long, unsigned long, unsigned long, long, unsigned long**) [opt] simulator.h:154
 v8::internal::(anonymous namespace)::Invoke(v8::internal::Isolate*, v8::internal::(anonymous namespace)::InvokeParams const&) [opt] execution.cc:427
 v8::internal::Execution::Call(v8::internal::Isolate*, v8::internal::Handle<v8::internal::Object>, v8::internal::Handle<v8::internal::Object>, int, v8::internal::Handle<v8::internal::Object>*) [opt] execution.cc:529
 v8::Function::Call(v8::Local<v8::Context>, v8::Local<v8::Value>, int, v8::Local<v8::Value>*) [opt] api.cc:5595
 blink::V8ScriptRunner::CallFunction(v8::Local<v8::Function>, blink::ExecutionContext*, v8::Local<v8::Value>, int, v8::Local<v8::Value>*, v8::Isolate*) v8_script_runner.cc:752
 blink::bindings::CallbackInvokeHelper<blink::CallbackFunctionWithTaskAttributionBase, (blink::bindings::CallbackInvokeHelperMode)0, (blink::bindings::CallbackReturnTypeIsPromise)0>::CallInternal(int, v8::Local<v8::Value>*) callback_invoke_helper.cc:152
 blink::bindings::CallbackInvokeHelper<blink::CallbackFunctionWithTaskAttributionBase, (blink::bindings::CallbackInvokeHelperMode)0, (blink::bindings::CallbackReturnTypeIsPromise)0>::Call(int, v8::Local<v8::Value>*) callback_invoke_helper.cc:173
 blink::V8Function::Invoke(blink::bindings::V8ValueOrScriptWrappableAdapter, blink::HeapVector<blink::ScriptValue, 0u> const&) v8_function.cc:67
 blink::V8Function::InvokeAndReportException(blink::bindings::V8ValueOrScriptWrappableAdapter, blink::HeapVector<blink::ScriptValue, 0u> const&) v8_function.cc:122
 blink::ScheduledAction::Execute(blink::ExecutionContext*) scheduled_action.cc:147
 blink::DOMTimer::Fired() dom_timer.cc:399
 blink::TimerBase::RunInternal() timer.cc:160
 void base::internal::FunctorTraits<void (blink::TimerBase::*)(), void>::Invoke<void (blink::TimerBase::*)(), blink::TimerBase*>(void (blink::TimerBase::*)(), blink::TimerBase*&&) bind_internal.h:746
 void base::internal::InvokeHelper<false, void, 0ul>::MakeItSo<void (blink::TimerBase::*)(), std::__Cr::tuple<WTF::UnretainedWrapper<blink::TimerBase>>>(void (blink::TimerBase::*&&)(), std::__Cr::tuple<WTF::UnretainedWrapper<blink::TimerBase>>&&) bind_internal.h:925
 void base::internal::Invoker<base::internal::BindState<void (blink::TimerBase::*)(), WTF::UnretainedWrapper<blink::TimerBase>>, void ()>::RunImpl<void (blink::TimerBase::*)(), std::__Cr::tuple<WTF::UnretainedWrapper<blink::TimerBase>>, 0ul>(void (blink::TimerBase::*&&)(), std::__Cr::tuple<WTF::UnretainedWrapper<blink::TimerBase>>&&, std::__Cr::integer_sequence<unsigned long, 0ul>) bind_internal.h:1025
 base::internal::Invoker<base::internal::BindState<void (blink::TimerBase::*)(), WTF::UnretainedWrapper<blink::TimerBase>>, void ()>::RunOnce(base::internal::BindStateBase*) bind_internal.h:976
 base::OnceCallback<void ()>::Run() && callback.h:152
 WTF::ThreadCheckingCallbackWrapper<base::OnceCallback<void ()>, void ()>::RunInternal(base::OnceCallback<void ()>*) functional.h:241
 WTF::ThreadCheckingCallbackWrapper<base::OnceCallback<void ()>, void ()>::Run() functional.h:226
 void base::internal::FunctorTraits<void (WTF::ThreadCheckingCallbackWrapper<base::OnceCallback<void ()>, void ()>::*)(), void>::Invoke<void (WTF::ThreadCheckingCallbackWrapper<base::OnceCallback<void ()>, void ()>::*)(), std::__Cr::unique_ptr<WTF::ThreadCheckingCallbackWrapper<base::OnceCallback<void ()>, void ()>, std::__Cr::default_delete<WTF::ThreadCheckingCallbackWrapper<base::OnceCallback<void ()>, void ()>>>>(void (WTF::ThreadCheckingCallbackWrapper<base::OnceCallback<void ()>, void ()>::*)(), std::__Cr::unique_ptr<WTF::ThreadCheckingCallbackWrapper<base::OnceCallback<void ()>, void ()>, std::__Cr::default_delete<WTF::ThreadCheckingCallbackWrapper<base::OnceCallback<void ()>, void ()>>>&&) bind_internal.h:746
 void base::internal::InvokeHelper<false, void, 0ul>::MakeItSo<void (WTF::ThreadCheckingCallbackWrapper<base::OnceCallback<void ()>, void ()>::*)(), std::__Cr::tuple<std::__Cr::unique_ptr<WTF::ThreadCheckingCallbackWrapper<base::OnceCallback<void ()>, void ()>, std::__Cr::default_delete<WTF::ThreadCheckingCallbackWrapper<base::OnceCallback<void ()>, void ()>>>>>(void (WTF::ThreadCheckingCallbackWrapper<base::OnceCallback<void ()>, void ()>::*&&)(), std::__Cr::tuple<std::__Cr::unique_ptr<WTF::ThreadCheckingCallbackWrapper<base::OnceCallback<void ()>, void ()>, std::__Cr::default_delete<WTF::ThreadCheckingCallbackWrapper<base::OnceCallback<void ()>, void ()>>>>&&) bind_internal.h:925
 void base::internal::Invoker<base::internal::BindState<void (WTF::ThreadCheckingCallbackWrapper<base::OnceCallback<void ()>, void ()>::*)(), std::__Cr::unique_ptr<WTF::ThreadCheckingCallbackWrapper<base::OnceCallback<void ()>, void ()>, std::__Cr::default_delete<WTF::ThreadCheckingCallbackWrapper<base::OnceCallback<void ()>, void ()>>>>, void ()>::RunImpl<void (WTF::ThreadCheckingCallbackWrapper<base::OnceCallback<void ()>, void ()>::*)(), std::__Cr::tuple<std::__Cr::unique_ptr<WTF::ThreadCheckingCallbackWrapper<base::OnceCallback<void ()>, void ()>, std::__Cr::default_delete<WTF::ThreadCheckingCallbackWrapper<base::OnceCallback<void ()>, void ()>>>>, 0ul>(void (WTF::ThreadCheckingCallbackWrapper<base::OnceCallback<void ()>, void ()>::*&&)(), std::__Cr::tuple<std::__Cr::unique_ptr<WTF::ThreadCheckingCallbackWrapper<base::OnceCallback<void ()>, void ()>, std::__Cr::default_delete<WTF::ThreadCheckingCallbackWrapper<base::OnceCallback<void ()>, void ()>>>>&&, std::__Cr::integer_sequence<unsigned long, 0ul>) bind_internal.h:1025
 base::internal::Invoker<base::internal::BindState<void (WTF::ThreadCheckingCallbackWrapper<base::OnceCallback<void ()>, void ()>::*)(), std::__Cr::unique_ptr<WTF::ThreadCheckingCallbackWrapper<base::OnceCallback<void ()>, void ()>, std::__Cr::default_delete<WTF::ThreadCheckingCallbackWrapper<base::OnceCallback<void ()>, void ()>>>>, void ()>::RunOnce(base::internal::BindStateBase*) bind_internal.h:976
 base::OnceCallback<void ()>::Run() && callback.h:152
 base::TaskAnnotator::RunTaskImpl(base::PendingTask&) task_annotator.cc:186
 void base::TaskAnnotator::RunTask<base::sequence_manager::internal::ThreadControllerImpl::DoWork(base::sequence_manager::internal::ThreadControllerImpl::WorkType)::$_0>(perfetto::StaticString, base::PendingTask&, base::sequence_manager::internal::ThreadControllerImpl::DoWork(base::sequence_manager::internal::ThreadControllerImpl::WorkType)::$_0&&) task_annotator.h:88
 base::sequence_manager::internal::ThreadControllerImpl::DoWork(base::sequence_manager::internal::ThreadControllerImpl::WorkType) thread_controller_impl.cc:213
 void base::internal::FunctorTraits<void (base::sequence_manager::internal::ThreadControllerImpl::*)(base::sequence_manager::internal::ThreadControllerImpl::WorkType), void>::Invoke<void (base::sequence_manager::internal::ThreadControllerImpl::*)(base::sequence_manager::internal::ThreadControllerImpl::WorkType), base::WeakPtr<base::sequence_manager::internal::ThreadControllerImpl> const&, base::sequence_manager::internal::ThreadControllerImpl::WorkType const&>(void (base::sequence_manager::internal::ThreadControllerImpl::*)(base::sequence_manager::internal::ThreadControllerImpl::WorkType), base::WeakPtr<base::sequence_manager::internal::ThreadControllerImpl> const&, base::sequence_manager::internal::ThreadControllerImpl::WorkType const&) bind_internal.h:746
 void base::internal::InvokeHelper<true, void, 0ul, 1ul>::MakeItSo<void (base::sequence_manager::internal::ThreadControllerImpl::* const&)(base::sequence_manager::internal::ThreadControllerImpl::WorkType), std::__Cr::tuple<base::WeakPtr<base::sequence_manager::internal::ThreadControllerImpl>, base::sequence_manager::internal::ThreadControllerImpl::WorkType> const&>(void (base::sequence_manager::internal::ThreadControllerImpl::* const&)(base::sequence_manager::internal::ThreadControllerImpl::WorkType), std::__Cr::tuple<base::WeakPtr<base::sequence_manager::internal::ThreadControllerImpl>, base::sequence_manager::internal::ThreadControllerImpl::WorkType> const&) bind_internal.h:953
 void base::internal::Invoker<base::internal::BindState<void (base::sequence_manager::internal::ThreadControllerImpl::*)(base::sequence_manager::internal::ThreadControllerImpl::WorkType), base::WeakPtr<base::sequence_manager::internal::ThreadControllerImpl>, base::sequence_manager::internal::ThreadControllerImpl::WorkType>, void ()>::RunImpl<void (base::sequence_manager::internal::ThreadControllerImpl::* const&)(base::sequence_manager::internal::ThreadControllerImpl::WorkType), std::__Cr::tuple<base::WeakPtr<base::sequence_manager::internal::ThreadControllerImpl>, base::sequence_manager::internal::ThreadControllerImpl::WorkType> const&, 0ul, 1ul>(void (base::sequence_manager::internal::ThreadControllerImpl::* const&)(base::sequence_manager::internal::ThreadControllerImpl::WorkType), std::__Cr::tuple<base::WeakPtr<base::sequence_manager::internal::ThreadControllerImpl>, base::sequence_manager::internal::ThreadControllerImpl::WorkType> const&, std::__Cr::integer_sequence<unsigned long, 0ul, 1ul>) bind_internal.h:1025
 base::internal::Invoker<base::internal::BindState<void (base::sequence_manager::internal::ThreadControllerImpl::*)(base::sequence_manager::internal::ThreadControllerImpl::WorkType), base::WeakPtr<base::sequence_manager::internal::ThreadControllerImpl>, base::sequence_manager::internal::ThreadControllerImpl::WorkType>, void ()>::Run(base::internal::BindStateBase*) bind_internal.h:989
 base::OnceCallback<void ()>::Run() && callback.h:152
 base::TaskAnnotator::RunTaskImpl(base::PendingTask&) task_annotator.cc:186
 void base::TaskAnnotator::RunTask<base::sequence_manager::internal::ThreadControllerWithMessagePumpImpl::DoWorkImpl(base::LazyNow*)::$_0>(perfetto::StaticString, base::PendingTask&, base::sequence_manager::internal::ThreadControllerWithMessagePumpImpl::DoWorkImpl(base::LazyNow*)::$_0&&) task_annotator.h:88
 base::sequence_manager::internal::ThreadControllerWithMessagePumpImpl::DoWorkImpl(base::LazyNow*) thread_controller_with_message_pump_impl.cc:486
 base::sequence_manager::internal::ThreadControllerWithMessagePumpImpl::DoWork() thread_controller_with_message_pump_impl.cc:351
 base::MessagePumpDefault::Run(base::MessagePump::Delegate*) message_pump_default.cc:40
 base::sequence_manager::internal::ThreadControllerWithMessagePumpImpl::Run(bool, base::TimeDelta) thread_controller_with_message_pump_impl.cc:651
 base::RunLoop::Run(base::Location const&) run_loop.cc:134
 base::Thread::Run(base::RunLoop*) thread.cc:342
 base::Thread::ThreadMain() thread.cc:414
 base::(anonymous namespace)::ThreadFunc(void*) platform_thread_posix.cc:101
 _pthread_start <unknown>:35

Thread #45 Chrome_ChildIOThread:
 kevent64 <unknown>:3
 base::MessagePumpKqueue::DoInternalWork(base::MessagePump::Delegate*, base::MessagePump::Delegate::NextWorkInfo*) message_pump_kqueue.cc:427
 base::MessagePumpKqueue::RunSimplified(base::MessagePump::Delegate*) message_pump_kqueue.cc:211
 base::MessagePumpKqueue::Run(base::MessagePump::Delegate*) message_pump_kqueue.cc:163
 base::sequence_manager::internal::ThreadControllerWithMessagePumpImpl::Run(bool, base::TimeDelta) thread_controller_with_message_pump_impl.cc:651
 base::RunLoop::Run(base::Location const&) run_loop.cc:134
 base::Thread::Run(base::RunLoop*) thread.cc:342
 content::(anonymous namespace)::ChildIOThread::Run(base::RunLoop*) child_process.cc:57
 base::Thread::ThreadMain() thread.cc:414
 base::(anonymous namespace)::ThreadFunc(void*) platform_thread_posix.cc:101
 _pthread_start <unknown>:35

Thread #46 GpuMemoryThread:
 mach_msg2_trap <unknown>:3
 mach_msg2_internal <unknown>:21
 mach_msg_overwrite <unknown>:120
 mach_msg <unknown>:7
 base::WaitableEvent::TimedWaitImpl(base::TimeDelta) waitable_event_mac.cc:110
 base::WaitableEvent::TimedWait(base::TimeDelta) waitable_event.cc:39
 base::WaitableEvent::Wait() waitable_event.cc:23
 base::MessagePumpDefault::Run(base::MessagePump::Delegate*) message_pump_default.cc:56
 base::sequence_manager::internal::ThreadControllerWithMessagePumpImpl::Run(bool, base::TimeDelta) thread_controller_with_message_pump_impl.cc:651
 base::RunLoop::Run(base::Location const&) run_loop.cc:134
 base::Thread::Run(base::RunLoop*) thread.cc:342
 base::Thread::ThreadMain() thread.cc:414
 base::(anonymous namespace)::ThreadFunc(void*) platform_thread_posix.cc:101
 _pthread_start <unknown>:35

Thread #47 Compositor:
 mach_msg2_trap <unknown>:3
 mach_msg2_internal <unknown>:21
 mach_msg_overwrite <unknown>:120
 mach_msg <unknown>:7
 base::WaitableEvent::TimedWaitImpl(base::TimeDelta) waitable_event_mac.cc:110
 base::WaitableEvent::TimedWait(base::TimeDelta) waitable_event.cc:39
 base::WaitableEvent::Wait() waitable_event.cc:23
 base::MessagePumpDefault::Run(base::MessagePump::Delegate*) message_pump_default.cc:56
 base::sequence_manager::internal::ThreadControllerWithMessagePumpImpl::Run(bool, base::TimeDelta) thread_controller_with_message_pump_impl.cc:651
 base::RunLoop::Run(base::Location const&) run_loop.cc:134
 blink::scheduler::NonMainThreadImpl::SimpleThreadImpl::Run() non_main_thread_impl.cc:182
 base::SimpleThread::ThreadMain() simple_thread.cc:78
 base::(anonymous namespace)::ThreadFunc(void*) platform_thread_posix.cc:101
 _pthread_start <unknown>:35

Thread #48 ThreadPoolSingleThreadSharedForegroundBlocking4:
 mach_msg2_trap <unknown>:3
 mach_msg2_internal <unknown>:21
 mach_msg_overwrite <unknown>:120
 mach_msg <unknown>:7
 base::WaitableEvent::TimedWaitImpl(base::TimeDelta) waitable_event_mac.cc:110
 base::WaitableEvent::TimedWait(base::TimeDelta) waitable_event.cc:39
 base::internal::WorkerThread::Delegate::WaitForWork(base::WaitableEvent*) worker_thread.cc:132
 base::internal::WorkerThread::RunWorker() worker_thread.cc:466
 base::internal::WorkerThread::RunSharedWorker() worker_thread.cc:366
 base::internal::WorkerThread::ThreadMain() worker_thread.cc:339
 base::(anonymous namespace)::ThreadFunc(void*) platform_thread_posix.cc:101
 _pthread_start <unknown>:35

Thread #49 Preload scanner:
 mach_msg2_trap <unknown>:3
 mach_msg2_internal <unknown>:21
 mach_msg_overwrite <unknown>:120
 mach_msg <unknown>:7
 base::WaitableEvent::TimedWaitImpl(base::TimeDelta) waitable_event_mac.cc:110
 base::WaitableEvent::TimedWait(base::TimeDelta) waitable_event.cc:39
 base::WaitableEvent::Wait() waitable_event.cc:23
 base::MessagePumpDefault::Run(base::MessagePump::Delegate*) message_pump_default.cc:56
 base::sequence_manager::internal::ThreadControllerWithMessagePumpImpl::Run(bool, base::TimeDelta) thread_controller_with_message_pump_impl.cc:651
 base::RunLoop::Run(base::Location const&) run_loop.cc:134
 blink::scheduler::NonMainThreadImpl::SimpleThreadImpl::Run() non_main_thread_impl.cc:182
 base::SimpleThread::ThreadMain() simple_thread.cc:78
 base::(anonymous namespace)::ThreadFunc(void*) platform_thread_posix.cc:101
 _pthread_start <unknown>:35

Thread #50 com.apple.NSEventThread:
 mach_msg2_trap <unknown>:3
 mach_msg2_internal <unknown>:21
 mach_msg_overwrite <unknown>:120
 mach_msg <unknown>:7
 __CFRunLoopServiceMachPort <unknown>:41
 __CFRunLoopRun <unknown>:303
 CFRunLoopRunSpecific <unknown>:153
 _NSEventThread <unknown>:37
 _pthread_start <unknown>:35

Thread #55 ThreadPoolBackgroundWorker:
 mach_msg2_trap <unknown>:3
 mach_msg2_internal <unknown>:21
 mach_msg_overwrite <unknown>:120
 mach_msg <unknown>:7
 base::WaitableEvent::TimedWaitImpl(base::TimeDelta) waitable_event_mac.cc:110
 base::WaitableEvent::TimedWait(base::TimeDelta) waitable_event.cc:39
 base::internal::WorkerThread::Delegate::WaitForWork(base::WaitableEvent*) worker_thread.cc:132
 base::internal::WorkerThread::RunWorker() worker_thread.cc:466
 base::internal::WorkerThread::RunBackgroundPooledWorker() worker_thread.cc:361
 base::internal::WorkerThread::ThreadMain() worker_thread.cc:315
 base::(anonymous namespace)::ThreadFunc(void*) platform_thread_posix.cc:101
 _pthread_start <unknown>:35

Thread #56:
 __psynch_cvwait <unknown>:3
 _pthread_cond_wait <unknown>:308
 std::__Cr::__libcpp_condvar_wait(_opaque_pthread_cond_t*, _opaque_pthread_mutex_t*) __threading_support:337
 std::__Cr::condition_variable::wait(std::__Cr::unique_lock<std::__Cr::mutex>&) condition_variable.cpp:45
 void std::__Cr::condition_variable::wait<angle::AsyncWorkerPool::threadLoop()::$_0>(std::__Cr::unique_lock<std::__Cr::mutex>&, angle::AsyncWorkerPool::threadLoop()::$_0) condition_variable.h:147
 angle::AsyncWorkerPool::threadLoop() WorkerThread.cpp:193
 decltype(*std::declval<angle::AsyncWorkerPool*>().*std::declval<void (angle::AsyncWorkerPool::*)()>()()) std::__Cr::__invoke<void (angle::AsyncWorkerPool::*)(), angle::AsyncWorkerPool*, void>(void (angle::AsyncWorkerPool::*&&)(), angle::AsyncWorkerPool*&&) invoke.h:357
 void std::__Cr::__thread_execute<std::__Cr::unique_ptr<std::__Cr::__thread_struct, std::__Cr::default_delete<std::__Cr::__thread_struct>>, void (angle::AsyncWorkerPool::*)(), angle::AsyncWorkerPool*, 2ul>(std::__Cr::tuple<std::__Cr::unique_ptr<std::__Cr::__thread_struct, std::__Cr::default_delete<std::__Cr::__thread_struct>>, void (angle::AsyncWorkerPool::*)(), angle::AsyncWorkerPool*>&, std::__Cr::__tuple_indices<2ul>) thread:339
 void* std::__Cr::__thread_proxy<std::__Cr::tuple<std::__Cr::unique_ptr<std::__Cr::__thread_struct, std::__Cr::default_delete<std::__Cr::__thread_struct>>, void (angle::AsyncWorkerPool::*)(), angle::AsyncWorkerPool*>>(void*) thread:350
 _pthread_start <unknown>:35

Thread #57:
 __psynch_cvwait <unknown>:3
 _pthread_cond_wait <unknown>:308
 std::__Cr::__libcpp_condvar_wait(_opaque_pthread_cond_t*, _opaque_pthread_mutex_t*) __threading_support:337
 std::__Cr::condition_variable::wait(std::__Cr::unique_lock<std::__Cr::mutex>&) condition_variable.cpp:45
 void std::__Cr::condition_variable::wait<angle::AsyncWorkerPool::threadLoop()::$_0>(std::__Cr::unique_lock<std::__Cr::mutex>&, angle::AsyncWorkerPool::threadLoop()::$_0) condition_variable.h:147
 angle::AsyncWorkerPool::threadLoop() WorkerThread.cpp:193
 decltype(*std::declval<angle::AsyncWorkerPool*>().*std::declval<void (angle::AsyncWorkerPool::*)()>()()) std::__Cr::__invoke<void (angle::AsyncWorkerPool::*)(), angle::AsyncWorkerPool*, void>(void (angle::AsyncWorkerPool::*&&)(), angle::AsyncWorkerPool*&&) invoke.h:357
 void std::__Cr::__thread_execute<std::__Cr::unique_ptr<std::__Cr::__thread_struct, std::__Cr::default_delete<std::__Cr::__thread_struct>>, void (angle::AsyncWorkerPool::*)(), angle::AsyncWorkerPool*, 2ul>(std::__Cr::tuple<std::__Cr::unique_ptr<std::__Cr::__thread_struct, std::__Cr::default_delete<std::__Cr::__thread_struct>>, void (angle::AsyncWorkerPool::*)(), angle::AsyncWorkerPool*>&, std::__Cr::__tuple_indices<2ul>) thread:339
 void* std::__Cr::__thread_proxy<std::__Cr::tuple<std::__Cr::unique_ptr<std::__Cr::__thread_struct, std::__Cr::default_delete<std::__Cr::__thread_struct>>, void (angle::AsyncWorkerPool::*)(), angle::AsyncWorkerPool*>>(void*) thread:350
 _pthread_start <unknown>:35

Thread #58:
 __psynch_cvwait <unknown>:3
 _pthread_cond_wait <unknown>:308
 std::__Cr::__libcpp_condvar_wait(_opaque_pthread_cond_t*, _opaque_pthread_mutex_t*) __threading_support:337
 std::__Cr::condition_variable::wait(std::__Cr::unique_lock<std::__Cr::mutex>&) condition_variable.cpp:45
 void std::__Cr::condition_variable::wait<angle::AsyncWorkerPool::threadLoop()::$_0>(std::__Cr::unique_lock<std::__Cr::mutex>&, angle::AsyncWorkerPool::threadLoop()::$_0) condition_variable.h:147
 angle::AsyncWorkerPool::threadLoop() WorkerThread.cpp:193
 decltype(*std::declval<angle::AsyncWorkerPool*>().*std::declval<void (angle::AsyncWorkerPool::*)()>()()) std::__Cr::__invoke<void (angle::AsyncWorkerPool::*)(), angle::AsyncWorkerPool*, void>(void (angle::AsyncWorkerPool::*&&)(), angle::AsyncWorkerPool*&&) invoke.h:357
 void std::__Cr::__thread_execute<std::__Cr::unique_ptr<std::__Cr::__thread_struct, std::__Cr::default_delete<std::__Cr::__thread_struct>>, void (angle::AsyncWorkerPool::*)(), angle::AsyncWorkerPool*, 2ul>(std::__Cr::tuple<std::__Cr::unique_ptr<std::__Cr::__thread_struct, std::__Cr::default_delete<std::__Cr::__thread_struct>>, void (angle::AsyncWorkerPool::*)(), angle::AsyncWorkerPool*>&, std::__Cr::__tuple_indices<2ul>) thread:339
 void* std::__Cr::__thread_proxy<std::__Cr::tuple<std::__Cr::unique_ptr<std::__Cr::__thread_struct, std::__Cr::default_delete<std::__Cr::__thread_struct>>, void (angle::AsyncWorkerPool::*)(), angle::AsyncWorkerPool*>>(void*) thread:350
 _pthread_start <unknown>:35

Thread #59:
 __psynch_cvwait <unknown>:3
 _pthread_cond_wait <unknown>:308
 std::__Cr::__libcpp_condvar_wait(_opaque_pthread_cond_t*, _opaque_pthread_mutex_t*) __threading_support:337
 std::__Cr::condition_variable::wait(std::__Cr::unique_lock<std::__Cr::mutex>&) condition_variable.cpp:45
 void std::__Cr::condition_variable::wait<angle::AsyncWorkerPool::threadLoop()::$_0>(std::__Cr::unique_lock<std::__Cr::mutex>&, angle::AsyncWorkerPool::threadLoop()::$_0) condition_variable.h:147
 angle::AsyncWorkerPool::threadLoop() WorkerThread.cpp:193
 decltype(*std::declval<angle::AsyncWorkerPool*>().*std::declval<void (angle::AsyncWorkerPool::*)()>()()) std::__Cr::__invoke<void (angle::AsyncWorkerPool::*)(), angle::AsyncWorkerPool*, void>(void (angle::AsyncWorkerPool::*&&)(), angle::AsyncWorkerPool*&&) invoke.h:357
 void std::__Cr::__thread_execute<std::__Cr::unique_ptr<std::__Cr::__thread_struct, std::__Cr::default_delete<std::__Cr::__thread_struct>>, void (angle::AsyncWorkerPool::*)(), angle::AsyncWorkerPool*, 2ul>(std::__Cr::tuple<std::__Cr::unique_ptr<std::__Cr::__thread_struct, std::__Cr::default_delete<std::__Cr::__thread_struct>>, void (angle::AsyncWorkerPool::*)(), angle::AsyncWorkerPool*>&, std::__Cr::__tuple_indices<2ul>) thread:339
 void* std::__Cr::__thread_proxy<std::__Cr::tuple<std::__Cr::unique_ptr<std::__Cr::__thread_struct, std::__Cr::default_delete<std::__Cr::__thread_struct>>, void (angle::AsyncWorkerPool::*)(), angle::AsyncWorkerPool*>>(void*) thread:350
 _pthread_start <unknown>:35

Thread #60:
 __psynch_cvwait <unknown>:3
 _pthread_cond_wait <unknown>:308
 std::__Cr::__libcpp_condvar_wait(_opaque_pthread_cond_t*, _opaque_pthread_mutex_t*) __threading_support:337
 std::__Cr::condition_variable::wait(std::__Cr::unique_lock<std::__Cr::mutex>&) condition_variable.cpp:45
 void std::__Cr::condition_variable::wait<angle::AsyncWorkerPool::threadLoop()::$_0>(std::__Cr::unique_lock<std::__Cr::mutex>&, angle::AsyncWorkerPool::threadLoop()::$_0) condition_variable.h:147
 angle::AsyncWorkerPool::threadLoop() WorkerThread.cpp:193
 decltype(*std::declval<angle::AsyncWorkerPool*>().*std::declval<void (angle::AsyncWorkerPool::*)()>()()) std::__Cr::__invoke<void (angle::AsyncWorkerPool::*)(), angle::AsyncWorkerPool*, void>(void (angle::AsyncWorkerPool::*&&)(), angle::AsyncWorkerPool*&&) invoke.h:357
 void std::__Cr::__thread_execute<std::__Cr::unique_ptr<std::__Cr::__thread_struct, std::__Cr::default_delete<std::__Cr::__thread_struct>>, void (angle::AsyncWorkerPool::*)(), angle::AsyncWorkerPool*, 2ul>(std::__Cr::tuple<std::__Cr::unique_ptr<std::__Cr::__thread_struct, std::__Cr::default_delete<std::__Cr::__thread_struct>>, void (angle::AsyncWorkerPool::*)(), angle::AsyncWorkerPool*>&, std::__Cr::__tuple_indices<2ul>) thread:339
 void* std::__Cr::__thread_proxy<std::__Cr::tuple<std::__Cr::unique_ptr<std::__Cr::__thread_struct, std::__Cr::default_delete<std::__Cr::__thread_struct>>, void (angle::AsyncWorkerPool::*)(), angle::AsyncWorkerPool*>>(void*) thread:350
 _pthread_start <unknown>:35

Thread #61:
 __psynch_cvwait <unknown>:3
 _pthread_cond_wait <unknown>:308
 std::__Cr::__libcpp_condvar_wait(_opaque_pthread_cond_t*, _opaque_pthread_mutex_t*) __threading_support:337
 std::__Cr::condition_variable::wait(std::__Cr::unique_lock<std::__Cr::mutex>&) condition_variable.cpp:45
 void std::__Cr::condition_variable::wait<angle::AsyncWorkerPool::threadLoop()::$_0>(std::__Cr::unique_lock<std::__Cr::mutex>&, angle::AsyncWorkerPool::threadLoop()::$_0) condition_variable.h:147
 angle::AsyncWorkerPool::threadLoop() WorkerThread.cpp:193
 decltype(*std::declval<angle::AsyncWorkerPool*>().*std::declval<void (angle::AsyncWorkerPool::*)()>()()) std::__Cr::__invoke<void (angle::AsyncWorkerPool::*)(), angle::AsyncWorkerPool*, void>(void (angle::AsyncWorkerPool::*&&)(), angle::AsyncWorkerPool*&&) invoke.h:357
 void std::__Cr::__thread_execute<std::__Cr::unique_ptr<std::__Cr::__thread_struct, std::__Cr::default_delete<std::__Cr::__thread_struct>>, void (angle::AsyncWorkerPool::*)(), angle::AsyncWorkerPool*, 2ul>(std::__Cr::tuple<std::__Cr::unique_ptr<std::__Cr::__thread_struct, std::__Cr::default_delete<std::__Cr::__thread_struct>>, void (angle::AsyncWorkerPool::*)(), angle::AsyncWorkerPool*>&, std::__Cr::__tuple_indices<2ul>) thread:339
 void* std::__Cr::__thread_proxy<std::__Cr::tuple<std::__Cr::unique_ptr<std::__Cr::__thread_struct, std::__Cr::default_delete<std::__Cr::__thread_struct>>, void (angle::AsyncWorkerPool::*)(), angle::AsyncWorkerPool*>>(void*) thread:350
 _pthread_start <unknown>:35

Thread #62:
 __psynch_cvwait <unknown>:3
 _pthread_cond_wait <unknown>:308
 std::__Cr::__libcpp_condvar_wait(_opaque_pthread_cond_t*, _opaque_pthread_mutex_t*) __threading_support:337
 std::__Cr::condition_variable::wait(std::__Cr::unique_lock<std::__Cr::mutex>&) condition_variable.cpp:45
 void std::__Cr::condition_variable::wait<angle::AsyncWorkerPool::threadLoop()::$_0>(std::__Cr::unique_lock<std::__Cr::mutex>&, angle::AsyncWorkerPool::threadLoop()::$_0) condition_variable.h:147
 angle::AsyncWorkerPool::threadLoop() WorkerThread.cpp:193
 decltype(*std::declval<angle::AsyncWorkerPool*>().*std::declval<void (angle::AsyncWorkerPool::*)()>()()) std::__Cr::__invoke<void (angle::AsyncWorkerPool::*)(), angle::AsyncWorkerPool*, void>(void (angle::AsyncWorkerPool::*&&)(), angle::AsyncWorkerPool*&&) invoke.h:357
 void std::__Cr::__thread_execute<std::__Cr::unique_ptr<std::__Cr::__thread_struct, std::__Cr::default_delete<std::__Cr::__thread_struct>>, void (angle::AsyncWorkerPool::*)(), angle::AsyncWorkerPool*, 2ul>(std::__Cr::tuple<std::__Cr::unique_ptr<std::__Cr::__thread_struct, std::__Cr::default_delete<std::__Cr::__thread_struct>>, void (angle::AsyncWorkerPool::*)(), angle::AsyncWorkerPool*>&, std::__Cr::__tuple_indices<2ul>) thread:339
 void* std::__Cr::__thread_proxy<std::__Cr::tuple<std::__Cr::unique_ptr<std::__Cr::__thread_struct, std::__Cr::default_delete<std::__Cr::__thread_struct>>, void (angle::AsyncWorkerPool::*)(), angle::AsyncWorkerPool*>>(void*) thread:350
 _pthread_start <unknown>:35

Thread #63:
 __psynch_cvwait <unknown>:3
 _pthread_cond_wait <unknown>:308
 std::__Cr::__libcpp_condvar_wait(_opaque_pthread_cond_t*, _opaque_pthread_mutex_t*) __threading_support:337
 std::__Cr::condition_variable::wait(std::__Cr::unique_lock<std::__Cr::mutex>&) condition_variable.cpp:45
 void std::__Cr::condition_variable::wait<angle::AsyncWorkerPool::threadLoop()::$_0>(std::__Cr::unique_lock<std::__Cr::mutex>&, angle::AsyncWorkerPool::threadLoop()::$_0) condition_variable.h:147
 angle::AsyncWorkerPool::threadLoop() WorkerThread.cpp:193
 decltype(*std::declval<angle::AsyncWorkerPool*>().*std::declval<void (angle::AsyncWorkerPool::*)()>()()) std::__Cr::__invoke<void (angle::AsyncWorkerPool::*)(), angle::AsyncWorkerPool*, void>(void (angle::AsyncWorkerPool::*&&)(), angle::AsyncWorkerPool*&&) invoke.h:357
 void std::__Cr::__thread_execute<std::__Cr::unique_ptr<std::__Cr::__thread_struct, std::__Cr::default_delete<std::__Cr::__thread_struct>>, void (angle::AsyncWorkerPool::*)(), angle::AsyncWorkerPool*, 2ul>(std::__Cr::tuple<std::__Cr::unique_ptr<std::__Cr::__thread_struct, std::__Cr::default_delete<std::__Cr::__thread_struct>>, void (angle::AsyncWorkerPool::*)(), angle::AsyncWorkerPool*>&, std::__Cr::__tuple_indices<2ul>) thread:339
 void* std::__Cr::__thread_proxy<std::__Cr::tuple<std::__Cr::unique_ptr<std::__Cr::__thread_struct, std::__Cr::default_delete<std::__Cr::__thread_struct>>, void (angle::AsyncWorkerPool::*)(), angle::AsyncWorkerPool*>>(void*) thread:350
 _pthread_start <unknown>:35

Thread #64:
 __psynch_cvwait <unknown>:3
 _pthread_cond_wait <unknown>:308
 std::__Cr::__libcpp_condvar_wait(_opaque_pthread_cond_t*, _opaque_pthread_mutex_t*) __threading_support:337
 std::__Cr::condition_variable::wait(std::__Cr::unique_lock<std::__Cr::mutex>&) condition_variable.cpp:45
 void std::__Cr::condition_variable::wait<angle::AsyncWorkerPool::threadLoop()::$_0>(std::__Cr::unique_lock<std::__Cr::mutex>&, angle::AsyncWorkerPool::threadLoop()::$_0) condition_variable.h:147
 angle::AsyncWorkerPool::threadLoop() WorkerThread.cpp:193
 decltype(*std::declval<angle::AsyncWorkerPool*>().*std::declval<void (angle::AsyncWorkerPool::*)()>()()) std::__Cr::__invoke<void (angle::AsyncWorkerPool::*)(), angle::AsyncWorkerPool*, void>(void (angle::AsyncWorkerPool::*&&)(), angle::AsyncWorkerPool*&&) invoke.h:357
 void std::__Cr::__thread_execute<std::__Cr::unique_ptr<std::__Cr::__thread_struct, std::__Cr::default_delete<std::__Cr::__thread_struct>>, void (angle::AsyncWorkerPool::*)(), angle::AsyncWorkerPool*, 2ul>(std::__Cr::tuple<std::__Cr::unique_ptr<std::__Cr::__thread_struct, std::__Cr::default_delete<std::__Cr::__thread_struct>>, void (angle::AsyncWorkerPool::*)(), angle::AsyncWorkerPool*>&, std::__Cr::__tuple_indices<2ul>) thread:339
 void* std::__Cr::__thread_proxy<std::__Cr::tuple<std::__Cr::unique_ptr<std::__Cr::__thread_struct, std::__Cr::default_delete<std::__Cr::__thread_struct>>, void (angle::AsyncWorkerPool::*)(), angle::AsyncWorkerPool*>>(void*) thread:350
 _pthread_start <unknown>:35

Thread #65:
 __psynch_cvwait <unknown>:3
 _pthread_cond_wait <unknown>:308
 std::__Cr::__libcpp_condvar_wait(_opaque_pthread_cond_t*, _opaque_pthread_mutex_t*) __threading_support:337
 std::__Cr::condition_variable::wait(std::__Cr::unique_lock<std::__Cr::mutex>&) condition_variable.cpp:45
 void std::__Cr::condition_variable::wait<angle::AsyncWorkerPool::threadLoop()::$_0>(std::__Cr::unique_lock<std::__Cr::mutex>&, angle::AsyncWorkerPool::threadLoop()::$_0) condition_variable.h:147
 angle::AsyncWorkerPool::threadLoop() WorkerThread.cpp:193
 decltype(*std::declval<angle::AsyncWorkerPool*>().*std::declval<void (angle::AsyncWorkerPool::*)()>()()) std::__Cr::__invoke<void (angle::AsyncWorkerPool::*)(), angle::AsyncWorkerPool*, void>(void (angle::AsyncWorkerPool::*&&)(), angle::AsyncWorkerPool*&&) invoke.h:357
 void std::__Cr::__thread_execute<std::__Cr::unique_ptr<std::__Cr::__thread_struct, std::__Cr::default_delete<std::__Cr::__thread_struct>>, void (angle::AsyncWorkerPool::*)(), angle::AsyncWorkerPool*, 2ul>(std::__Cr::tuple<std::__Cr::unique_ptr<std::__Cr::__thread_struct, std::__Cr::default_delete<std::__Cr::__thread_struct>>, void (angle::AsyncWorkerPool::*)(), angle::AsyncWorkerPool*>&, std::__Cr::__tuple_indices<2ul>) thread:339
 void* std::__Cr::__thread_proxy<std::__Cr::tuple<std::__Cr::unique_ptr<std::__Cr::__thread_struct, std::__Cr::default_delete<std::__Cr::__thread_struct>>, void (angle::AsyncWorkerPool::*)(), angle::AsyncWorkerPool*>>(void*) thread:350
 _pthread_start <unknown>:35

Thread #66:
 __psynch_cvwait <unknown>:3
 _pthread_cond_wait <unknown>:308
 std::__Cr::__libcpp_condvar_wait(_opaque_pthread_cond_t*, _opaque_pthread_mutex_t*) __threading_support:337
 std::__Cr::condition_variable::wait(std::__Cr::unique_lock<std::__Cr::mutex>&) condition_variable.cpp:45
 void std::__Cr::condition_variable::wait<angle::AsyncWorkerPool::threadLoop()::$_0>(std::__Cr::unique_lock<std::__Cr::mutex>&, angle::AsyncWorkerPool::threadLoop()::$_0) condition_variable.h:147
 angle::AsyncWorkerPool::threadLoop() WorkerThread.cpp:193
 decltype(*std::declval<angle::AsyncWorkerPool*>().*std::declval<void (angle::AsyncWorkerPool::*)()>()()) std::__Cr::__invoke<void (angle::AsyncWorkerPool::*)(), angle::AsyncWorkerPool*, void>(void (angle::AsyncWorkerPool::*&&)(), angle::AsyncWorkerPool*&&) invoke.h:357
 void std::__Cr::__thread_execute<std::__Cr::unique_ptr<std::__Cr::__thread_struct, std::__Cr::default_delete<std::__Cr::__thread_struct>>, void (angle::AsyncWorkerPool::*)(), angle::AsyncWorkerPool*, 2ul>(std::__Cr::tuple<std::__Cr::unique_ptr<std::__Cr::__thread_struct, std::__Cr::default_delete<std::__Cr::__thread_struct>>, void (angle::AsyncWorkerPool::*)(), angle::AsyncWorkerPool*>&, std::__Cr::__tuple_indices<2ul>) thread:339
 void* std::__Cr::__thread_proxy<std::__Cr::tuple<std::__Cr::unique_ptr<std::__Cr::__thread_struct, std::__Cr::default_delete<std::__Cr::__thread_struct>>, void (angle::AsyncWorkerPool::*)(), angle::AsyncWorkerPool*>>(void*) thread:350
 _pthread_start <unknown>:35

Thread #67:
 __psynch_cvwait <unknown>:3
 _pthread_cond_wait <unknown>:308
 std::__Cr::__libcpp_condvar_wait(_opaque_pthread_cond_t*, _opaque_pthread_mutex_t*) __threading_support:337
 std::__Cr::condition_variable::wait(std::__Cr::unique_lock<std::__Cr::mutex>&) condition_variable.cpp:45
 void std::__Cr::condition_variable::wait<angle::AsyncWorkerPool::threadLoop()::$_0>(std::__Cr::unique_lock<std::__Cr::mutex>&, angle::AsyncWorkerPool::threadLoop()::$_0) condition_variable.h:147
 angle::AsyncWorkerPool::threadLoop() WorkerThread.cpp:193
 decltype(*std::declval<angle::AsyncWorkerPool*>().*std::declval<void (angle::AsyncWorkerPool::*)()>()()) std::__Cr::__invoke<void (angle::AsyncWorkerPool::*)(), angle::AsyncWorkerPool*, void>(void (angle::AsyncWorkerPool::*&&)(), angle::AsyncWorkerPool*&&) invoke.h:357
 void std::__Cr::__thread_execute<std::__Cr::unique_ptr<std::__Cr::__thread_struct, std::__Cr::default_delete<std::__Cr::__thread_struct>>, void (angle::AsyncWorkerPool::*)(), angle::AsyncWorkerPool*, 2ul>(std::__Cr::tuple<std::__Cr::unique_ptr<std::__Cr::__thread_struct, std::__Cr::default_delete<std::__Cr::__thread_struct>>, void (angle::AsyncWorkerPool::*)(), angle::AsyncWorkerPool*>&, std::__Cr::__tuple_indices<2ul>) thread:339
 void* std::__Cr::__thread_proxy<std::__Cr::tuple<std::__Cr::unique_ptr<std::__Cr::__thread_struct, std::__Cr::default_delete<std::__Cr::__thread_struct>>, void (angle::AsyncWorkerPool::*)(), angle::AsyncWorkerPool*>>(void*) thread:350
 _pthread_start <unknown>:35

Thread #68 org.libusb.device-hotplug:
 mach_msg2_trap <unknown>:3
 mach_msg2_internal <unknown>:21
 mach_msg_overwrite <unknown>:120
 mach_msg <unknown>:7
 __CFRunLoopServiceMachPort <unknown>:41
 __CFRunLoopRun <unknown>:303
 CFRunLoopRunSpecific <unknown>:153
 CFRunLoopRun <unknown>:17
 darwin_event_thread_main darwin_usb.c:383
 _pthread_start <unknown>:35

Thread #69 UsbEventHandler:
 poll <unknown>:3
 handle_events io.c:1963
 libusb_handle_events_timeout_completed io.c:2126
 libusb_handle_events io.c:2202
 device::UsbContext::UsbEventHandler::Run() usb_context.cc:55
 base::SimpleThread::ThreadMain() simple_thread.cc:78
 base::(anonymous namespace)::ThreadFunc(void*) platform_thread_posix.cc:101
 _pthread_start <unknown>:35

Thread #71:
 __workq_kernreturn <unknown>:3

Thread #77:
 __workq_kernreturn <unknown>:3

Thread #78 ThreadPoolForegroundWorker:
 mach_msg2_trap <unknown>:3
 mach_msg2_internal <unknown>:21
 mach_msg_overwrite <unknown>:120
 mach_msg <unknown>:7
 base::WaitableEvent::TimedWaitImpl(base::TimeDelta) waitable_event_mac.cc:110
 base::WaitableEvent::TimedWait(base::TimeDelta) waitable_event.cc:39
 base::internal::WorkerThread::Delegate::WaitForWork(base::WaitableEvent*) worker_thread.cc:132
 base::internal::WorkerThread::RunWorker() worker_thread.cc:466
 base::internal::WorkerThread::RunPooledWorker() worker_thread.cc:356
 base::internal::WorkerThread::ThreadMain() worker_thread.cc:336
 base::(anonymous namespace)::ThreadFunc(void*) platform_thread_posix.cc:101
 _pthread_start <unknown>:35

Thread #79 ThreadPoolForegroundWorker:
 mach_msg2_trap <unknown>:3
 mach_msg2_internal <unknown>:21
 mach_msg_overwrite <unknown>:120
 mach_msg <unknown>:7
 base::WaitableEvent::TimedWaitImpl(base::TimeDelta) waitable_event_mac.cc:110
 base::WaitableEvent::TimedWait(base::TimeDelta) waitable_event.cc:39
 base::internal::WorkerThread::Delegate::WaitForWork(base::WaitableEvent*) worker_thread.cc:132
 base::internal::WorkerThread::RunWorker() worker_thread.cc:466
 base::internal::WorkerThread::RunPooledWorker() worker_thread.cc:356
 base::internal::WorkerThread::ThreadMain() worker_thread.cc:336
 base::(anonymous namespace)::ThreadFunc(void*) platform_thread_posix.cc:101
 _pthread_start <unknown>:35

Thread #80 ThreadPoolForegroundWorker:
 mach_msg2_trap <unknown>:3
 mach_msg2_internal <unknown>:21
 mach_msg_overwrite <unknown>:120
 mach_msg <unknown>:7
 base::WaitableEvent::TimedWaitImpl(base::TimeDelta) waitable_event_mac.cc:110
 base::WaitableEvent::TimedWait(base::TimeDelta) waitable_event.cc:39
 base::internal::WorkerThread::Delegate::WaitForWork(base::WaitableEvent*) worker_thread.cc:132
 base::internal::WorkerThread::RunWorker() worker_thread.cc:466
 base::internal::WorkerThread::RunPooledWorker() worker_thread.cc:356
 base::internal::WorkerThread::ThreadMain() worker_thread.cc:336
 base::(anonymous namespace)::ThreadFunc(void*) platform_thread_posix.cc:101
 _pthread_start <unknown>:35

Thread #81 ThreadPoolForegroundWorker:
 mach_msg2_trap <unknown>:3
 mach_msg2_internal <unknown>:21
 mach_msg_overwrite <unknown>:120
 mach_msg <unknown>:7
 base::WaitableEvent::TimedWaitImpl(base::TimeDelta) waitable_event_mac.cc:110
 base::WaitableEvent::TimedWait(base::TimeDelta) waitable_event.cc:39
 base::internal::WorkerThread::Delegate::WaitForWork(base::WaitableEvent*) worker_thread.cc:132
 base::internal::WorkerThread::RunWorker() worker_thread.cc:466
 base::internal::WorkerThread::RunPooledWorker() worker_thread.cc:356
 base::internal::WorkerThread::ThreadMain() worker_thread.cc:336
 base::(anonymous namespace)::ThreadFunc(void*) platform_thread_posix.cc:101
 _pthread_start <unknown>:35

Thread #83:
 __workq_kernreturn <unknown>:3

Thread #85 SharedWorker thread:
 v8::internal::PreParser::DeclareFunctionNameVar(v8::internal::AstRawString const*, v8::internal::FunctionSyntaxKind, v8::internal::DeclarationScope*) [opt] preparser.h:1147
 v8::internal::PreParser::DeclareFunctionNameVar(v8::internal::PreParserIdentifier const&, v8::internal::FunctionSyntaxKind, v8::internal::DeclarationScope*) [opt] preparser.h:1153
 v8::internal::ParserBase<v8::internal::PreParser>::ParseFunctionBody(v8::internal::PreParserScopedStatementList*, v8::internal::PreParserIdentifier, int, v8::internal::PreParserFormalParameters const&, v8::internal::FunctionKind, v8::internal::FunctionSyntaxKind, v8::internal::ParserBase<v8::internal::PreParser>::FunctionBodyType) [opt] parser-base.h:4478
 v8::internal::PreParser::ParseFunctionLiteral(v8::internal::PreParserIdentifier, v8::internal::Scanner::Location, v8::internal::FunctionNameValidity, v8::internal::FunctionKind, int, v8::internal::FunctionSyntaxKind, v8::internal::LanguageMode, v8::internal::ZoneList<v8::internal::AstRawString const*>*) [opt] preparser.cc:322
 v8::internal::ParserBase<v8::internal::PreParser>::ParseHoistableDeclaration(int, v8::base::Flags<v8::internal::ParseFunctionFlag, int, int>, v8::internal::ZoneList<v8::internal::AstRawString const*>*, bool) [opt] parser-base.h:4240
 v8::internal::ParserBase<v8::internal::PreParser>::ParseStatementList(v8::internal::PreParserScopedStatementList*, v8::internal::Token::Value) [opt] parser-base.h:5215
 v8::internal::PreParser::ParseStatementListAndLogFunction(v8::internal::PreParserFormalParameters*) [opt] preparser.cc:368
 v8::internal::PreParser::PreParseFunction(v8::internal::AstRawString const*, v8::internal::FunctionKind, v8::internal::FunctionSyntaxKind, v8::internal::DeclarationScope*, int*, v8::internal::ProducedPreparseData**) [opt] preparser.cc:168
 v8::internal::Parser::SkipFunction(v8::internal::AstRawString const*, v8::internal::FunctionKind, v8::internal::FunctionSyntaxKind, v8::internal::DeclarationScope*, int*, int*, v8::internal::ProducedPreparseData**) [opt] parser.cc:2904
 v8::internal::Parser::ParseFunctionLiteral(v8::internal::AstRawString const*, v8::internal::Scanner::Location, v8::internal::FunctionNameValidity, v8::internal::FunctionKind, int, v8::internal::FunctionSyntaxKind, v8::internal::LanguageMode, v8::internal::ZoneList<v8::internal::AstRawString const*>*) [opt] parser.cc:2773
 v8::internal::ParserBase<v8::internal::Parser>::ParseAsyncFunctionLiteral() [opt] parser-base.h:4941
 v8::internal::ParserBase<v8::internal::Parser>::ParsePrimaryExpression() [opt] parser-base.h:1960
 v8::internal::ParserBase<v8::internal::Parser>::ParseMemberExpression() [opt] parser-base.h:3730
 v8::internal::ParserBase<v8::internal::Parser>::ParseLeftHandSideExpression() [opt] parser-base.h:3454
 v8::internal::ParserBase<v8::internal::Parser>::ParsePostfixExpression() [opt] parser-base.h:3421
 v8::internal::ParserBase<v8::internal::Parser>::ParseUnaryExpression() [opt] parser-base.h:3411
 v8::internal::ParserBase<v8::internal::Parser>::ParseAwaitExpression() [opt] parser-base.h:3373
 v8::internal::ParserBase<v8::internal::Parser>::ParseUnaryExpression() [opt] parser-base.h:3409
 v8::internal::ParserBase<v8::internal::Parser>::ParseBinaryExpression(int) [opt] parser-base.h:3293
 v8::internal::ParserBase<v8::internal::Parser>::ParseLogicalExpression() [opt] parser-base.h:3148
 v8::internal::ParserBase<v8::internal::Parser>::ParseConditionalExpression() [opt] parser-base.h:3133
 v8::internal::ParserBase<v8::internal::Parser>::ParseAssignmentExpressionCoverGrammar() [opt] parser-base.h:2948
 v8::internal::ParserBase<v8::internal::Parser>::ParseArguments(v8::internal::ScopedList<v8::internal::Expression*, void*>*, bool*, v8::internal::ParsingArrowHeadFlag) [opt] parser-base.h:2890
 v8::internal::ParserBase<v8::internal::Parser>::ParseLeftHandSideContinuation(v8::internal::Expression*) [opt] parser-base.h:3567
 v8::internal::ParserBase<v8::internal::Parser>::ParseLeftHandSideExpression() [opt] parser-base.h:3456
 v8::internal::ParserBase<v8::internal::Parser>::ParsePostfixExpression() [opt] parser-base.h:3421
 v8::internal::ParserBase<v8::internal::Parser>::ParseUnaryExpression() [opt] parser-base.h:3411
 v8::internal::ParserBase<v8::internal::Parser>::ParseBinaryExpression(int) [opt] parser-base.h:3293
 v8::internal::ParserBase<v8::internal::Parser>::ParseLogicalExpression() [opt] parser-base.h:3148
 v8::internal::ParserBase<v8::internal::Parser>::ParseConditionalExpression() [opt] parser-base.h:3133
 v8::internal::ParserBase<v8::internal::Parser>::ParseAssignmentExpressionCoverGrammar() [opt] parser-base.h:2948
 v8::internal::ParserBase<v8::internal::Parser>::ParseExpressionCoverGrammar() [opt] parser-base.h:2117
 v8::internal::ParserBase<v8::internal::Parser>::ParseExpressionOrLabelledStatement(v8::internal::ZoneList<v8::internal::AstRawString const*>*, v8::internal::ZoneList<v8::internal::AstRawString const*>*, v8::internal::AllowLabelledFunctionStatement) [opt] parser-base.h:5528
 v8::internal::ParserBase<v8::internal::Parser>::ParseStatementList(v8::internal::ScopedList<v8::internal::Statement*, void*>*, v8::internal::Token::Value) [opt] parser-base.h:5215
 v8::internal::ParserBase<v8::internal::Parser>::ParseAsyncFunctionBody(v8::internal::Scope*, v8::internal::ScopedList<v8::internal::Statement*, void*>*) [opt] parser-base.h:4896
 v8::internal::ParserBase<v8::internal::Parser>::ParseFunctionBody(v8::internal::ScopedList<v8::internal::Statement*, void*>*, v8::internal::AstRawString const*, int, v8::internal::ParserFormalParameters const&, v8::internal::FunctionKind, v8::internal::FunctionSyntaxKind, v8::internal::ParserBase<v8::internal::Parser>::FunctionBodyType) [opt] parser-base.h:4405
 v8::internal::ParserBase<v8::internal::Parser>::ParseArrowFunctionLiteral(v8::internal::ParserFormalParameters const&) [opt] parser-base.h:4660
 v8::internal::ParserBase<v8::internal::Parser>::ParseAssignmentExpressionCoverGrammar() [opt] parser-base.h:2977
 v8::internal::ParserBase<v8::internal::Parser>::ParseAssignmentExpression() [opt] parser-base.h:2094
 v8::internal::ParserBase<v8::internal::Parser>::ParseAssignmentExpressionCoverGrammar() [opt] parser-base.h:3024
 v8::internal::ParserBase<v8::internal::Parser>::ParseExpressionCoverGrammar() [opt] parser-base.h:2117
 v8::internal::ParserBase<v8::internal::Parser>::ParseExpressionOrLabelledStatement(v8::internal::ZoneList<v8::internal::AstRawString const*>*, v8::internal::ZoneList<v8::internal::AstRawString const*>*, v8::internal::AllowLabelledFunctionStatement) [opt] parser-base.h:5528
 v8::internal::ParserBase<v8::internal::Parser>::ParseStatementList(v8::internal::ScopedList<v8::internal::Statement*, void*>*, v8::internal::Token::Value) [opt] parser-base.h:5215
 v8::internal::ParserBase<v8::internal::Parser>::ParseFunctionBody(v8::internal::ScopedList<v8::internal::Statement*, void*>*, v8::internal::AstRawString const*, int, v8::internal::ParserFormalParameters const&, v8::internal::FunctionKind, v8::internal::FunctionSyntaxKind, v8::internal::ParserBase<v8::internal::Parser>::FunctionBodyType) [opt] parser-base.h:4407
 v8::internal::Parser::ParseFunction(v8::internal::ScopedList<v8::internal::Statement*, void*>*, v8::internal::AstRawString const*, int, v8::internal::FunctionKind, v8::internal::FunctionSyntaxKind, v8::internal::DeclarationScope*, int*, int*, bool*, int*, int*, v8::internal::ZoneList<v8::internal::AstRawString const*>*) [opt] parser.cc:3120
 v8::internal::Parser::ParseFunctionLiteral(v8::internal::AstRawString const*, v8::internal::Scanner::Location, v8::internal::FunctionNameValidity, v8::internal::FunctionKind, int, v8::internal::FunctionSyntaxKind, v8::internal::LanguageMode, v8::internal::ZoneList<v8::internal::AstRawString const*>*) [opt] parser.cc:2781
 v8::internal::ParserBase<v8::internal::Parser>::ParseFunctionExpression() [opt] parser-base.h:3703
 v8::internal::ParserBase<v8::internal::Parser>::ParsePrimaryExpression() [opt] parser-base.h:1960
 v8::internal::ParserBase<v8::internal::Parser>::ParseMemberExpression() [opt] parser-base.h:3730
 v8::internal::ParserBase<v8::internal::Parser>::ParseLeftHandSideExpression() [opt] parser-base.h:3454
 v8::internal::ParserBase<v8::internal::Parser>::ParsePostfixExpression() [opt] parser-base.h:3421
 v8::internal::ParserBase<v8::internal::Parser>::ParseUnaryExpression() [opt] parser-base.h:3411
 v8::internal::ParserBase<v8::internal::Parser>::ParseUnaryOrPrefixExpression() [opt] parser-base.h:3315
 v8::internal::ParserBase<v8::internal::Parser>::ParseUnaryExpression() [opt] parser-base.h:3407
 v8::internal::ParserBase<v8::internal::Parser>::ParseBinaryExpression(int) [opt] parser-base.h:3293
 v8::internal::ParserBase<v8::internal::Parser>::ParseLogicalExpression() [opt] parser-base.h:3148
 v8::internal::ParserBase<v8::internal::Parser>::ParseConditionalExpression() [opt] parser-base.h:3133
 v8::internal::ParserBase<v8::internal::Parser>::ParseAssignmentExpressionCoverGrammar() [opt] parser-base.h:2948
 v8::internal::ParserBase<v8::internal::Parser>::ParseExpressionCoverGrammar() [opt] parser-base.h:2117
 v8::internal::ParserBase<v8::internal::Parser>::ParseExpressionOrLabelledStatement(v8::internal::ZoneList<v8::internal::AstRawString const*>*, v8::internal::ZoneList<v8::internal::AstRawString const*>*, v8::internal::AllowLabelledFunctionStatement) [opt] parser-base.h:5528
 v8::internal::ParserBase<v8::internal::Parser>::ParseStatementList(v8::internal::ScopedList<v8::internal::Statement*, void*>*, v8::internal::Token::Value) [opt] parser-base.h:5215
 v8::internal::Parser::DoParseProgram(v8::internal::Isolate*, v8::internal::ParseInfo*) [opt] parser.cc:659
 v8::internal::Parser::ParseProgram(v8::internal::Isolate*, v8::internal::Handle<v8::internal::Script>, v8::internal::ParseInfo*, v8::internal::MaybeHandle<v8::internal::ScopeInfo>) [opt] parser.cc:566
 v8::internal::parsing::ParseProgram(v8::internal::ParseInfo*, v8::internal::Handle<v8::internal::Script>, v8::internal::MaybeHandle<v8::internal::ScopeInfo>, v8::internal::Isolate*, v8::internal::parsing::ReportStatisticsMode) [opt] parsing.cc:58
 v8::internal::(anonymous namespace)::CompileToplevel(v8::internal::ParseInfo*, v8::internal::Handle<v8::internal::Script>, v8::internal::MaybeHandle<v8::internal::ScopeInfo>, v8::internal::Isolate*, v8::internal::IsCompiledScope*) [opt] compiler.cc:1525
 v8::internal::Compiler::CompileToplevel(v8::internal::ParseInfo*, v8::internal::Handle<v8::internal::Script>, v8::internal::Isolate*, v8::internal::IsCompiledScope*) [opt] compiler.cc:2751
 v8::internal::(anonymous namespace)::CompileScriptOnMainThread(v8::internal::UnoptimizedCompileFlags, v8::internal::Handle<v8::internal::String>, v8::internal::ScriptDetails const&, v8::internal::NativesFlag, v8::Extension*, v8::internal::Isolate*, v8::internal::MaybeHandle<v8::internal::Script>, v8::internal::IsCompiledScope*, bool (*)(int, void*), void*) [opt] compiler.cc:3344
 v8::internal::(anonymous namespace)::GetSharedFunctionInfoForScriptImpl(v8::internal::Isolate*, v8::internal::Handle<v8::internal::String>, v8::internal::ScriptDetails const&, v8::Extension*, v8::internal::AlignedCachedData*, v8::internal::BackgroundDeserializeTask*, bool (*)(int, void*), void*, v8::ScriptCompiler::CompileOptions, v8::ScriptCompiler::NoCacheReason, v8::internal::NativesFlag) [opt] compiler.cc:3613
 v8::internal::Compiler::GetSharedFunctionInfoForScript(v8::internal::Isolate*, v8::internal::Handle<v8::internal::String>, v8::internal::ScriptDetails const&, v8::ScriptCompiler::CompileOptions, v8::ScriptCompiler::NoCacheReason, v8::internal::NativesFlag) [opt] compiler.cc:3644
 v8::ScriptCompiler::CompileUnboundInternal(v8::Isolate*, v8::ScriptCompiler::Source*, v8::ScriptCompiler::CompileOptions, v8::ScriptCompiler::NoCacheReason) [opt] api.cc:2764
 v8::ScriptCompiler::Compile(v8::Local<v8::Context>, v8::ScriptCompiler::Source*, v8::ScriptCompiler::CompileOptions, v8::ScriptCompiler::NoCacheReason) [opt] api.cc:2794
 blink::(anonymous namespace)::CompileScriptInternal(v8::Isolate*, blink::ScriptState*, blink::ClassicScript const&, v8::ScriptOrigin, v8::ScriptCompiler::CompileOptions, v8::ScriptCompiler::NoCacheReason, absl::optional<blink::inspector_compile_script_event::V8ConsumeCacheResult>*) v8_script_runner.cc:158
 blink::V8ScriptRunner::CompileScript(blink::ScriptState*, blink::ClassicScript const&, v8::ScriptOrigin, v8::ScriptCompiler::CompileOptions, v8::ScriptCompiler::NoCacheReason) v8_script_runner.cc:244
 blink::V8ScriptRunner::CompileAndRunScript(blink::ScriptState*, blink::ClassicScript*, blink::ExecuteScriptPolicy, blink::V8ScriptRunner::RethrowErrorsOption) v8_script_runner.cc:518
 blink::ClassicScript::RunScriptOnScriptStateAndReturnValue(blink::ScriptState*, blink::ExecuteScriptPolicy, blink::V8ScriptRunner::RethrowErrorsOption) classic_script.cc:219
 blink::WorkerGlobalScope::RunWorkerScript() worker_global_scope.cc:514
 blink::WorkerGlobalScope::WorkerScriptFetchFinished(blink::Script&, absl::optional<v8_inspector::V8StackTraceId>) worker_global_scope.cc:471
 blink::WorkerGlobalScope::EvaluateClassicScript(blink::KURL const&, WTF::String, std::__Cr::unique_ptr<WTF::Vector<unsigned char, 0u, WTF::PartitionAllocator>, std::__Cr::default_delete<WTF::Vector<unsigned char, 0u, WTF::PartitionAllocator>>>, v8_inspector::V8StackTraceId const&) worker_global_scope.cc:455
 blink::SharedWorkerGlobalScope::DidFetchClassicScript(blink::WorkerClassicScriptLoader*, v8_inspector::V8StackTraceId const&) shared_worker_global_scope.cc:256
 void base::internal::FunctorTraits<void (blink::SharedWorkerGlobalScope::*)(blink::WorkerClassicScriptLoader*, v8_inspector::V8StackTraceId const&), void>::Invoke<void (blink::SharedWorkerGlobalScope::*)(blink::WorkerClassicScriptLoader*, v8_inspector::V8StackTraceId const&), cppgc::internal::BasicPersistent<blink::SharedWorkerGlobalScope, cppgc::internal::WeakPersistentPolicy, cppgc::internal::IgnoreLocationPolicy, cppgc::internal::DisabledCheckingPolicy> const&, cppgc::internal::BasicPersistent<blink::WorkerClassicScriptLoader, cppgc::internal::StrongPersistentPolicy, cppgc::internal::IgnoreLocationPolicy, cppgc::internal::DisabledCheckingPolicy>, v8_inspector::V8StackTraceId>(void (blink::SharedWorkerGlobalScope::*)(blink::WorkerClassicScriptLoader*, v8_inspector::V8StackTraceId const&), cppgc::internal::BasicPersistent<blink::SharedWorkerGlobalScope, cppgc::internal::WeakPersistentPolicy, cppgc::internal::IgnoreLocationPolicy, cppgc::internal::DisabledCheckingPolicy> const&, cppgc::internal::BasicPersistent<blink::WorkerClassicScriptLoader, cppgc::internal::StrongPersistentPolicy, cppgc::internal::IgnoreLocationPolicy, cppgc::internal::DisabledCheckingPolicy>&&, v8_inspector::V8StackTraceId&&) bind_internal.h:746
 void base::internal::InvokeHelper<true, void, 0ul, 1ul, 2ul>::MakeItSo<void (blink::SharedWorkerGlobalScope::*)(blink::WorkerClassicScriptLoader*, v8_inspector::V8StackTraceId const&), std::__Cr::tuple<cppgc::internal::BasicPersistent<blink::SharedWorkerGlobalScope, cppgc::internal::WeakPersistentPolicy, cppgc::internal::IgnoreLocationPolicy, cppgc::internal::DisabledCheckingPolicy>, cppgc::internal::BasicPersistent<blink::WorkerClassicScriptLoader, cppgc::internal::StrongPersistentPolicy, cppgc::internal::IgnoreLocationPolicy, cppgc::internal::DisabledCheckingPolicy>, v8_inspector::V8StackTraceId>>(void (blink::SharedWorkerGlobalScope::*&&)(blink::WorkerClassicScriptLoader*, v8_inspector::V8StackTraceId const&), std::__Cr::tuple<cppgc::internal::BasicPersistent<blink::SharedWorkerGlobalScope, cppgc::internal::WeakPersistentPolicy, cppgc::internal::IgnoreLocationPolicy, cppgc::internal::DisabledCheckingPolicy>, cppgc::internal::BasicPersistent<blink::WorkerClassicScriptLoader, cppgc::internal::StrongPersistentPolicy, cppgc::internal::IgnoreLocationPolicy, cppgc::internal::DisabledCheckingPolicy>, v8_inspector::V8StackTraceId>&&) bind_internal.h:953
 void base::internal::Invoker<base::internal::BindState<void (blink::SharedWorkerGlobalScope::*)(blink::WorkerClassicScriptLoader*, v8_inspector::V8StackTraceId const&), cppgc::internal::BasicPersistent<blink::SharedWorkerGlobalScope, cppgc::internal::WeakPersistentPolicy, cppgc::internal::IgnoreLocationPolicy, cppgc::internal::DisabledCheckingPolicy>, cppgc::internal::BasicPersistent<blink::WorkerClassicScriptLoader, cppgc::internal::StrongPersistentPolicy, cppgc::internal::IgnoreLocationPolicy, cppgc::internal::DisabledCheckingPolicy>, v8_inspector::V8StackTraceId>, void ()>::RunImpl<void (blink::SharedWorkerGlobalScope::*)(blink::WorkerClassicScriptLoader*, v8_inspector::V8StackTraceId const&), std::__Cr::tuple<cppgc::internal::BasicPersistent<blink::SharedWorkerGlobalScope, cppgc::internal::WeakPersistentPolicy, cppgc::internal::IgnoreLocationPolicy, cppgc::internal::DisabledCheckingPolicy>, cppgc::internal::BasicPersistent<blink::WorkerClassicScriptLoader, cppgc::internal::StrongPersistentPolicy, cppgc::internal::IgnoreLocationPolicy, cppgc::internal::DisabledCheckingPolicy>, v8_inspector::V8StackTraceId>, 0ul, 1ul, 2ul>(void (blink::SharedWorkerGlobalScope::*&&)(blink::WorkerClassicScriptLoader*, v8_inspector::V8StackTraceId const&), std::__Cr::tuple<cppgc::internal::BasicPersistent<blink::SharedWorkerGlobalScope, cppgc::internal::WeakPersistentPolicy, cppgc::internal::IgnoreLocationPolicy, cppgc::internal::DisabledCheckingPolicy>, cppgc::internal::BasicPersistent<blink::WorkerClassicScriptLoader, cppgc::internal::StrongPersistentPolicy, cppgc::internal::IgnoreLocationPolicy, cppgc::internal::DisabledCheckingPolicy>, v8_inspector::V8StackTraceId>&&, std::__Cr::integer_sequence<unsigned long, 0ul, 1ul, 2ul>) bind_internal.h:1025
 base::internal::Invoker<base::internal::BindState<void (blink::SharedWorkerGlobalScope::*)(blink::WorkerClassicScriptLoader*, v8_inspector::V8StackTraceId const&), cppgc::internal::BasicPersistent<blink::SharedWorkerGlobalScope, cppgc::internal::WeakPersistentPolicy, cppgc::internal::IgnoreLocationPolicy, cppgc::internal::DisabledCheckingPolicy>, cppgc::internal::BasicPersistent<blink::WorkerClassicScriptLoader, cppgc::internal::StrongPersistentPolicy, cppgc::internal::IgnoreLocationPolicy, cppgc::internal::DisabledCheckingPolicy>, v8_inspector::V8StackTraceId>, void ()>::RunOnce(base::internal::BindStateBase*) bind_internal.h:976
 base::OnceCallback<void ()>::Run() && callback.h:152
 WTF::ThreadCheckingCallbackWrapper<base::OnceCallback<void ()>, void ()>::RunInternal(base::OnceCallback<void ()>*) functional.h:241
 WTF::ThreadCheckingCallbackWrapper<base::OnceCallback<void ()>, void ()>::Run() functional.h:226
 void base::internal::FunctorTraits<void (WTF::ThreadCheckingCallbackWrapper<base::OnceCallback<void ()>, void ()>::*)(), void>::Invoke<void (WTF::ThreadCheckingCallbackWrapper<base::OnceCallback<void ()>, void ()>::*)(), std::__Cr::unique_ptr<WTF::ThreadCheckingCallbackWrapper<base::OnceCallback<void ()>, void ()>, std::__Cr::default_delete<WTF::ThreadCheckingCallbackWrapper<base::OnceCallback<void ()>, void ()>>>>(void (WTF::ThreadCheckingCallbackWrapper<base::OnceCallback<void ()>, void ()>::*)(), std::__Cr::unique_ptr<WTF::ThreadCheckingCallbackWrapper<base::OnceCallback<void ()>, void ()>, std::__Cr::default_delete<WTF::ThreadCheckingCallbackWrapper<base::OnceCallback<void ()>, void ()>>>&&) bind_internal.h:746
 void base::internal::InvokeHelper<false, void, 0ul>::MakeItSo<void (WTF::ThreadCheckingCallbackWrapper<base::OnceCallback<void ()>, void ()>::*)(), std::__Cr::tuple<std::__Cr::unique_ptr<WTF::ThreadCheckingCallbackWrapper<base::OnceCallback<void ()>, void ()>, std::__Cr::default_delete<WTF::ThreadCheckingCallbackWrapper<base::OnceCallback<void ()>, void ()>>>>>(void (WTF::ThreadCheckingCallbackWrapper<base::OnceCallback<void ()>, void ()>::*&&)(), std::__Cr::tuple<std::__Cr::unique_ptr<WTF::ThreadCheckingCallbackWrapper<base::OnceCallback<void ()>, void ()>, std::__Cr::default_delete<WTF::ThreadCheckingCallbackWrapper<base::OnceCallback<void ()>, void ()>>>>&&) bind_internal.h:925
 void base::internal::Invoker<base::internal::BindState<void (WTF::ThreadCheckingCallbackWrapper<base::OnceCallback<void ()>, void ()>::*)(), std::__Cr::unique_ptr<WTF::ThreadCheckingCallbackWrapper<base::OnceCallback<void ()>, void ()>, std::__Cr::default_delete<WTF::ThreadCheckingCallbackWrapper<base::OnceCallback<void ()>, void ()>>>>, void ()>::RunImpl<void (WTF::ThreadCheckingCallbackWrapper<base::OnceCallback<void ()>, void ()>::*)(), std::__Cr::tuple<std::__Cr::unique_ptr<WTF::ThreadCheckingCallbackWrapper<base::OnceCallback<void ()>, void ()>, std::__Cr::default_delete<WTF::ThreadCheckingCallbackWrapper<base::OnceCallback<void ()>, void ()>>>>, 0ul>(void (WTF::ThreadCheckingCallbackWrapper<base::OnceCallback<void ()>, void ()>::*&&)(), std::__Cr::tuple<std::__Cr::unique_ptr<WTF::ThreadCheckingCallbackWrapper<base::OnceCallback<void ()>, void ()>, std::__Cr::default_delete<WTF::ThreadCheckingCallbackWrapper<base::OnceCallback<void ()>, void ()>>>>&&, std::__Cr::integer_sequence<unsigned long, 0ul>) bind_internal.h:1025
 base::internal::Invoker<base::internal::BindState<void (WTF::ThreadCheckingCallbackWrapper<base::OnceCallback<void ()>, void ()>::*)(), std::__Cr::unique_ptr<WTF::ThreadCheckingCallbackWrapper<base::OnceCallback<void ()>, void ()>, std::__Cr::default_delete<WTF::ThreadCheckingCallbackWrapper<base::OnceCallback<void ()>, void ()>>>>, void ()>::RunOnce(base::internal::BindStateBase*) bind_internal.h:976
 base::OnceCallback<void ()>::Run() && callback.h:152
 blink::WorkerClassicScriptLoader::NotifyFinished() worker_classic_script_loader.cc:354
 blink::WorkerClassicScriptLoader::OnFinishedLoadingWorkerMainScript() worker_classic_script_loader.cc:310
 blink::WorkerMainScriptLoader::NotifyCompletionIfAppropriate() worker_main_script_loader.cc:271
 blink::WorkerMainScriptLoader::OnReadable(unsigned int) worker_main_script_loader.cc:233
 void base::internal::FunctorTraits<void (blink::WorkerMainScriptLoader::*)(unsigned int), void>::Invoke<void (blink::WorkerMainScriptLoader::*)(unsigned int), cppgc::internal::BasicPersistent<blink::WorkerMainScriptLoader, cppgc::internal::WeakPersistentPolicy, cppgc::internal::IgnoreLocationPolicy, cppgc::internal::DisabledCheckingPolicy> const&, unsigned int>(void (blink::WorkerMainScriptLoader::*)(unsigned int), cppgc::internal::BasicPersistent<blink::WorkerMainScriptLoader, cppgc::internal::WeakPersistentPolicy, cppgc::internal::IgnoreLocationPolicy, cppgc::internal::DisabledCheckingPolicy> const&, unsigned int&&) bind_internal.h:746
 void base::internal::InvokeHelper<true, void, 0ul>::MakeItSo<void (blink::WorkerMainScriptLoader::* const&)(unsigned int), std::__Cr::tuple<cppgc::internal::BasicPersistent<blink::WorkerMainScriptLoader, cppgc::internal::WeakPersistentPolicy, cppgc::internal::IgnoreLocationPolicy, cppgc::internal::DisabledCheckingPolicy>> const&, unsigned int>(void (blink::WorkerMainScriptLoader::* const&)(unsigned int), std::__Cr::tuple<cppgc::internal::BasicPersistent<blink::WorkerMainScriptLoader, cppgc::internal::WeakPersistentPolicy, cppgc::internal::IgnoreLocationPolicy, cppgc::internal::DisabledCheckingPolicy>> const&, unsigned int&&) bind_internal.h:953
 void base::internal::Invoker<base::internal::BindState<void (blink::WorkerMainScriptLoader::*)(unsigned int), cppgc::internal::BasicPersistent<blink::WorkerMainScriptLoader, cppgc::internal::WeakPersistentPolicy, cppgc::internal::IgnoreLocationPolicy, cppgc::internal::DisabledCheckingPolicy>>, void (unsigned int)>::RunImpl<void (blink::WorkerMainScriptLoader::* const&)(unsigned int), std::__Cr::tuple<cppgc::internal::BasicPersistent<blink::WorkerMainScriptLoader, cppgc::internal::WeakPersistentPolicy, cppgc::internal::IgnoreLocationPolicy, cppgc::internal::DisabledCheckingPolicy>> const&, 0ul>(void (blink::WorkerMainScriptLoader::* const&)(unsigned int), std::__Cr::tuple<cppgc::internal::BasicPersistent<blink::WorkerMainScriptLoader, cppgc::internal::WeakPersistentPolicy, cppgc::internal::IgnoreLocationPolicy, cppgc::internal::DisabledCheckingPolicy>> const&, std::__Cr::integer_sequence<unsigned long, 0ul>, unsigned int&&) bind_internal.h:1025
 base::internal::Invoker<base::internal::BindState<void (blink::WorkerMainScriptLoader::*)(unsigned int), cppgc::internal::BasicPersistent<blink::WorkerMainScriptLoader, cppgc::internal::WeakPersistentPolicy, cppgc::internal::IgnoreLocationPolicy, cppgc::internal::DisabledCheckingPolicy>>, void (unsigned int)>::Run(base::internal::BindStateBase*, unsigned int) bind_internal.h:989
 base::RepeatingCallback<void (unsigned int)>::Run(unsigned int) const & callback.h:333
 WTF::ThreadCheckingCallbackWrapper<base::RepeatingCallback<void (unsigned int)>, void (unsigned int)>::RunInternal(base::RepeatingCallback<void (unsigned int)>*, unsigned int&&) functional.h:236
 WTF::ThreadCheckingCallbackWrapper<base::RepeatingCallback<void (unsigned int)>, void (unsigned int)>::Run(unsigned int) functional.h:226
 void base::internal::FunctorTraits<void (WTF::ThreadCheckingCallbackWrapper<base::RepeatingCallback<void (unsigned int)>, void (unsigned int)>::*)(unsigned int), void>::Invoke<void (WTF::ThreadCheckingCallbackWrapper<base::RepeatingCallback<void (unsigned int)>, void (unsigned int)>::*)(unsigned int), std::__Cr::unique_ptr<WTF::ThreadCheckingCallbackWrapper<base::RepeatingCallback<void (unsigned int)>, void (unsigned int)>, std::__Cr::default_delete<WTF::ThreadCheckingCallbackWrapper<base::RepeatingCallback<void (unsigned int)>, void (unsigned int)>>> const&, unsigned int>(void (WTF::ThreadCheckingCallbackWrapper<base::RepeatingCallback<void (unsigned int)>, void (unsigned int)>::*)(unsigned int), std::__Cr::unique_ptr<WTF::ThreadCheckingCallbackWrapper<base::RepeatingCallback<void (unsigned int)>, void (unsigned int)>, std::__Cr::default_delete<WTF::ThreadCheckingCallbackWrapper<base::RepeatingCallback<void (unsigned int)>, void (unsigned int)>>> const&, unsigned int&&) bind_internal.h:746
 void base::internal::InvokeHelper<false, void, 0ul>::MakeItSo<void (WTF::ThreadCheckingCallbackWrapper<base::RepeatingCallback<void (unsigned int)>, void (unsigned int)>::* const&)(unsigned int), std::__Cr::tuple<std::__Cr::unique_ptr<WTF::ThreadCheckingCallbackWrapper<base::RepeatingCallback<void (unsigned int)>, void (unsigned int)>, std::__Cr::default_delete<WTF::ThreadCheckingCallbackWrapper<base::RepeatingCallback<void (unsigned int)>, void (unsigned int)>>>> const&, unsigned int>(void (WTF::ThreadCheckingCallbackWrapper<base::RepeatingCallback<void (unsigned int)>, void (unsigned int)>::* const&)(unsigned int), std::__Cr::tuple<std::__Cr::unique_ptr<WTF::ThreadCheckingCallbackWrapper<base::RepeatingCallback<void (unsigned int)>, void (unsigned int)>, std::__Cr::default_delete<WTF::ThreadCheckingCallbackWrapper<base::RepeatingCallback<void (unsigned int)>, void (unsigned int)>>>> const&, unsigned int&&) bind_internal.h:925
 void base::internal::Invoker<base::internal::BindState<void (WTF::ThreadCheckingCallbackWrapper<base::RepeatingCallback<void (unsigned int)>, void (unsigned int)>::*)(unsigned int), std::__Cr::unique_ptr<WTF::ThreadCheckingCallbackWrapper<base::RepeatingCallback<void (unsigned int)>, void (unsigned int)>, std::__Cr::default_delete<WTF::ThreadCheckingCallbackWrapper<base::RepeatingCallback<void (unsigned int)>, void (unsigned int)>>>>, void (unsigned int)>::RunImpl<void (WTF::ThreadCheckingCallbackWrapper<base::RepeatingCallback<void (unsigned int)>, void (unsigned int)>::* const&)(unsigned int), std::__Cr::tuple<std::__Cr::unique_ptr<WTF::ThreadCheckingCallbackWrapper<base::RepeatingCallback<void (unsigned int)>, void (unsigned int)>, std::__Cr::default_delete<WTF::ThreadCheckingCallbackWrapper<base::RepeatingCallback<void (unsigned int)>, void (unsigned int)>>>> const&, 0ul>(void (WTF::ThreadCheckingCallbackWrapper<base::RepeatingCallback<void (unsigned int)>, void (unsigned int)>::* const&)(unsigned int), std::__Cr::tuple<std::__Cr::unique_ptr<WTF::ThreadCheckingCallbackWrapper<base::RepeatingCallback<void (unsigned int)>, void (unsigned int)>, std::__Cr::default_delete<WTF::ThreadCheckingCallbackWrapper<base::RepeatingCallback<void (unsigned int)>, void (unsigned int)>>>> const&, std::__Cr::integer_sequence<unsigned long, 0ul>, unsigned int&&) bind_internal.h:1025
 base::internal::Invoker<base::internal::BindState<void (WTF::ThreadCheckingCallbackWrapper<base::RepeatingCallback<void (unsigned int)>, void (unsigned int)>::*)(unsigned int), std::__Cr::unique_ptr<WTF::ThreadCheckingCallbackWrapper<base::RepeatingCallback<void (unsigned int)>, void (unsigned int)>, std::__Cr::default_delete<WTF::ThreadCheckingCallbackWrapper<base::RepeatingCallback<void (unsigned int)>, void (unsigned int)>>>>, void (unsigned int)>::Run(base::internal::BindStateBase*, unsigned int) bind_internal.h:989
 base::RepeatingCallback<void (unsigned int)>::Run(unsigned int) const & callback.h:333
 mojo::SimpleWatcher::DiscardReadyState(base::RepeatingCallback<void (unsigned int)> const&, unsigned int, mojo::HandleSignalsState const&) simple_watcher.h:192
 void base::internal::FunctorTraits<void (*)(base::RepeatingCallback<void (unsigned int)> const&, unsigned int, mojo::HandleSignalsState const&), void>::Invoke<void (* const&)(base::RepeatingCallback<void (unsigned int)> const&, unsigned int, mojo::HandleSignalsState const&), base::RepeatingCallback<void (unsigned int)> const&, unsigned int, mojo::HandleSignalsState const&>(void (* const&)(base::RepeatingCallback<void (unsigned int)> const&, unsigned int, mojo::HandleSignalsState const&), base::RepeatingCallback<void (unsigned int)> const&, unsigned int&&, mojo::HandleSignalsState const&) bind_internal.h:636
 void base::internal::InvokeHelper<false, void, 0ul>::MakeItSo<void (* const&)(base::RepeatingCallback<void (unsigned int)> const&, unsigned int, mojo::HandleSignalsState const&), std::__Cr::tuple<base::RepeatingCallback<void (unsigned int)>> const&, unsigned int, mojo::HandleSignalsState const&>(void (* const&)(base::RepeatingCallback<void (unsigned int)> const&, unsigned int, mojo::HandleSignalsState const&), std::__Cr::tuple<base::RepeatingCallback<void (unsigned int)>> const&, unsigned int&&, mojo::HandleSignalsState const&) bind_internal.h:925
 void base::internal::Invoker<base::internal::BindState<void (*)(base::RepeatingCallback<void (unsigned int)> const&, unsigned int, mojo::HandleSignalsState const&), base::RepeatingCallback<void (unsigned int)>>, void (unsigned int, mojo::HandleSignalsState const&)>::RunImpl<void (* const&)(base::RepeatingCallback<void (unsigned int)> const&, unsigned int, mojo::HandleSignalsState const&), std::__Cr::tuple<base::RepeatingCallback<void (unsigned int)>> const&, 0ul>(void (* const&)(base::RepeatingCallback<void (unsigned int)> const&, unsigned int, mojo::HandleSignalsState const&), std::__Cr::tuple<base::RepeatingCallback<void (unsigned int)>> const&, std::__Cr::integer_sequence<unsigned long, 0ul>, unsigned int&&, mojo::HandleSignalsState const&) bind_internal.h:1025
 base::internal::Invoker<base::internal::BindState<void (*)(base::RepeatingCallback<void (unsigned int)> const&, unsigned int, mojo::HandleSignalsState const&), base::RepeatingCallback<void (unsigned int)>>, void (unsigned int, mojo::HandleSignalsState const&)>::Run(base::internal::BindStateBase*, unsigned int, mojo::HandleSignalsState const&) bind_internal.h:989
 base::RepeatingCallback<void (unsigned int, mojo::HandleSignalsState const&)>::Run(unsigned int, mojo::HandleSignalsState const&) const & callback.h:333
 mojo::SimpleWatcher::OnHandleReady(int, unsigned int, mojo::HandleSignalsState const&) simple_watcher.cc:278
 void base::internal::FunctorTraits<void (mojo::SimpleWatcher::*)(int, unsigned int, mojo::HandleSignalsState const&), void>::Invoke<void (mojo::SimpleWatcher::*)(int, unsigned int, mojo::HandleSignalsState const&), base::WeakPtr<mojo::SimpleWatcher> const&, int, unsigned int, mojo::HandleSignalsState>(void (mojo::SimpleWatcher::*)(int, unsigned int, mojo::HandleSignalsState const&), base::WeakPtr<mojo::SimpleWatcher> const&, int&&, unsigned int&&, mojo::HandleSignalsState&&) bind_internal.h:746
 void base::internal::InvokeHelper<true, void, 0ul, 1ul, 2ul, 3ul>::MakeItSo<void (mojo::SimpleWatcher::*)(int, unsigned int, mojo::HandleSignalsState const&), std::__Cr::tuple<base::WeakPtr<mojo::SimpleWatcher>, int, unsigned int, mojo::HandleSignalsState>>(void (mojo::SimpleWatcher::*&&)(int, unsigned int, mojo::HandleSignalsState const&), std::__Cr::tuple<base::WeakPtr<mojo::SimpleWatcher>, int, unsigned int, mojo::HandleSignalsState>&&) bind_internal.h:953
 void base::internal::Invoker<base::internal::BindState<void (mojo::SimpleWatcher::*)(int, unsigned int, mojo::HandleSignalsState const&), base::WeakPtr<mojo::SimpleWatcher>, int, unsigned int, mojo::HandleSignalsState>, void ()>::RunImpl<void (mojo::SimpleWatcher::*)(int, unsigned int, mojo::HandleSignalsState const&), std::__Cr::tuple<base::WeakPtr<mojo::SimpleWatcher>, int, unsigned int, mojo::HandleSignalsState>, 0ul, 1ul, 2ul, 3ul>(void (mojo::SimpleWatcher::*&&)(int, unsigned int, mojo::HandleSignalsState const&), std::__Cr::tuple<base::WeakPtr<mojo::SimpleWatcher>, int, unsigned int, mojo::HandleSignalsState>&&, std::__Cr::integer_sequence<unsigned long, 0ul, 1ul, 2ul, 3ul>) bind_internal.h:1025
 base::internal::Invoker<base::internal::BindState<void (mojo::SimpleWatcher::*)(int, unsigned int, mojo::HandleSignalsState const&), base::WeakPtr<mojo::SimpleWatcher>, int, unsigned int, mojo::HandleSignalsState>, void ()>::RunOnce(base::internal::BindStateBase*) bind_internal.h:976
 base::OnceCallback<void ()>::Run() && callback.h:152
 base::TaskAnnotator::RunTaskImpl(base::PendingTask&) task_annotator.cc:186
 void base::TaskAnnotator::RunTask<base::sequence_manager::internal::ThreadControllerWithMessagePumpImpl::DoWorkImpl(base::LazyNow*)::$_0>(perfetto::StaticString, base::PendingTask&, base::sequence_manager::internal::ThreadControllerWithMessagePumpImpl::DoWorkImpl(base::LazyNow*)::$_0&&) task_annotator.h:88
 base::sequence_manager::internal::ThreadControllerWithMessagePumpImpl::DoWorkImpl(base::LazyNow*) thread_controller_with_message_pump_impl.cc:486
 base::sequence_manager::internal::ThreadControllerWithMessagePumpImpl::DoWork() thread_controller_with_message_pump_impl.cc:351
 base::MessagePumpDefault::Run(base::MessagePump::Delegate*) message_pump_default.cc:40
 base::sequence_manager::internal::ThreadControllerWithMessagePumpImpl::Run(bool, base::TimeDelta) thread_controller_with_message_pump_impl.cc:651
 base::RunLoop::Run(base::Location const&) run_loop.cc:134
 blink::scheduler::NonMainThreadImpl::SimpleThreadImpl::Run() non_main_thread_impl.cc:182
 base::SimpleThread::ThreadMain() simple_thread.cc:78
 base::(anonymous namespace)::ThreadFunc(void*) platform_thread_posix.cc:101
 _pthread_start <unknown>:35

Thread #86 ServiceWorker thread:
 blink::mojom::blink::ServiceWorkerRequestValidator::Accept(mojo::Message*) service_worker.mojom-blink.cc:8496
 mojo::MessageDispatcher::Accept(mojo::Message*) message_dispatcher.cc:38
 mojo::InterfaceEndpointClient::HandleIncomingMessage(mojo::Message*) interface_endpoint_client.cc:701
 mojo::internal::MultiplexRouter::ProcessIncomingMessage(mojo::internal::MultiplexRouter::MessageWrapper*, mojo::internal::MultiplexRouter::ClientCallBehavior, base::SequencedTaskRunner*) multiplex_router.cc:1096
 mojo::internal::MultiplexRouter::ProcessTasks(mojo::internal::MultiplexRouter::ClientCallBehavior, base::SequencedTaskRunner*) multiplex_router.cc:925
 mojo::internal::MultiplexRouter::LockAndCallProcessTasks() multiplex_router.cc:1124
 void base::internal::FunctorTraits<void (mojo::internal::MultiplexRouter::*)(), void>::Invoke<void (mojo::internal::MultiplexRouter::*)(), scoped_refptr<mojo::internal::MultiplexRouter>>(void (mojo::internal::MultiplexRouter::*)(), scoped_refptr<mojo::internal::MultiplexRouter>&&) bind_internal.h:746
 void base::internal::InvokeHelper<false, void, 0ul>::MakeItSo<void (mojo::internal::MultiplexRouter::*)(), std::__Cr::tuple<scoped_refptr<mojo::internal::MultiplexRouter>>>(void (mojo::internal::MultiplexRouter::*&&)(), std::__Cr::tuple<scoped_refptr<mojo::internal::MultiplexRouter>>&&) bind_internal.h:925
 void base::internal::Invoker<base::internal::BindState<void (mojo::internal::MultiplexRouter::*)(), scoped_refptr<mojo::internal::MultiplexRouter>>, void ()>::RunImpl<void (mojo::internal::MultiplexRouter::*)(), std::__Cr::tuple<scoped_refptr<mojo::internal::MultiplexRouter>>, 0ul>(void (mojo::internal::MultiplexRouter::*&&)(), std::__Cr::tuple<scoped_refptr<mojo::internal::MultiplexRouter>>&&, std::__Cr::integer_sequence<unsigned long, 0ul>) bind_internal.h:1025
 base::internal::Invoker<base::internal::BindState<void (mojo::internal::MultiplexRouter::*)(), scoped_refptr<mojo::internal::MultiplexRouter>>, void ()>::RunOnce(base::internal::BindStateBase*) bind_internal.h:976
 base::OnceCallback<void ()>::Run() && callback.h:152
 base::TaskAnnotator::RunTaskImpl(base::PendingTask&) task_annotator.cc:186
 void base::TaskAnnotator::RunTask<base::sequence_manager::internal::ThreadControllerWithMessagePumpImpl::DoWorkImpl(base::LazyNow*)::$_0>(perfetto::StaticString, base::PendingTask&, base::sequence_manager::internal::ThreadControllerWithMessagePumpImpl::DoWorkImpl(base::LazyNow*)::$_0&&) task_annotator.h:88
 base::sequence_manager::internal::ThreadControllerWithMessagePumpImpl::DoWorkImpl(base::LazyNow*) thread_controller_with_message_pump_impl.cc:486
 base::sequence_manager::internal::ThreadControllerWithMessagePumpImpl::DoWork() thread_controller_with_message_pump_impl.cc:351
 base::MessagePumpDefault::Run(base::MessagePump::Delegate*) message_pump_default.cc:40
 base::sequence_manager::internal::ThreadControllerWithMessagePumpImpl::Run(bool, base::TimeDelta) thread_controller_with_message_pump_impl.cc:651
 base::RunLoop::Run(base::Location const&) run_loop.cc:134
 blink::scheduler::NonMainThreadImpl::SimpleThreadImpl::Run() non_main_thread_impl.cc:182
 base::SimpleThread::ThreadMain() simple_thread.cc:78
 base::(anonymous namespace)::ThreadFunc(void*) platform_thread_posix.cc:101
 _pthread_start <unknown>:35


