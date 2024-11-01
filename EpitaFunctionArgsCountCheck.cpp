#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang-tidy/ClangTidy.h"
#include "clang-tidy/ClangTidyCheck.h"
#include "clang-tidy/ClangTidyModule.h"
#include "clang-tidy/ClangTidyModuleRegistry.h"

using namespace clang;
using namespace clang::tidy;
using namespace clang::ast_matchers;

class EpitaFunctionArgsCountCheck : public ClangTidyCheck 
{
public:
  EpitaFunctionArgsCountCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  void registerMatchers(MatchFinder *Finder) override {
    Finder->addMatcher(functionDecl().bind("function"), this);
  }

  void check(const MatchFinder::MatchResult &Result) override {
    const auto *Func = Result.Nodes.getNodeAs<FunctionDecl>("function");
    if (Func->parameters().size() > 4) {
      diag(Func->getLocation(),
           "Functions MUST NOT take more than 4 arguments.");
    }
  }
};

namespace {

class EpitaFunctionArgsCountCheckModule : public ClangTidyModule
{
public:
    void addCheckFactories(ClangTidyCheckFactories& CheckFactories) override
    {
        CheckFactories.registerCheck<EpitaFunctionArgsCountCheck>("gabin-epitafunctionargscountcheck");
    }
};

}  // namespace
namespace clang::tidy {
// Register the module using this statically initialized variable.
static ClangTidyModuleRegistry::Add<::EpitaFunctionArgsCountCheckModule> epitaFunctionArgsCountCheckModuleInit("gabin-epitafunctionargscountcheck-module",
                                                                                       "Adds 'gabin-epitafunctionargscountcheck' checks.");

// This anchor is used to force the linker to link in the generated object file and thus register the module.
volatile int epitaFunctionArgsCountCheckAnchorSource = 0;
}
