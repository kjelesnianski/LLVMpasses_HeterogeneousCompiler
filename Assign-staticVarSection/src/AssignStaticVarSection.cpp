#include "llvm/Pass.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/GlobalVariable.h"
#include "llvm/IR/GlobalValue.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/raw_ostream.h"

#include "AssignStaticVarSection.h"

#define DEBUG_TYPE "assign-static-var-section"

using namespace llvm;

///////////////////////////////////////////////////////////////////////////////
// Public API
///////////////////////////////////////////////////////////////////////////////

/**
 * Default constructor.
 */
AssignStaticVarSection::AssignStaticVarSection() : ModulePass(ID), numInstrumented(0) {}

/**
 * Default destructor.
 */
AssignStaticVarSection::~AssignStaticVarSection() {}

/**
 * Change the linkage of static global variables to put them in their own
 * sections.
 */
bool AssignStaticVarSection::runOnModule(Module &M) {
  bool modified = false;
  Module::iterator it, ite;
  Module::global_iterator gl, gle; // for global variables

  // Iterate over all static globals and place them in their own section
  for(gl = M.global_begin(), gle = M.global_end(); gl != gle; gl++) {
    std::string secName = ".";
    if(gl->isThreadLocal()) secName += "t";

    if(gl->hasCommonLinkage() &&
       gl->getName().find(".cache.") != std::string::npos) {
      gl->setLinkage(GlobalValue::InternalLinkage);
      this->numInstrumented++;
    }

    // InternalLinkage is specifically for STATIC variables
    if(gl->hasInternalLinkage()) {
      if(gl->isConstant()) {
        //Belongs in RODATA
        assert(!gl->isThreadLocal() && "TLS data should not be in .rodata");
        secName += "rodata.";
      }
      else if(gl->getInitializer()->isZeroValue()) {
        //Belongs in BSS
        secName += "bss.";
      }
      else {
        //Belongs in DATA
        secName += "data.";
      }
    } else {
      continue;
    }
    gl->setSection(secName + gl->getName().str());
    this->numInstrumented++;
  }

  if(numInstrumented > 0) modified = true;
  
  return modified;
}

char AssignStaticVarSection::ID = 0;
static RegisterPass<AssignStaticVarSection> RPAssignStaticVarSection(
  "assign-static-var-section",
  "Allow static variables to have their own sections so that they can be aligned",
  false,
  false
);

