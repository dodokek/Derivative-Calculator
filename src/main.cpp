// #include "TXLib.h"
#include "backend.h"
#include "frontend.h"

int main()
{


    TreeNode* root = GetG();

    DrawTree (root);

    InitLatexFile (root);
    
    TreeNode* d_root = GetDerivative (root, TEX_OUT);
    DrawTree (d_root);

    PrintMacloren (root, 3);
    
    GeneratePdf ();


    DestructTree (root);
}