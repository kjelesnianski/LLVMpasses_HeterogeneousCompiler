#ifndef _ASSOCIATE_STRING_LITERAL_H
#define _ASSOCIATE_STRING_LITERAL_H

using namespace std;

namespace
{

class AssociateStringLiteral : public llvm::ModulePass
{
public:
	static char ID;
	size_t numInstrumented;

	AssociateStringLiteral();
	~AssociateStringLiteral();

	/* ModulePass virtual methods */
	virtual bool runOnModule(llvm::Module &M);

private:
  unsigned long long getTimestamp();
};

}

#endif /* _ASSOCIATE_STRING_LITERAL_H */
