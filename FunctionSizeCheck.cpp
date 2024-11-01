#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang-tidy/ClangTidy.h"
#include "clang-tidy/ClangTidyCheck.h"
#include "clang-tidy/ClangTidyModule.h"
#include "clang-tidy/ClangTidyModuleRegistry.h"
#include "clang/Lex/Lexer.h"    // Pour Lexer
#include <sstream>              // Pour std::istringstream

using namespace clang;
using namespace clang::tidy;
using namespace clang::ast_matchers;

class FunctionSizeCheck : public ClangTidyCheck {
    public:
        FunctionSizeCheck(StringRef Name, ClangTidyContext *Context)
            : ClangTidyCheck(Name, Context) {}

        void registerMatchers(MatchFinder *Finder) override {
            Finder->addMatcher(functionDecl(isDefinition()).bind("function"), this);
        }

        void check(const MatchFinder::MatchResult &Result) override 
        {
            /*const auto *Func = Result.Nodes.getNodeAs<FunctionDecl>("function");
              unsigned int lineCount = Result.SourceManager->getSpellingLineNumber(Func->getEndLoc()) -
              Result.SourceManager->getSpellingLineNumber(Func->getBeginLoc());*/

            const auto *Func = Result.Nodes.getNodeAs<FunctionDecl>("function");
            if (!Func)
                return;

            SourceManager *SM = Result.SourceManager;
            SourceLocation StartLoc = Func->getBeginLoc();
            SourceLocation EndLoc = Func->getEndLoc();

            // Get the range of the function's body
            CharSourceRange Range = CharSourceRange::getCharRange(StartLoc, EndLoc);
            std::string FunctionBody = Lexer::getSourceText(Range, *SM, LangOptions(), 0).str();

            // Count non-empty lines
            std::istringstream Stream(FunctionBody);
            std::string Line;
            unsigned int lineCount = 0;

            while (std::getline(Stream, Line)) {
                // Trim line to check if it's empty or contains only an opening/closing brace
                std::string trimmedLine = Line;
                trimmedLine.erase(std::remove(trimmedLine.begin(), trimmedLine.end(), ' '), trimmedLine.end());
                trimmedLine.erase(std::remove(trimmedLine.begin(), trimmedLine.end(), '\t'), trimmedLine.end());

                if (!trimmedLine.empty() && trimmedLine != "{" && trimmedLine != "}") {
                    lineCount++;
                }
            }

            if (lineCount > 30) {
                diag(Func->getLocation(),
                        "Too many non-empty lines inside this function. Expected under 30 but got %0.")
                    << lineCount;
            }
        }
};

namespace {

    class FunctionSizeCheckModule : public ClangTidyModule
    {
        public:
            void addCheckFactories(ClangTidyCheckFactories& CheckFactories) override
            {
                CheckFactories.registerCheck<FunctionSizeCheck>("gabin-functionsizecheck");
            }
    };

}  // namespace
namespace clang::tidy {
    // Register the module using this statically initialized variable.
    static ClangTidyModuleRegistry::Add<::FunctionSizeCheckModule> functionSizeCheckModuleInit("gabin-functionsizecheck-module",
            "Adds 'gabin-functionsizecheck' checks.");

    // This anchor is used to force the linker to link in the generated object file and thus register the module.
    volatile int functionSizeCheckAnchorSource = 0;
}
