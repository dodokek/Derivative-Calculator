#include "backend.h"
#include "frontend.h"

int main()
{


    TreeNode* root = GetG();

    DrawTree (root);

    InitLatexFile (root);
    
    // TreeNode* d_root = GetDerivative (root, root);
    // DrawTree (d_root);

    // PrintBranch (d_root, RESULT);

    PrintMacloren (root, 1);
    
    GeneratePdf ();


    DestructTree (root);
}