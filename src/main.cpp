#include "backend.h"
#include "frontend.h"

int main()
{


    TreeNode* root = GetG();

    DrawTree (root);


    // DrawTree (root);

    // InitLatexFile (root);
    
    // TreeNode* d_root = GetDerivative (root, root);
    // DrawTree (d_root);

    // PrintBranch (d_root, RESULT);

    
    // GeneratePdf ();


    // DestructTree (root);
}