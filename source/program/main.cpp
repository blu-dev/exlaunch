#include "lib.hpp"
#include "nn/os.hpp"
#include "nn/fs.hpp"

// MAKE_HOOK_T(void, stubCopyright, (bool),
//     /* Call the original implementation of this function, but with a false argument. */
//     impl(false);
// );

/* Declare function to dynamic link with. */
namespace nn::oe {
    void SetCopyrightVisibility(bool);
};

static void (*LOCK_IMPL)(nn::os::MutexType*) = nullptr;

void nn__os__lock_mutex(nn::os::MutexType* mutex) {
    static bool HAS_LOCKED = false;
    exl::util::Hook::SetDisableFlexibleHook(nn::os::LockMutex, true);
    if (!HAS_LOCKED) {
        nn::fs::CreateFile("sd:/1.txt", 0);
    } else {
        nn::fs::CreateFile("sd:/2.txt", 0);
    }
    HAS_LOCKED = true;
    LOCK_IMPL(mutex);
}

extern "C" void exl_main(void* x0, void* x1) {
    /* Setup hooking enviroment. */
    envSetOwnProcessHandle(exl::util::proc_handle::Get());
    exl::hook::Initialize();

    /* If an integer is provided instead of a symbol, it will be treated as an offset relative to main. */
    // INJECT_HOOK_T(nn::oe::SetCopyrightVisibility, stubCopyright);
    nn::fs::MountSdCardForDebug("sd");
    LOCK_IMPL = reinterpret_cast<void(*)(nn::os::MutexType*)>(exl::util::Hook::FlexibleHook(nn::os::LockMutex, nn__os__lock_mutex));
    /*
    For sysmodules/applets, you have to call the entrypoint when ready
    exl::hook::CallTargetEntrypoint(x0, x1);
    */
}

extern "C" NORETURN void exl_exception_entry() {
    /* TODO: exception handling */
    EXL_ABORT(0x420);
}