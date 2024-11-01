#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang-tidy/ClangTidy.h"
#include "clang-tidy/ClangTidyCheck.h"
#include "clang-tidy/ClangTidyModule.h"
#include "clang-tidy/ClangTidyModuleRegistry.h"
#include "clang/AST/Stmt.h"

using namespace clang;
using namespace clang::tidy;
using namespace clang::ast_matchers;

class SwitchCaseIndentationCheck : public ClangTidyCheck {
public:
  SwitchCaseIndentationCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  void registerMatchers(MatchFinder *Finder) override {
    Finder->addMatcher(switchStmt().bind("switch"), this);
  }

  void check(const MatchFinder::MatchResult &Result) override {
    const auto *SwitchStmt = Result.Nodes.getNodeAs<clang::SwitchStmt>("switch");

    for (const auto *Child : SwitchStmt->getBody()->children()) {
      if (const auto *Case = dyn_cast<CaseStmt>(Child)) {
        if (!Result.SourceManager->isMacroBodyExpansion(Case->getBeginLoc())) {
          diag(Case->getBeginLoc(), "The code associated with the `case` conditional MUST be indented from the `case`.");
        }
      }
    }
  }
};

namespace {

    class SwitchCaseIndentationCheckModule : public ClangTidyModule
    {
        public:
            void addCheckFactories(ClangTidyCheckFactories& CheckFactories) override
            {
                CheckFactories.registerCheck<SwitchCaseIndentationCheck>("gabin-switchcaseindentationcheck");
            }
    };

}  // namespace
namespace clang::tidy {
    // Register the module using this statically initialized variable.
    static ClangTidyModuleRegistry::Add<::SwitchCaseIndentationCheckModule> switchCaseIndentationCheckModuleInit("gabin-switchcaseindentationcheck-module",
            "Adds 'gabin-gabin-switchcaseindentationcheck-module' checks.");

    // This anchor is used to force the linker to link in the generated object file and thus register the module.
    volatile int switchCaseIndentationCheckAnchorSource = 0;
}
