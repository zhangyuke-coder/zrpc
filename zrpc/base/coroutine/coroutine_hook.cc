#include "coroutine_hook.h"

namespace {
static bool g_hook = true;
}
namespace zrpc {
void SetHook(bool value) {
    g_hook = value;
}
}