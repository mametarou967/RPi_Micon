@ startup
    .global _start
    .align
_start:
    // IRQモードのスタックポインタ設定
    // [ON]7:IRQ disable
    // [ON]6:FIQ disable
    // [OFF]5:Thumb state bit
    // [10010]4-0:Mode bit
    //  10011:スーパバイザ
    //  11011:未定義モード
    //  10010:IRQモード

    // 1101_0010=d2
    ldr r0, = 0x000000d2
    msr cpsr, r0
    ldr sp, = 0x00008000

    // スーパーバイザモード
    ldr r0, = 0x000000d3
    msr cpsr, r0
    ldr sp, = 0x06400000
    // main 呼び出し
    bl main
    // 終了(無限ループ)
    b .

.global _hangup
_hangup:
    ldr r0, = 0x000000d3
    msr cpsr, r0
    b .

_IRQ_interrupt:
    // スタックにレジスタの内容を退避させる
    stmfd   r13!, {r0-r12,lr}
    // C言語の割込みハンドラの呼び出し
    bl      IRQ_handler
    // スタックからレジスタの内容を復元させる
    ldmfd   r13!, {r0-r12,lr}
    // 戻りアドレスの修正(IRQはlr-4)して、元の実行番地に復帰
    subs    pc,lr,#4

// IRQ有効化(CPSR設定)
    .global enable_IRQ
enable_IRQ:
    mrs     r0,cpsr
    bic     r0,r0,#0x80
    msr     cpsr,r0
    bx      lr

// IRQ無効化(CPSR設定)
    .global disable_IRQ
disable_IRQ:
    mrs     r0,cpsr
    ldr     r1,=#0x80
    orr     r0,r0,r1
    msr     cpsr,r0
    bx      lr

// CPSRの値を取得して返す(r0は戻り値を入れるレジスタ)
    .global getmode
getmode:
    mrs     r0,cpsr
    bx      lr

// ベクターテーブル設定用
    .global _initial_vector_start
_initial_vector_start:
    ldr     pc, _vec_Reset
    ldr     pc, _vec_Undef
    ldr     pc, _vec_SWI
    ldr     pc, _vec_PrefAbort
    ldr     pc, _vec_DataAbort
    ldr     pc, _vec_Reserved
    ldr     pc, _vec_IRQ
    ldr     pc, _vec_FIQ
_vec_Reset:     .word   _start
_vec_Undef:     .word   _hangup
_vec_SWI:       .word   _hangup
_vec_PrefAbort: .word   _hangup
_vec_DataAbort: .word   _hangup
_vec_Reserved:  .word   _hangup
_vec_IRQ:       .word   _IRQ_interrupt
_vec_FIQ:       .word   _hangup

    .global _initial_vector_end
_initial_vector_end:
    mov r0,r0
