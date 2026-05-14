// RUN: mlir-opt -load-pass-plugin=%mlir_lib_dir/fatehov_k_lab4_MLIR%shlibext --pass-pipeline="builtin.module(fatehov_k_lab4)" %s | FileCheck %s

// CHECK: module {
// CHECK:   func.func private @trace_condition_then_begin()
// CHECK:   func.func private @trace_condition_then_end()
// CHECK:   func.func private @trace_condition_else_begin()
// CHECK:   func.func private @trace_condition_else_end()
// CHECK:   func.func @test(%arg0: i1) {
// CHECK:     scf.if %arg0 {
// CHECK:       func.call @trace_condition_then_begin() : () -> ()
// CHECK:       arith.constant 0 : i32
// CHECK:       func.call @trace_condition_then_end() : () -> ()
// CHECK:     } else {
// CHECK:       func.call @trace_condition_else_begin() : () -> ()
// CHECK:       arith.constant 1 : i32
// CHECK:       func.call @trace_condition_else_end() : () -> ()
// CHECK:     }
// CHECK:     return
// CHECK:   }
// CHECK: }

module {
  func.func @test(%arg0: i1) {
    scf.if %arg0 {
      %c0 = arith.constant 0 : i32
    } else {
      %c1 = arith.constant 1 : i32
    }
    return
  }
}
