/*
 * This file is a part of u8clip.
 *
 * Copyright (c) 2024 Flokart World, Inc.
 * All rights reserved.
 */

#include <windows.h>
#include <iostream>
#include <string>
#include <utility>

namespace {

template <typename T>
class scope_exit {
private:
    T M_core;

public:
    scope_exit(T &&core) : M_core(std::forward<T>(core)) {
    }

    ~scope_exit() {
        M_core();
    }
};

} // namespace

int main() {
    std::string input(std::istreambuf_iterator<char>(std::cin), {});
    int required_wchars =
            MultiByteToWideChar(CP_UTF8,
                                0,
                                input.data(),
                                static_cast<int> (input.size()),
                                nullptr,
                                0)
            + 1;
    for (auto c : input) {
        if (c == u8'\n') {
            ++required_wchars;
        }
    }

    HGLOBAL memory =
            GlobalAlloc(GMEM_FIXED, required_wchars * sizeof (wchar_t));
    if (!memory) {
        std::cerr << "ERROR: GlobalAlloc failed." << std::endl;
        return 1;
    }
    auto free = scope_exit([memory]() { GlobalFree(memory); });

    if (auto dst = reinterpret_cast<wchar_t *> (GlobalLock(memory)); dst) {
        auto end = dst + required_wchars;
        std::size_t last = 0;
        do {
            auto rest = static_cast<int> (end - dst);
            std::size_t delim_pos = input.find(u8'\n', last);
            if (delim_pos == std::string::npos) {
                MultiByteToWideChar(CP_UTF8,
                                    0,
                                    input.data() + last,
                                    static_cast<int> (input.size() - last),
                                    dst,
                                    rest);
                break;
            } else {
                int len =
                        MultiByteToWideChar(
                                CP_UTF8,
                                0,
                                input.data() + last,
                                static_cast<int> (delim_pos - last),
                                dst,
                                rest);
                dst[len] = L'\r';
                dst[len + 1] = L'\n';
                dst[len + 2] = L'\0';
                dst += len + 2;
                last = delim_pos + 1;
            }
        } while (last < input.size());

        GlobalUnlock(memory);
    } else {
        std::cerr << "ERROR: GlobalLock failed." << std::endl;
        return 2;
    }

    if (!OpenClipboard(nullptr)) {
        std::cerr << "ERROR: OpenClipboard failed." << std::endl;
        return 3;
    }
    auto close = scope_exit([]() { CloseClipboard(); });

    /* Required to take ownership */
    if (!EmptyClipboard()) {
        std::cerr << "ERROR: EmptyClipboard failed." << std::endl;
        return 4;
    }

    if (!SetClipboardData(CF_UNICODETEXT, memory)) {
        std::cerr << "ERROR: SetClipboardData failed." << std::endl;
        return 5;
    }

    return 0;
}
