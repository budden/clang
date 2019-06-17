// #include "ClangSACheckers.h"
#include "clang/StaticAnalyzer/Core/BugReporter/BugType.h"
#include "clang/StaticAnalyzer/Core/Checker.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/CallEvent.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/CheckerContext.h"

using namespace clang;
using namespace clang::ento;

 namespace {
 class MainCallChecker : public Checker <check::PreCall> {
    mutable std::unique_ptr<BugType> BT;

 public:
    void checkPreCall (const CallEvent &Call, CheckerContext &C) const;
 };
 }

 void MainCallChecker::checkPreCall(const CallEvent &Call, CheckerContext &C) const {
    if ( const IdentifierInfo *II = Call.getCalleeIdentifier())
     if (II -> isStr("main")) {
	 if (!BT)
	     BT.reset(new BugType (this , "Call to main", "Example checker"));
     ExplodedNode *N = C.generateErrorNode();
     auto Report = llvm::make_unique<BugReport>(*BT, BT->getName(), N);
     C.emitReport(std::move(Report));
    }
 }

 void ento::registerMainCallChecker(CheckerManager &Mgr) {
    Mgr.registerChecker<MainCallChecker>();
 }
