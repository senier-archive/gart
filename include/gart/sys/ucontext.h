#pragma once
#include <x86/ucontext.h>

#define ucontext __mcontext

#if defined(__x86_64__)
#define CTX_ESP mc_rsp
#define CTX_EIP mc_rip
#define CTX_eax mc_rax
#define CTX_METHOD mc_rdi
#define CTX_RDI mc_rdi
#define CTX_JMP_BUF mc_rdi
#else
#define CTX_ESP mc_esp
#define CTX_EIP mc_eip
#define CTX_EAX mc_eax
#define CTX_METHOD mc_eax
#define CTX_JMP_BUF mc_eax
#endif
