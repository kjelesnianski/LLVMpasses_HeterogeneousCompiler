#include <ctime>
#include "llvm/Pass.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/GlobalVariable.h"
#include "llvm/IR/GlobalValue.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/raw_ostream.h"

#include "AssociateStringLiteral.h"

#define DEBUG_TYPE "associate-string-literal"

using namespace llvm;

///////////////////////////////////////////////////////////////////////////////
// Public API
///////////////////////////////////////////////////////////////////////////////

/**
 * Default constructor.
 */
AssociateStringLiteral::AssociateStringLiteral() : ModulePass(ID), numInstrumented(0) {}

/**
 * Default destructor.
 */
AssociateStringLiteral::~AssociateStringLiteral() {}

/**
 * Make RO Data & inlined strings be associated with symbols such that they can be aligned
 */
bool AssociateStringLiteral::runOnModule(Module &M) {
  bool modified = false;
  string root, newName;
  Module::iterator it, ite;
  Module::global_iterator gl, gle; //for global variables
  //could also handle by just iterating through .str - .str.1 - .str.2 ....

  // Iterate over all globals and transform to associate symbol to strings for
  // each module
  for(gl = M.global_begin(), gle = M.global_end(); gl != gle; gl++) {
    // DONT NEED TO CHANGE NAME PER-SAY just change type
    // PrivateLinkage does NOT show up in any symbol table in the object file!
    if(gl->getLinkage() == GlobalValue::PrivateLinkage) {
      //change Linkage
      //FROM private unnamed_addr constant [num x i8]
      //TO global [num x i8]
      gl->setLinkage(GlobalValue::ExternalLinkage);

      // Make the global's name unique so we don't clash when linking with
      // other files
      string::size_type minusPath = M.getName().find_last_of('/');
      root = M.getName().substr(minusPath + 1);
      string::size_type pos = root.find_first_of('.');
      root = root.substr(0,pos);
      root += "_" + std::to_string(getTimestamp());
      newName = root + "_" + gl->getName().str();
      gl->setName(newName);

      // Also REMOVE unnamed_addr value
      if(gl->hasUnnamedAddr()) {
        gl->setUnnamedAddr(false);
      }
      this->numInstrumented++;
    } else {
      continue;
    }
  }

  if(numInstrumented > 0) modified = true;
  
  return modified;
}

/**
 * Our way of generating a unique suffix
 */
unsigned long long AssociateStringLiteral::getTimestamp()
{
  struct timespec time;
  clock_gettime(CLOCK_MONOTONIC, &time);
  return (time.tv_sec * 1000000000UL) | time.tv_nsec;
}

char AssociateStringLiteral::ID = 0;
static RegisterPass<AssociateStringLiteral> RPAssociateStringLiteral(
  "associate-string-literal",
  "Associate symbol to anonomous string literal",
  false,
  false
);

