#ifndef _ASSIGN_STATIC_VAR_SECTION_H
#define _ASSIGN_STATIC_VAR_SECTION_H

using namespace std;

namespace
{

class AssignStaticVarSection : public llvm::ModulePass
{
public:
	static char ID;
	size_t numInstrumented;

	AssignStaticVarSection();
	~AssignStaticVarSection();

	/* ModulePass virtual methods */
	virtual bool runOnModule(llvm::Module &M);
};

}

#endif /* _ASSIGN_STATIC_VAR_SECTION_H */
