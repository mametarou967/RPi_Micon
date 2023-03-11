@ startup
    .global _start
    .align
_start:
    ldr r0, = 0x000000d3
    msr cpsr, r0
    ldr sp, = 0x06400000
    // main 呼び出し
    bl main
    // 終了(無限ループ)
    b .