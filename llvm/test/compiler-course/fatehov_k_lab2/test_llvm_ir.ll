; RUN: opt -load-pass-plugin %llvmshlibdir/DecomposeRemPass%pluginext\
; RUN: -passes=decompose-rem -S %s | FileCheck %s

; CHECK-LABEL: @test_srem
; CHECK-NOT: srem
; CHECK: sdiv
; CHECK: mul
; CHECK: sub
define i32 @test_srem(i32 %a, i32 %b) {
entry:
  %result = srem i32 %a, %b
  ret i32 %result
}

; CHECK-LABEL: @test_urem
; CHECK-NOT: urem
; CHECK: udiv
; CHECK: mul
; CHECK: sub
define i32 @test_urem(i32 %a, i32 %b) {
entry:
  %result = urem i32 %a, %b
  ret i32 %result
}

; CHECK-LABEL: @test_frem
; CHECK-NOT: frem
; CHECK: fdiv
; CHECK: fmul
; CHECK: fsub
define float @test_frem(float %a, float %b) {
entry:
  %result = frem float %a, %b
  ret float %result
}

; CHECK-LABEL: @test_frem_double
; CHECK-NOT: frem
; CHECK: fdiv
; CHECK: fmul
; CHECK: fsub
define double @test_frem_double(double %a, double %b) {
entry:
  %result = frem double %a, %b
  ret double %result
}

; CHECK-LABEL: @test_no_rem
; CHECK: add
define i32 @test_no_rem(i32 %a, i32 %b) {
entry:
  %result = add i32 %a, %b
  ret i32 %result
}