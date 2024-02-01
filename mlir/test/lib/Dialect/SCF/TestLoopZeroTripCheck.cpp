//===- TestLoopZeroTripCheck.cpp -- Pass to test replaceWithZeroTripCheck -===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements the passes to test replaceWithZeroTripCheck for SCF
// dialect.
//
//===----------------------------------------------------------------------===//

#include "mlir/Dialect/Func/IR/FuncOps.h"
#include "mlir/Dialect/SCF/IR/SCF.h"
#include "mlir/IR/PatternMatch.h"
#include "mlir/Pass/Pass.h"

using namespace mlir;

namespace {

struct TestSCFWhileZeroTripCheckPass
    : public PassWrapper<TestSCFWhileZeroTripCheckPass,
                         OperationPass<func::FuncOp>> {
  MLIR_DEFINE_EXPLICIT_INTERNAL_INLINE_TYPE_ID(TestSCFWhileZeroTripCheckPass)

  StringRef getArgument() const final {
    return "test-scf-while-zero-trip-check";
  }
  StringRef getDescription() const final {
    return "test replaceWithZeroTripCheck of scf.while";
  }
  explicit TestSCFWhileZeroTripCheckPass() = default;
  TestSCFWhileZeroTripCheckPass(const TestSCFWhileZeroTripCheckPass &pass)
      : PassWrapper(pass) {}

  void runOnOperation() override {
    func::FuncOp func = getOperation();
    MLIRContext *context = &getContext();
    IRRewriter rewriter(context);
    func.walk([&](scf::WhileOp op) {
      auto result = op.replaceWithZeroTripCheck(rewriter);
      if (failed(result)) {
        signalPassFailure();
      }
    });
  }
};

} // namespace

namespace mlir {
namespace test {
void registerTestLoopZeroTripCheckPass() {
  PassRegistration<TestSCFWhileZeroTripCheckPass>();
}
} // namespace test
} // namespace mlir
